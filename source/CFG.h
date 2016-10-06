#pragma once

#include <map>
#include "CFGNode.h"

class CFG {
private:
	CFG() {};
	std::map<int, CFGNode*> roots;
	std::map<int, CFGNode*> allNodes;

	int currentProcIndex;

public:
	static CFG& getInstance() {
		static CFG instance;
		return instance;
	}

	CFG(CFG const&) = delete;
	void operator=(CFG const&) = delete;

	CFGNode* getRootNode(int procIndex);
	CFGNode* getNodeFromStatement(int statementNumber);

	void addProcedure(int procIndex);
	CFGNode* addStatement(int statementNumber, NodeType type, CFGNode* from);
};