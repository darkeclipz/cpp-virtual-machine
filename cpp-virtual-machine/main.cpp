#include "Debugger.h"

int main() {
	ConsoleScreen console(CONSOLE_WIDTH, CONSOLE_HEIGHT, "Virtual Machine");
	VirtualMachine vm(65536);
	//uint8_t* rom = new uint8_t[256]{};
	//vm.load_rom(rom, 256, 0);
	Debugger debugger(vm, console);
}
