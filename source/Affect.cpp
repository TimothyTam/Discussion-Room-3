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
	} else if (affectReverseCalculated.count(line2)) {
		return (affectReverse[line2].count(line1) == 1);
	}
	else {
		//Calculate it if they same proc.
		PKB& pkb = PKB::getInstance();

		if (!pkb.areInSameProc(line1, line2)) {
			return false;
		}
		
		CFGNode* node = PKB::getInstance().getCFGNodeFromStatement(line1);
		if (node != nullptr && node->type == NodeType::Assign) {
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
	affectTrans.clear();
	affectTransCalculated.clear();
	affectTransReverse.clear();
	affectTransReverseCalculated.clear();
	allAffectCalculated = false;
	allAffectTransCalculated = false;
}

//Affect*(1,s1)
//Precondition : None. lineNo can be anything.
//Postcondition:
//Return value: Vector of statement numbers s1 that meets the condition Affects*(lineNo, s1);
// Do not use Affect to calculate Affect*. vi() == vector<int>
vi Affect::getTransitiveAffectSpecificGeneric(int lineNo) {
	vi result;
	CFGNode* node = CFG::getInstance().getNodeFromStatement(lineNo);
	if (node == nullptr || node->type != NodeType::Assign) {
		return result;
	}
	
	calculateTransitiveAffectSpecificGeneric(node);

	for (pair<int, int> pair : affectTrans[lineNo]) {
		if (pair.second == 1) {
			result.push_back(pair.first);
		}
	}
	
	return result;
}

void Affect::calculateTransitiveAffectSpecificGeneric(CFGNode* node) {
	if (affectTransCalculated.count(node->statementNumber) == 0 && !allAffectTransCalculated) {
		map_i_i modified;
		vi m = PKB::getInstance().getModifySpecificGeneric(node->statementNumber, NodeType::Assign);
		if (!m.empty()) {
			modified[m.at(0)] = node->statementNumber;
		}
		if (!node->to.empty()) {
			affectTransLineCalculated.clear();
			calculateTransitiveAffectSpecificGeneric(node->statementNumber, node->to.at(0), modified, NULL);
		}
		affectTransCalculated[node->statementNumber] = 1;
	}
}

pair<map_i_i, bool> Affect::calculateTransitiveAffectSpecificGeneric(int startLineNo, CFGNode* node, const map_i_i& parentModified, CFGNode* parentNode) {
	map_i_i modified;
	modified.insert(parentModified.begin(), parentModified.end());

	stack<CFGNode*> navi;
	bool updated = false;
	pair<map_i_i, bool> result;

	if (parentNode != NULL) {
		navi.push(parentNode);
	}
	
	PKB& pkb = PKB::getInstance();

	while (node != NULL) {
		if (!navi.empty()) {
			if (navi.top()->type == NodeType::While && navi.top() == node) {
				if (!updated) {
					result.first = modified;
					result.second = updated;
					return result;
				} else {
					updated = false;
					node = node->to.at(0);
					continue;
				}
			} else if (navi.top()->type == NodeType::If && node->from.size() > 1) {
				if (node->type != NodeType::While || node->statementNumber < navi.top()->statementNumber) {
					result.first = modified;
					result.second = updated;
					return result;
				}
			}
		}	
		if (node->type == NodeType::While) {
			pair<map_i_i, bool> modifiedWhile = calculateTransitiveAffectSpecificGeneric(startLineNo, node->to.at(0), modified, node);
			modified.insert(modifiedWhile.first.begin(), modifiedWhile.first.end());
			if (modifiedWhile.second) {
				updated = true;
			}
		} else if (node->type == NodeType::If) {
			pair<map_i_i, bool> modifiedIf = calculateTransitiveAffectSpecificGeneric(startLineNo, node->to.at(0), modified, node);
			pair<map_i_i, bool> modifiedElse = calculateTransitiveAffectSpecificGeneric(startLineNo, node->to.at(1), modified, node);
			modified.clear();
			modified.insert(modifiedIf.first.begin(), modifiedIf.first.end());
			modified.insert(modifiedElse.first.begin(), modifiedElse.first.end());
			if (modifiedIf.second || modifiedElse.second) {
				updated = true;
			}
			node = node->end.at(0);
		} else if (node->type == NodeType::Call) {
			vi m = pkb.getModifySpecificGeneric(node->statementNumber, NodeType::Call);
			for (auto const& v : m) {
				modified.erase(v);
			}
		} else if (node->type == NodeType::Assign) {
			vi m = pkb.getModifySpecificGeneric(node->statementNumber, NodeType::Assign);
			if (affectTrans[startLineNo].count(node->statementNumber) == 0) {
				for (auto const& pair : modified) {
					if (pkb.whetherStmtUses(node->statementNumber, pair.first)) {
						affectTrans[startLineNo][node->statementNumber] = 1;
						affectTransReverse[node->statementNumber][startLineNo] = 1;
						
						affectTrans[pair.second][node->statementNumber] = 1;
						affectTransReverse[node->statementNumber][pair.second] = 1;
			
						break;
					}
				}
			}
			bool keep = false;
			if (affectTrans[startLineNo].count(node->statementNumber) > 0) {
				keep = true;
				if (affectTransLineCalculated.count(node->statementNumber) == 0 && modified.count(m.at(0)) == 0) {
					affectTransLineCalculated[node->statementNumber] = 1;
					updated = true;
				}
				modified[m.at(0)] = node->statementNumber;
			}
			if (!keep && node->statementNumber != startLineNo) {
				modified.erase(m.at(0));
			}
		}
		if (node->to.empty()) {
			node = NULL;
		} else if (node->type == NodeType::While) {
			if (node->to.size() > 1) {
				node = node->to.at(1);
			} else {
				node = NULL;
			}
		} else {
			node = node->to.at(0);
		}
	}
	
	result.first = modified;
	result.second = updated;
	return result;
}

//Affect*(s1,1)
//Refer to above, opposite.
vi Affect::getTransitiveAffectGenericSpecific(int lineNo) {

	PKB& pkb = PKB::getInstance();
	vi result;

	//TO-DO: Check if GG is calculated before, 

	if (!(allAffectTransCalculated || affectTransReverseCalculated.count(lineNo))) {
		//Calculate.
		CFGNode* node = CFG::getInstance().getNodeFromStatement(lineNo);


		vi potential = pkb.getTransitiveNextGenericSpecific(lineNo, NodeType::Assign);

		for (int first : potential) {
			calculateTransitiveAffectSpecificGeneric(CFG::getInstance().getNodeFromStatement(first));
		}

		affectTransReverseCalculated[lineNo] = 1;
	}

	for (pair<int, int> pair : affectTransReverse[lineNo]) {
		if (pair.second == 1) {
			result.push_back(pair.first);
		}
	}

	return result;
}

//Affect*(s1,s2)
//Returns all statement pairs such that s1 affects s2 <s1,s2>.
// vp_i_i == vector<pair<int,int>>
vp_i_i Affect::getTransitiveAffectGenericGeneric() {
	PKB& pkb = PKB::getInstance();
	vp_i_i result;

	for (int i : pkb.getAllEntityIndex(NodeType::Assign)) {
		calculateTransitiveAffectSpecificGeneric(pkb.getCFGNodeFromStatement(i));
	}

	allAffectTransCalculated = true;

	//get everything
	for (pair<int, unordered_map<int, int>> first : affectTrans) {
		for (pair<int, int> second : first.second) {
			result.push_back(make_pair(first.first, second.first));
		}
	}
	return result;
}

//Affect*(1,2)
//Returns true or false depending on whether lineNo affects* lineNo2
//Remember to do Affect(s1,s2) above.
bool Affect::whetherTransitiveAffect(int lineNo, int lineNo2) {
	if (affectTransCalculated.count(lineNo)) {
		return (affectTrans[lineNo].count(lineNo2) == 1);
	}
	else if (affectTransReverseCalculated.count(lineNo2)) {
		return (affectTransReverse[lineNo2].count(lineNo) == 1);
	}
	else {
		//Calculate it.
		PKB& pkb = PKB::getInstance();

		if (!pkb.areInSameProc(lineNo, lineNo2)) {
			return false;
		}

		CFGNode* node = pkb.getCFGNodeFromStatement(lineNo);
		if (node != nullptr && node->type == NodeType::Assign) {
			calculateTransitiveAffectSpecificGeneric(node);
			return (affectTrans[lineNo].count(lineNo2) == 1);
		}
	}
	return false;
}
