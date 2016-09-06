#include "Modify.h"


int Modify::generateModifyTable(TNode* root) {
	vector<TNode*> procedures = root->childs;
	size_t i;
	for (i = 0; i < procedures.size(); i++) {
		generateModifyTableOfProcedure(root, procedures.at(i)->value); 
	}

	//Update the Proc Modifying Var Tables if got multiple Proc (Not for Iteration 1)
	// updateProcModifyVarTable
	
	buildReverseModifyTable();

	return 1;
}

//Extra Heavy because building reverse lookup table from Modify Table. Might consider generating this table while building modifying table.
// Or not as Generating Table time not taken into account for marks :)
void Modify::buildReverseModifyTable() {
	size_t i;

	for (i = 0; i < stmtModifyingVar.size(); i++) {
		vector<int> modified = stmtModifyingVar[i];
		for (int j : modified) {
			vector<int> result;
			result = varModifiedByStmt[j];
			if (!(std::find(result.begin(), result.end(), i) != result.end())) {
				result.push_back(i);
			}
		}
	}
}

void updateProcModifyVarTable() {
	//DO NOTHING FOR ITERATION 1
}

void addUniquelyToTable(map<int, vector<int>> table, int key, int value) {
	vector<int> result;

	result = table[key];
	if (!(std::find(result.begin(), result.end(), value) != result.end())) {
		result.push_back(value);
	}
	table[key] = result;
}

//Returns what is modified
set<int> Modify::generateModifyTableOfProcedure(TNode* current, int procedure) {
	set<int> addToTable;

	if (current->type == NodeType::Assign) {
		int varIndex;

		try {
			varIndex = current->childs.at(0)->value;
		}
		catch (const std::out_of_range& oor) {
			cout << "Assign Node have no left child (Child index 0/Modified Var) ??? This error should never be reached.";
			exit(1);
		}
		
		addToTable.insert(varIndex);

		//Add to procModifyingVar Table
		addUniquelyToTable(procModifyingVar, varIndex, procedure);
		addUniquelyToTable(varModifiedByProc, procedure, varIndex); 
		
	}
	else if (current->type == NodeType::If) {
		set<int> firstResult = generateModifyTableOfProcedure(current->childs.at(1), procedure);
		set<int> secondResult = generateModifyTableOfProcedure(current->childs.at(2), procedure);

		addToTable.insert(firstResult.begin(), firstResult.end());
		addToTable.insert(secondResult.begin(), secondResult.end());
	}
	else if (current->type == NodeType::While) {
		set<int> result = generateModifyTableOfProcedure(current->childs.at(1), procedure);
		addToTable.insert(result.begin(), result.end());
	}
	else if (current->type == NodeType::Call) {
		//NOT USED IN ITERATION 1.
		int procIndex;
		procIndex = current->value;

		procModifyingProc[procedure].insert(procIndex);
		procModifiedByProc[procIndex].insert(procedure);
		callsNodes.push_back(current);

		return addToTable;
	}
	else {
		//Go to each child and carry on.
		for (TNode* child : current->childs) {
			set<int> result = generateModifyTableOfProcedure(child, procedure);
			addToTable.insert(result.begin(), result.end());
		}
		return addToTable;
	}
	vector<int> output(addToTable.begin(), addToTable.end());
	stmtModifyingVar.insert(pair<int, vector<int> >(current->statementNumber, output));

	return addToTable;
}


vector<int> Modify::getVarModifiedByStmt(int lineNo) {
	return varModifiedByStmt[lineNo];
}
vector<int> Modify::getStmtModifyingVar(int varIndex) {
	return stmtModifyingVar[varIndex];
}
vector<int> Modify::getVarModifiedByProc(int procIndex) {
	return varModifiedByProc[procIndex];
}
vector<int> Modify::getProcModifyingVar(int varIndex) {
	return procModifyingVar[varIndex];
}
bool Modify::whetherProcModifies(int proc, int varIndex) {
	vector<int> vars = varModifiedByProc[proc];
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}

bool Modify::whetherStmtModifies(int lineNo, int varIndex) {
	vector<int> vars = varModifiedByStmt[lineNo];
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}