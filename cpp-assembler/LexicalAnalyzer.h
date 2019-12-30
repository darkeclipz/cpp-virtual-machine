#pragma once
#include "Token.h"

class LexicalAnalyzer
{
private:
	void open_file_stream();

public:
	LexicalAnalyzer();
	~LexicalAnalyzer();
	Token next_token();
	bool empty();
};

