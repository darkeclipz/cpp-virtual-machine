#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <windows.h>

const unsigned short COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const unsigned short COLOR_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned short COLOR_RED = FOREGROUND_RED;
const unsigned short COLOR_GRAY = 0x08;

class VirtualMachine
{
public:
	enum FLAGS {
		C = (1 << 0),	// Carry flag
		Z = (1 << 1),	// Zero flag
		I = (1 << 2),	// Disable interrupts
		B = (1 << 3),	// Break
		O = (1 << 4),	// Overflow
		E = (1 << 5),	// Equality
		L = (1 << 6),	// Less than
		G = (1 << 7)	// Greater than
	};
	enum REGISTERS {
		AX = (1 << 0), // reg A
		BX = (1 << 1), // reg B
		CX = (1 << 2), // reg C
		DX = (1 << 3), // reg D
		SP = (1 << 4), // stack pointer
		SB = (1 << 5), // stack base
		BP = (1 << 6), // base pointer (for relative addressing)
		IP = (1 << 7)  // instruction pointer
	};
private:
	int m_memory_size;
	uint8_t* m_memory;

	// Registers
	uint16_t ax;	// Register A
	uint16_t bx;	// Register B
	uint16_t cx;	// Register C
	uint16_t dx;	// Register D

	uint16_t sp;	// Stack Pointer
	uint16_t sb;	// Stack base
	uint16_t bp;	// Base Pointer
	uint16_t ip;	// Instruction Pointer

	uint8_t m_flags;	// Flags

	// Opcodes
	uint8_t NOP(); uint8_t HLT(); uint8_t MRC(); uint8_t MMM();
	uint8_t MRR(); uint8_t MRM(); uint8_t MMR(); uint8_t ARC();
	uint8_t JMP(); uint8_t MPP(); uint8_t SHL();

private:
	void clock();

	void irq();
	void nmi(); // ??

	uint16_t read_reg(uint8_t reg);
	void write_reg(uint8_t reg, uint16_t value);

	uint8_t opcode;

public:
	VirtualMachine(int memory_size);
	~VirtualMachine();
	uint8_t read_mem(uint16_t address);
	void write_mem(uint16_t address, uint8_t value);
	void load_rom(uint8_t* rom, int size, uint8_t address);
	uint8_t GetFlag(FLAGS flag);
	void SetFlag(FLAGS flag, bool value);
	uint16_t GetAX();
	uint16_t GetBX();
	uint16_t GetCX();
	uint16_t GetDX();
	uint16_t GetSP();
	uint16_t GetSB();
	uint16_t GetBP();
	uint16_t GetIP();
	void step();
	void reset();
	const uint16_t vga_address = 0x2000;

	struct INSTRUCTION
	{
		std::string name;
		uint8_t(VirtualMachine::* operate)(void) = nullptr;
		uint8_t instruction_size;
		std::string disassemble_symbol;
	};

	std::vector<INSTRUCTION> lookup;

};

