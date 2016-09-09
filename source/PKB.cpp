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