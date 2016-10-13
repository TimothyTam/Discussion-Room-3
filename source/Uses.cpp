#include "Uses.h"

//Generates the uses table using the root AST node
//Reject if node is not root
//Goes through each procedure and builds the table
//Builds reverse lookup table at the end.
int Use::generateUseTable(TNode* root) {
	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return 0;
	}

	for (int i = 0; i < root->childs.size(); i++) {
		procUsingProc[i] = si();
		procUsedByProc[i] = si();
	}

	for (TNode* procedure : root->childs) {
		si procVarSet;
		procVarSet = generateUseTableForSingleProcedure(procedure,
														 procedure->value);

		vi output(procVarSet.begin(), procVarSet.end());
		procVarTable.insert(make_pair(procedure->value, output));
	}

	//Update the Proc Using Var Tables if got multiple Proc (Not for Iteration 1)
	updateProcUsesVarTable();

	updateUsesTableForCallStmtsAndTheirParents();

	buildReverseTable(true);
	buildReverseTable(false);

	buildStmtPairs();
	buildProcPairs();

	build2DArrayTable();

	return 1;
}

// Builds the reverse lookup table for the original tables.
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

	for (itSet = UsedBySet.begin(); itSet != UsedBySet.end(); itSet++) {
		(*UsedByX)[itSet->first].assign(itSet->second.begin(),
										 itSet->second.end());
	}
}

//Builds Uses Table for a single procedure.
//Returns variables that are used.
si Use::generateUseTableForSingleProcedure(TNode* current, int procedure) {
	si addToTable;
	try {
		if (current->type == NodeType::Assign) {
			try {
				si result = getVarsInSubTree(current->childs.at(1));
				addToTable.insert(result.begin(), result.end());
			}
			catch (const std::out_of_range& oor) {
				std::cout << "Assign Node have no left child (Child index 0/Used Var)\
								??? This error should never be reached.";
				exit(1);
			}
		}
		else if (current->type == NodeType::If) {
			si firstResult = generateUseTableForSingleProcedure(current->childs.at(1),
																 procedure);
			si secondResult = generateUseTableForSingleProcedure(current->childs.at(2),
																  procedure);
			addToTable.insert(current->childs.at(0)->value);
			addToTable.insert(firstResult.begin(), firstResult.end());
			addToTable.insert(secondResult.begin(), secondResult.end());
		}
		else if (current->type == NodeType::While) {
			si result = generateUseTableForSingleProcedure(current->childs.at(1), procedure);
			addToTable.insert(current->childs.at(0)->value);
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

// Returns all variable in subtree.
si Use::getVarsInSubTree(TNode* current) {
	si result;
	if (current->type == NodeType::Variable) {
		result.insert(current->value);
	}
	else if (current->type == NodeType::Plus || 
			 current->type == NodeType::Minus || 
			 current->type == NodeType::Times) {
		si leftResult = getVarsInSubTree(current->childs[0]);
		si rightresult = getVarsInSubTree(current->childs[1]);

		result.insert(leftResult.begin(), leftResult.end());
		result.insert(rightresult.begin(), rightresult.end());
	}

	return result;
}

vi Use::getUsesSpecificGeneric(int lineNo, NodeType type) {
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

vi Use::getUsesGenericSpecific(int varIndex, NodeType type) {
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
		else {
			if (varStmtTable.count(varIndex) == 1) {
				for (int stmt : varStmtTable[varIndex]) {
					if (pkb.getStmt(stmt).second->type == type) {
						result.push_back(stmt);
					}
				}
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
	vi vars;

	if (procVarTable.count(proc) == 1) {
		vars = procVarTable[proc];
		return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
	}
	return false;
}

bool Use::whetherStmtUses(int lineNo, int varIndex) {
	vi vars;

	if (stmtVarTable.count(lineNo) == 1) {
		vars = stmtVarTable[lineNo];
		return (std::find(vars.begin(), vars.end(), varIndex) != vars.end());
	}
	return false;
}

//ITERATION 2
void Use::buildStmtPairs() {
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

void Use::buildProcPairs() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (it = procVarTable.begin(); it != procVarTable.end(); it++) {
		for (int i : it->second) {
			procPairs.push_back(make_pair(it->first, i));
		}
	}
}

vp_i_i Use::getUsesGenericGeneric(NodeType type) {
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

void Use::updateProcUsesVarTable() {
	map_i_si::iterator it;
	vi leafNode;

	while (procUsingProc.size() > 0) {
		//Find Proc that does not modify any Proc (Leaf Nodes)
		for (it = procUsingProc.begin(); it != procUsingProc.end(); it++) {
			if (it->second.size() == 0) {
				leafNode.push_back(it->first);
			}
		}

		//Add The Leaf Nodes Proc to Their Parents
		for (int i : leafNode) {
			si parents = procUsedByProc[i];
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
			procUsingProc.erase(i);
			si parents = procUsedByProc[i];
			for (int j : parents) {
				//procUsingProc[j].erase(i);

				si result = procUsingProc[j];
				result.erase(i);
				procUsingProc[j] = result;

			}
			procUsedByProc.erase(i);
		}


	}


}

void Use::updateUsesTableForCallStmtsAndTheirParents() {

	for (TNode* callNode : callsNodes) {
		callNode->statementNumber;
		vi varsToAdd = procVarTable[callNode->value];
		stmtVarTable[callNode->statementNumber] = varsToAdd;

		NodeType type = NodeType::Call;
		TNode* current = callNode;
		while (type != NodeType::Procedure) {
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

void Use::build2DArrayTable() {
	int tableHeight = PKB::getInstance().getStmtCount();
	int tableWidth = PKB::getInstance().getVarTableSize();

	for (int i = 0; i <= tableHeight; i++) {
		vector<int> width;
		for (int i = 0; i < tableWidth; i++) {
			width.push_back(0);
		}
		stmtVarArray.push_back(width);
	}

	for (int i = 1; i <= tableHeight; i++) {
		vi to = stmtVarTable[i];
		for (int j = 0; j < to.size(); j++) {
			stmtVarArray[i][to.at(j)] = 1;
		}
	}
}