#include "VirtualMachine.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include "ConsoleScreen.h"
#include <sstream>

const int CONSOLE_WIDTH = 120;
const int CONSOLE_HEIGHT = 40;
const int SCREEN_WIDTH = 60;
const int SCREEN_HEIGHT = 20;
const int SPLIT_X = 80;
const int SPLIT_Y = 21;

std::string to_hex_string(uint16_t value, int width) {
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(width) << std::hex << value;
	return ss.str();
}

void display_screen(VirtualMachine& vm, ConsoleScreen& cs) {
	for (unsigned int x = 0; x < SCREEN_WIDTH; x++) {
		for (unsigned int y = 0; y < SCREEN_HEIGHT; y++) {
			cs.SetChar(x, y, vm.read_mem(vm.vga_address + SCREEN_WIDTH * y + x));
			cs.SetAttribute(x, y, COLOR_WHITE);
		}
	}
}

void display_memory(VirtualMachine& vm, ConsoleScreen& cs) {
	std::stringstream ss;
	for (int i = 0; i < CONSOLE_HEIGHT - SPLIT_Y - 2; i++) {

		ss << "[" << std::setfill('0') << std::setw(4) << std::hex << 16 * i << "] ";

		for (int j = 0; j < 16; j++) {
			ss << std::setfill('0') << std::setw(2) << std::hex << (int)vm.read_mem(16 * i + j) << " ";

			if ((16 * i + j) == vm.GetIP()) {
				cs.SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_RED);
			}
			else if ((int)vm.read_mem(16 * i + j) != 0) {
				cs.SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_WHITE);
			}
			else {
				cs.SetAttributes(8 + 3 * j, SPLIT_Y + i, 2, COLOR_GRAY);
			}
		}

		cs.SetString(1, SPLIT_Y + i, ss.str().c_str());
		cs.SetAttributes(1, SPLIT_Y + i, 6, COLOR_WHITE);
		ss.str("");
		ss.clear();
	}
}

void display_disassemble(VirtualMachine& vm, ConsoleScreen& cs) {
	std::vector<std::string> lines = vm.disassemble_lines(vm.GetIP() - 1, CONSOLE_HEIGHT - SPLIT_Y - 2);
	for (int i = 0; i < lines.size(); i++) {
		cs.SetString(SPLIT_X + 1, SPLIT_Y + i, lines[i].c_str());
		cs.SetAttributes(SPLIT_X + 1, SPLIT_Y + i, 12, COLOR_GRAY);
		cs.SetAttributes(SPLIT_X + 1 + 12, SPLIT_Y + i, 20, COLOR_WHITE);
	}
}


void display_cpu(VirtualMachine& vm, ConsoleScreen& cs) {
	// yes this is one ugly function, luckily it wont change.

	// list with strings
	std::vector<std::string> names = { "AX", "BX", "CX", "DX", "", "SP", "BP", "", "IP", "", "C", "Z", "I", "B", "O", "E", "L", "G" };

	// list with values
	std::vector <std::string> annotations = { "(carry)", "(zero)", "(interrupt)", "(break)", "(overflow)", "(equality)", "(less than)", "(greater than)" };

	std::stringstream ss;
	for (int i = 0; i < names.size(); i++) {
		if (names[i] != "") {
			ss << names[i] << " = ";

			// Grab the correct value from the CPU.
			uint16_t value = 0;
			switch (i) {
			case 0: value = vm.GetAX(); break;
			case 1: value = vm.GetBX(); break;
			case 2: value = vm.GetCX(); break;
			case 3: value = vm.GetDX(); break;
			case 5: value = vm.GetSP(); break;
			case 6: value = vm.GetBP(); break;
			case 8: value = vm.GetIP(); break;
			case 10: value = vm.GetFlag(VirtualMachine::FLAGS::C); break;
			case 11: value = vm.GetFlag(VirtualMachine::FLAGS::Z); break;
			case 12: value = vm.GetFlag(VirtualMachine::FLAGS::I); break;
			case 13: value = vm.GetFlag(VirtualMachine::FLAGS::B); break;
			case 14: value = vm.GetFlag(VirtualMachine::FLAGS::O); break;
			case 15: value = vm.GetFlag(VirtualMachine::FLAGS::E); break;
			case 16: value = vm.GetFlag(VirtualMachine::FLAGS::L); break;
			case 17: value = vm.GetFlag(VirtualMachine::FLAGS::G); break;
			}

			// White if it is != 0.
			if (value == 0) { cs.SetAttributes(SPLIT_X + 1, i + 1, CONSOLE_WIDTH - SPLIT_X - 1, COLOR_GRAY); }
			else { cs.SetAttributes(SPLIT_X + 1, i + 1, CONSOLE_WIDTH - SPLIT_X - 1, COLOR_WHITE); }

			// Print as hex
			ss << to_hex_string(value, i < 10 ? 4 : 1); // 1 char width for boolean (registers)
			if (i >= 10) { // the rest are flags
				ss << " " << annotations[i - 10];
			}
			cs.SetString(SPLIT_X + 1, i + 1, ss.str().c_str());

			ss.str("");
			ss.clear();
		}
	}
}

std::string error_string;

void get_input(VirtualMachine& vm, ConsoleScreen& cs) {
	// NOTE: cant write to screen in this function, it gets overwritten.
	error_string = "";
	if (vm.GetFlag(VirtualMachine::FLAGS::B)) {
		std::cout << "debug> ";
		std::string input;
		std::cin >> input;
		if (input == "step") { vm.step(); }
		else if (input == "run") { vm.SetFlag(VirtualMachine::FLAGS::B, false); }
		else if (input == "reset") { vm.reset(); }
		else if (input != "") {
			error_string = "invalid command you idiot";
		}
	}
}

void display_state(VirtualMachine& vm, ConsoleScreen& cs) {
	cs.DrawString(CONSOLE_WIDTH - 1 - error_string.size(), CONSOLE_HEIGHT - 1, error_string.c_str(), COLOR_RED);
	if (!vm.GetFlag(VirtualMachine::FLAGS::B)) {
		cs.DrawString(0, CONSOLE_HEIGHT - 1, "running (F5 to halt)", COLOR_GRAY);
	}
	display_screen(vm, cs);
	display_memory(vm, cs);
	display_disassemble(vm, cs);
	display_cpu(vm, cs);
}

int main() {
	ConsoleScreen console(CONSOLE_WIDTH, CONSOLE_HEIGHT, "Virtual Machine");
	VirtualMachine vm(1024 * 32);
	uint8_t* rom = new uint8_t[256]{
		//0x09, VirtualMachine::REGISTERS::AX, 0xB0,
		//0x09, VirtualMachine::REGISTERS::BX, 0x0B,
		//0x09, VirtualMachine::REGISTERS::CX, 0xB0,
		//0x09, VirtualMachine::REGISTERS::DX, 0xAB,
		//0x10, VirtualMachine::REGISTERS::BX, VirtualMachine::REGISTERS::CX,

		//0x09, VirtualMachine::REGISTERS::AX, 'H',
		//0x12, 0x80, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'e',
		//0x12, 0x81, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'l',
		//0x12, 0x82, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'l',
		//0x12, 0x83, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'o',
		//0x12, 0x84, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'w',
		//0x12, 0x86, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'o',
		//0x12, 0x87, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'r',
		//0x12, 0x88, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'l',
		//0x12, 0x89, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, 'd',
		//0x12, 0x8a, VirtualMachine::REGISTERS::AX,

		//0x09, VirtualMachine::REGISTERS::AX, ' ',
		//0x12, 0x80, VirtualMachine::REGISTERS::AX,
		//0x12, 0x81, VirtualMachine::REGISTERS::AX,
		//0x12, 0x82, VirtualMachine::REGISTERS::AX,
		//0x12, 0x83, VirtualMachine::REGISTERS::AX,
		//0x12, 0x84, VirtualMachine::REGISTERS::AX,
		//0x12, 0x86, VirtualMachine::REGISTERS::AX,
		//0x12, 0x87, VirtualMachine::REGISTERS::AX,
		//0x12, 0x88, VirtualMachine::REGISTERS::AX,
		//0x12, 0x89, VirtualMachine::REGISTERS::AX,
		//0x12, 0x8a, VirtualMachine::REGISTERS::AX,

		//0x21, 0x00

		0x9, VirtualMachine::REGISTERS::AX, 65,
		0x9, VirtualMachine::REGISTERS::CX, 0x80,

		0x14, VirtualMachine::REGISTERS::CX, VirtualMachine::REGISTERS::AX,
		100, VirtualMachine::REGISTERS::CX, 1,
		100, VirtualMachine::REGISTERS::AX, 1,


		0x21, 0x06,

		0x05
	};
	vm.load_rom(rom, 256, 0);
	while (true) {

		if (GetKeyState(VK_F5) & 0x8000) {
			vm.SetFlag(VirtualMachine::FLAGS::B, true);
		}

		if (!vm.GetFlag(VirtualMachine::FLAGS::B)) {
			vm.step();
		}

		console.ClearBuffer();
		display_state(vm, console);
		console.DrawBuffer();
		get_input(vm, console);
	}
	delete[] rom;
}
