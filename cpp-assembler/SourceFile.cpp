#include "SourceFile.h"
#include <iostream>

SourceFile::SourceFile() {
	index = 0;
	buffered_char = 0;
}

SourceFile::SourceFile(std::string file_path) {
	index = 0;
	buffered_char = 0;
	file.open(file_path);
	if (!file) {
		throw std::invalid_argument("file not found.");
	}
}

SourceFile::~SourceFile() {
	if (!closed) {
		file.close();
	}
}

bool SourceFile::end() {
	if (file.eof() && index >= current_line.size()) {
		closed = true;
		file.close();
		return true;
	}
	return false;
}

void SourceFile::next_line() {
	std::string line;
	std::getline(file, line);
	current_line = line + '\n';
}

char SourceFile::next_char() {

	if (buffered_char) {
		char ch = buffered_char;
		buffered_char = 0;
		return ch;
	}

	if (index >= current_line.size()) {
		next_line();
		index = 0;
	}

	return current_line[index++];
}

void SourceFile::put_back(char c) {
	buffered_char = c;
}

