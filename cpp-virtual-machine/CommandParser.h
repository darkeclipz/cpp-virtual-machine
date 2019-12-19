#pragma once
#include "Tokenizer.h"
#include <sstream>

class CommandParser : public Tokenizer {
public:
	void check_argument_count(int n);
	std::string read_arg_string(int index);
	uint8_t read_arg_uint8(int index);
};