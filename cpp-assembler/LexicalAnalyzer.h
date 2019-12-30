#pragma once
#include "Token.h"
#include "SourceFile.h"
#include <map>
#include "../cpp-virtual-machine/VirtualMachine.h"

class LexicalAnalyzer
{
private:
	VirtualMachine* vm;
	SourceFile* source;
	Token buffered_token;
	bool buffer_full = false;
	std::map<std::string, bool> mnemonic_table;
	std::map<std::string, uint16_t> symbol_table;
	std::map<std::string, bool> function_table;
	std::map<std::string, uint8_t> register_table;
private:
	void init_mnemonic_table();
	void init_symbol_table();
	void init_function_table();
	void init_register_table();
	bool is_digit(char ch);
	bool is_alphanumeric(char ch);
	bool is_whitespace(char ch);
	bool is_delimeter(char ch);
	void read_whitespace();
	void read_comment();
	std::string read_word();
	std::string read_str();
public:
	LexicalAnalyzer(SourceFile& source);
	~LexicalAnalyzer();
	bool next_token(Token& tok);
	void put_back(Token tok);	
	bool end();
	Token::TOKEN_TYPE token_type(std::string value);
};

