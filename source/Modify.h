#pragma once
#include "vector"
#include "map"
#include "set"

#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::map<int, vi> map_i_vi;
typedef std::map<int, si> map_i_si;
typedef std::vector<std::pair<int, int>> vp_i_i;

class Modify {
private:
	map_i_vi stmtVarTable;			//Stmt No	| Var
	map_i_vi varStmtTable;			//Var		| Stmt No
	map_i_vi procVarTable;			//Proc No	| Var
	map_i_vi varProcTable;			//VAr		| Proc
	si generateModifyTableForSingleProcedure(TNode* current, int procedure);

	//Used internally to update other tables. Will be empty at the end.
	map_i_si procModifyingProc;
	map_i_si procModifiedByProc;
	std::vector<TNode*> callsNodes;

	void buildReverseTable(bool stmtModify);

	Modify() {};

public:
	static Modify& getInstance()
	{
		static Modify instance;
		return instance;
	}

	Modify(Modify const&) = delete;
	void operator=(Modify const&) = delete;

	int generateModifyTable(TNode* root);

	vi getModifySpecificGeneric(int lineNo, NodeType type);
	vi getModifyGenericSpecific(int varIndex, NodeType type);

	vp_i_i getModifyGenericGeneric(NodeType type); // Select <w,v> such that uses(w,v)

	bool whetherProcModifies(int procedure, int varIndex);
	bool whetherStmtModifies(int lineNo, int varIndex);
};