#pragma once

#include <vector>

enum class NodeType { Program, Procedure, StmtLst, Assign, Variable, Plus, Minus, Times, Constant, While, If, Call, Invalid };

class TNode {
public:
	TNode* parent;
	std::vector<TNode*> childs;
	NodeType type;
	int value;
	int statementNumber;

	TNode();
	//The constructor takes just the NodeType, then we can change the TNode's variables directly
	TNode(NodeType t);
};