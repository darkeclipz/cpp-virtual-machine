#include "CommandParser.h"

void CommandParser::check_argument_count(int n) {
	if (this->tokens()->size() == n) {
		return;
	}
	throw std::invalid_argument("expected " + std::to_string(n - 1) + " args");
}

std::string CommandParser::read_arg_string(int index) {
	return (*this->tokens())[index];
}

uint8_t CommandParser::read_arg_uint8(int index) {
	return read_arg_uint16(index) & 0x00FF;
}
