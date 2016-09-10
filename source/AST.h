#pragma once
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "TNode.h"



class AST {
private:
	
	AST();

public:
	static AST& getInstance() {
		static AST instance;
		return instance;
	}

	AST(AST const&) = delete;
	void operator=(AST const&) = delete;

	TNode* rootNode;

	TNode* createEntityNode(TNode* parent, NodeType type, std::string value);
	TNode* createConstantNode(TNode* parent, NodeType type, int value);
	void prin(std::string s);
};
