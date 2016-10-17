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

int PKB::getVarTableSize() {
	return VarTable::getInstance().size;
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
	CallTable::getInstance().generateCallTable(root);
	Follow::getInstance().generateFollowTable(root);
	Modify::getInstance().generateModifyTable(root);
	Use::getInstance().generateUseTable(root);
	Parent::getInstance().generateParentData(root);
	Pattern::getInstance().generatePatternData(root);
	Next::getInstance().generateNextTable();
	MiscTables::getInstance().generateDataForMiscTables(root);
	
	for (vpair stmt : stmtList) {
		insertStatementBasedOnType(stmt.second->statementNumber, stmt.second, stmt.second->type);
	}
	
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
	} else if (type == NodeType::Variable) {
		result = VarTable::getInstance().indexToName;
	} else if (type == NodeType::Constant || type == NodeType::StmtLst) {
		result = MiscTables::getInstance().getAllEntityName(type);
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

bool PKB::whetherParent(int lineNo1, int lineNo2) {
	return Parent::getInstance().whetherParent(lineNo1, lineNo2);
}

bool PKB::whetherTransitiveParent(int lineNo1, int lineNo2) {
	return Parent::getInstance().whetherTransitiveParent(lineNo1, lineNo2);
}


//ITERATION 2

//NEW APIs

vp_i_i PKB::getFollowGenericGeneric(NodeType type1, NodeType type2) {
	return Follow::getInstance().getFollowGenericGeneric(type1, type2);
}

vp_i_i PKB::getTransitiveFollowGenericGeneric(NodeType type1, NodeType type2) {
	return Follow::getInstance().getTransitiveFollowGenericGeneric(type1, type2);
}

vp_i_i PKB::getModifyGenericGeneric(NodeType type) {
	return Modify::getInstance().getModifyGenericGeneric(type);
}

vp_i_i PKB::getUsesGenericGeneric(NodeType type) {
	return Use::getInstance().getUsesGenericGeneric(type);
}

vp_i_i PKB::getParentGenericGeneric(NodeType type1, NodeType type2) {
	return Parent::getInstance().getParentGenericGeneric(type1, type2);
}
vp_i_i PKB::getTransitiveParentGenericGeneric(NodeType type1, NodeType type2) {
	return Parent::getInstance().getTransitiveParentGenericGeneric(type1, type2);
}

//Call
void PKB::addToCallNodeToStringTable(TNode* node, string procName) {
	CallTable::getInstance().addToCallNodeToStringTable(node, procName);
	
}

vp_i_i PKB::callsGenericGeneric() {
	return CallTable::getInstance().callsGenericGeneric();
}

vi PKB::callsSpecificGeneric(int procIndex) {
	return CallTable::getInstance().callsSpecificGeneric(procIndex);
}

vi PKB::callsGenericSpecific(int procIndex) {
	return CallTable::getInstance().callsGenericSpecific(procIndex);
}

vp_i_i PKB::callsTransitiveGenericGeneric() {
	return CallTable::getInstance().callsTransitiveGenericGeneric();
}

vi PKB::callsTransitiveSpecificGeneric(int procIndex) {
	return CallTable::getInstance().callsTransitiveSpecificGeneric(procIndex);
}

vi PKB::callsTransitiveGenericSpecific(int procIndex) {
	return CallTable::getInstance().callsTransitiveGenericSpecific(procIndex);
}

bool PKB::whetherCalls(int a, int b) {
	return CallTable::getInstance().whetherCalls(a,b);
}

bool PKB::whetherTransitiveCalls(int a, int b) {
	return CallTable::getInstance().whetherTransitiveCalls(a, b);
}
//Next
vp_i_i PKB::getNextGenericGeneric(NodeType typeA, NodeType typeB) {
	return Next::getInstance().getNextGenericGeneric(typeA, typeB);
}

vi PKB::getNextSpecificGeneric(int lineNo, NodeType type) {
	return Next::getInstance().getNextSpecificGeneric(lineNo, type);
}

vi PKB::getNextGenericSpecific(int lineNo, NodeType type) {
	return Next::getInstance().getNextGenericSpecific(lineNo, type);
}

vp_i_i PKB::getTransitiveNextGenericGeneric(NodeType typeA, NodeType typeB) {
	return Next::getInstance().getTransitiveNextGenericGeneric(typeA, typeB);
}

vi PKB::getTransitiveNextSpecificGeneric(int lineNo, NodeType type) {
	return Next::getInstance().getTransitiveNextSpecificGeneric(lineNo, type);
}

vi PKB::getTransitiveNextGenericSpecific(int lineNo, NodeType type) {
	return Next::getInstance().getTransitiveNextGenericSpecific(lineNo, type);
}

bool PKB::whetherNext(int a, int b) {
	return Next::getInstance().whetherNext(a, b);
}

bool PKB::whetherTransitiveNext(int a, int b) {
	return Next::getInstance().whetherTransitivelyNext(a, b);
}

//CFG
CFGNode* PKB::getCFGRootNode(int procIndex) {
	return CFG::getInstance().getRootNode(procIndex);
}
CFGNode* PKB::getCFGNodeFromStatement(int statementNumber) {
	return CFG::getInstance().getNodeFromStatement(statementNumber);
}

//Pattern

vi PKB::getPatternAssignSpecificGeneric(int stmtNoOfAssign, string expression) {
	return Pattern::getInstance().getPatternAssignSpecificGeneric(stmtNoOfAssign, expression);
}

vi PKB::getPatternIfSpecificGeneric(int stmtNoOfIf) {
	return Pattern::getInstance().getPatternIfSpecificGeneric(stmtNoOfIf);
}

vi PKB::getPatternWhileSpecificGeneric(int stmtNoOfWhile) {
	return Pattern::getInstance().getPatternWhileSpecificGeneric(stmtNoOfWhile);
}

vi PKB::getPatternAssignGenericSpecific(int varIndex, string expression) {
	return Pattern::getInstance().getPatternAssignGenericSpecific(varIndex, expression);
}

vi PKB::getPatternIfGenericSpecific(int varIndex) {
	return Pattern::getInstance().getPatternIfGenericSpecific(varIndex);
}

vi PKB::getPatternWhileGenericSpecific(int varIndex) {
	return Pattern::getInstance().getPatternWhileGenericSpecific(varIndex);
}

vp_i_i PKB::getPatternAssignGenericGeneric(string expression) {
	return Pattern::getInstance().getPatternAssignGenericGeneric(expression);
}
vp_i_i PKB::getPatternIfGenericGeneric() {
	return Pattern::getInstance().getPatternIfGenericGeneric();
}
vp_i_i PKB::getPatternWhileGenericGeneric() {
	return Pattern::getInstance().getPatternWhileGenericGeneric();
}

bool PKB::whetherPatternAssign(int assignStmt, int varIndex, string expression) {
	return Pattern::getInstance().whetherPatternAssign(assignStmt, varIndex, expression);
}
bool PKB::whetherPatternIf(int ifStmt, int varIndex) {
	return Pattern::getInstance().whetherPatternIf(ifStmt, varIndex);
}
bool PKB::whetherPatternWhile(int whileStmt, int varIndex) {
	return Pattern::getInstance().whetherPatternWhile(whileStmt, varIndex);
}

void PKB::newQuery() {
	Next::getInstance().newQuery();
}

string PKB::getCalledValue(int stmt) {
	if (stmt < 1 || stmt > stmtCount) {
		return "$";
	}
	if (stmtList[stmt].second->type != NodeType::Call) {
		return "$";
	}
	
	return ProcTable::getInstance().getProcNameFromIndex(stmtList[stmt].second->value);
}

void PKB::addConstantToStorage(int constant) {
	MiscTables::getInstance().addConstantToStorage(constant);
}