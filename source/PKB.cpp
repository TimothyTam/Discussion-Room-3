#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"
#include "AST.h"
#include "VarTable.h"
#include "ProcTable.h"

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
		for (i = 1; i < stmtList.size(); i++) {
			result.push_back(i);
		}
	}

	if (type == NodeType::Assign || type == NodeType::If || type == NodeType::While || type == NodeType::Call) {
		for (i = 1; i < stmtList.size(); i++) {
			if (stmtList[i].second->type == type) {
				result.push_back(i);
			}
		}
	}

	return result;
}


int getStmtFollowedByStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtFollowedByStmt(lineNo, type);
}

int getStmtFollowingStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtFollowingStmt(lineNo, type);
}

vi getStmtsFollowedByStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsFollowedByStmt(typeA, typeB);
}

vi getStmtsFollowingStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsFollowingStmt(typeA, typeB);
}

bool whetherFollows(int a, int b) {
	return Follow::getInstance().whetherFollows(a, b);
}

vi getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtsTransitivelyFollowedByStmt(lineNo, type);
}

vi getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type) {
	return Follow::getInstance().getStmtsTransitivelyFollowingStmt(lineNo, type);
}

vi getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsTransitivelyFollowedByStmt(typeA, typeB);
}

vi getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsTransitivelyFollowingStmt(typeA, typeB);
}

bool whetherTransitivelyFollows(int a, int b) {
	return Follow::getInstance().whetherTransitivelyFollows(a, b);
}

vi getVarModifiedByStmt(int lineNo, NodeType type) {
	return Modify::getInstance().getVarModifiedByStmt(lineNo, type);
}

vi getStmtModifyingVar(int varIndex, NodeType type) {
	return Modify::getInstance().getStmtModifyingVar(varIndex, type);
}

bool whetherProcModifies(int procedure, int varIndex) {
	return Modify::getInstance().whetherProcModifies(procedure, varIndex);
}

bool whetherStmtModifies(int lineNo, int varIndex) {
	return Modify::getInstance().whetherStmtModifies(lineNo, varIndex);
}

vi getVarUsedByStmt(int lineNo, NodeType type) {
	return Use::getInstance().getVarUsedByStmt(lineNo, type);
} 

vi getStmtUsingVar(int varIndex, NodeType type) {
	return Use::getInstance().getStmtUsingVar(varIndex, type);
}

bool whetherProcUses(int procedure, int varIndex) {
	return Use::getInstance().whetherProcUses(procedure, varIndex);
}

bool whetherStmtUses(int lineNo, int varIndex) {
	return Use::getInstance().whetherStmtUses(lineNo, varIndex);
}





