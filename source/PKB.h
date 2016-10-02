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
#include "DesignExtractor.h"

#include "Follow.h"
#include "Modify.h"
#include "Uses.h"
#include "Parent.h"
#include "Pattern.h"

using namespace std;

class TNode;
enum class NodeType;

typedef short PROC;
typedef std::pair<std::string, TNode*> vpair;
typedef std::vector<int> vi;
typedef std::vector<TNode*> vt;


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


	//VarTable
	int storeVariable(std::string varName);
	int getVarIndexFromName(std::string varName);
	std::string getVarNameFromIndex(int varIndex);

	//StmtLst
	vpair getStmt(int stmtNo);
	int getStmtCount();
	NodeType getNodeTypeOfStmt(int stmtNo);


	void buildAllTables();
	void insertStatementBasedOnType(int stmtNo, TNode* stmt, NodeType type);

	//For Procedures and Variable Names
	std::vector<std::string> getAllEntityName(NodeType type);
	//For Procedures, Variable, Statements, Assign, Call, If, While
	vi getAllEntityIndex(NodeType type);
	//For Statements, Assign, Call, If, While
	vt getAllTNodesForStmt(NodeType type);

	//Follow
	int getStmtFollowedByStmt(int lineNo, NodeType type);
	int getStmtFollowingStmt(int lineNo, NodeType type);
	vi getStmtsFollowedByStmt(NodeType typeA, NodeType typeB);
	vi getStmtsFollowingStmt(NodeType typeA, NodeType typeB);

	bool whetherFollows(int a, int b);

	vi getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type);
	vi getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type);
	vi getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB);
	vi getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB);
	bool whetherTransitivelyFollows(int a, int b);

	//Modify
	vi getVarModifiedByStmt(int lineNo, NodeType type);
	vi getStmtModifyingVar(int varIndex, NodeType type);
	bool whetherProcModifies(int procedure, int varIndex);
	bool whetherStmtModifies(int lineNo, int varIndex);

	//Use
	vi getVarUsedByStmt(int lineNo, NodeType type);
	vi getStmtUsingVar(int varIndex, NodeType type);
	bool whetherProcUses(int procedure, int varIndex);
	bool whetherStmtUses(int lineNo, int varIndex);


	//Pattern
	vi getPatternAssign(int varIndex, string expr);

	//Parent
	vi getChildOfStmt(int lineNo, NodeType type);
	int getParentOfStmt(int lineNo, NodeType type);
	vi getTransitiveChildOfStmt(int lineNo, NodeType);
	vi getTransitiveParentOfStmt(int lineNo, NodeType);

	vi getChildOfStmt(NodeType typeA, NodeType typeB);
	vi getTransitiveChildOfStmt(NodeType typeA, NodeType typeB);
	vi getParentOfStmt(NodeType typeA, NodeType typeB);
	vi getTransitiveParentOfStmt(NodeType typeA, NodeType typeB);

	bool whetherParent(int lineNo, int lineNo2);
	bool whetherTransitiveParent(int lineNo, int lineNo2);

	//Call
	void addToCallTable(TNode* node, string procName);
};
