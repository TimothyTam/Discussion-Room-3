#pragma once
#include "vector"
#include "map"
#include "set"
#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::map<int, vi> map_i_vi;
typedef std::map<int, si> map_i_si;
typedef std::map<int, int> map_i_i;

class Follow {
private:
	// key	| Value
	map_i_i follows;		// 1	| 2
	map_i_i followedBy;		// 2	| 1
	map_i_vi followsT;		// 1	| 2 3
	map_i_vi followedByT;	// 3	| 1 2
	Follow() {};

	bool isValidNodeType(NodeType type);
	int getStmtsXStmt(bool stmtFollowingStmt, int lineNo, NodeType type);
	vi getStmtsXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB);
	vi getStmtsTransitivelyXStmt(bool stmtFollowingStmt, int lineNo, NodeType type);
	vi getStmtsTransitivelyXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB);

public:
	static Follow& getInstance()
	{
		static Follow instance;
		return instance;
	}

	Follow(Follow const&) = delete;
	void operator=(Follow const&) = delete;

	void generateFollowTable(TNode* root);

	int getStmtFollowedByStmt(int lineNo, NodeType type);
	int getStmtFollowingStmt(int lineNo, NodeType type);
	vi getStmtsFollowedByStmt(NodeType typeA, NodeType typeB);
	vi getStmtsFollowingStmt(NodeType typeA, NodeType typeB);

	bool whetherFollows(int a, int b);

	vi getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type);
	vi getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type);
	vi getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB);
	vi getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB);
	bool whetherTransitivelyFollows(int a, int b);

};