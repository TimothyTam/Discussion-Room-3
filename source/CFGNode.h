#pragma once

#include <set>
#include <vector>

#include "TNode.h"

class CFGNode {
public:
	int statementNumber;
	NodeType type;
	std::vector<CFGNode*> from;
	std::vector<CFGNode*> to;
	std::vector<CFGNode*> end;
	bool isEnd;

	CFGNode(int statementNumber, NodeType type);
};