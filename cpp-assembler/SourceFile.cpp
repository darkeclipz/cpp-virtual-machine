#include "SourceFile.h"
#include <iostream>

SourceFile::SourceFile(std::string file_path) {
	line_index = 0;
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
	if (file.eof()) {
		file.close();
		closed = true;
		return true;
	}
	return false;
}

void SourceFile::next_line() {
	std::string line;
	file >> line;
	current_line = line;
}

char SourceFile::next_char() {
	if (line_index > current_line.size()) {
		next_line();
		line_index = 0;
	}

	return current_line[line_index++];
}

void SourceFile::put_back(char c) {
	buffered_char = c;
}

