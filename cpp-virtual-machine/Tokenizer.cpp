#include "Tokenizer.h"

const char EMPTY_BUFFER = 0x00;

Tokenizer::Tokenizer() {
	m_script = "";
	m_index = 0;
	m_tokens = {};
	m_char_buffer = EMPTY_BUFFER;
}

Tokenizer::~Tokenizer() {
	m_script = "";
	m_tokens.clear();
}

char Tokenizer::next_char() {
	if (m_char_buffer != EMPTY_BUFFER) {
		char c = m_char_buffer;
		m_char_buffer = EMPTY_BUFFER;
		return c;
	}
	return m_script[m_index++];
}

void Tokenizer::put_back(char c) {
	m_char_buffer = c;
}

bool is_whitespace(char c) {
	return c == ' ' || c == '\r' || c == '\t' || c == '\n';
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
	while (is_whitespace(c)) {
		c = next_char();
	}
	put_back(c);
}

std::string Tokenizer::parse_word() {
	std::string word;
	char c = next_char();
	while (!is_whitespace(c) && c != ',') {
		word += c;
		c = next_char();
	}
	if (c) {
		word += c;
	}
	return word;
}

void Tokenizer::tokenize(std::string script) {
	m_script = script;
	m_index = 0;
	m_tokens.clear();
	while (m_index < m_script.size()) {
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