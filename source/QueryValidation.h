#pragma once
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "RelTable.h"
#include "QueryUtility.h"

using namespace std;
using namespace std::regex_constants;

class QueryValidation {
	
private:
	RelTable table;
	unordered_map<string, QueryUtility::SynonymType> declarationList;
	string selectList;
	vector<QueryUtility::ClauseType> clauseEnum;
	vector<vector<string>> clauseParam;
	enum attrName { procName, varName, value, stmtNo, none };

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
	attrName stringToAttrName(string clause);

public:
	QueryValidation();
	bool isValidQuery(string query);
	unordered_map<string, QueryUtility::SynonymType> getDeclaration();
	string getSelect();
	vector<QueryUtility::ClauseType> getClauseEnum();
	vector<vector<string>> getClauseParam();
};