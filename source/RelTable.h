#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include "QueryUtility.h"

using namespace std;

class RelTable {
	
private:
	unordered_map < QueryUtility::ClauseType, vector<vector<string>>> relTable;
	bool checkArgument(string arg, vector<string> type);

public:
	RelTable(); //constructor
	QueryUtility::ClauseType getIndex(string query);
	bool isValidArgument(QueryUtility::ClauseType type, string arg1, string arg2);
	bool checkNumOfArgument(QueryUtility::ClauseType type, string clause);
};