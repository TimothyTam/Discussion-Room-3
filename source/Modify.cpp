#include "Modify.h"


int Modify::generateModifyTable(TNode* root) {
	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return 0;
	}

	for (TNode* procedure : root->childs) {
		si procModifyingVarSet;
		procModifyingVarSet = generateModifyTableOfProcedure(procedure, procedure->value);

		vi output(procModifyingVarSet.begin(), procModifyingVarSet.end());
		procModifyingVar.insert(make_pair(procedure->value, output));
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
		xModifyingVar = &stmtModifyingVar;
		modifiedByX = &varModifiedByStmt;
	}
	else {
		xModifyingVar = &procModifyingVar;
		modifiedByX = &varModifiedByProc;
	}

	for (it = xModifyingVar->begin(); it != xModifyingVar->end(); it++) {
		for (int j : it->second) {
			si existing;
			existing = modifiedBySet[j];
			existing.insert(it->first);
		}
	}

	//Doing this is less expensive than adding uniquely to vector. (Also sorts the result)
	for (itSet = modifiedBySet.begin(); itSet != modifiedBySet.end(); it++) {
		vi vec = (*modifiedByX)[itSet->first];
		vec.assign(itSet->second.begin(), itSet->second.end());
	}
}

void updateProcModifyVarTable() {
	//DO NOTHING FOR ITERATION 1
}

//Returns what is modified
si Modify::generateModifyTableOfProcedure(TNode* current, int procedure) {
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
			si firstResult = generateModifyTableOfProcedure(current->childs.at(1), procedure);
			si secondResult = generateModifyTableOfProcedure(current->childs.at(2), procedure);

			addToTable.insert(firstResult.begin(), firstResult.end());
			addToTable.insert(secondResult.begin(), secondResult.end());
		}
		else if (current->type == NodeType::While) {
			si result = generateModifyTableOfProcedure(current->childs.at(1), procedure);
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
				si result = generateModifyTableOfProcedure(child, procedure);
				addToTable.insert(result.begin(), result.end());
			}
			return addToTable;
		}

		vi output(addToTable.begin(), addToTable.end());
		stmtModifyingVar.insert(make_pair(current->statementNumber, output));
	} catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
	}
	return addToTable;
}

vi Modify::getVarModifiedByStmt(int lineNo, NodeType type) {
	PKB pkb = PKB::getInstance();

	if (lineNo != -1) {
		if (type == NodeType::Procedure) {
			return varModifiedByProc[lineNo];
		}
		if (type == NodeType::StmtLst || pkb.getStmt(lineNo).second->type == type) {
			return varModifiedByStmt[lineNo];
		}
		return vi();
	}
	
	// lineNo = -1, iterate the entire table. Returns all var that are Modified.
	map_i_vi* xModifyingVar;
	map_i_vi* modifiedByX;
	bool checkType = true;

	if (type == NodeType::Procedure) {
		modifiedByX = &varModifiedByProc;
		checkType = false;
	}
	else {
		modifiedByX = &varModifiedByStmt;
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
			if (pkb.getStmt(it->first).second->type == type)
			{
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
	PKB pkb = PKB::getInstance();

	if (varIndex != -1) {
		if (type == NodeType::Procedure) {
			return procModifyingVar[varIndex];
		}
		if (type == NodeType::StmtLst) {
			return stmtModifyingVar[varIndex];
		}

		vi result;
		for (int stmt : stmtModifyingVar[varIndex]) {
			if (pkb.getStmt(stmt).second->type == type) {
				result.push_back(stmt);
			}
		}

		return result;
	}

	// lineNo = -1, iterate the entire table. Returns all Statements that are Modified.
	map_i_vi* xModifyingVar;
	map_i_vi* modifiedByX;
	bool checkType = true;

	if (type == NodeType::Procedure) {
		modifiedByX = &procModifyingVar;
		checkType = false;
	}
	else {
		modifiedByX = &stmtModifyingVar;
		if (type == NodeType::StmtLst) {
			checkType = false;
		}
	}

	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	vi result;
	si resultSet;

	//TO-DO. Use Reverse table to check first->type == type. Reduces a lot of time.
	for (it = (*modifiedByX).begin(); it != (*modifiedByX).end(); it++) {
		if (checkType) {
			for (int stmt : it->second) {
				if (pkb.getStmt(stmt).second->type == type)
				{
					resultSet.insert(stmt);
				}
			}
		}
		else {
			for (int stmt : it->second) {
				resultSet.insert(stmt);
			}
		}
	}

	result.assign(resultSet.begin(), resultSet.end());

	return result;
}

bool Modify::whetherProcModifies(int proc, int varIndex) {
	vi vars = varModifiedByProc[proc];
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}

bool Modify::whetherStmtModifies(int lineNo, int varIndex) {
	vi vars = varModifiedByStmt[lineNo];
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}