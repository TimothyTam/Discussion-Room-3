#include "CFG.h"

using namespace std;

CFGNode* CFG::getRootNode(int procIndex) {
	map<int, CFGNode*>::iterator p = roots.find(procIndex);
	if (p != roots.end()) {
		return p->second;
	}
	return NULL;
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
				node->from.push_back(from);
				from->to.push_back(node);
			}
		} else {
			node->from.push_back(from);
			from->to.push_back(node);
		}
	}
	return node;
}