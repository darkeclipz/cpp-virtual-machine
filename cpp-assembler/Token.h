#pragma once
#include <string>

class Token {
public:
	enum class TOKEN_TYPE {
		MNEMONIC,
		CONSTANT,
		REG,
		MEM,
		PTR,
		IDENTIFIER,
		STRING,
		PREPROCESS,
		LABEL,
		FUNCTION,
		CHAR
	};

private:
	TOKEN_TYPE m_type;
	std::string m_value;
	std::string type_to_str(TOKEN_TYPE type);
public:
	Token();
	Token(TOKEN_TYPE type, std::string value);
	std::string str();
};