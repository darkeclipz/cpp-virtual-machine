#pragma once
#include "VirtualMachine.h"
#include "ConsoleScreen.h"
#include "Assembler.h"
#include "Disassembler.h"
#include <string>
#include <iomanip>
#include <sstream>

class Debugger
{
private:
	std::string error_string;
	uint16_t display_memory_address_start;
	bool exit = false;
	VirtualMachine* vm;
	ConsoleScreen* console;
	Disassembler* disassembler;
	void display_memory();
	void display_screen();
	void display_disassemble();
	void display_cpu();
	void display_state();
	void handle_input();
	std::string to_hex_string(uint16_t value, int width);
public:
	Debugger(VirtualMachine& vm, ConsoleScreen& cs, Disassembler& da);
	~Debugger();
	void cycle();
};

