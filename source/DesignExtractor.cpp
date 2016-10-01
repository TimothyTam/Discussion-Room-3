#pragma once
#include "DesignExtractor.h"

void DesignExtractor::buildAllTables(TNode* root) {
	Follow::getInstance().generateFollowTable(root);
	Modify::getInstance().generateModifyTable(root);
	Use::getInstance().generateUseTable(root);
	Parent::getInstance().generateParentData(root);
}

void DesignExtractor::extractStmtBasedOnType(vector<vpair> stmtList) {
	PKB& inst = PKB::getInstance();
	for (vpair stmt : stmtList) {
		inst.insertStatementBasedOnType(stmt.second->statementNumber, stmt.second, stmt.second->type);
	}
}