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

};
