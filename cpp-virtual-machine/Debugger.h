#pragma once
#include "VirtualMachine.h"
#include "ConsoleScreen.h"
#include "Assembler.h"
#include "Disassembler.h"
#include "CommandParser.h"
#include <string>
#include <iomanip>
#include <sstream>

const unsigned short COLOR_WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
const unsigned short COLOR_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const unsigned short COLOR_RED = FOREGROUND_RED;
const unsigned short COLOR_GRAY = 0x08;
const unsigned short COLOR_RED_INVERT = BACKGROUND_RED;

class Debugger
{
private:
	std::string error_string;
	uint16_t display_memory_address_start;
	bool exit = false;
	uint16_t memory_address_cursor = 0;
	VirtualMachine* vm;
	ConsoleScreen* console;
	Assembler assembler;
	Disassembler disassembler;
	void display_memory();
	void display_screen();
	void display_disassemble();
	void display_cpu();
	void display_state();
	void handle_input();
	std::string to_hex_string(uint16_t value, int width);
	void execute_command(CommandParser& command);
	void display_memory_address(uint16_t addr);
	void cursor(uint16_t addr);
	void write_cursor(uint8_t value);
	long long cycles;
public:
	Debugger(VirtualMachine& vm, ConsoleScreen& cs);
	~Debugger();
	void cycle();
};

