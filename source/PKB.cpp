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

// returns -1 if there is no such varName
int PKB::getVarIndexFromName(string varName) {
	return VarTable::getInstance().getVarIndexFromName(varName);
}

// returns NULL if varIndex < 0 or varIndex >= size of varTable
string PKB::getVarNameFromIndex(int varIndex) {
	return VarTable::getInstance().getVarNameFromIndex(varIndex);
}

int PKB::addProcedure(string procName) {
	return ProcTable::getInstance().addProc(procName);
}

string PKB::getProcNameFromIndex(int procIndex) {
	return ProcTable::getInstance().getProcNameFromIndex(procIndex);
}
int PKB::getProcIndexFromName(string procName) {
	return ProcTable::getInstance().getProcIndexFromName(procName);
}
int PKB::getProcTableSize() {
	return ProcTable::getInstance().getSize();
}

int PKB::addStatement(string statement, TNode* node) {
	this->stmtList.push_back(make_pair(statement, node));
	this->stmtCount++;
	node->statementNumber = this->stmtCount;
	return stmtCount;
}

void PKB::addProcedureForCFG(int procIndex) {
	CFG::getInstance().addProcedure(procIndex);
}

CFGNode* PKB::addStatementForCFG(int statementNumber, NodeType type, CFGNode* from) {
	return CFG::getInstance().addStatement(statementNumber, type, from);
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
	TNode* root = AST::getInstance().getRootNode();
	DesignExtractor::buildAllTables(root);
	DesignExtractor::extractStmtBasedOnType(stmtList);
}

void PKB::insertStatementBasedOnType(int stmtNo, TNode* stmt, NodeType type) {
	if (type == NodeType::Assign) {
		this->assignStmt.push_back(stmtNo);
		this->assignTNodes.push_back(stmt);
	}
	else if (type == NodeType::If) {
		this->ifStmt.push_back(stmtNo);
		this->ifTNodes.push_back(stmt);
	}
	else if (type == NodeType::While) {
		this->whileStmt.push_back(stmtNo);
		this->whileTNodes.push_back(stmt);
	}
	else if (type == NodeType::Call) {
		this->callStmt.push_back(stmtNo);
		this->callTNodes.push_back(stmt);
	}
}

vector<string> PKB::getAllEntityName(NodeType type) {
	vector<string> result;
	if (type == NodeType::Procedure) {
		result = ProcTable::getInstance().indexToName;
	}
	if (type == NodeType::Variable) {
		result = VarTable::getInstance().indexToName;
	}

	return result;
}

vi PKB::getAllEntityIndex(NodeType type) {
	vi result;
	size_t i;
	if (type == NodeType::Procedure) {
		for (i = 0; i < ProcTable::getInstance().indexToName.size(); i++) {
			result.push_back(i);
		}
	}

	if (type == NodeType::Variable) {
		for (i = 0; i < VarTable::getInstance().indexToName.size(); i++) {
			result.push_back(i);
		}
	}

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

// Follows
// Follows(10, s1) (Only 1 Result for s1; the Stmt No. or 0)
int PKB::getFollowSpecificGeneric(int lineNo, NodeType type) {
	return Follow::getInstance().getFollowSpecificGeneric(lineNo, type);
}

// Follows(s1, 10) (Only 1 Result for s1; the Stmt No. or 0)
int PKB::getFollowGenericSpecific(int lineNo, NodeType type) {
	return Follow::getInstance().getFollowGenericSpecific(lineNo, type);
}

// Select s2 Follows(s1,s2). typeA = s1.type
vi PKB::getStmtsFollowedByStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsFollowedByStmt(typeA, typeB);
}

// Select s1 Follows(s1,s2). typeA = s1.type
vi PKB::getStmtsFollowingStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsFollowingStmt(typeA, typeB);
}

bool PKB::whetherFollows(int a, int b) {
	return Follow::getInstance().whetherFollows(a, b);
}

// Follows*(10, s1)
vi PKB::getTransitiveFollowSpecificGeneric(int lineNo, NodeType type) {
	return Follow::getInstance().getTransitiveFollowSpecificGeneric(lineNo, type);
}

// Select s2 Follows*(s1,s2). typeA = s1.type
vi PKB::getTransitiveFollowGenericSpecific(int lineNo, NodeType type) {
	return Follow::getInstance().getTransitiveFollowGenericSpecific(lineNo, type);
}

// Select s1 Follows*(s1,s2). typeA = s1.type
vi PKB::getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsTransitivelyFollowedByStmt(typeA, typeB);
}

// Follows* (10, 11)
vi PKB::getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB) {
	return Follow::getInstance().getStmtsTransitivelyFollowingStmt(typeA, typeB);
}

bool PKB::whetherTransitivelyFollows(int a, int b) {
	return Follow::getInstance().whetherTransitivelyFollows(a, b);
}

// Modifies
vi PKB::getModifySpecificGeneric(int lineNo, NodeType type) {
	return Modify::getInstance().getModifySpecificGeneric(lineNo, type);
}

vi PKB::getModifyGenericSpecific(int varIndex, NodeType type) {
	return Modify::getInstance().getModifyGenericSpecific(varIndex, type);
}

bool PKB::whetherProcModifies(int procedure, int varIndex) {
	return Modify::getInstance().whetherProcModifies(procedure, varIndex);
}

bool PKB::whetherStmtModifies(int lineNo, int varIndex) {
	return Modify::getInstance().whetherStmtModifies(lineNo, varIndex);
}

// Uses
vi PKB::getUsesSpecificGeneric(int lineNo, NodeType type) {
	return Use::getInstance().getUsesSpecificGeneric(lineNo, type);
} 

vi PKB::getUsesGenericSpecific(int varIndex, NodeType type) {
	return Use::getInstance().getUsesGenericSpecific(varIndex, type);
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


// Parent

vi PKB::getParentSpecificGeneric(int lineNo, NodeType type)
{
	return Parent::getInstance().getParentSpecificGeneric(lineNo, type);
}

int PKB::getParentGenericSpecific(int lineNo, NodeType type)
{
	return Parent::getInstance().getParentGenericSpecific(lineNo, type);
}

vi PKB::getTransitiveParentSpecificGeneric(int lineNo, NodeType type) {
	return Parent::getInstance().getTransitiveParentSpecificGeneric(lineNo, type);
}

vi PKB::getTransitiveParentGenericSpecific(int lineNo, NodeType type) {
	return Parent::getInstance().getTransitiveParentGenericSpecific(lineNo, type);
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


//ITERATION 2

//NEW APIs
vp_i_i PKB::getModifyGenericGeneric(NodeType type) {
	return Modify::getInstance().getModifyGenericGeneric(type);
}

//Call
void PKB::addToCallNodeToStringTable(TNode* node, string procName) {
	CallTable::getInstance().addToCallNodeToStringTable(node, procName);
}
