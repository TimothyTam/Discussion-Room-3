#pragma once
#include "vector"
#include "map"
#include "set"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::map<int, vi> map_i_vi;
typedef std::map<int, si> map_i_si;
typedef std::map<int, int> map_i_i;

#include "TNode.h"
#include "PKB.h"

class Follow {
private:
	map_i_i follows;		// 1 | 2
	map_i_i followedBy;		// 2 | 1
	map_i_vi followsT;		// 1 | 2 3
	map_i_vi followedByT;	// 3 | 1 2
	Follow() {};

public:
	static Follow& getInstance()
	{
		static Follow instance;
		return instance;
	}

	Follow(Follow const&) = delete;
	void operator=(Follow const&) = delete;

	void generateFollowTable(TNode* root);

	vi getStmtFollowingStmt(int lineNo, NodeType type);		// Follows(s1, 10) (Only 1 Result if lineNo != -1)
	vi getStmtFollowedByStmt(int lineNo, NodeType type);		// Follows(10, s1) (Only 1 Result if lineNo != -1)
	bool whetherFollows(int a, int b);							// Follows(10, 11)

	vi getStmtTransitivelyFollowedByStmt(int lineNo, NodeType type);	// Follows*(10, s1)
	vi  getStmtTransitivelyFollowingStmtT(int lineNo, NodeType type);	// Follows*(s1, 10)
	bool whetherTransitivelyFollows(int a, int b);				//Follows* (10, 11)

};