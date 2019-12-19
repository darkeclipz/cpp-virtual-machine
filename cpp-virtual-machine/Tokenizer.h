#pragma once
#include <string>
#include <vector>

class Tokenizer
{
private:
	int m_index;
	std::string m_script;
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
};

