#include "Node.h"
#include <iostream>

Node::Node() {
	this->m_token;
	this->m_parent = nullptr;
	std::cout << "Node created" << std::endl;
}

Node::~Node() {
	this->m_parent = nullptr;
	this->m_children.clear();
	std::cout << "Node deleted" << std::endl;
}

void Node::add_node(Node n, Token token) {
	n.m_parent = this;
	n.m_token = token;
	this->m_children.push_back(n);
}

std::vector<Node> Node::children() {
	return m_children;
}

Token* Node::token() {
	return &m_token;
}
