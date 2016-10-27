#include "CFG.h"

using namespace std;

CFGNode* CFG::getRootNode(int procIndex) {
	unordered_map<int, CFGNode*>::iterator p = roots.find(procIndex);
	if (p != roots.end()) {
		return p->second;
	}
	return nullptr;
}

CFGNode* CFG::getNodeFromStatement(int statementNumber) {
	unordered_map<int, CFGNode*>::iterator p = allNodes.find(statementNumber);
	if (p != allNodes.end()) {
		return p->second;
	}
	return nullptr;
}

void CFG::addProcedure(int procIndex) {
	roots.insert(pair<int, CFGNode*>(procIndex, NULL));
	currentProcIndex = procIndex;
}

CFGNode* CFG::addStatement(int statementNumber, NodeType type, CFGNode* from) {
	CFGNode* node = new CFGNode(statementNumber, type);
	if (from == NULL) {
		roots[currentProcIndex] = node;
	} else {
		if (from->isEnd && !from->end.empty()) {
			for (auto const& n : from->end) {
				node->from.push_back(n);
				n->to.push_back(node);
			}
		} else {
			node->from.push_back(from);
			from->to.push_back(node);
		}
	}
	allNodes[statementNumber] = node;
	return node;
}