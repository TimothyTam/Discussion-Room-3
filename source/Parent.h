#pragma once
#include "vector"
#include "map"
#include "TNode.h"

typedef std::vector<int> vi;
typedef std::map<int, vi> map_i_vi;

class Parent {
private:
	std::map<int, int> parentOfStmt;
	map_i_vi childOfStmt;
	map_i_vi transitiveParentOfStmt;
	map_i_vi transitiveChildOfStmt;
	void buildFromNode(TNode* currentNode);
	void buildTransitiveData();
	vi buildTransitiveFromStmt(int currentStmt, std::vector<bool>* done);


public:
	Parent();
	
	static Parent& getInstance()
	{
		static Parent instance;
		return instance;
	}
	
	vi getChildOfStmt(int lineNo, NodeType type);
	int getParentOfStmt(int lineNo, NodeType type);

	vi getChildOfStmt(NodeType typeA, NodeType typeB);	// e.g. Select a Parent(a,w) typeA = assign, typeB = while
	int getParentOfStmt(NodeType typeA, NodeType typeB);// e.g. Select w Parent(a,w) typeA = assign, typeB = while

	bool whetherParent(int lineNo, int lineNo2);

	vi getTransitiveChildOfStmt(int lineNo, NodeType type);
	vi getTransitiveParentOfStmt(int lineNo, NodeType type);

	vi getTransitiveChildOfStmt(NodeType typeA, NodeType typeB);
	vi getTransitiveParentOfStmt(NodeType typeA, NodeType typeB);

	bool whetherTransitiveParent(int lineNo, int lineNo2);

	void generateParentData(TNode* rootNode);
};

