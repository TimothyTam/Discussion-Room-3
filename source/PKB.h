#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "TNode.h"
#include "AST.h"
#include "VarTable.h"
#include "ProcTable.h"

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


	//Used internally by AST
	int addProcedure(std::string procName);


	//VarTable
	int storeVariable(std::string varName);
	int getVarIndexFromName(std::string varName); // returns -1 if there is no such varName
	std::string getVarNameFromIndex(int varIndex); // returns NULL if varIndex < 0 or varIndex >= size of varTable

	//StmtLst
	vpair getStmt(int stmtNo);
	int getStmtCount();
	NodeType getNodeTypeOfStmt(int stmtNo);


	void PKB::buildAllTables();

	//For Procedures and Variable Names
	std::vector<std::string> getAllEntityName(NodeType type);
	//For Procedures, Variable, Statements, Assign, Call, If, While
	vi getAllEntityIndex(NodeType type);
	vt getAllTNodesForStmt(NodeType type);

	//Follow
	int getStmtFollowedByStmt(int lineNo, NodeType type);		// Follows(10, s1) (Only 1 Result for s1; the Stmt No. or 0)
	int getStmtFollowingStmt(int lineNo, NodeType type);		// Follows(s1, 10) (Only 1 Result for s1; the Stmt No. or 0)
	vi getStmtsFollowedByStmt(NodeType typeA, NodeType typeB);	// Select s2 Follows(s1,s2). typeA = s1.type
	vi getStmtsFollowingStmt(NodeType typeA, NodeType typeB);	// Select s1 Follows(s1,s2). typeA = s1.type

	bool whetherFollows(int a, int b);							// Follows(10, 11)

	vi getStmtsTransitivelyFollowedByStmt(int lineNo, NodeType type);		// Follows*(10, s1)
	vi getStmtsTransitivelyFollowingStmt(int lineNo, NodeType type);		// Follows*(s1, 10)
	vi getStmtsTransitivelyFollowedByStmt(NodeType typeA, NodeType typeB);	// Select s2 Follows*(s1,s2). typeA = s1.type
	vi getStmtsTransitivelyFollowingStmt(NodeType typeA, NodeType typeB);	// Select s1 Follows*(s1,s2). typeA = s1.type
	bool whetherTransitivelyFollows(int a, int b);							// Follows* (10, 11)
	
	//Modify
	
	vi getVarModifiedByStmt(int lineNo, NodeType type);			//Modifies(10, v)
	vi getStmtModifyingVar(int varIndex, NodeType type);		//Modifies(s, 10)
	bool whetherProcModifies(int procedure, int varIndex);
	bool whetherStmtModifies(int lineNo, int varIndex);

	//Use

	vi getVarUsedByStmt(int lineNo, NodeType type);				//Uses(10, v)
	vi getStmtUsingVar(int varIndex, NodeType type);			//Uses(s, 10)
	bool whetherProcUses(int procedure, int varIndex);
	bool whetherStmtUses(int lineNo, int varIndex);


	//Pattern
	vi getPatternAssign(int varIndex, string expr);



	//Parent
	vi getChildOfStmt(int lineNo, NodeType type);
	int getParentOfStmt(int lineNo, NodeType type);
	vi getTransitiveChildOfStmt(int lineNo, NodeType);
	vi getTransitiveParentOfStmt(int lineNo, NodeType);

	vi getChildOfStmt(NodeType typeA, NodeType typeB);	// e.g. Select a Parent(w,a) typeA = while, typeB = assign
	vi getTransitiveChildOfStmt(NodeType typeA, NodeType typeB); // Similar
	vi getParentOfStmt(NodeType typeA, NodeType typeB);  // e.g. Select w Parent(w,a) typeA = while, typeB = assign
	vi getTransitiveParentOfStmt(NodeType typeA, NodeType typeB); // Similar

	bool whetherParent(int lineNo, int lineNo2);
	bool whetherTransitiveParent(int lineNo, int lineNo2);



};
