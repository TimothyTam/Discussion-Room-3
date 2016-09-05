#pragma once

#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

class RelTable {

private:
	unordered_map < string, vector<vector<string>>> relTable;
	bool checkArgument(string arg, vector<string> type);

public:
	RelTable(); //constructor
	string getIndex(string query);
	bool isValidArgument(string type, string arg1, string arg2);
	bool checkNumOfArgument(string type, int num);
};