#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

#include "Follow.h"
#include "Modify.h"
#include "Uses.h"
#include "Parent.h"

class TNode;
enum class NodeType;

typedef short PROC;
typedef std::pair<std::string, TNode*> vpair;
typedef std::vector<int> vi;


class VarTable;

class PKB {
private:
	std::vector<vpair> stmtList;
	int stmtCount;
public:
	
	PKB();
	static PKB& getInstance() {
		static PKB instance;
		return instance;
	}
	
	/* To be used by Parser*/
	TNode* createEntityNode(TNode* parent, NodeType type, std::string value);
	TNode* createConstantNode(TNode* parent, NodeType type, int value);
	int addStatement(std::string statement, TNode* node);


	int addProcedure(std::string procName);



	/* store a variable in VarTable if it does not exist
	*  also return the index of the variable, no matter whether exist or not */
	int storeVariable(std::string varName);

	// returns -1 if there is no such varName
	int getVarIndexFromName(std::string varName);

	// returns NULL if varIndex < 0 or varIndex >= size of varTable
	std::string getVarNameFromIndex(int varIndex);

	vpair getStmt(int stmtNo);
	int getStmtCount();


	void PKB::buildAllTables();

	//For Procedures and Variable Names
	std::vector<std::string> getAllEntity(NodeType type);
	//For Statements, Assign, Call, If, While
	vi getAllEntityForStmt(NodeType type);


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

	//Parent - Add after parent is updated.





};
