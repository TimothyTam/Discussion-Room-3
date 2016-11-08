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
		/*
		stmtTransPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtTransPairs[i].push_back(vp_i_i());
		}*/
		nextTrans.push_back(map_vmap());
		nextTransReverse.push_back(map_vmap());
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
	allNextTransCalculated = true;
}

// Fastest Next* for Cyclic Graph. Go to each Node and DFS.
void Next::buildTransitiveTableForProcedure(CFGNode* current, map<CFGNode*, int> &visited) {
	vi nextStmts;
	if (visited.count(current) == 1) {
		return;
	}
	visited[current] = 1;	
	int location = getLocationOfStmt(current->type);

	calculateTransitiveNextSpecificGeneric(current->statementNumber, location);

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

void Next::resetIsVisitedFlagReverse(CFGNode* current) {
	if (!(current->visited)) {
		return;
	}

	current->visited = false;

	//Go up parent. stmtNo won't change.
	for (CFGNode* i : current->from) {
		resetIsVisitedFlagReverse(i);
	}
}

void Next::depthFirstSearch(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode) {
//void Next::depthFirstSearch(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode, map<CFGNode*, int> visited) {
	if (current->visited) {
		return;
	}
	current->visited = true;
	
	//Put into Table
	int location = getLocationOfStmt(current->type);
	
	if (nextTransReverse[location][current->statementNumber].empty()) {
		for (int i = 0; i < 4; i++) {
			nextTransReverse[location][current->statementNumber].push_back(map_i_i());
		}
	}

	nextTrans[typeOfStartNode][stmtNoOfStartNode][location][current->statementNumber] = 1;
	nextTransReverse[location][current->statementNumber][typeOfStartNode][stmtNoOfStartNode] = 1;

	//Return if hit yourself. But put into Table first.
	if (current->statementNumber == stmtNoOfStartNode) {
		return;
	}

	//Go down child. stmtNo won't change.
	for (CFGNode* i : current->to) {
		depthFirstSearch(i, stmtNoOfStartNode, typeOfStartNode);
	}
}

void Next::depthFirstSearchReverse(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode) {
	//void Next::depthFirstSearch(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode, map<CFGNode*, int> visited) {
	if (current->visited) {
		return;
	}
	current->visited = true;

	//Put into Table
	int location = getLocationOfStmt(current->type);

	nextTransReverse[typeOfStartNode][stmtNoOfStartNode][location][current->statementNumber] = 1;

	//Return if hit yourself. But put into Table first.
	if (current->statementNumber == stmtNoOfStartNode) {
		return;
	}

	//Go up parent. stmtNo won't change.
	for (CFGNode* i : current->from) {
		depthFirstSearchReverse(i, stmtNoOfStartNode, typeOfStartNode);
	}
}

//Next*(11,a)
vi Next::getTransitiveNextSpecificGeneric(int lineNo, NodeType type) {
	vi result;
	PKB& pkb = PKB::getInstance();
	
	NodeType lineNoType = pkb.getNodeTypeOfStmt(lineNo);
	if (lineNoType == NodeType::Invalid) {
		return result;
	}

	int location = getLocationOfStmt(lineNoType);

	calculateTransitiveNextSpecificGeneric(lineNo, location);

	if (type == NodeType::StmtLst) {
		for (map_i_i secondMap : nextTrans[location][lineNo]) {
			for (std::pair<int, int> secondValue : secondMap) {
				result.push_back(secondValue.first);
			}
		}
	}
	else {
		int location2 = getLocationOfStmt(type);
		if (location2 != -1) {
			for (std::pair<int, int> secondValue : nextTrans[location][lineNo][location2]) {
				result.push_back(secondValue.first);
			}
		}
	}

	return result;
}

void Next::calculateTransitiveNextSpecificGeneric(int lineNo, int location) {
	if (nextTrans[location].count(lineNo) == 0) {

		if (nextTrans[location][lineNo].empty()) {
			for (int i = 0; i < 4; i++) {
				nextTrans[location][lineNo].push_back(map_i_i());
			}
		}

		CFGNode* current = PKB::getInstance().getCFGNodeFromStatement(lineNo);

		for (CFGNode* i : current->to) {
			depthFirstSearch(i, lineNo, location);
			resetIsVisitedFlag(i);
		}
	}
}

//Next(a,11)
vi Next::getTransitiveNextGenericSpecific(int lineNo, NodeType type) {
	vi result;
	PKB& pkb = PKB::getInstance();

	NodeType lineNoType = pkb.getNodeTypeOfStmt(lineNo);
	if (lineNoType == NodeType::Invalid) {
		return result;
	}

	int location = getLocationOfStmt(lineNoType);

	calculateTransitiveNextGenericSpecific(lineNo, location);

	if (type == NodeType::StmtLst) {
		for (map_i_i secondMap : nextTransReverse[location][lineNo]) {
			for (std::pair<int, int> secondValue : secondMap) {
				result.push_back(secondValue.first);
			}
		}
	}
	else {
		int location2 = getLocationOfStmt(type);
		if (location2 != -1 && nextTransReverse[location].count(lineNo) == 1) {
			for (std::pair<int, int> secondValue : nextTransReverse[location][lineNo][location2]) {
				result.push_back(secondValue.first);
			}
		}
	}
	return result;
}

void Next::calculateTransitiveNextGenericSpecific(int lineNo, int location) {
	if (nextTransReverseCalculated.count(lineNo) == 0 && !allNextTransCalculated) {
		CFGNode* current = PKB::getInstance().getCFGNodeFromStatement(lineNo);

		if (nextTransReverse[location][lineNo].empty()) {
			for (int i = 0; i < 4; i++) {
				nextTransReverse[location][lineNo].push_back(map_i_i());
			}
		}

		for (CFGNode* i : current->from) {
			depthFirstSearchReverse(i, lineNo, location);
			resetIsVisitedFlagReverse(i);
		}
		nextTransReverseCalculated[lineNo] = 1;
	}
}

vp_i_i Next::getTransitiveNextGenericGeneric(NodeType typeA, NodeType typeB) {
	vp_i_i result;
	bool allFirst = false;
	bool allSecond = false;
	int location1 = -1;
	int location2 = -1;

	if (!allNextTransCalculated) {
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

		for (std::pair<int,vmap> firstValue : nextTrans[location1]) {
			for (std::pair<int,int> secondValue : firstValue.second[location2]) {
				result.push_back(make_pair(firstValue.first, secondValue.first));
			}
		}

		return result;
	}

	// Next*(s,w)
	if (allFirst && !allSecond) {
		location2 = getLocationOfStmt(typeB);
		if (location2 == -1) {
			return result;
		}
		for (map_vmap allValues : nextTrans) {
			for (std::pair<int, vmap> firstValue : allValues) {
				for (std::pair<int, int> secondValue : firstValue.second[location2]) {
					result.push_back(make_pair(firstValue.first, secondValue.first));
				}
			}
		}

		return result;
	}

	// Next*(w,s)
	if (!allFirst && allSecond) {
		location1 = getLocationOfStmt(typeA);
		if (location1 == -1) {
			return result;
		}


		for (std::pair<int, vmap> firstValue : nextTrans[location1]) {
			for (map_i_i tempValues : firstValue.second) {
				for (std::pair<int, int> secondValue : tempValues) {
					result.push_back(make_pair(firstValue.first, secondValue.first));
				}
			}
		}

		return result;
	}

	// Next*(s,s). This translates to all statements with Next*.
	
	for (map_vmap allValues : nextTrans) {
		for (std::pair<int, vmap> firstValue : allValues) {
			for (map_i_i tempValues : firstValue.second) {
				for (std::pair<int, int> secondValue : tempValues) {
					result.push_back(make_pair(firstValue.first, secondValue.first));
				}
			}
		}
	}

	return result;
}
bool Next::whetherTransitivelyNext(int a, int b) {
	PKB& pkb = PKB::getInstance();
	int location1 = getLocationOfStmt(pkb.getNodeTypeOfStmt(a));
	int location2 = getLocationOfStmt(pkb.getNodeTypeOfStmt(b));

	if (location1 == -1 || location2 == -1) {
		return false;
	}

	if (nextTrans[location1].count(a)) {
		return (nextTrans[location1][a][location2].count(b) == 1);
	}
	else if (nextTransReverseCalculated.count(b)) {
		return (nextTransReverse[location2][b][location1].count(a) == 1);
	}
	else {
		//Calculate it.
		calculateTransitiveNextSpecificGeneric(a, location1);
		return (nextTrans[location1][a][location2].count(b) == 1);
	}
	return false;
}


void Next::newQuery() {
	
	for (int i = 0; i < 4; i++) {
		nextTrans[i].clear();
		nextTransReverse[i].clear();
	}
	allNextTransCalculated = false;
	nextTransReverseCalculated.clear();
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