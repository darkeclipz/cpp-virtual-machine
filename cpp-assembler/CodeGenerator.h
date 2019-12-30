#pragma once
#include "SyntaxAnalyzer.h"

class CodeGenerator
{
private:
	SyntaxAnalyzer* syntax;
	Node* m_root;
	int m_index;
	int m_nodes;
public:
	CodeGenerator(SyntaxAnalyzer& sa);
	~CodeGenerator();
	Node next_node();
	bool end();
};

