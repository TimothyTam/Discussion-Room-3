#include "Follow.h"


void Follow::generateFollowTable(TNode* current) {
	std::vector<TNode*> childs = current->childs;
	size_t i;

	if (current->type == NodeType::Program) {
		for (i = 0; i < childs.size(); i++) {
			generateFollowTable(childs.at(i));
		}
	}
	else if (current->type == NodeType::Procedure) {
		generateFollowTable(childs.at(0));

	} else if (current->type == NodeType::StmtLst) {
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
			vi::const_iterator last = transistivelyFollows.begin() + childs.size() - 1;
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
	else if (current->type == NodeType::If){
		generateFollowTable(childs.at(1)); // Then Stmt
		generateFollowTable(childs.at(2)); // Else Stmt
		return;
	}


}

//get Stmt that is following Stmt. --- TO:DO --- NOT DONE BECAUSE I'M CONFUSED
vi Follow::getStmtFollowingStmt(int lineNo, NodeType type) {
	PKB pkb = PKB::getInstance();
	vi result;

	if (lineNo != -1) {
		if (type == NodeType::StmtLst || pkb.getStmt(followedBy[lineNo]).second->type == type) {
			result.push_back(followedBy[lineNo]);
		}
		return result;
	}

	// lineNo = -1, iterate the entire table. Returns all Statements that are Modified.
	bool checkType = true;
	if (type == NodeType::StmtLst) {
		checkType = false;
	}

	map_i_i::iterator it;
	map_i_si::iterator itSet;
	si resultSet;

	for (it = followedBy.begin(); it != followedBy.end(); it++) {
		if (checkType) {
			if (pkb.getStmt(it->second).second->type == type)
			{
				resultSet.insert(it->second);
			}
		}
		else {
			resultSet.insert(it->second);
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

// get Stmt that is followed by Stmt
vi Follow::getStmtFollowedByStmt(int lineNo, NodeType type) {
	PKB pkb = PKB::getInstance();
	vi result;

	if (lineNo != -1) {
		if (type == NodeType::StmtLst || pkb.getStmt(follows[lineNo]).second->type == type) {
			result.push_back(follows[lineNo]);
		}
		return result;
	}

	// lineNo = -1, iterate the entire table. Returns all Statements that are Modified.
	bool checkType = true;
	if (type == NodeType::StmtLst) {
		checkType = false;
	}

	map_i_i::iterator it;
	map_i_si::iterator itSet;
	si resultSet;

	for (it = follows.begin(); it != follows.end(); it++) {
		if (checkType) {
			if (pkb.getStmt(it->second).second->type == type)
			{
				resultSet.insert(it->second);
			}
		}
		else {
			resultSet.insert(it->second);
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

bool Follow::whetherFollows(int a, int b) {	
	return (followedBy[b] == a);
}
vi Follow::getStmtTransitivelyFollowedByStmt(int lineNo, NodeType type) {
	return followedByT[lineNo];
}
vi  Follow::getStmtTransitivelyFollowingStmtT(int lineNo, NodeType type) {
	return followsT[lineNo];
}
bool Follow::whetherTransitivelyFollows(int a, int b) {
	vi stmts = followedByT[b];
	return (std::find(stmts.begin(), stmts.end(), a) != stmts.end());
}