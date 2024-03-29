#include "Parent.h"
#include "TNode.h"
#include "vector"
#include "PKB.h"
#include <algorithm>

//recursively build the tables storing Parent relationships
void Parent::buildFromNode(TNode* currentNode) {
	vector<TNode*>* childs = &(currentNode->childs);
	

	if (currentNode->type == NodeType::If || currentNode->type == NodeType::While) {

		vector<int> childsOfThisNode = vector<int>();

		for (unsigned i = 1; i < childs->size(); i++) {
			TNode* stmtLst = childs->at(i);
			//now lets add all the childs of this stmtLst as childs of currentNode;
			for (unsigned j = 0; j < stmtLst->childs.size(); j++) {
				TNode* currentChild = stmtLst->childs[j];

				parentOfStmt[currentChild->statementNumber] = currentNode->statementNumber;
				childsOfThisNode.push_back(currentChild->statementNumber);
			}
		}

		childOfStmt[currentNode->statementNumber] = childsOfThisNode;
	}

	for (unsigned i = 0; i < childs->size(); i++) {
		buildFromNode(childs->at(i));
	}
}


vi Parent::getParentSpecificGeneric(int lineNo, NodeType type) {
	if (childOfStmt.count(lineNo) == 0) {
		return vi();
	}
	return filterStmts(childOfStmt[lineNo], type);
}

int Parent::getParentGenericSpecific(int lineNo, NodeType type) {
	
	if (parentOfStmt.count(lineNo) == 1) {
		if (type == NodeType::StmtLst ||
			PKB::getInstance().getStmt(parentOfStmt[lineNo]).second->type == type) {
			return parentOfStmt[lineNo];
		}
	}
	return -1;
}

vi Parent::getTransitiveParentSpecificGeneric(int lineNo, NodeType type)
{
	if (transitiveChildOfStmt.count(lineNo) == 0) {
		return vi();
	}
	return filterStmts(transitiveChildOfStmt[lineNo], type);
}


vi Parent::getTransitiveParentGenericSpecific(int lineNo, NodeType type) {

	if (transitiveParentOfStmt.count(lineNo) == 0) {
		return vi();
	}
	return filterStmts(transitiveParentOfStmt[lineNo], type);
}

bool Parent::whetherTransitiveParent(int lineNo, int lineNo2)
{
	if (lineNo < 1 || lineNo > tableSize || lineNo2 < 0 || lineNo2 > tableSize) return false;
	return stmtVarTransArray[lineNo][lineNo2];
}

bool Parent::whetherParent(int lineNo, int lineNo2) {
	if (parentOfStmt.count(lineNo2) == 0) {
		return false;
	}
	return parentOfStmt[lineNo2] == lineNo;
}

//Filter the stmt# based on the statement type (while, assign, if, call)
vi Parent::filterStmts(vi stmList, NodeType type) {
	vi result = vi();
	if (type == NodeType::StmtLst) return stmList;

	for (size_t i = 0; i < stmList.size(); i++) {
		int currentStmt = stmList[i];

		// add the currentStmt if its type is the same as type
		if (PKB::getInstance().getStmt(currentStmt).second->type == type) {
			result.push_back(currentStmt);
		}
	}
	return result;
}

void Parent::generateParentData(TNode* rootNode) {
	buildFromNode(rootNode);
	buildTransitiveData();
	buildStmtPairs();
	build2DArrayTable();
}

//recursive function to build the star tables
vi Parent::buildTransitiveFromStmt(int currentStmt, vector<bool>* done) {
	// if already done building from here, return the result in transitiveChildOf
	if (done->at(currentStmt)) return transitiveChildOfStmt[currentStmt];

	// else, build the transitive childs from here

	//initialise to the list of direct childs;
	vi childsOfCurrentStmt = childOfStmt[currentStmt];
	transitiveChildOfStmt[currentStmt] = vi(childsOfCurrentStmt);
	
	for (unsigned i = 0; i < childsOfCurrentStmt.size(); i++) {
		//for each child of the CurrentStatement, if the child has children also, then add those grandchildren to the club

		if (childOfStmt.count(childsOfCurrentStmt[i]) == 0) continue;
		vi grandChilds = buildTransitiveFromStmt(childsOfCurrentStmt[i], done);

		//add grandchildren
		transitiveChildOfStmt[currentStmt].insert(transitiveChildOfStmt[currentStmt].end(), grandChilds.begin(), grandChilds.end());
	}

	//mark this statement as done
	done->at(currentStmt) = true;
	
	//now we got all the grandchildren, let populate the respective transitiveParentOfStmt entries
	for (unsigned i = 0; i < transitiveChildOfStmt[currentStmt].size(); i++) {
		int grandChild = transitiveChildOfStmt[currentStmt][i];
		if (transitiveParentOfStmt.count(grandChild) == 0) {
			transitiveParentOfStmt[grandChild] = vi();
		}
		transitiveParentOfStmt[grandChild].push_back(currentStmt);
	}

	return transitiveChildOfStmt[currentStmt];
}

//build the star tables
void Parent::buildTransitiveData() {
	const int maxStmt = PKB::getInstance().getStmtCount();
	vector<bool> done;
	done.assign(maxStmt + 10, false);

	//iterate through the childOfStmt map, build from each parent node
	for (map_i_vi::iterator it = childOfStmt.begin(); it != childOfStmt.end(); ++it) {
		vi dummy = buildTransitiveFromStmt(it->first, &done);
	}
}

//Iter 2
void Parent::buildStmtPairs() {
	buildStmtPairsFollow();
	buildStmtPairsFollowTrans();
}

void Parent::buildStmtPairsFollow() {
	map_i_i::iterator it;
	PKB& inst = PKB::getInstance();

	for (int i = 0; i < 4; i++) {
		stmtPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtPairs[i].push_back(vp_i_i());
		}
	}

	for (it = parentOfStmt.begin(); it != parentOfStmt.end(); it++) {
		NodeType type1 = inst.getStmt(it->second).second->type;
		NodeType type2 = inst.getStmt(it->first).second->type;
		int location1 = -1;
		int location2 = -1;

		location1 = getLocationOfStmt(type1);
		location2 = getLocationOfStmt(type2);

		_ASSERT((location1 != -1 || location2 != -1) && "Failed to build Statement Pairs in Parent.");

		stmtPairs[location1][location2].push_back(make_pair(it->second, it->first));
	}
}

void Parent::buildStmtPairsFollowTrans() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (int i = 0; i < 4; i++) {
		stmtTransPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtTransPairs[i].push_back(vp_i_i());
		}
	}

	for (it = transitiveChildOfStmt.begin(); it != transitiveChildOfStmt.end(); it++) {
		NodeType type1 = inst.getStmt(it->first).second->type;
		int location1 = -1;
		int location2 = -1;

		location1 = getLocationOfStmt(type1);

		for (int i : it->second) {
			NodeType type2 = inst.getStmt(i).second->type;
			location2 = getLocationOfStmt(type2);

			_ASSERT((location1 != -1 || location2 != -1) && "Failed to build Transitive Statement Pairs in Parent.");

			stmtTransPairs[location1][location2].push_back(make_pair(it->first, i));
		}
	}
}

vp_i_i Parent::getParentGenericGeneric(NodeType typeA, NodeType typeB) {
	return getParentGenericGeneric(typeA, typeB, false);
}

vp_i_i Parent::getTransitiveParentGenericGeneric(NodeType typeA, NodeType typeB) {
	return getParentGenericGeneric(typeA, typeB, true);
}

vp_i_i Parent::getParentGenericGeneric(NodeType typeA, NodeType typeB, bool transitive) {
	bool allFirst = false;
	bool allSecond = false;
	int location1 = -1;
	int location2 = -1;

	stmtPairFollow* stmtPair;
	if (typeA == NodeType::StmtLst) {
		allFirst = true;
	}
	if (typeB == NodeType::StmtLst) {
		allSecond = true;
	}

	if (transitive) {
		stmtPair = &stmtTransPairs;
	}
	else {
		stmtPair = &stmtPairs;
	}

	//Both looking for unique type. Not all statements. Parent(a,w)
	if (!allFirst && !allSecond) {
		location1 = getLocationOfStmt(typeA);
		location2 = getLocationOfStmt(typeB);
		return (*stmtPair)[location1][location2];
	}

	// Parent(s,w)
	if (allFirst && !allSecond) {
		location2 = getLocationOfStmt(typeB);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), (*stmtPair)[i][location2].begin(), (*stmtPair)[i][location2].end());
		}
		return result;
	}
	// Parent(w,s)
	if (!allFirst && allSecond) {
		location1 = getLocationOfStmt(typeA);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), (*stmtPair)[location1][i].begin(), (*stmtPair)[location1][i].end());
		}
		return result;
	}

	// Parent(s,s). This translates to all statements with follow.
	vp_i_i result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.insert(result.end(), (*stmtPair)[i][j].begin(), (*stmtPair)[i][j].end());
		}
	}
	return result;
}

int Parent::getLocationOfStmt(NodeType type) {
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


void Parent::build2DArrayTable() {
	tableSize = PKB::getInstance().getStmtCount();

	for (int i = 0; i <= tableSize; i++) {
		vector<bool> width;
		for (int i = 0; i <= tableSize; i++) {
			width.push_back(false);
		}
		stmtVarTransArray.push_back(width);
	}

	for (int i = 1; i <= tableSize; i++) {
		vi to = transitiveChildOfStmt[i];
		for (int j = 0; j < to.size(); j++) {
			stmtVarTransArray[i][to.at(j)] = true;
		}
	}
}