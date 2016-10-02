#include "Modify.h"

//Generates the modify table using the root AST node
//Reject if node is not root
//Goes through each procedure and builds the table
//Builds reverse lookup table at the end.
int Modify::generateModifyTable(TNode* root) {
	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return 0;
	}

	for (TNode* procedure : root->childs) {
		si procVarSet;
		procVarSet = generateModifyTableForSingleProcedure(procedure,
															procedure->value);

		vi output(procVarSet.begin(), procVarSet.end());
		procVarTable.insert(make_pair(procedure->value, output));
	}

	//Update the Proc Modifying Var Tables if got multiple Proc (Not for Iteration 1)
	// updateProcModifyVarTable

	// updateModifyTableForCallStmtsAndTheirParents

	buildReverseTable(true);
	buildReverseTable(false);

	return 1;
}

// Builds the reverse lookup table for the original tables.
void Modify::buildReverseTable(bool stmtModify) {
	map_i_si modifiedBySet;
	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	map_i_vi *xModifyingVar;
	map_i_vi *modifiedByX;

	if (stmtModify) {
		xModifyingVar = &stmtVarTable;
		modifiedByX = &varStmtTable;
	}
	else {
		xModifyingVar = &procVarTable;
		modifiedByX = &varProcTable;
	}

	for (it = xModifyingVar->begin(); it != xModifyingVar->end(); it++) {
		for (int j : it->second) {
			modifiedBySet[j].insert(it->first);
		}
	}

	for (itSet = modifiedBySet.begin(); itSet != modifiedBySet.end(); itSet++) {
		(*modifiedByX)[itSet->first].assign(itSet->second.begin(),
											 itSet->second.end());
	}
}

void updateProcModifyVarTable() {
	//DO NOTHING FOR ITERATION 1
}

//Builds Modify Table for a single procedure.
//Returns variables that are modified.
si Modify::generateModifyTableForSingleProcedure(TNode* current, int procedure) {
	si addToTable;
	try {
		if (current->type == NodeType::Assign) {
			int varIndex;

			try {
				varIndex = current->childs.at(0)->value;
				addToTable.insert(varIndex);
			}
			catch (const std::out_of_range& oor) {
				std::cout << "Assign Node have no left child (Child index 0/Modified Var)\
								??? This error should never be reached.";
				exit(1);
			}
		}
		else if (current->type == NodeType::If) {
			si firstResult = generateModifyTableForSingleProcedure(current->childs.at(1),
																	procedure);
			si secondResult = generateModifyTableForSingleProcedure(current->childs.at(2),
																	 procedure);

			addToTable.insert(firstResult.begin(), firstResult.end());
			addToTable.insert(secondResult.begin(), secondResult.end());
		}
		else if (current->type == NodeType::While) {
			si result = generateModifyTableForSingleProcedure(current->childs.at(1),
																procedure);
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
				si result = generateModifyTableForSingleProcedure(child, procedure);
				addToTable.insert(result.begin(), result.end());
			}
			return addToTable;
		}

		vi output(addToTable.begin(), addToTable.end());
		stmtVarTable.insert(make_pair(current->statementNumber, output));
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return addToTable;
}

vi Modify::getModifySpecificGeneric(int lineNo, NodeType type) {
	PKB& pkb = PKB::getInstance();

	if (lineNo != -1) {
		if (type == NodeType::Procedure) {
			if (procVarTable.count(lineNo) == 1) {
				return procVarTable[lineNo];
			}
		}
		else if (type == NodeType::StmtLst ||
					pkb.getStmt(lineNo).second->type == type) {
			if (stmtVarTable.count(lineNo) == 1) {
				return stmtVarTable[lineNo];
			}
		}
		return vi();
	}

	// lineNo = -1, iterate the entire table. Returns all var that are Modified.
	map_i_vi* modifiedByX;
	bool checkType = true;

	if (type == NodeType::Procedure) {
		modifiedByX = &procVarTable;
		checkType = false;
	}
	else {
		modifiedByX = &stmtVarTable;
		if (type == NodeType::StmtLst) {
			checkType = false;
		}

	}

	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	vi result;
	si resultSet;

	for (it = (*modifiedByX).begin(); it != (*modifiedByX).end(); it++) {
		if (checkType) {
			if (pkb.getStmt(it->first).second->type == type) {
				for (int var : it->second) {
					resultSet.insert(var);
				}
			}
		}
		else {
			for (int var : it->second) {
				resultSet.insert(var);
			}
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

vi Modify::getModifyGenericSpecific(int varIndex, NodeType type) {
	PKB& pkb = PKB::getInstance();
	vi result;

	if (varIndex != -1) {
		if (type == NodeType::Procedure) {
			if (varProcTable.count(varIndex) == 1) {
				return varProcTable[varIndex];
			}
		}
		else if (type == NodeType::StmtLst) {
			if (varStmtTable.count(varIndex) == 1) {
				return varStmtTable[varIndex];
			}
		}
		else if (varStmtTable.count(varIndex) == 1) {
			for (int stmt : varStmtTable.at(varIndex)) {
				if (pkb.getStmt(stmt).second->type == type) {
					result.push_back(stmt);
				}
			}
		}

		return result;
	}

	// lineNo = -1, iterate the entire table. Returns all Statements that are Modified.
	map_i_vi* modifiedByX;
	bool checkType = true;

	if (type == NodeType::Procedure) {
		modifiedByX = &procVarTable;
		checkType = false;
	}
	else {
		modifiedByX = &stmtVarTable;
		if (type == NodeType::StmtLst) {
			checkType = false;
		}
	}

	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	si resultSet;

	for (it = (*modifiedByX).begin(); it != (*modifiedByX).end(); it++) {
		if (checkType) {
			if (pkb.getStmt(it->first).second->type == type) {
				if (it->second.size() > 0) {
					resultSet.insert(it->first);
				}
			}
		}
		else {
			if (it->second.size() > 0) {
				resultSet.insert(it->first);
			}
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

bool Modify::whetherProcModifies(int proc, int varIndex) {
	vi vars;

	if (procVarTable.count(proc) == 1) {
		vars = procVarTable.at(proc);
		return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
	}

	return false;
}

bool Modify::whetherStmtModifies(int lineNo, int varIndex) {
	vi vars;

	if (stmtVarTable.count(lineNo) == 1) {
		vars = stmtVarTable.at(lineNo);
		return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
	}

	return false;
}