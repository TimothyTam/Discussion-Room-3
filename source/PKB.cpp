#pragma once
#include "PKB.h"


PKB::PKB() {
	this->stmtList = vector <pair< string, TNode* >> ();
	stmtCount = 0;
}


TNode* PKB::createEntityNode(TNode* parent, NodeType type, string value) {
	return AST::getInstance().createEntityNode(parent, type, value);
}

TNode* PKB::createConstantNode(TNode* parent, NodeType type, int value) {
	return AST::getInstance().createConstantNode(parent, type, value);
}


int PKB::storeVariable(string varName) {
	return VarTable::getInstance().storeVariable(varName);
}

int PKB::getVarIndexFromName(string varName) {
	return VarTable::getInstance().getVarIndexFromName(varName);
}

string PKB::getVarNameFromIndex(int varIndex) {
	return VarTable::getInstance().getVarNameFromIndex(varIndex);
}

int PKB::addProcedure(string procName) {
	return ProcTable::getInstance().addProc(procName);
}

int PKB::addStatement(string statement, TNode* node) {
	this->stmtList.push_back(make_pair(statement, node));
	node->statementNumber = this->stmtCount;
	this->stmtCount++;
	return stmtCount - 1;
}

pair<string, TNode*>  PKB::getStmt(int stmtNo) {
	return this->stmtList[stmtNo];
}

int PKB::getStmtCount() {
	return this->stmtCount;
}

void PKB::buildAllTables() {
	TNode* root = AST::getInstance().rootNode;
	Follow::getInstance().generateFollowTable(root);
	Modify::getInstance().generateModifyTable(root);
	Use::getInstance().generateUseTable(root);
	Parent::getInstance().generateParentData(root);
	for (vpair stmt : stmtList) {
		if (stmt.second->type == NodeType::Assign) {
			this->assignStmt.push_back(stmt.second->statementNumber);
		}
		else if (stmt.second->type == NodeType::If) {
			this->ifStmt.push_back(stmt.second->statementNumber);
		}
		else if (stmt.second->type == NodeType::While) {
			this->whileStmt.push_back(stmt.second->statementNumber);
		}
		else if (stmt.second->type == NodeType::Call) {
			this->callStmt.push_back(stmt.second->statementNumber);
		}
	}
}

vector<string> PKB::getAllEntity(NodeType type) {
	vector<string> result;
	if (type == NodeType::Procedure) {
		result = ProcTable::getInstance().indexToName;
	}
	if (type == NodeType::Variable) {
		//Immediately returns the name. Please raise issue on chat if index is needed.
		result = VarTable::getInstance().indexToName;
	}

	return result;
}

vi PKB::getAllEntityForStmt(NodeType type) {
	vector<int> result;
	size_t i;
	if (type == NodeType::StmtLst) {
		//Returns Vector of 1 to LastStmtNo
		for (i = 1; i < stmtList.size()+1; i++) {
			result.push_back(i);
		}
	}

	if (type == NodeType::Assign) {
		return assignStmt;
	}
	else if (type == NodeType::If) {
		return ifStmt;
	}
	else if (type == NodeType::While) {
		return whileStmt;
	}
	else if (type == NodeType::Call) {
		return callStmt;
	}

	return result;
}


int PKB::getStmtFollowedByStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtFollowedByStmt(lineNo, type);
}

int PKB::getStmtFollowingStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtFollowingStmt(lineNo, type);
}

vi PKB::getStmtsFollowedByStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsFollowedByStmt(typeA, typeB);
}

vi PKB::getStmtsFollowingStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsFollowingStmt(typeA, typeB);
}

bool PKB::whetherFollows(int a, int b) {
	return Follow::getInstance().whetherFollows(a, b);
}

vi PKB::getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtsTransitivelyFollowedByStmt(lineNo, type);
}

vi PKB::getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtsTransitivelyFollowingStmt(lineNo, type);
}

vi PKB::getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsTransitivelyFollowedByStmt(typeA, typeB);
}

vi PKB::getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsTransitivelyFollowingStmt(typeA, typeB);
}

bool PKB::whetherTransitivelyFollows(int a, int b) {
	return Follow::getInstance().whetherTransitivelyFollows(a, b);
}

vi PKB::getVarModifiedByStmt(int lineNo, NodeType type) {
	return Modify::getInstance().getVarModifiedByStmt(lineNo, type);
}

vi PKB::getStmtModifyingVar(int varIndex, NodeType type) {
	return Modify::getInstance().getStmtModifyingVar(varIndex, type);
}

bool PKB::whetherProcModifies(int procedure, int varIndex) {
	return Modify::getInstance().whetherProcModifies(procedure, varIndex);
}

bool PKB::whetherStmtModifies(int lineNo, int varIndex) {
	return Modify::getInstance().whetherStmtModifies(lineNo, varIndex);
}

vi PKB::getVarUsedByStmt(int lineNo, NodeType type) {
	return Use::getInstance().getVarUsedByStmt(lineNo, type);
} 

vi PKB::getStmtUsingVar(int varIndex, NodeType type) {
	return Use::getInstance().getStmtUsingVar(varIndex, type);
}

bool PKB::whetherProcUses(int procedure, int varIndex) {
	return Use::getInstance().whetherProcUses(procedure, varIndex);
}

bool PKB::whetherStmtUses(int lineNo, int varIndex) {
	return Use::getInstance().whetherStmtUses(lineNo, varIndex);
}

vi PKB::getPatternAssign(int varIndex, string expression) {
	return Pattern::getInstance().getPatternAssign(varIndex, expression);
}



