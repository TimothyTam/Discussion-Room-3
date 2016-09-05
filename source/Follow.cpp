#include "Follow.h"

void Follow::generateFollowTable(TNode* current) {
	vector<TNode> childs = current->childs;
	size_t i;

	if (current->type == NodeType::StmtLst) {
		vector<int> transistivelyfollowedBy;
		vector<int> transistivelyFollows;

		for (i = 1; i < childs.size(); i++) {
			transistivelyFollows.push_back(childs.at(i).statementNumber);
		}
	
		for (i = 0; i < childs.size() - 1; i++) {
			
			//add child[i].StmtNo and child[i+1].StmtNo into follow and followedBy table
			follows.insert(make_pair(childs.at(i).statementNumber, childs.at(i + 1).statementNumber));
			followedBy.insert(make_pair(childs.at(i + 1).statementNumber, childs.at(i).statementNumber));
			
			//add child[i] and child[i+1,2,3,4,childs.size()] into follow*(i,_) table
			vector<int>::const_iterator first = transistivelyFollows.begin() + i;
			vector<int>::const_iterator last = transistivelyFollows.begin() + childs.size() - 1;
			vector<int> newVec(first, last);

			followsT.insert(make_pair(childs.at(i).statementNumber, newVec));
			
			//add child[0,1,2,i-1] and child[i] into follows*(_,i) table
			followedByT.insert(make_pair(childs.at(i).statementNumber, transistivelyfollowedBy));
			transistivelyfollowedBy.push_back(childs.at(i).statementNumber);
		}

		//add child[0,1,2,n-2] and child[childs.size()-1] into follows*(_,i) table
		if (childs.size() >= 2) {
			followedByT.insert(make_pair(childs.at(childs.size() - 1).statementNumber, transistivelyfollowedBy));
		}

	
		for (i = 0; i < childs.size(); i++) {
			generateFollowTable(&(childs.at(i)));
		}

	}
	else if (current->type == NodeType::While) {
		generateFollowTable(&(childs.at(1)));
	}
	else if (current->type == NodeType::If){
		generateFollowTable(&(childs.at(1))); // Then Stmt
		generateFollowTable(&(childs.at(2))); // Else Stmt
	}
}


int Follow::getStmtFollowingStmt(int lineNo) {
	return follows[lineNo];
}

int Follow::getStmtFollowedByStmt(int lineNo) {
	return followedBy[lineNo];
}

bool Follow::whetherFollows(int a, int b) {	
	return (followedBy[b] == a);
}
vector<int> Follow::getStmtTransitivelyFollowedByStmt(int lineNo) {
	return followedByT[lineNo];
}
vector<int>  Follow::getStmtTransitivelyFollowingStmtT(int lineNo) {
	return followsT[lineNo];
}
bool Follow::whetherTransitivelyFollows(int a, int b) {
	vector<int> stmts = followedByT[b];
	return (std::find(stmts.begin(), stmts.end(), a) != stmts.end());
}