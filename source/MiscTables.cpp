#include "MiscTables.h"

void MiscTables::generateDataForMiscTables(TNode* root) {

	if (root->type != NodeType::Program) {
		printf("Only accepts Program Root Node");
		return;
	}
	int procIndex = 0;
	for (TNode* procedure : root->childs) {
		generateDataForMiscTablesSingleProc(procedure, procIndex);
		procIndex++;
	}

	for (int stmtNo : firstStatementOfStmtLstsInt) {
		firstStatementOfStmtLsts.push_back(to_string(stmtNo));
	}

	for (int constant : constantsInt) {
		constants.push_back(to_string(constant));
	}
}

void MiscTables::generateDataForMiscTablesSingleProc(TNode* current, int procIndex) {
	if (current->type == NodeType::StmtLst) {
		firstStatementOfStmtLstsInt.insert(current->childs[0]->statementNumber);
		for (TNode* child : current->childs) {
			generateDataForMiscTablesSingleProc(child, procIndex);
		}
	}
	else if (current->type == NodeType::If) {
		generateDataForMiscTablesSingleProc(current->childs[1], procIndex);
		generateDataForMiscTablesSingleProc(current->childs[2], procIndex);
		stmtNoToProcIndex[current->statementNumber] = procIndex;
	}
	else if (current->type == NodeType::While) {
		generateDataForMiscTablesSingleProc(current->childs[1], procIndex);
		stmtNoToProcIndex[current->statementNumber] = procIndex;
	}
	else if (current->type == NodeType::Procedure) {
		generateDataForMiscTablesSingleProc(current->childs[0], procIndex);
	}
	else if (current->type == NodeType::Call || current->type == NodeType::Assign) {
		stmtNoToProcIndex[current->statementNumber] = procIndex;
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

int MiscTables::getProcIndexFromStmtNo(int stmtNo) {
	if (stmtNoToProcIndex.count(stmtNo) == 1) {
		return stmtNoToProcIndex[stmtNo];
	}
	return -1;
}

bool MiscTables::areInSameProc(int stmtNo, int stmtNo2) {
	return (stmtNoToProcIndex[stmtNo] == stmtNoToProcIndex[stmtNo2]);
}