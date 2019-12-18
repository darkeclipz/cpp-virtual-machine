#include "Debugger.h"

const int SCREEN_WIDTH = 76;
const int SCREEN_HEIGHT = 20;

Debugger::Debugger(VirtualMachine& vm, ConsoleScreen& cs, Disassembler& da) {
	this->vm = &vm;
	this->console = &cs;
	this->disassembler = &da;
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
			case 0: value = vm->GetAX(); break;
			case 1: value = vm->GetBX(); break;
			case 2: value = vm->GetCX(); break;
			case 3: value = vm->GetDX(); break;
			case 5: value = vm->GetSP(); break;
			case 6: value = vm->GetSB(); break;
			case 8: value = vm->GetBP(); break;
			case 9: value = vm->GetIP(); break;
			case 11: value = vm->GetFlag(VirtualMachine::FLAGS::C); break;
			case 12: value = vm->GetFlag(VirtualMachine::FLAGS::Z); break;
			case 13: value = vm->GetFlag(VirtualMachine::FLAGS::I); break;
			case 14: value = vm->GetFlag(VirtualMachine::FLAGS::B); break;
			case 15: value = vm->GetFlag(VirtualMachine::FLAGS::O); break;
			case 16: value = vm->GetFlag(VirtualMachine::FLAGS::E); break;
			case 17: value = vm->GetFlag(VirtualMachine::FLAGS::L); break;
			case 18: value = vm->GetFlag(VirtualMachine::FLAGS::G); break;
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

void Debugger::display_memory() {
	std::stringstream ss;
	for (int i = 0; i < CONSOLE_HEIGHT - SPLIT_Y - 2; i++) {

		ss << "[" << std::setfill('0') << std::setw(4) << std::hex << 16 * (i + display_memory_address_start) << "] ";

		for (int j = 0; j < 16; j++) {
			ss << std::setfill('0') << std::setw(2) << std::hex << (int)vm->read_mem(16 * (i + display_memory_address_start) + j) << " ";

			if ((16 * (i + display_memory_address_start) + j) == vm->GetIP()) {
				console->SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_RED);
			}
			else if ((int)vm->read_mem(16 * (i + display_memory_address_start) + j) != 0) {
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
	std::vector<std::string> lines = disassembler->disassemble_lines(vm->GetIP(), CONSOLE_HEIGHT - SPLIT_Y - 2);
	for (int i = 0; i < lines.size(); i++) {
		console->SetString(SPLIT_X + 1, SPLIT_Y + i, lines[i].c_str());
		console->SetAttributes(SPLIT_X + 1, SPLIT_Y + i, 12, COLOR_GRAY);
		console->SetAttributes(SPLIT_X + 1 + 12, SPLIT_Y + i, 20, COLOR_WHITE);
	}
}

void Debugger::handle_input() {
	// NOTE: cant write to screen in this function, it gets overwritten.
	error_string = "";
	if (vm->GetFlag(VirtualMachine::FLAGS::B)) {
		std::cout << "debug> ";
		std::string input;
		std::cin >> input;
		if (input == "step") { vm->step(); }
		else if (input == "run") { vm->SetFlag(VirtualMachine::FLAGS::B, false); }
		else if (input == "reset") { vm->reset(); }
		else if (input == "exit") { exit = true; }
		else if (input != "") {
			error_string = "invalid command you idiot";
		}
	}
}

void Debugger::display_state() {
	console->DrawString(CONSOLE_WIDTH - 1 - error_string.size(), CONSOLE_HEIGHT - 1, error_string.c_str(), COLOR_RED);
	if (!vm->GetFlag(VirtualMachine::FLAGS::B)) {
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
			vm->SetFlag(VirtualMachine::FLAGS::B, true);
		}
		if (!vm->GetFlag(VirtualMachine::FLAGS::B)) {
			vm->step();
		}
		console->ClearBuffer();
		display_state();
		console->DrawBuffer();
		handle_input();
	}
}