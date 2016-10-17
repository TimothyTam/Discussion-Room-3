#include "MiscTables.h"

void MiscTables::generateDataForMiscTables(TNode* root) {

	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return;
	}

	for (TNode* procedure : root->childs) {
		generateDataForMiscTablesSingleProc(procedure);
	}

	for (int stmtNo : firstStatementOfStmtLstsInt) {
		firstStatementOfStmtLsts.push_back(to_string(stmtNo));
	}

	for (int constant : constantsInt) {
		constants.push_back(to_string(constant));
	}
}

void MiscTables::generateDataForMiscTablesSingleProc(TNode* current) {
	if (current->type == NodeType::StmtLst) {
		firstStatementOfStmtLstsInt.insert(current->childs[0]->statementNumber);
		for (TNode* child : current->childs) {
			generateDataForMiscTablesSingleProc(child);
		}
	}
	else if (current->type == NodeType::If) {
		generateDataForMiscTablesSingleProc(current->childs[1]);
		generateDataForMiscTablesSingleProc(current->childs[2]);
	}
	else if (current->type == NodeType::While) {
		generateDataForMiscTablesSingleProc(current->childs[1]);
	}
	else if (current->type == NodeType::Procedure) {
		generateDataForMiscTablesSingleProc(current->childs[0]);
	}
}

void MiscTables::addConstantToStorage(int constant) {
	constantsInt.insert(constant);
}

vs MiscTables::getAllEntityName(NodeType type) {
	if (type == NodeType::Constant) {
		return constants;
	}
	else if (type == NodeType::StmtLst) {
		return firstStatementOfStmtLsts;
	}
	return vs();
}