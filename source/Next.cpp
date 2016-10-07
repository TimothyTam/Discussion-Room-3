#include "Next.h"

void Next::generateNextTable() {
	PKB& inst = PKB::getInstance();
	int numOfProcs = inst.getProcTableSize();
	for (int i = 0; i < numOfProcs; i++) {
		updateNextTable(inst.getCFGRootNode(i));
		//updateAllStmtPairsTable(inst.getCFGRootNode[i]);
	}
	buildReverseTable();

	buildStmtPairs();

	//Create 2d Array to Hold Next* information.
	int tableSize = PKB::getInstance().getStmtCount();

	int** arr = new int*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		arr[i] = new int[tableSize];
	}

	newQuery();
}

void Next::updateNextTable(CFGNode* current) {
	vi nextStmts;
	for (CFGNode* i : current->to) {
		nextStmts.push_back(i->statementNumber);
		updateNextTable(i);
	}
	next[current->statementNumber] = nextStmts;
}

void Next::buildReverseTable() {
	map_i_si nextStmtsSet;
	map_i_vi::iterator it;
	map_i_si::iterator itSet;


	for (it = next.begin(); it != next.end(); it++) {
		for (int j : it->second) {
			nextStmtsSet[j].insert(it->first);
		}
	}

	for (itSet = nextStmtsSet.begin(); itSet != nextStmtsSet.end(); itSet++) {
		nextReverse[itSet->first].assign(itSet->second.begin(),
			itSet->second.end());
	}
}

void Next::buildStmtPairs() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (int i = 0; i < 4; i++) {
		stmtPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtPairs[i].push_back(vp_i_i());
		}
	}

	for (it = next.begin(); it != next.end(); it++) {
		NodeType type1 = inst.getStmt(it->first).second->type;
		int location1 = -1;
		int location2 = -1;

		location1 = getLocationOfStmt(type1);

		for (int i : it->second) {
			NodeType type2 = inst.getStmt(i).second->type;
			location2 = getLocationOfStmt(type2);

			_ASSERT((location1 != -1 || location2 != -1) && "Failed to build Statement Pairs in Next.");

			stmtPairs[location1][location2].push_back(make_pair(it->first, i));
		}
	}
}
/*
void Next::updateAllStmtPairsTable(CFGNode* current) {
	for (CFGNode* next : current->to) {
		updateAllStmtPairsTableSingleNode(next, current);
		updateAllStmtPairsTable(next);
	}
}

void Next::updateAllStmtPairsTableSingleNode(CFGNode* current, CFGNode* original) {
	
	NodeType type1 = original->type;
	NodeType type2 = current->type;
	int location1 = getLocationOfStmt(type1);
	int location2 = getLocationOfStmt(type2);
	
	_ASSERT((location1 != -1 || location2 != -1) && "Failed to build ALL Statement Pairs in Next.");

	stmtPairs[location1][location2].push_back(make_pair(original->statementNumber, current->statementNumber));
	
	if (current == original) {
		return;
	}

	for (CFGNode* next : current->to) {
		updateAllStmtPairsTableSingleNode(next, original);
	}
}*/

int Next::getLocationOfStmt(NodeType type) {
	if (type == NodeType::Assign) {
		return 0;
	}
	else if (type == NodeType::While) {
		return 1;
	}
	else if (type == NodeType::If) {
		return 2;
	}
	else if (type == NodeType::Call) {
		return 3;
	}
	return -1;
}

vp_i_i Next::getNextGenericGeneric(NodeType typeA, NodeType typeB) {
	bool allFirst = false;
	bool allSecond = false;
	int location1 = -1;
	int location2 = -1;

	if (typeA == NodeType::StmtLst) {
		allFirst = true;
	}
	if (typeB == NodeType::StmtLst) {
		allSecond = true;
	}

	//Both looking for unique type. Not all statements. Follow(a,w)
	if (!allFirst && !allSecond) {
		location1 = getLocationOfStmt(typeA);
		location2 = getLocationOfStmt(typeB);
		return stmtPairs[location1][location2];
	}

	// Follow(s,w)
	if (allFirst && !allSecond) {
		location2 = getLocationOfStmt(typeB);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), stmtPairs[i][location2].begin(), stmtPairs[i][location2].end());
		}
		return result;
	}
	// Follow(w,s)
	if (!allFirst && allSecond) {
		location1 = getLocationOfStmt(typeA);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), stmtPairs[location1][i].begin(), stmtPairs[location1][i].end());
		}
		return result;
	}

	// Follow(s,s). This translates to all statements with follow.
	vp_i_i result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.insert(result.end(), stmtPairs[i][j].begin(), stmtPairs[i][j].end());
		}
	}
	return result;
}

//Next(11,a)
vi Next::getNextSpecificGeneric(int lineNo, NodeType type) {
	PKB& pkb = PKB::getInstance();
	vi result;
	vi temp = next.count(lineNo) == 1 ? next[lineNo] : vi();
	for (int stmt : temp) {
		if (type == pkb.getStmt(stmt).second->type) {
			result.push_back(stmt);
		}
	}
	return result;
}

//Next(a,11)
vi Next::getNextGenericSpecific(int lineNo, NodeType type) {
	PKB& pkb = PKB::getInstance();
	vi result;
	vi temp = nextReverse.count(lineNo) == 1 ? nextReverse[lineNo] : vi();
	for (int stmt : temp) {
		if (type == pkb.getStmt(stmt).second->type) {
			result.push_back(stmt);
		}
	}
	return result;
}

bool Next::whetherNext(int a, int b) {
	vi stmts;
	if (nextReverse.count(b) == 1) {
		stmts = nextReverse[b];
		return (std::find(stmts.begin(), stmts.end(), a) != stmts.end());
	}
	return false;
}

void Next::buildTransitiveTable() {
	PKB& inst = PKB::getInstance();
	int tableSize = inst.getStmtCount();

	for (int i = 0; i < tableSize; i++) {
		vi to = next[i];
		for (int j = 0; j < to.size(); j++) {
			arr[i][to.at(j)] = 1;
		}
	}

	// Run Floyd Warshall Algorithm
	for (int k = 0; k < tableSize; k++) {
		for (int i = 0; i < tableSize; i++) {
			for (int j = 0; j < tableSize; j++) {
				arr[i][j] = arr[i][j] || (arr[i][k] && arr[k][j]);
			}
		}
	}

	// Store to stmtPairs table
	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			if (arr[i][j] == 1) {
				NodeType type1 = inst.getStmt(i).second->type;
				NodeType type2 = inst.getStmt(j).second->type;
				int location1 = getLocationOfStmt(type1);
				int location2 = getLocationOfStmt(type2);

				stmtTransPairs[location1][location2].push_back(make_pair(i,j));
			}
		}
	}
	isNewQuery = false;
}

//Next*(11,a)
vi Next::getTransitiveNextSpecificGeneric(int lineNo, NodeType type) {
	NodeType specificType;
	int specificLocation;
	vi result;

	//Out of range.
	if (lineNo < 1 || lineNo >= PKB::getInstance().getStmtCount()) {
		return vi();
	}

	specificType = PKB::getInstance().getStmt(lineNo).second->type;
	specificLocation = getLocationOfStmt(specificType);

	if (isNewQuery) {
		buildTransitiveTable();
	}

	if (type == NodeType::StmtLst) {
		for (int i = 0; i < 4; i++) {
			for (pair<int,int> j : stmtTransPairs[specificLocation][i]) {
				if (j.first == lineNo) {
					result.push_back(j.second);
				}
			}
		}
		return result;
	}
	
	int location2 = getLocationOfStmt(type);

	for (pair<int, int> j : stmtTransPairs[specificLocation][location2]) {
		if (j.first == lineNo) {
			result.push_back(j.second);
		}
	}
	return result;
}

//Next(a,11)
vi Next::getTransitiveNextGenericSpecific(int lineNo, NodeType type) {
	NodeType specificType;
	int specificLocation;
	vi result;

	//Out of range.
	if (lineNo < 1 || lineNo >= PKB::getInstance().getStmtCount()) {
		return vi();
	}

	specificType = PKB::getInstance().getStmt(lineNo).second->type;
	specificLocation = getLocationOfStmt(specificType);

	if (isNewQuery) {
		buildTransitiveTable();
	}

	if (type == NodeType::StmtLst) {
		for (int i = 0; i < 4; i++) {
			for (pair<int, int> j : stmtTransPairs[i][specificLocation]) {
				if (j.first == lineNo) {
					result.push_back(j.second);
				}
			}
		}
		return result;
	}

	int location2 = getLocationOfStmt(type);

	for (pair<int, int> j : stmtTransPairs[location2][specificLocation]) {
		if (j.first == lineNo) {
			result.push_back(j.second);
		}
	}
	return result;
}
vp_i_i Next::getTransitiveNextGenericGeneric(NodeType typeA, NodeType typeB) {
	bool allFirst = false;
	bool allSecond = false;
	int location1 = -1;
	int location2 = -1;

	if (isNewQuery) {
		buildTransitiveTable();
	}

	if (typeA == NodeType::StmtLst) {
		allFirst = true;
	}
	if (typeB == NodeType::StmtLst) {
		allSecond = true;
	}

	//Both looking for unique type. Not all statements. Follow(a,w)
	if (!allFirst && !allSecond) {
		location1 = getLocationOfStmt(typeA);
		location2 = getLocationOfStmt(typeB);
		return stmtTransPairs[location1][location2];
	}

	// Next*(s,w)
	if (allFirst && !allSecond) {
		location2 = getLocationOfStmt(typeB);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), stmtTransPairs[i][location2].begin(), stmtTransPairs[i][location2].end());
		}
		return result;
	}

	// Next*(w,s)
	if (!allFirst && allSecond) {
		location1 = getLocationOfStmt(typeA);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), stmtTransPairs[location1][i].begin(), stmtTransPairs[location1][i].end());
		}
		return result;
	}

	// Next*(s,s). This translates to all statements with Next*.
	vp_i_i result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.insert(result.end(), stmtTransPairs[i][j].begin(), stmtTransPairs[i][j].end());
		}
	}
	return result;
}
bool Next::whetherTransitivelyNext(int a, int b) {
	if (isNewQuery) {
		buildTransitiveTable();
	}
	PKB& inst = PKB::getInstance();
	int stmtCount = inst.getStmtCount();

	//Out of range.
	if (a < 1 || a >= stmtCount || b < 1 || b >= stmtCount) {
		return false;
	}

	int location1 = getLocationOfStmt(inst.getStmt(a).second->type);
	int location2 = getLocationOfStmt(inst.getStmt(b).second->type);

	for (pair<int, int> i : stmtTransPairs[location1][location2]) {
		if (i.first == a && i.second == b) {
			return true;
		}
	}
	return false;
}


void Next::newQuery() {
	int tableSize = PKB::getInstance().getStmtCount();
	isNewQuery = true;

	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			arr[i][j] = 0;
		}
	}

	stmtTransPairs.clear();
	for (int i = 0; i < 4; i++) {
		stmtTransPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtTransPairs[i].push_back(vp_i_i());
		}
	}
}