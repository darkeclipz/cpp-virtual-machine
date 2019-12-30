#pragma once
#include "Tokenizer.h"
#include "VirtualMachine.h"
#include <map>

class Assembler
{
private:
	Tokenizer m_tokenizer;
	std::vector<std::string> m_tokens;
	unsigned int m_index;
	std::vector<uint8_t> m_binary;
	std::vector<std::string> m_token_buffer;
	VirtualMachine* vm;
	std::vector<VirtualMachine::Instruction>* lookup;
	std::map<std::string, uint8_t> mnemonic_table;
	bool empty();
	std::string next_token();
	void put_back(std::string token);
	VirtualMachine::OPCODE_ARGUMENTS arg_type(std::string token);
	char arg_char(VirtualMachine::OPCODE_ARGUMENTS arg_type);
	void write_opcode(std::string mnemonic);
	void write_arg(std::string arg, VirtualMachine::OPCODE_ARGUMENTS arg_type);
public:
	Assembler();
	~Assembler();
	void assemble(std::string script);
	void attach(VirtualMachine& vm);
	std::vector<uint8_t>* binary();
	std::string read_file(std::string path);
	void write_file(std::string path, uint8_t binary[]);
};

