#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef short PROC;

class TNode;
class EntityNode;
class ConstantNode;
enum NodeType;

class VarTable;  // no need to #include "VarTable.h" as all I need is pointer

class PKB {
public:
	static VarTable* varTable; 
	static int setProcToAST(PROC p, TNode* r);
	static TNode* getRootAST(PROC p);
	static EntityNode* createEntityNode(TNode* parent, NodeType type, string value);
	static ConstantNode* createConstantNode(TNode* parent, NodeType type, int value);
	static int addStatement(string statement, TNode* node);
};