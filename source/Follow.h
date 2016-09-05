#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

#include "TNode.h"

class Follow {
private:
	map<int, int> follows;		//Follows(s1, 10)
	map<int, int> followedBy;	//Follows(10, si)
	map<int, vector<int>> followsT; 
	map<int, vector<int>> followedByT; 
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
	int getStmtFollowedByStmt(int lineNo);						// Follows(10, s1) (Only 1 Result; Returns 0 if none)
	int getStmtFollowingStmt(int lineNo);						// Follows(s1, 10) (Only 1 Result; Returns 0 if none)
	bool whetherFollows(int a, int b);							// Follows(10, 11)
	vector<int> getStmtTransitivelyFollowedByStmt(int lineNo);	// Follows*(10, s1)
	vector<int>  getStmtTransitivelyFollowingStmtT(int lineNo);	// Follows*(s1, 10)
	bool whetherTransitivelyFollows(int a, int b);				//Follows* (10, 11)

};