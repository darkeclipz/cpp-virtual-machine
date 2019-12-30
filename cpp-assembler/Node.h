#pragma once
#include <vector>
#include "Token.h"

class Node
{
private:
	Node* m_parent = nullptr;
	std::vector<Node> m_children;
	Token m_token;
public:
	Node();
	~Node();
	void add_node(Node n, Token tok);
	std::vector<Node> children();
	Token* token();
};

