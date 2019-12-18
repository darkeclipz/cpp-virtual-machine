#include "Debugger.h"

int main() {
	ConsoleScreen console(CONSOLE_WIDTH, CONSOLE_HEIGHT, "Virtual Machine");
	VirtualMachine vm(1024 * 32);
	uint8_t* rom = new uint8_t[256]{
		0x9,  VirtualMachine::REGISTERS::AX, 65,								// MOV AX, 'A'		; Put 'A' in register A (char)
		0x9,  VirtualMachine::REGISTERS::CX, 0x20,								// MOV CX 0x2000	; Put 0x2000 in register C (screen pos)
		140,  VirtualMachine::REGISTERS::CX, 8,									// SHL CX, 8		; Shift CX 8 bits to the left.
		0x14, VirtualMachine::REGISTERS::CX, VirtualMachine::REGISTERS::AX,		// MOV [cx], [ax]	; Move 'A' onto the screen
		100,  VirtualMachine::REGISTERS::CX, 1,									// INC cx			; Next screen pos
		100,  VirtualMachine::REGISTERS::AX, 1,									// INC ax			; Next char
		0x21, 0x09,																// JMP 6			; Repeat
		0x05																	// HLT	
	};
	vm.load_rom(rom, 256, 0);
	Disassembler disassembler(vm);
	Debugger debugger(vm, console, disassembler);
}
