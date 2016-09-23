#pragma once
#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "RelTable.h"
#include "QueryUtility.h"

using namespace std;

class QueryValidation {
	
private:
	RelTable table;
	unordered_set<string> entityType;
	unordered_map<string, string> declarationList;
	string selectList;
	vector<QueryUtility::ClauseType> clauseEnum;
	vector<vector<string>> clauseParam;

	bool checkDeclaration(string declarations);
	bool isValidDeclaration(string entity);
	bool checkSelect(string select);
	bool checkTuple(string select);
	bool isValidSuchThat(string suchthat);
	bool isValidPattern(string pattern);
	bool isRelationshipValid(string relationship);
	string getArgument(string query);
	string getArgumentAssign(string query);
	string getPatternType(string clause);
	bool isValidWith(string withs);
	bool checkWithClause(string with);

	int isString(string arg);

public:
	QueryValidation();
	bool isValidQuery(string query);
	unordered_map<string, string> getDeclaration();
	string getSelect();
	vector<QueryUtility::ClauseType> getClauseEnum();
	vector<vector<string>> getClauseParam();
};