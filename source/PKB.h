#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

typedef short PROC;

class TNode;
enum NodeType;

class VarTable;

class PKB {
public:
	VarTable* varTable; 
	int setProcToAST(PROC p, TNode* r);
	TNode* getRootAST(PROC p);
	TNode* createEntityNode(TNode* parent, NodeType type, string value);
	TNode* createConstantNode(TNode* parent, NodeType type, int value);
	int addStatement(string statement, TNode* node);
};