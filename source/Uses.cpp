#include "Uses.h"

int Use::generateUseTable(TNode* root) {
	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return 0;
	}

	for (TNode* procedure : root->childs) {
		si procVarSet;
		procVarSet = generateUseTableForSingleProcedure(procedure, procedure->value);

		vi output(procVarSet.begin(), procVarSet.end());
		procVarTable.insert(make_pair(procedure->value, output));
	}

	//Update the Proc Using Var Tables if got multiple Proc (Not for Iteration 1)
	// updateProcUseVarTable

	// updateUseTableForCallStmtsAndTheirParents

	buildReverseTable(true);
	buildReverseTable(false);

	return 1;
}

void Use::buildReverseTable(bool stmtUse) {
	map_i_si UsedBySet;
	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	map_i_vi *xUsingVar;
	map_i_vi *UsedByX;

	if (stmtUse) {
		xUsingVar = &stmtVarTable;
		UsedByX = &varStmtTable;
	}
	else {
		xUsingVar = &procVarTable;
		UsedByX = &varProcTable;
	}

	for (it = xUsingVar->begin(); it != xUsingVar->end(); it++) {
		for (int j : it->second) {
			UsedBySet[j].insert(it->first);
		}
	}

	//Doing this is less expensive than adding uniquely to vector. (Also sorts the result)

	for (itSet = UsedBySet.begin(); itSet != UsedBySet.end(); itSet++) {
		(*UsedByX)[itSet->first].assign(itSet->second.begin(), itSet->second.end());
	}
}

void updateProcUseVarTable() {
	//DO NOTHING FOR ITERATION 1
}

//Returns what is Used
si Use::generateUseTableForSingleProcedure(TNode* current, int procedure) {
	si addToTable;
	try {
		if (current->type == NodeType::Assign) {
			try {
				si result = getVarsInSubTree(current->childs.at(1));
				addToTable.insert(result.begin(), result.end());
			}
			catch (const std::out_of_range& oor) {
				std::cout << "Assign Node have no left child (Child index 0/Used Var) ??? This error should never be reached.";
				exit(1);
			}
		}
		else if (current->type == NodeType::If) {
			si firstResult = generateUseTableForSingleProcedure(current->childs.at(1), procedure);
			si secondResult = generateUseTableForSingleProcedure(current->childs.at(2), procedure);

			addToTable.insert(firstResult.begin(), firstResult.end());
			addToTable.insert(secondResult.begin(), secondResult.end());
		}
		else if (current->type == NodeType::While) {
			si result = generateUseTableForSingleProcedure(current->childs.at(1), procedure);
			addToTable.insert(result.begin(), result.end());
		}
		else if (current->type == NodeType::Call) {
			//NOT USED IN ITERATION 1.
			int procIndex;
			procIndex = current->value;

			procUsingProc[procedure].insert(procIndex);
			procUsedByProc[procIndex].insert(procedure);
			callsNodes.push_back(current);

			return addToTable;
		}
		else {
			//Go to each child and carry on.
			for (TNode* child : current->childs) {
				si result = generateUseTableForSingleProcedure(child, procedure);
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

si Use::getVarsInSubTree(TNode* current) {
	si result;
	if (current->type == NodeType::Variable) {
		result.insert(current->value);
	}
	else if (current->type == NodeType::Plus || current->type == NodeType::Minus || current->type == NodeType::Times) {
		si leftResult = getVarsInSubTree(current->childs[0]);
		si rightresult = getVarsInSubTree(current->childs[1]);

		result.insert(leftResult.begin(), leftResult.end());
		result.insert(rightresult.begin(), rightresult.end());
	}

	return result;
}

vi Use::getVarUsedByStmt(int lineNo, NodeType type) {
	PKB& pkb = PKB::getInstance();

	if (lineNo != -1) {
		if (type == NodeType::Procedure) {
			return procVarTable[lineNo];
		}
		if (type == NodeType::StmtLst || pkb.getStmt(lineNo).second->type == type) {
			return stmtVarTable[lineNo];
		}
		return vi();
	}

	// lineNo = -1, iterate the entire table. Returns all var that are Used.
	map_i_vi* UsedByX;
	bool checkType = true;

	if (type == NodeType::Procedure) {
		UsedByX = &procVarTable;
		checkType = false;
	}
	else {
		UsedByX = &stmtVarTable;
		if (type == NodeType::StmtLst) {
			checkType = false;
		}

	}

	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	vi result;
	si resultSet;

	for (it = (*UsedByX).begin(); it != (*UsedByX).end(); it++) {
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

vi Use::getStmtUsingVar(int varIndex, NodeType type) {
	PKB& pkb = PKB::getInstance();

	if (varIndex != -1) {
		if (type == NodeType::Procedure) {
			return varProcTable[varIndex];
		}
		if (type == NodeType::StmtLst) {
			return varStmtTable[varIndex];
		}

		vi result;
		for (int stmt : varStmtTable[varIndex]) {
			if (pkb.getStmt(stmt).second->type == type) {
				result.push_back(stmt);
			}
		}

		return result;
	}

	// lineNo = -1, iterate the entire table. Returns all Statements that are Used.
	map_i_vi* UsedByX;
	bool checkType = true;

	if (type == NodeType::Procedure) {
		UsedByX = &procVarTable;
		checkType = false;
	}
	else {
		UsedByX = &stmtVarTable;
		if (type == NodeType::StmtLst) {
			checkType = false;
		}
	}

	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	vi result;
	si resultSet;

	for (it = (*UsedByX).begin(); it != (*UsedByX).end(); it++) {
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

bool Use::whetherProcUses(int proc, int varIndex) {
	vi vars = procVarTable[proc];
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}

bool Use::whetherStmtUses(int lineNo, int varIndex) {
	vi vars = stmtVarTable[lineNo];
	return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
}