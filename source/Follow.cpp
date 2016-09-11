#include "Follow.h"

void Follow::generateFollowTable(TNode* current) {
	std::vector<TNode*> childs = current->childs;
	size_t i;
	try {
		if (current->type == NodeType::Program) {
			for (TNode* child : childs) {
				generateFollowTable(child);
			}
		}
		else if (current->type == NodeType::Procedure) {
			generateFollowTable(childs.at(0));
		}
		else if (current->type == NodeType::StmtLst) {
			vi transistivelyfollowedBy;
			vi transistivelyFollows;

			for (i = 1; i < childs.size(); i++) {
				transistivelyFollows.push_back(childs.at(i)->statementNumber);
			}

			for (i = 0; i < childs.size() - 1; i++) {
				//add child[i].StmtNo and child[i+1].StmtNo into follow and followedBy table
				follows.insert(std::make_pair(childs.at(i)->statementNumber, childs.at(i + 1)->statementNumber));
				followedBy.insert(std::make_pair(childs.at(i + 1)->statementNumber, childs.at(i)->statementNumber));

				//add child[i] and child[i+1,2,3,4,childs.size()] into follow*(i,_) table
				vi::const_iterator first = transistivelyFollows.begin() + i;
				vi::const_iterator last = transistivelyFollows.end();
				vi newVec(first, last);

				followsT.insert(make_pair(childs.at(i)->statementNumber, newVec));

				//add child[0,1,2,i-1] and child[i] into follows*(_,i) table
				followedByT.insert(make_pair(childs.at(i)->statementNumber, transistivelyfollowedBy));
				transistivelyfollowedBy.push_back(childs.at(i)->statementNumber);
			}

			//add child[0,1,2,n-2] and child[childs.size()-1] into follows*(_,i) table
			if (childs.size() >= 2) {
				followedByT.insert(make_pair(childs.at(childs.size() - 1)->statementNumber, transistivelyfollowedBy));
			}

			for (i = 0; i < childs.size(); i++) {
				generateFollowTable(childs.at(i));
			}
		}
		else if (current->type == NodeType::While) {
			generateFollowTable(childs.at(1));
			return;
		}
		else if (current->type == NodeType::If) {
			generateFollowTable(childs.at(1)); // Then Stmt
			generateFollowTable(childs.at(2)); // Else Stmt
			return;
		}
	} catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}

}

//Non-Transistive Methods
//(lineNo, s1). Return s1, given lineNo. 
int Follow::getStmtFollowedByStmt(int lineNo, NodeType type) {
	return getStmtsXStmt(false, lineNo, type);
}
//(s1,lineNo). Return s1, given lineNo.
int Follow::getStmtFollowingStmt(int lineNo, NodeType type) {
	return getStmtsXStmt(true, lineNo, type);
}

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

//(s1,s2). Return s2.
vi Follow::getStmtsFollowedByStmt(NodeType typeA, NodeType typeB) {
	return getStmtsXStmt(false, typeA, typeB);
}

//(s1,s2). Return s1.
vi Follow::getStmtsFollowingStmt(NodeType typeA, NodeType typeB) {
	return getStmtsXStmt(true, typeA, typeB);
}

bool Follow::whetherFollows(int a, int b) {
	return followedBy.count(b) == 1 ? followedBy[b] == a : false;
}

vi Follow::getStmtsXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB) {
	PKB& pkb = PKB::getInstance();
	map_i_i::iterator it;
	si resultSet;
	vi result;

	bool checkTypeA = typeA != NodeType::StmtLst;
	bool checkTypeB = typeB != NodeType::StmtLst;

	if (!isValidNodeType(typeA) || !isValidNodeType(typeB)){
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
		resultSet.insert(stmtsFollowingStmt ? it->first : it->second);
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

//Transistive Methods
vi Follow::getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type) {
	return getStmtsTransitivelyXStmt(false, lineNo, type);
}
vi Follow::getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type) {
	return getStmtsTransitivelyXStmt(true, lineNo, type);
}

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


// Select s2 Follows*(s1,s2). typeA = s1.type
vi Follow::getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB) {
	return getStmtsTransitivelyXStmt(false, typeA, typeB);
}

// Select s1 Follows*(s1,s2). typeA = s1.type
vi Follow::getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB) {
	return getStmtsTransitivelyXStmt(true, typeA, typeB);
}

//stmtsFollowingStmt = true => Select s1 Follows*(s1,s2). typeA = s1.type
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
		//Generic
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
	vi stmts;
	if (followedByT.count(b) == 1) {
		stmts = followedByT[b];
		return (std::find(stmts.begin(), stmts.end(), a) != stmts.end());
	}
	return false;
}

bool Follow::isValidNodeType(NodeType type) {
	return (type == NodeType::Assign || type == NodeType::If || type == NodeType::While || type == NodeType::Call || type == NodeType::StmtLst);
}
