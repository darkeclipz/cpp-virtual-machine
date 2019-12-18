#include "Debugger.h"

int main() {
	ConsoleScreen console(CONSOLE_WIDTH, CONSOLE_HEIGHT, "Virtual Machine");
	VirtualMachine vm(1024 * 32);
	uint8_t* rom = new uint8_t[256]{


		0x10, 0x01, 0xB0, 0x0B,

		0x11, 0x02, 0x01,
		0x11, 0x04, 0x01,
		0x11, 0x08, 0x01,

		0x10, 0x01, 0x00, 0x00,
		0x10, 0x04, 0x20, 0x00,

		0x19, 0x04, 0x01,

		0x40, 0x01, 0x00, 0x01,
		0x40, 0x04, 0x00, 0x01,

		0x20, 0x00, 21,

		0x05																	// HLT	
	};
	vm.load_rom(rom, 256, 0);
	Disassembler disassembler(vm);
	Debugger debugger(vm, console, disassembler);
}
