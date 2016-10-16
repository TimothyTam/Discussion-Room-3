#include "Follow.h"

void Follow::generateFollowTable(TNode* root) {
	if (root->type != NodeType::Program) {
		throw runtime_error("Misused generateFollowTableRecursive. Must put root node");
	}

	generateFollowTableRecursive(root);
	buildStmtPairs();

	build2DArrayTable();
}

//Generates the follow table using the root node of the AST.
//Undefined behaviour if current is not root.
//Throws exception if the AST is different from expected
void Follow::generateFollowTableRecursive(TNode* current) {
	std::vector<TNode*> childs = current->childs;
	size_t i;
	try {
		if (current->type == NodeType::Program) {
			for (TNode* child : childs) {
				generateFollowTableRecursive(child);
			}
		}
		else if (current->type == NodeType::Procedure) {
			generateFollowTableRecursive(childs.at(0));
		}
		else if (current->type == NodeType::StmtLst) {
			vi transistivelyfollowedBy;
			vi transistivelyFollows;

			for (i = 1; i < childs.size(); i++) {
				transistivelyFollows.push_back(childs.at(i)->statementNumber);
			}

			for (i = 0; i < childs.size() - 1; i++) {
				//add child[i].StmtNo and child[i+1].StmtNo into follow and followedBy table
				follows.insert(std::make_pair(childs.at(i)->statementNumber,
												childs.at(i + 1)->statementNumber));
				followedBy.insert(std::make_pair(childs.at(i + 1)->statementNumber, 
												childs.at(i)->statementNumber));

				//add child[i] and child[i+1,2,3,4,childs.size()] into follow*(i,_) table
				vi::const_iterator first = transistivelyFollows.begin() + i;
				vi::const_iterator last = transistivelyFollows.end();
				vi newVec(first, last);

				followsT.insert(make_pair(childs.at(i)->statementNumber, newVec));

				//add child[0,1,2,i-1] and child[i] into follows*(_,i) table
				if (transistivelyfollowedBy.size() != 0) {
					followedByT.insert(make_pair(childs.at(i)->statementNumber, 
													transistivelyfollowedBy));
				}
				transistivelyfollowedBy.push_back(childs.at(i)->statementNumber);
			}

			//add child[0,1,2,n-2] and child[childs.size()-1] into follows*(_,i) table
			if (childs.size() >= 2 && transistivelyfollowedBy.size() != 0) {
				followedByT.insert(make_pair(childs.at(childs.size() - 1)->
												statementNumber, transistivelyfollowedBy));
			}

			for (i = 0; i < childs.size(); i++) {
				generateFollowTableRecursive(childs.at(i));
			}
		}
		else if (current->type == NodeType::While) {
			generateFollowTableRecursive(childs.at(1));
			return;
		}
		else if (current->type == NodeType::If) {
			generateFollowTableRecursive(childs.at(1)); // Then Stmt
			generateFollowTableRecursive(childs.at(2)); // Else Stmt
			return;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		throw runtime_error("Error Generating Follow Table" + string(oor.what()));
	}

}

//Non-Transistive Methods
//(lineNo, s1). Return s1, given lineNo. 
int Follow::getFollowSpecificGeneric(int lineNo, NodeType type) {
	return getStmtsXStmt(false, lineNo, type);
}
//(s1,lineNo). Return s1, given lineNo.
int Follow::getFollowGenericSpecific(int lineNo, NodeType type) {
	return getStmtsXStmt(true, lineNo, type);
}

// This method returns the stmt following/followed by lineNo.
// If stmtFollowingStmt is true, it will return following.
// If type is not equal to StmtLst, then it will check whether
// the resulting stmt is of the same type as type. It returns 
// the stmt if the type are equal.
int Follow::getStmtsXStmt(bool stmtFollowingStmt, int lineNo, NodeType type) {
	PKB& pkb = PKB::getInstance();
	int stmt;

	if (stmtFollowingStmt) {
		stmt = (followedBy.count(lineNo) == 1) ? stmt = followedBy[lineNo] : 0;
	}
	else {
		stmt = (follows.count(lineNo) == 1) ? stmt = follows[lineNo] : 0;
	}

	if (type == NodeType::StmtLst || stmt == 0) {
		return stmt;
	}
	else {
		if (pkb.getStmt(stmt).second->type == type) {
			return stmt;
		}
	}
	return 0;
}

bool Follow::whetherFollows(int a, int b) {
	return followedBy.count(b) == 1 ? followedBy[b] == a : false;
}
/*
//(s1,s2). Return s2.
vi Follow::getStmtsFollowedByStmt(NodeType typeA, NodeType typeB) {
	return getStmtsXStmt(false, typeA, typeB);
}

//(s1,s2). Return s1.
vi Follow::getStmtsFollowingStmt(NodeType typeA, NodeType typeB) {
	return getStmtsXStmt(true, typeA, typeB);
}*/

// This method returns the stmts that are following/followed any stmt.
// If stmtFollowingStmt is true, it will return following.
// If type is not equal to StmtLst, then it will check whether
// the resulting stmt is of the same type as type. It adds to the result
// if they are the same type.
vi Follow::getStmtsXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB) {
	PKB& pkb = PKB::getInstance();
	map_i_i::iterator it;
	si resultSet;
	vi result;

	bool checkTypeA = typeA != NodeType::StmtLst;
	bool checkTypeB = typeB != NodeType::StmtLst;

	if (!isValidNodeType(typeA) || !isValidNodeType(typeB)) {
		printf("type must be Assign, If, While, Call or StmtLst");
	}

	if (checkTypeA && checkTypeB) {
		for (it = follows.begin(); it != follows.end(); it++) {
			if (pkb.getStmt(it->first).second->type == typeA && pkb.getStmt(it->second).second->type == typeB) {
				resultSet.insert(stmtsFollowingStmt ? it->first : it->second);
			}
		}
	}
	else if (checkTypeA) {
		for (it = follows.begin(); it != follows.end(); it++) {
			if (pkb.getStmt(it->first).second->type == typeA) {
				resultSet.insert(stmtsFollowingStmt ? it->first : it->second);
			}
		}
	}
	else if (checkTypeB) {
		for (it = followedBy.begin(); it != followedBy.end(); it++) {
			if (pkb.getStmt(it->first).second->type == typeB) {
				resultSet.insert(stmtsFollowingStmt ? it->second : it->first);
			}
		}
	}
	else {
		//Returns all statements
		map_i_i* table = stmtsFollowingStmt ? &follows : &followedBy;

		for (it = (*table).begin(); it != (*table).end(); it++) {
			resultSet.insert(it->first);
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

//Transistive Methods
vi Follow::getTransitiveFollowSpecificGeneric(int lineNo, NodeType type) {
	return getStmtsTransitivelyXStmt(false, lineNo, type);
}
vi Follow::getTransitiveFollowGenericSpecific(int lineNo, NodeType type) {
	return getStmtsTransitivelyXStmt(true, lineNo, type);
}

// This method returns the stmt transitively following/followed by lineNo.
// If stmtFollowingStmt is true, it will return following.
// If type is not equal to StmtLst, then it will check whether
// the resulting stmt is of the same type as type. It adds to the result
// if they are the same type.
vi Follow::getStmtsTransitivelyXStmt(bool stmtFollowingStmt, int lineNo, NodeType type) {
	vi result;
	vi temp;

	if (stmtFollowingStmt) {
		temp = followedByT.count(lineNo) == 1 ? followedByT[lineNo] : vi();
	}
	else {
		temp = followsT.count(lineNo) == 1 ? followsT[lineNo] : vi();
	}

	if (type == NodeType::StmtLst) {
		return temp;
	}

	PKB& pkb = PKB::getInstance();
	for (int stmt : temp) {
		if (type == pkb.getStmt(stmt).second->type) {
			result.push_back(stmt);
		}
	}

	return result;
}

// This method returns the stmt transitively following/followed by any stmt.
// If stmtFollowingStmt is true, it will return following.
// If type is not equal to StmtLst, then it will check whether
// the resulting stmt is of the same type as type. It adds to the result
// if they are the same type.
vi Follow::getStmtsTransitivelyXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB) {
	PKB& pkb = PKB::getInstance();
	map_i_vi::iterator it;
	si resultSet;
	vi result;

	bool checkTypeA = typeA != NodeType::StmtLst;
	bool checkTypeB = typeB != NodeType::StmtLst;

	if (!isValidNodeType(typeA) || !isValidNodeType(typeB)) {
		printf("type must be Assign, If, While, Call or StmtLst");
	}

	if (checkTypeA && checkTypeB) {
		map_i_vi* table = (stmtsFollowingStmt ? &followsT : &followedByT);
		NodeType type1 = (stmtsFollowingStmt ? typeA : typeB);
		NodeType type2 = (stmtsFollowingStmt ? typeB : typeA);

		for (it = (*table).begin(); it != (*table).end(); it++) {
			if (pkb.getStmt(it->first).second->type == type1) {
				//Loop through it->second and check type2 and insert it->first if any of the stmt is correct
				for (int stmt : it->second) {
					if (pkb.getStmt(stmt).second->type == type2) {
						resultSet.insert(it->first);
						break;
					}
				}
			}
		}
	}
	else if (checkTypeA) {
		//Use FollowsT Table
		for (it = followsT.begin(); it != followsT.end(); it++) {
			if (pkb.getStmt(it->first).second->type == typeA) {
				//Insert first if true. Insert all of second if false.
				if (stmtsFollowingStmt) {
					resultSet.insert(it->first);
				}
				else {
					for (int stmt : it->second) {
						resultSet.insert(stmt);
					}
				}
			}
		}
	}
	else if (checkTypeB) {
		//Use FollowedByT[] Table
		for (it = followedByT.begin(); it != followedByT.end(); it++) {
			if (pkb.getStmt(it->first).second->type == typeB) {
				//Insert all of second if true. Insert first if false. 
				if (stmtsFollowingStmt) {
					for (int stmt : it->second) {
						resultSet.insert(stmt);
					}
				}
				else {
					resultSet.insert(it->first);
				}
			}
		}
	}
	else {
		map_i_vi* table = (stmtsFollowingStmt ? &followsT : &followedByT);
		for (it = (*table).begin(); it != (*table).end(); it++) {
			resultSet.insert(it->first);
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

bool Follow::whetherTransitivelyFollows(int a, int b) {
	if (a < 1 || a > tableSize || b < 0 || b > tableSize) return false;
	return stmtVarTransArray[a][b];
}

bool Follow::isValidNodeType(NodeType type) {
	return (type == NodeType::Assign || type == NodeType::If || type == NodeType::While || type == NodeType::Call || type == NodeType::StmtLst);
}

//Iteration 2
void Follow::buildStmtPairs() {
	buildStmtPairsFollow();
	buildStmtPairsFollowTrans();
}

void Follow::buildStmtPairsFollow() {
	map_i_i::iterator it;
	PKB& inst = PKB::getInstance();

	for (int i = 0; i < 4; i++) {
		stmtPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtPairs[i].push_back(vp_i_i());
		}
	}

	for (it = follows.begin(); it != follows.end(); it++) {
		NodeType type1 = inst.getStmt(it->first).second->type;
		NodeType type2 = inst.getStmt(it->second).second->type;
		int location1 = -1;
		int location2 = -1;

		location1 = getLocationOfStmt(type1);
		location2 = getLocationOfStmt(type2);

		_ASSERT((location1 != -1 || location2 != -1) && "Failed to build Statement Pairs in Follow.");

		stmtPairs[location1][location2].push_back(make_pair(it->first, it->second));
	}
}

void Follow::buildStmtPairsFollowTrans() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (int i = 0; i < 4; i++) {
		stmtTransPairs.push_back(vector<vp_i_i>());
		for (int j = 0; j < 4; j++) {
			stmtTransPairs[i].push_back(vp_i_i());
		}
	}
	
	for (it = followsT.begin(); it != followsT.end(); it++) {
		NodeType type1 = inst.getStmt(it->first).second->type;
		int location1 = -1;
		int location2 = -1;

		location1 = getLocationOfStmt(type1);

		for (int i : it->second) {
			NodeType type2 = inst.getStmt(i).second->type;
			location2 = getLocationOfStmt(type2);

			_ASSERT((location1 != -1 || location2 != -1) && "Failed to build Transitive Statement Pairs in Follow.");

			stmtTransPairs[location1][location2].push_back(make_pair(it->first, i));
		}
	}
}

vp_i_i Follow::getFollowGenericGeneric(NodeType typeA, NodeType typeB) {
	return getFollowGenericGeneric(typeA, typeB, false);
}

vp_i_i Follow::getTransitiveFollowGenericGeneric(NodeType typeA, NodeType typeB) {
	return getFollowGenericGeneric(typeA, typeB, true);
}

vp_i_i Follow::getFollowGenericGeneric(NodeType typeA, NodeType typeB, bool transitive) {
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

	//Both looking for unique type. Not all statements. Follow(a,w)
	if (!allFirst && !allSecond) {
		location1 = getLocationOfStmt(typeA);
		location2 = getLocationOfStmt(typeB);
		return (*stmtPair)[location1][location2];
	}

	// Follow(s,w)
	if (allFirst && !allSecond) {
		location2 = getLocationOfStmt(typeB);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), (*stmtPair)[i][location2].begin(), (*stmtPair)[i][location2].end());
		}
		return result;
	}
	// Follow(w,s)
	if (!allFirst && allSecond) {
		location1 = getLocationOfStmt(typeA);
		vp_i_i result;
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), (*stmtPair)[location1][i].begin(), (*stmtPair)[location1][i].end());
		}
		return result;
	}

	// Follow(s,s). This translates to all statements with follow.
	vp_i_i result;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result.insert(result.end(), (*stmtPair)[i][j].begin(), (*stmtPair)[i][j].end());
		}
	}
	return result;
}

int Follow::getLocationOfStmt(NodeType type) {
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

void Follow::build2DArrayTable() {
	tableSize = PKB::getInstance().getStmtCount();

	for (int i = 0; i <= tableSize; i++) {
		vector<bool> width;
		for (int i = 0; i <= tableSize; i++) {
			width.push_back(false);
		}
		stmtVarTransArray.push_back(width);
	}

	for (int i = 1; i <= tableSize; i++) {
		vi to = followsT[i];
		for (int j = 0; j < to.size(); j++) {
			stmtVarTransArray[i][to.at(j)] = true;
		}
	}
}