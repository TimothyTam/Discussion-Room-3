#include "Modify.h"

int Modify::generateModifyTable(TNode* root) {
	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return 0;
	}

	for (TNode* procedure : root->childs) {
		si procVarSet;
		procVarSet = generateModifyTableForSingleProcedure(procedure, procedure->value);

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

	//Doing this is less expensive than adding uniquely to vector. (Also sorts the result)

	for (itSet = modifiedBySet.begin(); itSet != modifiedBySet.end(); itSet++) {
		(*modifiedByX)[itSet->first].assign(itSet->second.begin(), itSet->second.end());
	}
}

void updateProcModifyVarTable() {
	//DO NOTHING FOR ITERATION 1
}

//Returns what is modified
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
				std::cout << "Assign Node have no left child (Child index 0/Modified Var) ??? This error should never be reached.";
				exit(1);
			}
		}
		else if (current->type == NodeType::If) {
			si firstResult = generateModifyTableForSingleProcedure(current->childs.at(1), procedure);
			si secondResult = generateModifyTableForSingleProcedure(current->childs.at(2), procedure);

			addToTable.insert(firstResult.begin(), firstResult.end());
			addToTable.insert(secondResult.begin(), secondResult.end());
		}
		else if (current->type == NodeType::While) {
			si result = generateModifyTableForSingleProcedure(current->childs.at(1), procedure);
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
	} catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return addToTable;
}

vi Modify::getVarModifiedByStmt(int lineNo, NodeType type) {
	PKB& pkb = PKB::getInstance();

	if (lineNo != -1) {
		if (type == NodeType::Procedure) {
			try {
				return procVarTable.at(lineNo);
			}
			catch (const std::out_of_range& oor) {
				return vi();
			}
		}
		if (type == NodeType::StmtLst || pkb.getStmt(lineNo).second->type == type) {
			try {
				return stmtVarTable.at(lineNo);
			}
			catch (const std::out_of_range& oor) {
				return vi();
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

vi Modify::getStmtModifyingVar(int varIndex, NodeType type) {
	PKB& pkb = PKB::getInstance();

	if (varIndex != -1) {
		if (type == NodeType::Procedure) {
			try {
				return varProcTable.at(varIndex);
			}
			catch (const std::out_of_range& oor) {
				return vi();
			}
		}
		if (type == NodeType::StmtLst) {
			try {
				return varStmtTable.at(varIndex);
			}
			catch (const std::out_of_range& oor) {
				return vi();
			}
		}

		vi result;
		try {
			for (int stmt : varStmtTable.at(varIndex)) {
				if (pkb.getStmt(stmt).second->type == type) {
					result.push_back(stmt);
				}
			}
		} catch (const std::out_of_range& oor) {
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
	vi result;
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
	try {
		vars = procVarTable.at(proc);
	}
	catch (const std::out_of_range& oor) {
	}
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}

bool Modify::whetherStmtModifies(int lineNo, int varIndex) {
	vi vars;
	try {
		vars = stmtVarTable.at(lineNo);
	}
	catch (const std::out_of_range& oor) {
	}
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}