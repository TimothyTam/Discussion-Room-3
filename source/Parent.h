#pragma once
#include "vector"
#include "unordered_map"
#include "TNode.h"

typedef std::vector<int> vi;
typedef std::unordered_map<int, vi> map_i_vi;
typedef std::vector<std::pair<int, int>> vp_i_i;
typedef std::vector<std::vector<std::vector<std::pair<int, int>>>> stmtPairFollow;

using namespace std;

class Parent {
private:
	Parent() {};

	//maps storing all the information
	std::unordered_map<int, int> parentOfStmt;
	map_i_vi childOfStmt;
	map_i_vi transitiveParentOfStmt;
	map_i_vi transitiveChildOfStmt;

	//recursively build the tables storing Parent relationships
	void buildFromNode(TNode* currentNode);

	//build the star tables
	void buildTransitiveData();

	//recursive function to build the star tables
	vi buildTransitiveFromStmt(int currentStmt, std::vector<bool>* done);

	//Filter the stmt# based on the statement type (while, assign, if, call)
	vi filterStmts(vi stmList, NodeType type);
	
	//Iter2

	//All stmt pairs. Sorted by Type.
	// [0] = Assign, [1] = While, [2] = If, [3] = Call
	// To get Parent(a,w), stmtPairs[0][1];
	// To get Parent(s,w), stmtPairs[0-3][1];
	// To get Parent(a,s), stmtPairs[0][0-3];
	stmtPairFollow stmtPairs;
	stmtPairFollow stmtTransPairs;

	vp_i_i getParentGenericGeneric(NodeType typeA, NodeType typeB, bool transitive);

	void buildStmtPairs();
	void buildStmtPairsFollow();
	void buildStmtPairsFollowTrans();

	int getLocationOfStmt(NodeType type);

public:
	
	static Parent& getInstance()
	{
		static Parent instance;
		return instance;
	}

	Parent(Parent const&) = delete;
	void operator=(Parent const&) = delete;

	vi getParentSpecificGeneric(int lineNo, NodeType type);
	int getParentGenericSpecific(int lineNo, NodeType type);
	vi getTransitiveParentSpecificGeneric(int lineNo, NodeType);
	vi getTransitiveParentGenericSpecific(int lineNo, NodeType);

	vp_i_i getParentGenericGeneric(NodeType type1, NodeType type2);  // select <a,w> such that Parent(a,w)
	vp_i_i getTransitiveParentGenericGeneric(NodeType type1, NodeType type2); // select <a,w> such that Parent*(a,w)

	bool whetherParent(int lineNo, int lineNo2);
	bool whetherTransitiveParent(int lineNo, int lineNo2);
	void generateParentData(TNode* rootNode);
};

