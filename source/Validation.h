#pragma once
#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "RelTable.h"

using namespace std;

class validation {

private:
	RelTable table;
	bool checkSynonym(string query);
	bool isValidSynonym(string entity);
	bool isRelationshipValid(string relationship);
	string patternCheck(string query);
	string getArgument(string query);
	string getArgumentAssign(string query);
	bool withClause(string query);
	int isString(string arg);
	bool checkNumOfArgument(string type, string relationship);
	bool checkSelect(string select);
	bool checkTuple(string select);

public:
	unordered_set<string> entityType;
	unordered_map<string, string> synonym;
	validation();
	bool isValidQuery(string query);

};