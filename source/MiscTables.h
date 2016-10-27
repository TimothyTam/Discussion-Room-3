#pragma once
#include "vector"
#include "unordered_map"
#include "set"

#include "TNode.h"
#include "PKB.h"

#include <algorithm>

typedef std::vector<string> vs;
typedef std::unordered_map<int, int> map_i_i;

class MiscTables {
private:

	vs constants;
	si constantsInt;

	vs firstStatementOfStmtLsts;
	si firstStatementOfStmtLstsInt;

	map_i_i stmtNoToProcIndex;

	MiscTables() {};

	void MiscTables::generateDataForMiscTablesSingleProc(TNode* current, int procIndex);
public:
	static MiscTables& getInstance()
	{
		static MiscTables instance;
		return instance;
	}

	MiscTables(MiscTables const&) = delete;
	void operator=(MiscTables const&) = delete;

	void addConstantToStorage(int constant);

	void generateDataForMiscTables(TNode* astRoot);

	vs getAllEntityName(NodeType type);

	int getProcIndexFromStmtNo(int stmtNo);

	bool areInSameProc(int stmtNo, int stmtNo2);
};