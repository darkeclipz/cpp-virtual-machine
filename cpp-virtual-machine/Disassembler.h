#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "VirtualMachine.h"

class Disassembler
{
private:
	VirtualMachine* vm;
public:
	Disassembler(VirtualMachine& vm);
	std::vector<std::string> disassemble_lines(uint16_t address, int lines);
};

