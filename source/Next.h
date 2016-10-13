#pragma once
#include "set"
#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::unordered_map<int, vi> map_i_vi;
typedef std::unordered_map<int, si> map_i_si;
typedef std::unordered_map<int, int> map_i_i;
typedef std::vector<std::pair<int, int>> vp_i_i;
typedef std::vector<std::vector<std::vector<std::pair<int, int>>>> stmtPairNext;


class Next {
private:
	// key	| Value
	map_i_vi next;			// 1	| 2,3 //Max 2
	map_i_vi nextReverse;	// 2	| 1 //Max infinite

	map_i_vi nextTrans;			// 1	| 2,3 //Max 2
	map_i_vi nextTransReverse;	// 2	| 1 //Max infinite


	//All stmt pairs. Sorted by Type.
	// [0] = Assign, [1] = While, [2] = If, [3] = Call
	// To get Next(a,w), stmtPairs[0][1];
	// To get Next(s,w), stmtPairs[0-3][1];
	// To get Next(a,s), stmtPairs[0][0-3];
	stmtPairNext stmtPairs;
	stmtPairNext stmtTransPairs;

	Next() {};

	//I LOVE A LOT OF TABLES. <3
	void updateNextTable(CFGNode* root);
	void buildTransitiveTable();
	void buildReverseTable();
	void buildStmtPairs();

	int getLocationOfStmt(NodeType type);

	bool isNewQuery;
	
	void buildTransitiveTableForProcedure(CFGNode* current, map<CFGNode*, int> &visited);
	void depthFirstSearch(CFGNode* current, int stmtNoOfStartNode, int typeOfStartNode);
	void resetIsVisitedFlag(CFGNode* current);

public:
	static Next& getInstance()
	{
		static Next instance;
		return instance;
	}

	Next(Next const&) = delete;
	void operator=(Next const&) = delete;

	void generateNextTable();

	vi getNextSpecificGeneric(int lineNo, NodeType type);
	vi getNextGenericSpecific(int lineNo, NodeType type);
	vp_i_i getNextGenericGeneric(NodeType typeA, NodeType typeB);
	bool whetherNext(int a, int b);

	vi getTransitiveNextSpecificGeneric(int lineNo, NodeType type);
	vi getTransitiveNextGenericSpecific(int lineNo, NodeType type);
	vp_i_i getTransitiveNextGenericGeneric(NodeType typeA, NodeType typeB);
	bool whetherTransitivelyNext(int a, int b);

	void newQuery();
};