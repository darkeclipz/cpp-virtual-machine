#include "LexicalAnalyzer.h"
#include "sstream";
#include "../cpp-utils/utils.cpp"

LexicalAnalyzer::LexicalAnalyzer(SourceFile& source) {
	this->vm = new VirtualMachine(1);
	this->source = &source;
	init_mnemonic_table();
	init_symbol_table();
	init_function_table();
	init_register_table();
}

LexicalAnalyzer::~LexicalAnalyzer() {
	this->source = nullptr;
	delete this->vm;
}

void LexicalAnalyzer::init_mnemonic_table() {
	// Mnemonic identifiers should be capital cased.
	for (auto& instruction : vm->lookup) {
		std::string mnemonic = to_upper(instruction.mnemonic);
		if (!mnemonic_table.count(mnemonic)) {
			mnemonic_table[mnemonic] = true;
		}
	}
}

void LexicalAnalyzer::init_function_table() {
	// Function identifiers must be capital cased.
	function_table["DB"] = true;
}

void LexicalAnalyzer::init_symbol_table() {
	// Symbol identifiers must be capital cased.
	symbol_table["$"] = 0;
}

void LexicalAnalyzer::init_register_table() {
	// Register identifiers must be capital cased.
	register_table["AX"] = VirtualMachine::REGISTERS::AX;
	register_table["BX"] = VirtualMachine::REGISTERS::BX;
	register_table["CX"] = VirtualMachine::REGISTERS::CX;
	register_table["DX"] = VirtualMachine::REGISTERS::DX;
	register_table["SP"] = VirtualMachine::REGISTERS::SP;
	register_table["SB"] = VirtualMachine::REGISTERS::SB;
	register_table["BP"] = VirtualMachine::REGISTERS::BP;
	register_table["IP"] = VirtualMachine::REGISTERS::IP;
}

bool LexicalAnalyzer::end() {
	return source->end();
}

bool LexicalAnalyzer::is_alphanumeric(char ch) {
	return ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '$'; // $ is current address.
}

bool LexicalAnalyzer::is_digit(char ch) {
	return ch >= '0' && ch <= '9';
}

void LexicalAnalyzer::read_whitespace() {
	char ch = source->next_char();
	while (is_whitespace(ch)) {
		ch = source->next_char();
	}
	source->put_back(ch);
}

void LexicalAnalyzer::read_comment() {
	char ch = source->next_char();
	if (ch == ';') {
		while (ch != '\n') {
			ch = source->next_char();
		}
	}
	source->put_back(ch);
}

bool LexicalAnalyzer::is_whitespace(char ch) {
	switch (ch) {
	case ' ':
	case '\n':
	case '\r':
	case '\t':
	case ',':
		return true;
	default:
		return false;
	}
}

bool LexicalAnalyzer::is_delimeter(char ch) {
	return ch == ';' || ch == ',';
}

Token::TOKEN_TYPE LexicalAnalyzer::token_type(std::string value) {
	if (value[0] == '%') {
		return Token::TOKEN_TYPE::PREPROCESS;
	}
	else if (register_table.count(to_upper(value))) {
		return Token::TOKEN_TYPE::REG;
	}
	else if (mnemonic_table.count(to_upper(value))) {
		return Token::TOKEN_TYPE::MNEMONIC;
	}
	else if (function_table.count(to_upper(value))) {
		return Token::TOKEN_TYPE::FUNCTION;
	}
	else if (value[0] == '"' && value[value.size() - 1] == '"') {
		return Token::TOKEN_TYPE::STRING;
	}
	else if (value[0] == '\'' && value[value.size() - 1] == '\'') {
		return Token::TOKEN_TYPE::CHAR;
	}
	else if (value[value.size() - 1] == ':') {
		return Token::TOKEN_TYPE::LABEL;
	}
	else if (value[0] == '[' && value[value.size()-1] == ']') {
		value = value.substr(1, value.size() - 2);
		if (is_digit(value[0]) && value.substr(0, 2) == "0x") {
			return Token::TOKEN_TYPE::MEM;
		}
		else if (register_table.count(to_upper(value))) {
			return Token::TOKEN_TYPE::PTR;
		}
		else if (!register_table.count(to_upper(value))) {
			return Token::TOKEN_TYPE::MEM;
		}
	}
	else {
		if (is_alphanumeric(value[0])) {
			return Token::TOKEN_TYPE::IDENTIFIER;
		}
		else if (is_digit(value[0]) || value.substr(0, 2) == "0x") {
			return Token::TOKEN_TYPE::CONSTANT;
		}
	}

	throw std::invalid_argument("Unable to determine token type for " + value);
}

std::string LexicalAnalyzer::read_str() {
	bool escape = false;
	std::stringstream str;
	str << source->next_char();
	char ch = source->next_char();
	for (; !source->end() && (ch != '"' || escape); ch = source->next_char()) {
		escape = ch == '\\';
		str << ch;
	}
	str << ch;
	return str.str();
}

std::string LexicalAnalyzer::read_word() {
	std::stringstream word;
	char ch = source->next_char();
	for (; !source->end() && !is_whitespace(ch) && !is_delimeter(ch); ch = source->next_char()) {
		word << ch;
	}
	source->put_back(ch);
	return word.str();
}

bool LexicalAnalyzer::next_token(Token& tok) {
	if (buffer_full) {
		buffer_full = false;
		tok = buffered_token;
		return true;
	}
	std::string word;
	read_whitespace();
	read_comment();
	char ch = source->next_char();
	source->put_back(ch);
	if (ch == '"') {
		word = read_str();
	}
	else {
		word = read_word();
	}
	if (word.size() > 0) {
		std::string val = word;
		tok = Token(token_type(val), val);
		return true;
	}
	if (!source->end()) {
		return next_token(tok);
	}
	throw std::invalid_argument("an error occured.");
}

void LexicalAnalyzer::put_back(Token tok) {
	buffered_token = tok;
	buffer_full = true;
}