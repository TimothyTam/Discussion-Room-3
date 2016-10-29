#include "Affect.h"

vi Affect::getAffectSpecificGeneric(int lineNo) {
	vi result;
	PKB& pkb = PKB::getInstance();

	CFGNode* node = CFG::getInstance().getNodeFromStatement(lineNo);
	if (node == nullptr || node->type != NodeType::Assign) {
		return result;
	}

	calculateAffectSpecificGeneric(node);

	for (pair<int, int> pair : affect[lineNo]) {
		if (pair.second == 1) {
			result.push_back(pair.first);
		}
	}
	return result;
}

void Affect::calculateAffectSpecificGeneric(CFGNode* node) {
	int lineNo = node->statementNumber;

	// Not calculated before.
	if (affect.count(lineNo) == 0) {
		PKB& pkb = PKB::getInstance();
		unordered_map<int, int> stmtsUsedMap;
		//Find what it modifies (Only 1 result, guaranteed assign statement, checked above)
		int modified = pkb.getModifySpecificGeneric(lineNo, NodeType::StmtLst)[0];

		//Find statements that uses it
		vi stmtUses = pkb.getUsesGenericSpecific(modified, NodeType::Assign);

		for (int use : stmtUses) {
			if (pkb.areInSameProc(use, lineNo) && pkb.whetherTransitiveNext(lineNo, use)) {
				stmtsUsedMap[use] = 1;
			}
		}

		if (stmtsUsedMap.empty()) {
			affect[lineNo];
			return;
		}

		//Go and DFS till stmtsUsedMap become empty.
		unordered_map<CFGNode*, int> visited;

		for (CFGNode* child : node->to) {
			depthFirstSearchAffectSpecificGeneric(child, stmtsUsedMap, modified, visited, lineNo);
		}
	}
}

void Affect::depthFirstSearchAffectSpecificGeneric(CFGNode* current, unordered_map<int, int>& stmtsUsedMap, int modified, unordered_map<CFGNode*,int>& visited, int lineNo) {
	PKB& pkb = PKB::getInstance();

	if (visited.count(current) == 1 || stmtsUsedMap.empty()) {
		return;
	}

	visited[current] = 1;

	if (current->type == NodeType::Assign) {
		if (stmtsUsedMap.count(current->statementNumber)) {
			//insert to affect table
			affect[lineNo][current->statementNumber] = 1;
			affectReverse[current->statementNumber][lineNo] = 1;
			stmtsUsedMap.erase(current->statementNumber);
			if (stmtsUsedMap.empty()) return;
		}

		if (pkb.whetherStmtModifies(current->statementNumber, modified)) {
			return;
		}
	}
	else if (current->type == NodeType::Call) {
		if (pkb.whetherStmtModifies(current->statementNumber, modified)) {
			return;
		}
	}

	for (CFGNode* child : current->to) {
		depthFirstSearchAffectSpecificGeneric(child, stmtsUsedMap, modified, visited, lineNo);
	}
}

vi Affect::getAffectGenericSpecific(int lineNo) {
	vi result;
	PKB& pkb = PKB::getInstance();

	CFGNode* node = CFG::getInstance().getNodeFromStatement(lineNo);
	if (node == nullptr || node->type != NodeType::Assign) {
		return result;
	}

	// Not calculated before.
	if (affectReverseCalculated.count(lineNo) == 0) {
		//Find what it uses
		vi used = pkb.getUsesSpecificGeneric(lineNo, NodeType::Assign);
		
		//Find statements that modifies what is used.
		for (int use : used) {
			vi modifiedTemp = pkb.getModifyGenericSpecific(use, NodeType::Assign);

			//DFS them all. 
			//(Temporary solution till I figure out how to do Affect backwards without copying &visited every fork(exponential))
			//(Or a method that don't require the same amount of DFS backwards)
			for (int modifiedStmt : modifiedTemp) {
				if (pkb.areInSameProc(modifiedStmt, lineNo) && pkb.whetherTransitiveNext(modifiedStmt, lineNo)) {
					calculateAffectSpecificGeneric(CFG::getInstance().getNodeFromStatement(modifiedStmt));
				}
			}
		}
		affectReverseCalculated[lineNo] = 1;
	}

	for (pair<int, int> pair : affectReverse[lineNo]) {
		if (pair.second == 1) {
			result.push_back(pair.first);
		}
	}
	return result;
}

vp_i_i Affect::getAffectGenericGeneric() {
	PKB& pkb = PKB::getInstance();
	vp_i_i result;
	
	for (int i : pkb.getAllEntityIndex(NodeType::Assign)) {
		calculateAffectSpecificGeneric(pkb.getCFGNodeFromStatement(i));
	}

	//get everything
	for (pair<int,unordered_map<int,int>> first : affect) {
		for (pair<int, int> second : first.second) {
			result.push_back(make_pair(first.first, second.first));
		}
	}
	return result;
}

bool Affect::whetherAffect(int line1, int line2) {
	if (affect.count(line1)) {
		return (affect[line1].count(line2) == 1);
	} else if (affectReverse.count(line2)) {
		return (affectReverse[line2].count(line1) == 1);
	}
	else {
		//Calculate it.
		CFGNode* node = PKB::getInstance().getCFGNodeFromStatement(line1);
		if (node != nullptr) {
			calculateAffectSpecificGeneric(node);
			return (affect[line1].count(line2) == 1);
		}
	}
	return false;
}

void Affect::newQuery() {
	//Reset all tables.
	affect.clear();
	affectReverse.clear();
	affectReverseCalculated.clear();
}

//Affect*(1,s1)
//Precondition : None. lineNo can be anything.
//Postcondition:
//Return value: Vector of statement numbers s1 that meets the condition Affects*(lineNo, s1);
// Do not use Affect to calculate Affect*. vi() == vector<int>
vi Affect::getTransitiveAffectSpecificGeneric(int lineNo) {
	return vi();
}

//Affect*(s1,1)
//Refer to above, opposite.
vi Affect::getTransitiveAffectGenericSpecific(int lineNo) {
	return vi();
}

//Affect*(s1,s2)
//Returns all statement pairs such that s1 affects s2 <s1,s2>.
// vp_i_i == vector<pair<int,int>>
vp_i_i Affect::getTransitiveAffectGenericGeneric() {
	return vp_i_i();
}

//Affect*(1,2)
//Returns true or false depending on whether lineNo affects* lineNo2
//Remember to do Affect(s1,s2) above.
bool Affect::whetherTransitiveAffect(int lineNo, int lineNo2) {
	return false;
}