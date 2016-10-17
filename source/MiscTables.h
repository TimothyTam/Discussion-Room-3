#pragma once
#include "vector"
#include "unordered_map"
#include "set"

#include "TNode.h"
#include "PKB.h"

#include <algorithm>

typedef std::vector<string> vs;

class MiscTables {
private:

	vs constants;
	si constantsInt;

	vs firstStatementOfStmtLsts;
	si firstStatementOfStmtLstsInt;

	MiscTables() {};

	void MiscTables::generateDataForMiscTablesSingleProc(TNode* current);
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
};