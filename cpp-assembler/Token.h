#pragma once
#include <string>

class Token {
public:
	enum class TOKEN_TYPE {
		MNEMONIC,
		CONST,
		REG,
		MEM,
		PTR,
		IDENTIFIER,
		STRING,
		PREPROCESS
	};

private:
	TOKEN_TYPE m_type;
	std::string m_value;

public:
	Token(TOKEN_TYPE type, std::string value);
};