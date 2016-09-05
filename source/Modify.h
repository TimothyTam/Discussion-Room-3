#pragma once
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

#include "TNode.h"

class Modify {
private:
	map<int, vector<int>> stmtModifyingVar;
	map<int, vector<int>> varModifiedByStmt;
	map<int, vector<int>> procModifyingVar;
	map<int, vector<int>> varModifiedByProc;
	set<int> generateModifyTableOfProcedure(TNode* current, int procedure);

	//Used internally to update other tables. Will be empty at the end.
	map<int, set<int>> procModifyingProc;
	map<int, set<int>> procModifiedByProc;
	vector<TNode*> callsNodes;

	void buildReverseModifyTable();
public:
	int generateModifyTable(TNode* root);
	vector<int> getVarModifiedByStmt(int lineNo);
	vector<int> getStmtModifyingVar(int varIndex);
	vector<int> getVarModifiedByProc(int procIndex);
	vector<int> getProcModifyingVar(int varIndex);
	bool whetherModify(string entity, int varIndex);
};