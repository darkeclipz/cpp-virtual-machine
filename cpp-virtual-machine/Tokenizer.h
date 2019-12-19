#pragma once
#include <string>
#include <vector>

class Tokenizer
{
private:
	int m_index;
	std::string m_string;
	std::vector<std::string> m_tokens;
	char m_char_buffer;
public:
	Tokenizer();
	~Tokenizer();
	char next_char();
	void put_back(char c);
	void tokenize(std::string script);
	void parse_whitespace();
	void parse_comment();
	std::string parse_word();
	std::vector<std::string>* tokens();
	uint8_t parse_reg(std::string str);
	uint8_t parse_reg_unsafe(std::string str);
	uint16_t read_arg_uint16(int index);
	uint16_t read_arg_uint16(std::string);
};

