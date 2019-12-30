#include <iostream>
#include "../cpp-virtual-machine/VirtualMachine.cpp"
#include "SourceFile.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "CodeGenerator.h"

int main() {
	SourceFile source("..\\x64\\Debug\\print_hearts.vasm");
	LexicalAnalyzer lexer(source);
	SyntaxAnalyzer syntax(lexer);
	CodeGenerator generator(syntax);

	while (!generator.end()) {
		Node node = generator.next_node();
		std::cout << node.token()->str() << '\n';
	}
}