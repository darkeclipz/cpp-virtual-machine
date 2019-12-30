#include "Token.h"

Token::Token() {
	m_type = TOKEN_TYPE::CONSTANT;
}

Token::Token(TOKEN_TYPE type, std::string val) {
	m_type = type;
	m_value = val;
}

std::string Token::str() {
	return type_to_str(this->m_type) + "(" + this->m_value + ")";
}

std::string Token::type_to_str(TOKEN_TYPE type) {
	switch (type) {
	case TOKEN_TYPE::CONSTANT: return "constant";
	case TOKEN_TYPE::IDENTIFIER: return "identifier";
	case TOKEN_TYPE::MEM: return "mem";
	case TOKEN_TYPE::MNEMONIC: return "mnemonic";
	case TOKEN_TYPE::PREPROCESS: return "preprocess";
	case TOKEN_TYPE::PTR: return "ptr";
	case TOKEN_TYPE::REG: return "reg";
	case TOKEN_TYPE::STRING: return "string";
	case TOKEN_TYPE::LABEL: return "label";
	case TOKEN_TYPE::FUNCTION: return "function";
	case TOKEN_TYPE::CHAR: return "char";
	default: return "undefined";
	}
}