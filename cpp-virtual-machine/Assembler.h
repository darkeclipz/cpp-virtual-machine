#pragma once
#include "Tokenizer.h"

class Assembler
{
private:
	Tokenizer m_tokenizer;
	std::vector<uint8_t> m_binary;
public:
	Assembler();
	~Assembler();
	std::vector<uint8_t>* assemble(std::string script);
};

