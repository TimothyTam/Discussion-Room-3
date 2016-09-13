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

class Use {
private:
	map_i_vi stmtVarTable;			//Stmt No	| Var
	map_i_vi varStmtTable;			//Var		| Stmt No
	map_i_vi procVarTable;			//Proc No	| Var
	map_i_vi varProcTable;			//VAr		| Proc
	si generateUseTableForSingleProcedure(TNode* current, int procedure);

	//Used internally to update other tables. Will be empty at the end.
	map_i_si procUsingProc;
	map_i_si procUsedByProc;
	std::vector<TNode*> callsNodes;

	void buildReverseTable(bool stmtUse);
	si getVarsInSubTree(TNode* current);

	Use() {};

public:
	static Use& getInstance()
	{
		static Use instance;
		return instance;
	}

	Use(Use const&) = delete;
	void operator=(Use const&) = delete;

	int generateUseTable(TNode* root);
	vi getVarUsedByStmt(int lineNo, NodeType type);
	vi getStmtUsingVar(int varIndex, NodeType type);
	bool whetherProcUses(int procedure, int varIndex);
	bool whetherStmtUses(int lineNo, int varIndex);
};