#include "Next.h"

void Next::generateNextTable() {
	PKB& inst = PKB::getInstance();
	int numOfProcs = inst.getProcTableSize();
	for (int i = 0; i < numOfProcs; i++) {
		CFGNode* proc = inst.getCFGRootNode(i);
		updateNextTable(proc);
		resetIsVisitedFlag(proc);
	}
	buildReverseTable();

	build2DArrayTable();

	buildStmtPairs();
	
	for (int i = 0; i < 4; i++) {
		stmtTransPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtTransPairs[i].push_back(vp_i_i());
		}
	}

	newQuery();
}

void Next::updateNextTable(CFGNode* current) {
	vi nextStmts;

	if (current->visited) return;
	current->visited = true;
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
	if (type == NodeType::StmtLst) {
		return temp;
	}
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
	if (type == NodeType::StmtLst) {
		return temp;
	}
	for (int stmt : temp) {
		if (type == pkb.getStmt(stmt).second->type) {
			result.push_back(stmt);
		}
	}
	return result;
}

bool Next::whetherNext(int a, int b) {
	if (a < 1 || a > tableSize || b < 0 || b > tableSize) return false;
	return nextArray[a][b];
}
void Next::buildTransitiveTable() {
	PKB& inst = PKB::getInstance();
	int numOfProcs = inst.getProcTableSize();
	map<CFGNode*, int> visited;
	for (int i = 0; i < numOfProcs; i++) {
		CFGNode* rootNode = inst.getCFGRootNode(i);
		buildTransitiveTableForProcedure(rootNode, visited);
		visited.clear();
	}
	isNewQuery = false;
}

// Fastest Next* for Cyclic Graph. Go to each Node and DFS.
void Next::buildTransitiveTableForProcedure(CFGNode* current, map<CFGNode*, int> &visited) {
	vi nextStmts;
	if (visited.count(current) == 1) {
		return;
	}
	visited[current] = 1;	
	int location = getLocationOfStmt(current->type);


	for (CFGNode* i : current->to) {
		depthFirstSearch(i, current->statementNumber, location);
		resetIsVisitedFlag(i);
	}

	for (CFGNode* i : current->to) {
		buildTransitiveTableForProcedure(i, visited);
	}
}

void Next::resetIsVisitedFlag(CFGNode* current) {
	if (!(current->visited)) {
		return;
	}

	current->visited = false;

	//Go down child. stmtNo won't change.
	for (CFGNode* i : current->to) {
		resetIsVisitedFlag(i);
	}
}

void Next::depthFirstSearch(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode) {
//void Next::depthFirstSearch(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode, map<CFGNode*, int> visited) {
	if (current->visited) {
		return;
	}
	current->visited = true;
	
	//Put into Table
	stmtTransPairs[typeOfStartNode][getLocationOfStmt(current->type)].push_back(make_pair(stmtNoOfStartNode, current->statementNumber));
	nextTrans[stmtNoOfStartNode][current->statementNumber] = 1;
	nextTransReverse[current->statementNumber][stmtNoOfStartNode] = 1;

	//Return if hit yourself. But put into Table first.
	if (current->statementNumber == stmtNoOfStartNode) {
		return;
	}

	//Go down child. stmtNo won't change.
	for (CFGNode* i : current->to) {
		depthFirstSearch(i, stmtNoOfStartNode, typeOfStartNode);
	}
}

//Next*(11,a)
vi Next::getTransitiveNextSpecificGeneric(int lineNo, NodeType type) {
	vi result;

	if (lineNo < 1 || lineNo > PKB::getInstance().getStmtCount()) {
		return result;
	}

	if (isNewQuery) {
		buildTransitiveTable();
	}

	if (type == NodeType::StmtLst) {
		if (nextTrans.count(lineNo) == 1) {
			for (auto stmt : nextTrans[lineNo]) {
				result.push_back(stmt.first);
			}
		}
		return result;
	}

	PKB& pkb = PKB::getInstance();
	
	if (nextTrans.count(lineNo) == 1) {
		for (auto stmt : nextTrans[lineNo]) {
			if (type == pkb.getStmt(stmt.first).second->type) {
				result.push_back(stmt.first);
			}
		}
	}
	return result;
}

//Next(a,11)
vi Next::getTransitiveNextGenericSpecific(int lineNo, NodeType type) {
	vi result;

	if (lineNo < 1 || lineNo > PKB::getInstance().getStmtCount()) {
		return result;
	}

	if (isNewQuery) {
		buildTransitiveTable();
	}

	if (type == NodeType::StmtLst) {
		if (nextTransReverse.count(lineNo) == 1) {
			for (auto stmt : nextTransReverse[lineNo]) {
				result.push_back(stmt.first);
			}
		}
	}
	
	PKB& pkb = PKB::getInstance();
	if (nextTransReverse.count(lineNo) == 1) {
		for (auto stmt : nextTransReverse[lineNo]) {
			if (type == pkb.getStmt(stmt.first).second->type) {
				result.push_back(stmt.first);
			}
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

	//Both looking for unique type. Not all statements. Next(a,w)
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

	if (nextTrans.count(a) && nextTrans[a].count(b)) {
		return true;
	}
	return false;
}


void Next::newQuery() {
	isNewQuery = true;
	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			stmtTransPairs[i][j].clear();
		}
	}
	nextTrans.clear();
	nextTransReverse.clear();
}


void Next::build2DArrayTable() {
	tableSize = PKB::getInstance().getStmtCount();

	for (int i = 0; i <= tableSize; i++) {
		vector<bool> width;
		for (int i = 0; i <= tableSize; i++) {
			width.push_back(false);
		}
		nextArray.push_back(width);
	}

	for (int i = 1; i <= tableSize; i++) {
		vi to = next[i];
		for (int j = 0; j < to.size(); j++) {
			nextArray[i][to.at(j)] = true;
		}
	}
}