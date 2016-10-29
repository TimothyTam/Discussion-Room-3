#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "TNode.h"
#include "AST.h"
#include "CFG.h"
#include "VarTable.h"
#include "ProcTable.h"

#include "Follow.h"
#include "Modify.h"
#include "Uses.h"
#include "Parent.h"
#include "Pattern.h"
#include "CallTable.h"
#include "Next.h"
#include "MiscTables.h"
#include "Affect.h"


using namespace std;

class TNode;
enum class NodeType;

typedef short PROC;
typedef std::pair<std::string, TNode*> vpair;
typedef std::vector<int> vi;
typedef std::vector<TNode*> vt;
typedef std::vector<pair<int, int>> vp_i_i;


class VarTable;

class PKB {
private:
	std::vector<vpair> stmtList;
	int stmtCount;

	vi assignStmt;
	vi ifStmt;
	vi whileStmt;
	vi callStmt;

	vt assignTNodes;
	vt ifTNodes;
	vt whileTNodes;
	vt callTNodes;

	PKB();

	void insertStatementBasedOnType(int stmtNo, TNode* stmt, NodeType type);
public:
	static PKB& getInstance() {
		static PKB instance;
		return instance;
	}
	PKB(PKB const&) = delete;
	void operator=(PKB const&) = delete;

	/* To be used by Parser*/
	TNode* createEntityNode(TNode* parent, NodeType type, std::string value);
	TNode* createConstantNode(TNode* parent, NodeType type, int value);
	int addStatement(std::string statement, TNode* node);
	void addProcedureForCFG(int procIndex);
	CFGNode* addStatementForCFG(int statementNumber, NodeType type, CFGNode* from);

	//Used internally by AST
	int addProcedure(std::string procName);

	//ProcTable
	std::string getProcNameFromIndex(int procIndex);
	int getProcIndexFromName(string procName);
	int getProcTableSize();

	//VarTable
	int storeVariable(std::string varName);
	int getVarIndexFromName(std::string varName);
	std::string getVarNameFromIndex(int varIndex);	
	int getVarTableSize();

	//StmtLst
	vpair getStmt(int stmtNo);
	int getStmtCount();
	NodeType getNodeTypeOfStmt(int stmtNo);

	void buildAllTables();
	
	//For Procedures and Variable Names
	std::vector<std::string> getAllEntityName(NodeType type);
	//For Procedures, Variable, Statements, Assign, Call, If, While
	vi getAllEntityIndex(NodeType type);
	//For Statements, Assign, Call, If, While
	vt getAllTNodesForStmt(NodeType type);

	//Follow
	int getFollowSpecificGeneric(int lineNo, NodeType type);
	int getFollowGenericSpecific(int lineNo, NodeType type);
	bool whetherFollows(int a, int b);

	vi getTransitiveFollowSpecificGeneric(int lineNo, NodeType type);
	vi getTransitiveFollowGenericSpecific(int lineNo, NodeType type);
	bool whetherTransitivelyFollows(int a, int b);

	//Modify
	vi getModifySpecificGeneric(int lineNo, NodeType type);
	vi getModifyGenericSpecific(int varIndex, NodeType type);
	bool whetherProcModifies(int procedure, int varIndex);
	bool whetherStmtModifies(int lineNo, int varIndex);

	//Use
	vi getUsesSpecificGeneric(int lineNo, NodeType type);
	vi getUsesGenericSpecific(int varIndex, NodeType type);
	bool whetherProcUses(int procedure, int varIndex);
	bool whetherStmtUses(int lineNo, int varIndex);

	//Parent
	vi getParentSpecificGeneric(int lineNo, NodeType type);
	int getParentGenericSpecific(int lineNo, NodeType type);
	vi getTransitiveParentSpecificGeneric(int lineNo, NodeType);
	vi getTransitiveParentGenericSpecific(int lineNo, NodeType);

	bool whetherParent(int lineNo, int lineNo2);
	bool whetherTransitiveParent(int lineNo, int lineNo2);

	//Iteration 2

	//New APIs
	vp_i_i getFollowGenericGeneric(NodeType type1, NodeType type2); 
	vp_i_i getTransitiveFollowGenericGeneric(NodeType type1, NodeType type2); 

	vp_i_i getModifyGenericGeneric(NodeType type); 
	vp_i_i getUsesGenericGeneric(NodeType type); 

	vp_i_i getParentGenericGeneric(NodeType type1, NodeType type2); 
	vp_i_i getTransitiveParentGenericGeneric(NodeType type1, NodeType type2); 

	//Call
	void addToCallNodeToStringTable(TNode* node, string procName);

	CFGNode* getCFGRootNode(int procIndex);
	CFGNode* getCFGNodeFromStatement(int statementNumber);

	//Call
	vp_i_i callsGenericGeneric();
	vi callsSpecificGeneric(int procIndex);
	vi callsGenericSpecific(int procIndex);
	vp_i_i callsTransitiveGenericGeneric();
	vi callsTransitiveSpecificGeneric(int procIndex);
	vi callsTransitiveGenericSpecific(int procIndex);
	bool whetherCalls(int a, int b);
	bool whetherTransitiveCalls(int a, int b);

	//Next
	vp_i_i getNextGenericGeneric(NodeType typeA, NodeType typeB);
	vi getNextSpecificGeneric(int lineNo, NodeType type);
	vi getNextGenericSpecific(int lineNo, NodeType type);
	vp_i_i getTransitiveNextGenericGeneric(NodeType typeA, NodeType typeB);
	vi getTransitiveNextSpecificGeneric(int lineNo, NodeType type);
	vi getTransitiveNextGenericSpecific(int lineNo, NodeType type);
	bool whetherNext(int a, int b);
	bool whetherTransitiveNext(int a, int b);

	//Clears Next*, Affect and Affect* table
	void newQuery();

	//Pattern
	vi getPatternAssignGenericSpecific(int varIndex, string expr);
	vi getPatternAssignSpecificGeneric(int stmtNoOfAssign, string expression);
	vp_i_i getPatternAssignGenericGeneric(string expr);

	vi getPatternIfGenericSpecific(int varIndex);
	vi getPatternIfSpecificGeneric(int stmtNoOfIf);
	vp_i_i getPatternIfGenericGeneric();

	vi getPatternWhileGenericSpecific(int varIndex);
	vi getPatternWhileSpecificGeneric(int stmtNoOfWhile);
	vp_i_i getPatternWhileGenericGeneric();

	bool whetherPatternAssign(int assignStmt, int varIndex, string expression);
	bool whetherPatternIf(int ifStmt, int varIndex);
	bool whetherPatternWhile(int whileStmt, int varIndex);

	string getCalledValue(int stmtNo);

	void addConstantToStorage(int constant);

	int getProcIndexFromStmtNo(int stmtNo);

	bool areInSameProc(int stmtNo, int stmtNo2);

	//Affect
	
	vi getAffectSpecificGeneric(int lineNo);
	vi getAffectGenericSpecific(int lineNo);
	vp_i_i getAffectGenericGeneric();
	bool whetherAffect(int lineNo, int lineNo2);
	
	vi getTransitiveAffectSpecificGeneric(int lineNo);
	vi getTransitiveAffectGenericSpecific(int lineNo);
	vp_i_i getTransitiveAffectGenericGeneric();
	bool whetherTransitiveAffect(int lineNo, int lineNo2);
	
};
