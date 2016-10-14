#pragma once
#include "vector"
#include "unordered_map"
#include "set"
#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::unordered_map<int, vi> map_i_vi;
typedef std::unordered_map<int, si> map_i_si;
typedef std::unordered_map<int, int> map_i_i;
typedef std::vector<std::pair<int, int>> vp_i_i;
typedef std::vector<std::vector<std::vector<std::pair<int, int>>>> stmtPairFollow;

class Follow {
private:
	// key	| Value
	map_i_i follows;		// 1	| 2
	map_i_i followedBy;		// 2	| 1
	map_i_vi followsT;		// 1	| 2 3
	map_i_vi followedByT;	// 3	| 1 2
	Follow() {};

	//All stmt pairs. Sorted by Type.
	// [0] = Assign, [1] = While, [2] = If, [3] = Call
	// To get Follow(a,w), stmtPairs[0][1];
	// To get Follow(s,w), stmtPairs[0-3][1];
	// To get Follow(a,s), stmtPairs[0][0-3];
	stmtPairFollow stmtPairs;
	stmtPairFollow stmtTransPairs;

	bool isValidNodeType(NodeType type);
	int getStmtsXStmt(bool stmtFollowingStmt, int lineNo, NodeType type);
	vi getStmtsXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB);
	vi getStmtsTransitivelyXStmt(bool stmtFollowingStmt, int lineNo, NodeType type);
	vi getStmtsTransitivelyXStmt(bool stmtsFollowingStmt, NodeType typeA, NodeType typeB);
	vp_i_i getFollowGenericGeneric(NodeType typeA, NodeType typeB, bool transitive);


	void buildStmtPairs();
	void buildStmtPairsFollow();
	void buildStmtPairsFollowTrans();

	int getLocationOfStmt(NodeType type);

	void generateFollowTableRecursive(TNode* root);

public:
	static Follow& getInstance()
	{
		static Follow instance;
		return instance;
	}

	Follow(Follow const&) = delete;
	void operator=(Follow const&) = delete;

	void generateFollowTable(TNode* root);

	int getFollowSpecificGeneric(int lineNo, NodeType type);
	int getFollowGenericSpecific(int lineNo, NodeType type);

	vi getTransitiveFollowSpecificGeneric(int lineNo, NodeType type);
	vi getTransitiveFollowGenericSpecific(int lineNo, NodeType type);

	vp_i_i getFollowGenericGeneric(NodeType typeA, NodeType typeB);
	vp_i_i getTransitiveFollowGenericGeneric(NodeType typeA, NodeType typeB);

	bool whetherFollows(int a, int b);
	bool whetherTransitivelyFollows(int a, int b);

};