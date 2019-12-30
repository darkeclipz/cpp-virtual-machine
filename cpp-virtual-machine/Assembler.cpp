#include "Assembler.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../cpp-utils/utils.cpp"

Assembler::Assembler() {
	m_tokenizer = Tokenizer();
	m_tokens = {};
	m_binary = {};
}

Assembler::~Assembler() {
	m_binary.clear();
}

void Assembler::attach(VirtualMachine& vm) {
	this->vm = &vm;
	this->lookup = &vm.lookup;
	for (int i = 0; i < 256; i++) {
		mnemonic_table[(*lookup)[i].full_name] = i;
	}
}

std::string Assembler::read_file(std::string path) {
	std::ifstream file;
	file.open(path);
	std::string line;
	std::stringstream content;
	if (!file) {
		throw std::invalid_argument(path);
	}
	while (file >> line) {
		content << line << ' ';
	}
	file.close();
	return content.str();
}

void Assembler::write_file(std::string path, uint8_t binary[]) {
	std::fstream file;
	file.open(path, std::ios::out | std::ios::binary);
	file.write((char*)&binary[0], *binary);
	file.close();
}

std::vector<uint8_t>* Assembler::binary() {
	return &m_binary;
}

bool Assembler::empty() {
	return m_index >= m_tokens.size();
}

std::string Assembler::next_token() {
	if (empty()) {
		return "";
	}
	if (m_token_buffer.size() > 0) {
		std::string token = m_token_buffer.back();
		m_token_buffer.pop_back();
		return token;
	}
	return m_tokens[m_index++];
}

void Assembler::put_back(std::string token) {
	m_token_buffer.push_back(token);
}

VirtualMachine::OPCODE_ARGUMENTS Assembler::arg_type(std::string token) {
	uint8_t reg = m_tokenizer.parse_reg_unsafe(token);
	// First check if it is a register
	if (reg) {
		return VirtualMachine::OPCODE_ARGUMENTS::R;
	}
	// Maybe its memory, its indicated with [ .. ]
	if (token[0] == '[' && token[token.size()-1] == ']') {

		reg = m_tokenizer.parse_reg_unsafe(token.substr(1, token.size() - 2));
		// Check if .. is a register, if so it is a pointer.
		if (reg) {
			return VirtualMachine::OPCODE_ARGUMENTS::P;
		}

		// Must be a constant (memory address) then.
		return VirtualMachine::OPCODE_ARGUMENTS::M;
	}
	// Only thing left is a constant
	return VirtualMachine::OPCODE_ARGUMENTS::C;
}

char Assembler::arg_char(VirtualMachine::OPCODE_ARGUMENTS arg_type) {
	switch (arg_type) {
	case VirtualMachine::OPCODE_ARGUMENTS::C: return 'C';
	case VirtualMachine::OPCODE_ARGUMENTS::R: return 'R';
	case VirtualMachine::OPCODE_ARGUMENTS::M: return 'M';
	case VirtualMachine::OPCODE_ARGUMENTS::P: return 'P';
	}
}

void Assembler::write_opcode(std::string mnemonic) {
	uint8_t opcode = mnemonic_table[mnemonic];
	m_binary.push_back(opcode); // write opcode
}

void Assembler::write_arg(std::string arg, VirtualMachine::OPCODE_ARGUMENTS arg_type) {
	uint16_t val;
	uint8_t reg;
	switch (arg_type) {
	case VirtualMachine::OPCODE_ARGUMENTS::M:
		// [30] to 30, or [0x100] to 0x100
		arg = arg.substr(1, arg.size() - 2);
	case VirtualMachine::OPCODE_ARGUMENTS::C:
		// if its M or C, its a number or hex (2 bytes)
		val = m_tokenizer.read_arg_uint16(arg);
		m_binary.push_back(val >> 8);
		m_binary.push_back(val);
		break;
	case VirtualMachine::OPCODE_ARGUMENTS::P:
		// a pointer is like [ax], so we cast it to ax
		arg = arg.substr(1, arg.size() - 2);
	case VirtualMachine::OPCODE_ARGUMENTS::R:
		// get the byte for the register
		reg = m_tokenizer.parse_reg_unsafe(arg);
		m_binary.push_back(reg);
		break;
	}
}

void Assembler::assemble(std::string script) {
	if (mnemonic_table.size() == 0) {
		throw std::invalid_argument("empty mnemonic table, first use asm.attach(vm).");
	}
	m_binary.clear();
	m_tokenizer.tokenize(script);
	m_tokens = *m_tokenizer.tokens();
	m_index = 0;
	while (!empty()) {
		// get the first token, like MOV
		std::string mnemonic = to_upper(next_token());
		// if it doesnt have args, like HLT, we write that opcode
		if (mnemonic_table.count(mnemonic) > 0) {
			write_opcode(mnemonic);
			continue;
		}
		// try as a 1 argument opcode
		std::string arg0 = next_token();
		auto arg0_type = arg_type(arg0);  // determine the type of the argument: C, R, M, P
		mnemonic += arg_char(arg0_type);  // append to end of mnemonic, e.g. JMPC or JMPR.
		// check if this opcode exists
		// opcodes come directly from the VM function lookup table.
		if (mnemonic_table.count(mnemonic) > 0) {
			write_opcode(mnemonic);
			write_arg(arg0, arg0_type);
			continue;
		}
		// try as a two argument opcode
		std::string arg1 = next_token();
		auto arg1_type = arg_type(arg1); // get the argument type C, R, M, P
		mnemonic += arg_char(arg1_type); // append to end of mnemonic, so its is like MOVRC or MOVRR.
		// does this exist?
		if (mnemonic_table.count(mnemonic) > 0) {
			write_opcode(mnemonic);
			write_arg(arg0, arg0_type);
			write_arg(arg1, arg1_type);
			continue;
		}
		// came here? then we don't know what it is
		std::string error = "unkown mnemonic: " + mnemonic;
		throw std::exception(error.c_str());
	}
}

