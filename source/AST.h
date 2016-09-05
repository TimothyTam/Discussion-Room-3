#pragma once
#include <iostream>
#include <stdio.h>
#include <string>

class TNode;
enum class NodeType;

class AST {
private:
	
	AST();

public:
	static AST& getInstance() {
		static AST instance;
		return instance;
	}
	TNode* rootNode;

	TNode* createEntityNode(TNode* parent, NodeType type, std::string value);
	TNode* createConstantNode(TNode* parent, NodeType type, int value);

};
