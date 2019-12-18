#include "Disassembler.h"

Disassembler::Disassembler(VirtualMachine& vm) {
	this->vm = &vm;
}

std::vector<std::string> Disassembler::disassemble_lines(uint16_t address, int n) {
	std::vector<std::string> lines;
	if (address > 0x00FF) address = 0;
	for (int i = 0; lines.size() < n; i += vm->lookup[vm->read_mem(address + i)].instruction_size) {
		std::stringstream ss;
		std::stringstream ssargs;
		for (int j = 0; j < vm->lookup[vm->read_mem(address + i)].instruction_size; j++)
		{
			ss << std::setfill('0') << std::setw(2) << std::hex << (int)vm->read_mem(address + i + j) << " ";
			if (vm->read_mem(address + i) != 0 && j > 0)
				ssargs << (j > 1 ? ", " : "") << std::setfill('0') << std::setw(2) << std::hex << (int)vm->read_mem(address + i + j);
		}

		int size = ss.str().size();
		while (14 - size > 0) {
			ss << " ";
			size++;
		}

		ss << vm->lookup[vm->read_mem(address + i)].name << " " << ssargs.str();

		lines.push_back(ss.str());
		ssargs.str("");
		ssargs.clear();
		ss.str("");
		ss.clear();
	}
	return lines;
}