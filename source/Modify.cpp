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

	for (int i = 0; i < root->childs.size(); i++) {
		procModifyingProc[i] = si();
		procModifiedByProc[i] = si();
	}

	for (TNode* procedure : root->childs) {
		si procVarSet;
		procVarSet = generateModifyTableForSingleProcedure(procedure,
															procedure->value);

		vi output(procVarSet.begin(), procVarSet.end());
		procVarTable.insert(make_pair(procedure->value, output));
	}

	//Update the Proc Modifying Var Tables if got multiple Proc (Not for Iteration 1)
	updateProcModifyVarTable();

	updateModifyTableForCallStmtsAndTheirParents();

	buildReverseTable(true);
	buildReverseTable(false);

	buildStmtPairs();
	buildProcPairs();

	build2DArrayTable();

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
	if (proc < 0 || proc >= procTableHeight || varIndex < 0 || varIndex >= tableWidth) return false;
	return procVarArray[proc][varIndex];
}


bool Modify::whetherStmtModifies(int lineNo, int varIndex) {
	if (lineNo < 1 || lineNo > tableHeight || varIndex < 0 || varIndex >= tableWidth) return false;
	return stmtVarArray[lineNo][varIndex];
}

//ITERATION 2
void Modify::buildStmtPairs() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();
	
	for (int i = 0; i < 4; i++) {
		stmtPairs.push_back(vp_i_i());
	}


	for (it = stmtVarTable.begin(); it != stmtVarTable.end(); it++) {
		NodeType type = inst.getStmt(it->first).second->type;
		int location = -1;
		
		if (type == NodeType::Assign) {
			location = 0;
		}
		else if (type == NodeType::While) {
			location = 1;
		}
		else if (type == NodeType::If) {
			location = 2;
		}
		else if (type == NodeType::Call) {
			location = 3;
		}

		for (int i : it->second) {
			stmtPairs[location].push_back(make_pair(it->first, i));
		}
	}
}

void Modify::buildProcPairs() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (it = procVarTable.begin(); it != procVarTable.end(); it++) {
		for (int i : it->second) {
			procPairs.push_back(make_pair(it->first, i));
		}
	}
}

vp_i_i Modify::getModifyGenericGeneric(NodeType type) {
	vp_i_i result;
	if (type == NodeType::Procedure) {
		return procPairs;
	}
	if (type == NodeType::StmtLst) {
		for (int i = 0; i < 4; i++) {
			result.insert(result.end(), stmtPairs[i].begin(), stmtPairs[i].end());
		}
		return result;
	}
	else if (type == NodeType::Assign) {
		return stmtPairs[0];
	}
	else if (type == NodeType::While) {
		return stmtPairs[1];
	}
	else if (type == NodeType::If) {
		return stmtPairs[2];
	}
	else if (type == NodeType::Call) {
		return stmtPairs[3];
	}
	return result;
}

void Modify::updateProcModifyVarTable() {
	map_i_si::iterator it;
	vi leafNode;

	while (procModifyingProc.size() > 0) {
		//Find Proc that does not modify any Proc (Leaf Nodes)
		for (it = procModifyingProc.begin(); it != procModifyingProc.end(); it++) {
			if (it->second.size() == 0) {
				leafNode.push_back(it->first);
			}
		}

		//Add The Leaf Nodes Proc to Their Parents
		for (int i : leafNode) {
			si parents = procModifiedByProc[i];
			for (int j : parents) {
				// Add vars the leaf proc modifies to procs that calls them
				vi leafVars = procVarTable[i];
				vi parentVars = procVarTable[j];

				parentVars.insert(parentVars.end(), leafVars.begin(), leafVars.end());

				//Sort and remove duplicate
				std::sort(parentVars.begin(), parentVars.end());
				parentVars.erase(unique(parentVars.begin(), parentVars.end()), parentVars.end());

				procVarTable[j] = parentVars;
			}
		}


		for (int i : leafNode) {
			procModifyingProc.erase(i);
			si parents = procModifiedByProc[i];
			for (int j : parents) {
				//procModifyingProc[j].erase(i);
				
				si result = procModifyingProc[j];
				result.erase(i);
				procModifyingProc[j] = result;

			}
			procModifiedByProc.erase(i);
		}


	}


}

void Modify::updateModifyTableForCallStmtsAndTheirParents() {

	for (TNode* callNode : callsNodes) {
		callNode->statementNumber;
		vi varsToAdd = procVarTable[callNode->value];
		stmtVarTable[callNode->statementNumber] = varsToAdd;

		NodeType type = NodeType::Call;
		TNode* current = callNode;
		while(type != NodeType::Procedure) {
			current = current->parent;
			type = current->type;
			if (type == NodeType::If || type == NodeType::While) {
				vi result = stmtVarTable[current->statementNumber];
				result.insert(result.end(), varsToAdd.begin(), varsToAdd.end());

				//Sort and remove duplicate
				std::sort(result.begin(), result.end());
				result.erase(unique(result.begin(), result.end()), result.end());
				stmtVarTable[current->statementNumber] = result;
			}
		}
	}
}

void Modify::build2DArrayTable() {
	tableHeight = PKB::getInstance().getStmtCount();
	tableWidth = PKB::getInstance().getVarTableSize();
	procTableHeight = PKB::getInstance().getProcTableSize();

	for (int i = 0; i <= tableHeight; i++) {
		vector<bool> width;
		for (int i = 0; i < tableWidth; i++) {
			width.push_back(false);
		}
		stmtVarArray.push_back(width);
	}

	for (int i = 1; i <= tableHeight; i++) {
		vi to = stmtVarTable[i];
		for (int j = 0; j < to.size(); j++) {
			stmtVarArray[i][to.at(j)] = true;
		}
	}

	for (int i = 0; i < procTableHeight; i++) {
			vector<bool> width;
			for (int i = 0; i < tableWidth; i++) {
				width.push_back(false);
			}
			procVarArray.push_back(width);
		}

	for (int i = 0; i < procTableHeight; i++) {
		vi to = procVarTable[i];
		for (int j = 0; j < to.size(); j++) {
			procVarArray[i][to.at(j)] = true;
		}
	}

}