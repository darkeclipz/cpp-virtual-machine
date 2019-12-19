#include "Debugger.h"

const int SCREEN_WIDTH = 76;
const int SCREEN_HEIGHT = 20;

Debugger::Debugger(VirtualMachine& vm, ConsoleScreen& cs) {
	this->vm = &vm;
	this->console = &cs;
	this->assembler.attach(vm);
	this->disassembler.attach(vm);
	display_memory_address_start = 0;
	cycle();
}

Debugger::~Debugger() {

}

std::string Debugger::to_hex_string(uint16_t value, int width) {
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(width) << std::hex << value;
	return ss.str();
}

void Debugger::display_cpu() {
	// yes this is one ugly function, luckily it wont change.

	// list with strings
	std::vector<std::string> names = { "AX", "BX", "CX", "DX", "", "SP", "SB", "", "BP", "IP", "", "C", "Z", "I", "B", "O", "E", "L", "G" };

	// list with values
	std::vector <std::string> annotations = { "(carry)", "(zero)", "(interrupt)", "(break)", "(overflow)", "(equality)", "(less than)", "(greater than)" };

	std::stringstream ss;
	for (int i = 0; i < names.size(); i++) {
		if (names[i] != "") {
			ss << names[i] << " = ";

			// Grab the correct value from the CPU.
			uint16_t value = 0;
			switch (i) {
			case 0: value  = vm->read_reg(VirtualMachine::REGISTERS::AX); break;
			case 1: value  = vm->read_reg(VirtualMachine::REGISTERS::BX); break;
			case 2: value  = vm->read_reg(VirtualMachine::REGISTERS::CX); break;
			case 3: value  = vm->read_reg(VirtualMachine::REGISTERS::DX); break;
			case 5: value  = vm->read_reg(VirtualMachine::REGISTERS::SP); break;
			case 6: value  = vm->read_reg(VirtualMachine::REGISTERS::SB); break;
			case 8: value  = vm->read_reg(VirtualMachine::REGISTERS::BP); break;
			case 9: value  = vm->read_reg(VirtualMachine::REGISTERS::IP); break;
			case 11: value = vm->read_flag(VirtualMachine::FLAGS::C); break;
			case 12: value = vm->read_flag(VirtualMachine::FLAGS::Z); break;
			case 13: value = vm->read_flag(VirtualMachine::FLAGS::I); break;
			case 14: value = vm->read_flag(VirtualMachine::FLAGS::B); break;
			case 15: value = vm->read_flag(VirtualMachine::FLAGS::O); break;
			case 16: value = vm->read_flag(VirtualMachine::FLAGS::E); break;
			case 17: value = vm->read_flag(VirtualMachine::FLAGS::L); break;
			case 18: value = vm->read_flag(VirtualMachine::FLAGS::G); break;
			}

			// White if it is != 0.
			if (value == 0) { console->SetAttributes(SPLIT_X + 1, i + 1, CONSOLE_WIDTH - SPLIT_X - 1, COLOR_GRAY); }
			else { console->SetAttributes(SPLIT_X + 1, i + 1, CONSOLE_WIDTH - SPLIT_X - 1, COLOR_WHITE); }

			// Print as hex
			int flags_start_index = 11;
			ss << to_hex_string(value, i < flags_start_index ? 4 : 1); // 1 char width for boolean (registers)
			if (i >= flags_start_index) { // the rest are flags
				ss << " " << annotations[i - flags_start_index];
			}
			console->SetString(SPLIT_X + 1, i + 1, ss.str().c_str());

			ss.str("");
			ss.clear();
		}
	}
}

void Debugger::display_memory_address(uint16_t addr) {
	if (addr < 0) {
		addr = 0;
	}
	if (addr > 0xffff) {
		addr = 0xffff;
	}
	display_memory_address_start = addr;
}

void Debugger::cursor(uint16_t addr) {
	if (addr < 0) {
		addr = 0;
	}
	if (addr > 0xffff) {
		addr = 0xffff;
	}
	memory_address_cursor = addr;
}

void Debugger::write_cursor(uint8_t val) {
	vm->write_mem(memory_address_cursor, val);
	memory_address_cursor++;
}

void Debugger::display_memory() {
	std::stringstream ss;
	for (int i = 0; i < CONSOLE_HEIGHT - SPLIT_Y - 2; i++) {

		ss << "[" << std::setfill('0') << std::setw(4) << std::hex << 16 * (i + display_memory_address_start / 16) << "] ";

		for (int j = 0; j < 16; j++) {
			ss << std::setfill('0') << std::setw(2) << std::hex << (int)vm->read_mem(16 * (i + display_memory_address_start / 16) + j) << " ";

			if ((16 * (i + display_memory_address_start / 16) + j) == memory_address_cursor && vm->read_flag(VirtualMachine::FLAGS::B)) {
				console->SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_RED_INVERT);
			}
			else if ((16 * (i + display_memory_address_start / 16) + j) == vm->read_reg(VirtualMachine::REGISTERS::IP)) {
				console->SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_RED);
			}
			else if ((int)vm->read_mem(16 * (i + display_memory_address_start / 16) + j) != 0) {
				console->SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_WHITE);
			}
			else {
				console->SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_GRAY);
			}
		}

		console->SetString(1, SPLIT_Y + i, ss.str().c_str());
		console->SetAttributes(1, SPLIT_Y + i, 6, COLOR_WHITE);
		ss.str("");
		ss.clear();
	}
}

void Debugger::display_screen() {
	for (unsigned int x = 0; x < SCREEN_WIDTH - 1; x++) {
		for (unsigned int y = 0; y < SCREEN_HEIGHT - 1; y++) {
			console->SetChar(x + 2, y + 1, vm->read_mem(vm->vga_address + SCREEN_WIDTH * y + x));
			console->SetAttribute(x + 2, y + 1, COLOR_WHITE);
		}
	}
}

void Debugger::display_disassemble() {
	bool expand_lines = true;
	uint16_t ip = vm->read_reg(VirtualMachine::REGISTERS::IP);
	unsigned int n_lines = CONSOLE_HEIGHT - SPLIT_Y - 2;
	std::vector<std::string> lines = disassembler.disassemble_lines(ip, n_lines, expand_lines);
	for (int i = 0; i < lines.size(); i++) {
		console->SetString(SPLIT_X + 1, SPLIT_Y + i, lines[i].c_str());
		console->SetAttributes(SPLIT_X + 1, SPLIT_Y + i, 12, COLOR_GRAY);
		console->SetAttributes(SPLIT_X + 1 + 12, SPLIT_Y + i, 20, COLOR_WHITE);
	}
}

void Debugger::execute_command(CommandParser& command) {
	std::string cmd = (*command.tokens())[0];
	if (cmd == "run" || cmd == "r") { vm->write_flag(VirtualMachine::FLAGS::B, false); }
	else if (cmd == "step" || cmd == "s") { vm->step(); }
	else if (cmd == "reset") { vm->reset(); }
	else if (cmd == "exit") { exit = true; }
	else if (cmd == "set_reg") {
		command.check_argument_count(3);
		uint8_t reg = command.parse_reg(command.read_arg_string(1));
		uint16_t val = command.read_arg_uint16(2);
		vm->write_reg(reg, val);
	}
	else if (cmd == "set_mem") {
		command.check_argument_count(3);
		uint16_t addr = command.read_arg_uint16(1);
		uint16_t val = command.read_arg_uint16(2);
		vm->write_mem(addr, val);
	}
	else if (cmd == "mem" || cmd == "m") {
		command.check_argument_count(2);
		uint16_t addr = command.read_arg_uint16(1);
		display_memory_address(addr);
	}
	else if (cmd == "cursor" || cmd == "c") {
		command.check_argument_count(2);
		uint16_t addr = command.read_arg_uint16(1);
		cursor(addr);
	}
	else if (cmd == "assemble" || cmd == "a") {
		std::stringstream ss;
		for (auto& w : *command.tokens())
			ss << w << " ";
		std::string operation = ss.str();
		operation = operation.substr(cmd.size() + 1, operation.size() - 2);
		assembler.assemble(operation);
		for (auto opcode : *assembler.binary()) 
			write_cursor(opcode);
	}
	else if (cmd == "reload") {}
	else if (cmd == "load") {}
	else if (cmd == "save") {
		// save <file_name> <mem_start> <mem_end> 
		// or just save <file name>  to dump it all into a file.

	}
	else if (cmd == "b") { /* breakpoint */ }
	else if (cmd != "") {
		std::stringstream msg;
		msg << "invalid cmd: ";
		for (auto c : *command.tokens()) {
			msg << c << " ";
		}
		error_string = msg.str();
	}
}

void Debugger::handle_input() {
	// NOTE: cant write to screen in this function, it gets overwritten.
	error_string = "";
	if (vm->read_flag(VirtualMachine::FLAGS::B)) {
		std::cout << "debug> ";
		std::string input;
		std::getline(std::cin, input);
		if (input.size() == 0) {
			return;
		}
		CommandParser command;
		command.tokenize(input);
		if (command.tokens()->size() == 0) {
			error_string = "no input found";
			return;
		}

		try {
			execute_command(command);
		}
		catch (const std::exception & ex) {
			error_string = ex.what();
		}
	}
}

void Debugger::display_state() {
	console->DrawString(CONSOLE_WIDTH - 1 - error_string.size(), CONSOLE_HEIGHT - 1, error_string.c_str(), COLOR_RED);
	if (!vm->read_flag(VirtualMachine::FLAGS::B)) {
		console->DrawString(0, CONSOLE_HEIGHT - 1, "running (F5 to halt)", COLOR_GRAY);
	}
	display_screen();
	display_memory();
	display_disassemble();
	display_cpu();
}

void Debugger::cycle() {
	while (!exit) {
		if (GetKeyState(VK_F5) & 0x8000) {
			vm->write_flag(VirtualMachine::FLAGS::B, true);
		}
		if (!vm->read_flag(VirtualMachine::FLAGS::B)) {
			vm->step();
		}
		console->ClearBuffer();
		display_state();
		console->DrawBuffer();
		handle_input();
		cycles++;
	}
}