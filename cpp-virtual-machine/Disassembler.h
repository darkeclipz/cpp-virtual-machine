#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "VirtualMachine.h"

class Disassembler
{
private:
	VirtualMachine* vm = nullptr;
public:
	Disassembler();
	Disassembler(VirtualMachine& vm);
	void attach(VirtualMachine& vm);
	std::vector<std::string> disassemble_lines(uint16_t address, int lines, bool expand_names);
};

