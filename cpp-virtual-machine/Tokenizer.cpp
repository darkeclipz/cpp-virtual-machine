#include "Tokenizer.h"
#include "VirtualMachine.h"
#include <sstream>

const char EMPTY_BUFFER = 0x00;

Tokenizer::Tokenizer() {
	m_string = "";
	m_index = 0;
	m_tokens = {};
	m_char_buffer = EMPTY_BUFFER;
}

Tokenizer::~Tokenizer() {
	m_string = "";
	m_tokens.clear();
}

char Tokenizer::next_char() {
	if (m_char_buffer != EMPTY_BUFFER) {
		char c = m_char_buffer;
		m_char_buffer = EMPTY_BUFFER;
		return c;
	}
	return m_string[m_index++];
}

void Tokenizer::put_back(char c) {
	m_char_buffer = c;
}

bool is_whitespace(char c) {
	return c == ' ' || c == '\r' || c == '\t' || c == '\n' || c == 0x0 || c == ',';
}

void Tokenizer::parse_comment() {
	char c = next_char();
	if (c == ';') {
		while (c != '\n') {
			c = next_char();
		}
	}
	put_back(c);
}

void Tokenizer::parse_whitespace() {
	char c = next_char();
	while (is_whitespace(c) && m_index < m_string.size()) {
		c = next_char();
	}
	put_back(c);
}

std::string Tokenizer::parse_word() {
	std::string word;
	char c = next_char();
	while (!is_whitespace(c)) {
		word += c;
		c = next_char();
	}
	if (c && !is_whitespace(c)) {
		word += c;
	}
	return word;
}

void Tokenizer::tokenize(std::string str) {
	m_string = str;
	m_index = 0;
	m_tokens.clear();
	while (m_index < m_string.size()) {
		parse_whitespace();
		parse_comment();
		std::string word = parse_word();
		if (word.size() > 0) {
			m_tokens.push_back(word);
		}
	}
}

std::vector<std::string>* Tokenizer::tokens() {
	return &m_tokens;
}

uint8_t Tokenizer::parse_reg_unsafe(std::string s) {
	if (s == "ax") { return VirtualMachine::REGISTERS::AX; }
	else if (s == "bx") { return VirtualMachine::REGISTERS::BX; }
	else if (s == "cx") { return VirtualMachine::REGISTERS::CX; }
	else if (s == "dx") { return VirtualMachine::REGISTERS::DX; }
	else if (s == "sp") { return VirtualMachine::REGISTERS::SP; }
	else if (s == "sb") { return VirtualMachine::REGISTERS::SB; }
	else if (s == "bp") { return VirtualMachine::REGISTERS::BP; }
	else if (s == "ip") { return VirtualMachine::REGISTERS::IP; }
	else return 0x00;
}

uint8_t Tokenizer::parse_reg(std::string s) {
	if (s == "ax") { return VirtualMachine::REGISTERS::AX; }
	else if (s == "bx") { return VirtualMachine::REGISTERS::BX; }
	else if (s == "cx") { return VirtualMachine::REGISTERS::CX; }
	else if (s == "dx") { return VirtualMachine::REGISTERS::DX; }
	else if (s == "sp") { return VirtualMachine::REGISTERS::SP; }
	else if (s == "sb") { return VirtualMachine::REGISTERS::SB; }
	else if (s == "bp") { return VirtualMachine::REGISTERS::BP; }
	else if (s == "ip") { return VirtualMachine::REGISTERS::IP; }
	else { throw std::invalid_argument("invalid register"); }
}

uint16_t Tokenizer::read_arg_uint16(int index) {
	std::string x = (*this->tokens())[index];
	if (x.substr(0, 2) == "0x") {
		std::stringstream ss; // this is in hex
		uint16_t res;
		ss << std::hex << x.substr(2, x.size() - 1);
		ss >> res;
		return res;
	}
	else if (x.size() == 3 && x[0] == '\'') {
		return (uint16_t)x[1]; // this is a char
	}
	else {
		return std::stoi(x) & 0xFFFF; // this is a number
	}
}

uint16_t Tokenizer::read_arg_uint16(std::string x) {
	if (x.substr(0, 2) == "0x") {
		std::stringstream ss; // this is in hex
		uint16_t res;
		ss << std::hex << x.substr(2, x.size() - 1);
		ss >> res;
		return res;
	}
	else if (x.size() == 3 && x[0] == '\'') {
		return (uint16_t)x[1]; // this is a char
	}
	else {
		return std::stoi(x) & 0xFFFF; // this is a number
	}
}

