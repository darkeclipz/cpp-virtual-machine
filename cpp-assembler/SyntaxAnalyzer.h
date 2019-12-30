#pragma once
#include "LexicalAnalyzer.h";
#include "Node.h"

class SyntaxAnalyzer
{
private:
	LexicalAnalyzer* lexer;
	Node root;
private:
	Node* generate_abstract_syntax_tree();
public:
	SyntaxAnalyzer(LexicalAnalyzer& lexer);
	~SyntaxAnalyzer();
	Node* syntax_tree();
};

