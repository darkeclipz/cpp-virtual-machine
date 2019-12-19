#pragma once
#include <cstdint>
#include <iostream>
#include <vector>
#include <windows.h>
#include <map>

class VirtualMachine
{
public:
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
	enum class OPCODE_ARGUMENTS {

		NONE,

		C,		// Constant
		M,		// Memory
		P,		// Pointer
		R,		// Register

		RC,		// Register <- Constant
		RR,		// Register <- Register
		RM,		// Register <- Memory
		RP,		// Register <- Pointer

		MC,		// Memory <- Constant
		MR,		// Memory <- Register
		MM,		// Memory <- Memory
		MP,		// Memory <- Pointer

		PC,		// Pointer <- Constant
		PR,		// Pointer <- Register
		PM,		// Pointer <- Memory
		PP,		// Pointer <- Pointer

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
	uint8_t NOP(); uint8_t HLT(); uint8_t MOVRC(); uint8_t MOVMM();
	uint8_t MOVRR(); uint8_t MOVRM(); uint8_t MOVMR(); uint8_t ADDRC();
	uint8_t JMPC(); uint8_t MOVPR(); uint8_t SHLRC();

private:
	void clock();
	uint8_t opcode;

public:
	VirtualMachine(int memory_size);
	~VirtualMachine();
	uint8_t read_mem(uint16_t address);
	void write_mem(uint16_t address, uint8_t value);
	void load_rom(uint8_t* rom, int size, uint8_t address);
	uint16_t read_reg(uint8_t reg);
	void write_reg(uint8_t reg, uint16_t value);
	uint8_t read_flag(uint8_t flag);
	void write_flag(uint8_t flag, bool value);
	void step();
	void reset();
	const uint16_t vga_address = 0x2000;

	struct Instruction
	{
		std::string full_name;
		uint8_t(VirtualMachine::* operate)(void) = nullptr;
		uint8_t instruction_size;
		std::string mnemonic;
		OPCODE_ARGUMENTS opcode_arguments;
	};

	std::vector<Instruction> lookup;
};

