#include "Debugger.h"

int main() {
	ConsoleScreen console(CONSOLE_WIDTH, CONSOLE_HEIGHT, "Virtual Machine");
	VirtualMachine vm(65536);

	Assembler assembler;
	assembler.attach(vm);
	std::string program_path = "C:\\Users\\Isomorphism\\Documents\\Repos\\cpp-virtual-machine\\x64\\Debug\\print_hearts.vasm";
	std::string program = assembler.read_file(program_path);
	assembler.assemble(program);
	int binary_size = assembler.binary()->size();
	uint8_t* rom = new uint8_t[binary_size];
	for (int i = 0; i < binary_size; i++) {
		rom[i] = (*assembler.binary())[i];
	}
	vm.load_rom(rom, binary_size, 0);
	assembler.write_file("C:\\Users\\Isomorphism\\Documents\\Repos\\cpp-virtual-machine\\x64\\Debug\\print_hearts.vbin", rom);

	
	Debugger debugger(vm, console);
}
