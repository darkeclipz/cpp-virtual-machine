#include "Assembler.h"

Assembler::Assembler() {
	m_tokenizer = Tokenizer();
	m_binary = {};
}

Assembler::~Assembler() {
	m_binary.clear();
}

std::vector<std::uint8_t>* Assembler::assemble(std::string script) {
	return &m_binary;
}