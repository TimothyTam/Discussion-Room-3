#pragma once
#include "vector"
#include "map"
#include "TNode.h"

typedef std::vector<int> vi;
typedef std::map<int, vi> map_i_vi;

class Parent {
private:
	//maps storing all the information
	std::map<int, int> parentOfStmt;
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
	
	//Filter the pairs of <parent,child> based on the statement types of 
	//the parent and the child, output vector of children if getChild or 
	//vector of parents if !getChild 
	vi filterChildMap(map_i_vi childMap, NodeType typeOfParent, NodeType typeOfChild, bool getChild);

public:
	Parent();

	static Parent& getInstance()
	{
		static Parent instance;
		return instance;
	}

	vi getChildOfStmt(int lineNo, NodeType type);
	int getParentOfStmt(int lineNo, NodeType type);
	vi getTransitiveChildOfStmt(int lineNo, NodeType);
	vi getTransitiveParentOfStmt(int lineNo, NodeType);
	vi getChildOfStmt(NodeType typeA, NodeType typeB);	// e.g. Select a Parent(w,a) typeA = while, typeB = assign
	vi getParentOfStmt(NodeType typeA, NodeType typeB);// e.g. Select w Parent(w,a) typeA = while, typeB = assign
	vi getTransitiveChildOfStmt(NodeType typeA, NodeType typeB);
	vi getTransitiveParentOfStmt(NodeType typeA, NodeType typeB);
	bool whetherParent(int lineNo, int lineNo2);
	bool whetherTransitiveParent(int lineNo, int lineNo2);
	void generateParentData(TNode* rootNode);
};

