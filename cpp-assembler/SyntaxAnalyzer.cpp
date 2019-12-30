#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer& lexer) {
	this->lexer = &lexer;
	this->root = *generate_abstract_syntax_tree();
}

SyntaxAnalyzer::~SyntaxAnalyzer() {
	this->lexer = nullptr;
	for (auto& n : this->root.children()) {
		delete &n;
	}
}

Node* SyntaxAnalyzer::generate_abstract_syntax_tree() {
	Token tok;
	while (!lexer->end()) {
		lexer->next_token(tok);
		Node* n = new Node();
		root.add_node(*n, tok);
	}
	return &this->root;
}

Node* SyntaxAnalyzer::syntax_tree() {
	return &root;
}