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

class Use {
private:
	map_i_vi stmtVarTable;			//Stmt No	| Var
	map_i_vi varStmtTable;			//Var		| Stmt No
	map_i_vi procVarTable;			//Proc No	| Var
	map_i_vi varProcTable;			//VAr		| Proc
	si generateUseTableForSingleProcedure(TNode* current, int procedure);

	//0 = Assign, 1 = While, 2 = If, 3 = Call
	std::vector<vp_i_i> stmtPairs;

	// Same table as procVarTable, only stored in pair
	vp_i_i procPairs;

	//Used internally to update other tables. Will be empty at the end.
	map_i_si procUsingProc;
	map_i_si procUsedByProc;
	std::vector<TNode*> callsNodes;

	void buildStmtPairs();
	void buildProcPairs();

	void buildReverseTable(bool stmtUse);
	si getVarsInSubTree(TNode* current);

	Use() {};

	void updateProcUsesVarTable();

	void updateUsesTableForCallStmtsAndTheirParents();

public:
	static Use& getInstance()
	{
		static Use instance;
		return instance;
	}

	Use(Use const&) = delete;
	void operator=(Use const&) = delete;

	int generateUseTable(TNode* root);
	vi getUsesSpecificGeneric(int lineNo, NodeType type);
	vi getUsesGenericSpecific(int varIndex, NodeType type);

	vp_i_i getUsesGenericGeneric(NodeType type); // Select <w,v> such that uses(w,v)

	bool whetherProcUses(int procedure, int varIndex);
	bool whetherStmtUses(int lineNo, int varIndex);
};