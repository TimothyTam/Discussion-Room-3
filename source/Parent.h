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
	vi buildTransitiveFromStmt(int currentStmt, vector<bool>* done);


public:
	Parent();
	
	static Parent& getInstance()
	{
		static Parent instance;
		return instance;
	}
	
	vi getChildOfStmt(int lineNo);
	int getParentOfStmt(int lineNo);
	bool whetherParent(int lineNo, int lineNo2);
	vi getTransitiveChildOfStmt(int lineNo);
	vi getTransitiveParentOfStmt(int lineNo);
	bool whetherTransitiveParent(int lineNo, int lineNo2);

	void generateParentData(TNode* rootNode);


};

