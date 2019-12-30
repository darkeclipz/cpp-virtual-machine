#pragma once
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class SourceFile
{
private:
	char buffered_char;
	std::string current_line;
	int line_index;
	std::ifstream file;
	bool closed = false;
public:
	SourceFile(std::string file_path);
	~SourceFile();
	char next_char();
	void next_line();
	void put_back(char c);
	bool end();
};