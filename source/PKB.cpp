#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"
#include "../SPA/AST.h"
#include "../SPA/VarTable.h"
#include "../SPA/ProcTable.h"

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