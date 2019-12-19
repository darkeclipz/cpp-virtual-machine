#include "Disassembler.h"

Disassembler::Disassembler() { }

void Disassembler::attach(VirtualMachine& vm) {
	this->vm = &vm;
}

Disassembler::Disassembler(VirtualMachine& vm) {
	attach(vm);
}

std::vector<std::string> Disassembler::disassemble_lines(uint16_t address, int n, bool expand_names) {
	if (!vm) {
		throw std::exception("vm not attached");
	}
	std::vector<std::string> lines;
	for (int i = 0; lines.size() < n; i += vm->lookup[vm->read_mem(address + i)].instruction_size) {
		
		VirtualMachine::Instruction instruction = vm->lookup[vm->read_mem(address + i)];
		std::stringstream ss;
		std::stringstream ssargs;
		

		for (int j = 0; j < vm->lookup[vm->read_mem(address + i)].instruction_size; j++)
		{
			ss << std::setfill('0') << std::setw(2) << std::hex << (int)vm->read_mem(address + i + j) << " ";
			if (vm->read_mem(address + i) != 0 && j > 0) {


				switch (instruction.opcode_arguments)
				{
				case VirtualMachine::OPCODE_ARGUMENTS::NONE:
					break;

				}
				

				ssargs << (j > 1 ? ", " : "") << std::setfill('0') << std::setw(2) << std::hex << (int)vm->read_mem(address + i + j);
			}
				
		}

		int size = ss.str().size();
		while (14 - size > 0) {
			ss << " ";
			size++;
		}

		
		ss << (expand_names ? instruction.full_name : instruction.mnemonic) << " " << ssargs.str();
		lines.push_back(ss.str());
		ssargs.str("");
		ssargs.clear();
		ss.str("");
		ss.clear();
	}
	return lines;
}