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
	map_i_i follows;		// 1 | 2
	map_i_i followedBy;		// 2 | 1
	map_i_vi followsT;		// 1 | 2 3
	map_i_vi followedByT;	// 3 | 1 2
	Follow() {};

	bool isValidNodeType(NodeType type);
	vi getStmtsXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB);
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

	int getStmtFollowedByStmt(int lineNo, NodeType type);		// Follows(10, s1) (Only 1 Result for s1; the Stmt No. or 0)
	int getStmtFollowingStmt(int lineNo, NodeType type);		// Follows(s1, 10) (Only 1 Result for s1; the Stmt No. or 0)
	vi getStmtsFollowedByStmt(NodeType typeA, NodeType typeB);	// Select s2 Follows(s1,s2). typeA = s1.type
	vi getStmtsFollowingStmt(NodeType typeA, NodeType typeB);	// Select s1 Follows(s1,s2). typeA = s1.type

	bool whetherFollows(int a, int b);							// Follows(10, 11)

	vi getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type);		// Follows*(10, s1)
	vi getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type);		// Follows*(s1, 10)
	vi getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB);	// Select s2 Follows*(s1,s2). typeA = s1.type
	vi getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB);	// Select s1 Follows*(s1,s2). typeA = s1.type
	bool whetherTransitivelyFollows(int a, int b);				//Follows* (10, 11)

};