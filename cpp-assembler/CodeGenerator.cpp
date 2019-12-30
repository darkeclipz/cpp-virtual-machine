#include "CodeGenerator.h"

CodeGenerator::CodeGenerator(SyntaxAnalyzer& sa) {
	syntax = &sa;
	m_index = 0;
	m_root = syntax->syntax_tree();
	m_nodes = m_root->children().size();
}

CodeGenerator::~CodeGenerator() {
	syntax = nullptr;
}

bool CodeGenerator::end() {
	return m_index >= m_nodes;
}

Node CodeGenerator::next_node() {
	return m_root->children()[m_index++];
}

