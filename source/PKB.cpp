#pragma once
#include "PKB.h"


PKB::PKB() {
	this->stmtList = vector <pair< string, TNode* >> ();
	stmtCount = 0;
	TNode dummyNode = TNode(NodeType::Invalid);
	this->stmtList.push_back(make_pair(string("Dummy Node to fill stmt[0]"), &dummyNode));
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
	this->stmtCount++;
	node->statementNumber = this->stmtCount;
	return stmtCount - 1;
}

pair<string, TNode*>  PKB::getStmt(int stmtNo) {
	if (stmtNo == 0 || stmtNo > stmtCount) {
		return this->stmtList[0];
	}
	return this->stmtList[stmtNo];
}

int PKB::getStmtCount() {
	return this->stmtCount;
}

NodeType PKB::getNodeTypeOfStmt(int stmtNo)
{
	return this->stmtList[stmtNo].second->type;
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
			this->assignTNodes.push_back(stmt.second);
		}
		else if (stmt.second->type == NodeType::If) {
			this->ifStmt.push_back(stmt.second->statementNumber);
			this->ifTNodes.push_back(stmt.second);
		}
		else if (stmt.second->type == NodeType::While) {
			this->whileStmt.push_back(stmt.second->statementNumber);
			this->whileTNodes.push_back(stmt.second);
		}
		else if (stmt.second->type == NodeType::Call) {
			this->callStmt.push_back(stmt.second->statementNumber);
			this->callTNodes.push_back(stmt.second);
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
	vi result;
	size_t i;
	if (type == NodeType::StmtLst) {
		//Returns Vector of 1 to LastStmtNo
		for (i = 1; i < getStmtCount() + 1; i++) {
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

vt PKB::getAllTNodesForStmt(NodeType type) {
	vt result;
	
	if (type == NodeType::StmtLst) {
		//Returns Vector of 1 to LastStmtNo
		for (vpair stmtPair : stmtList) {
			result.push_back(stmtPair.second);
		}
	}

	if (type == NodeType::Assign) {
		return assignTNodes;
	}
	else if (type == NodeType::If) {
		return ifTNodes;
	}
	else if (type == NodeType::While) {
		return whileTNodes;
	}
	else if (type == NodeType::Call) {
		return callTNodes;
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


// -----------------------------------------------------------------Parent ---

vi PKB::getChildOfStmt(int lineNo, NodeType type)
{
	return Parent::getInstance().getChildOfStmt(lineNo, type);
}

int PKB::getParentOfStmt(int lineNo, NodeType type)
{
	return Parent::getInstance().getParentOfStmt(lineNo, type);
}

vi PKB::getTransitiveChildOfStmt(int lineNo, NodeType type) {
	return Parent::getInstance().getTransitiveChildOfStmt(lineNo, type);
}

vi PKB::getTransitiveParentOfStmt(int lineNo, NodeType type) {
	return Parent::getInstance().getTransitiveParentOfStmt(lineNo, type);
}

vi PKB::getChildOfStmt(NodeType a, NodeType b) {
	return Parent::getInstance().getChildOfStmt(a, b);
}

vi PKB::getParentOfStmt(NodeType a, NodeType b) {
	return Parent::getInstance().getParentOfStmt(a, b);
}

vi PKB::getTransitiveChildOfStmt(NodeType a, NodeType b) {
	return Parent::getInstance().getTransitiveChildOfStmt(a, b);
}

vi PKB::getTransitiveParentOfStmt(NodeType a, NodeType b) {
	return Parent::getInstance().getTransitiveParentOfStmt(a, b);
}

bool PKB::whetherParent(int lineNo1, int lineNo2) {
	return Parent::getInstance().whetherParent(lineNo1, lineNo2);
}

bool PKB::whetherTransitiveParent(int lineNo1, int lineNo2) {
	return Parent::getInstance().whetherTransitiveParent(lineNo1, lineNo2);
}
