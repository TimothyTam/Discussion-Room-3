#include "RelTable.h"

using namespace std;
RelTable::RelTable() {
	vector<string> type1;
	vector<string> type2;

	type1 = { "procedure", "string", "prog_line","stmt","assign","while","if" ,"_" };
	type2 = { "variable", "string","_" };
	relTable["Modifies"] = { type1,type2 };

	type1 = { "procedure", "string", "prog_line","stmt","assign","while","if" ,"_" };
	type2 = { "variable", "string","_" };
	relTable["Uses"] = { type1,type2 };

	type1 = { "prog_line","stmt","assign","while","if" ,"_" };
	type2 = { "prog_line","stmt","assign","while","if" ,"_" };
	relTable["Follows"] = { type1,type2 };

	type1 = { "prog_line","stmt","while","if" ,"_" };
	type2 = { "prog_line","stmt","assign","while","if" ,"_" };
	relTable["Parent"] = { type1,type2 };

	type1 = { "string","_" };
	type2 = { "string","_" };
	relTable["passign"] = { type1,type2 };

	type1 = { "string","_" };
	type2 = { "_" };
	relTable["pwhile"] = { type1,type2 };

	type1 = { "string","_" };
	type2 = { "_" };
	relTable["pif"] = { type1,type2 };

	type1 = {};
	type2 = {};
	relTable["Calls"] = { type1,type2 };

	type1 = {};
	type2 = {};
	relTable["Next"] = { type1,type2 };

	type1 = {};
	type2 = {};
	relTable["Affects"] = { type1,type2 };
}

string RelTable::getIndex(string query) {
	string type = query.substr(0, query.find_first_of(" ("));
	if (relTable.find(type) == relTable.end()) {
		return string();
	}
	else {
		return type;
	}
}

//arg1 and arg2 is the data type
bool RelTable::isValidArgument(string type, string arg1, string arg2) {
	vector<vector<string>> arguments = relTable.find(type)->second;
	vector<string> type1 = arguments.at(0);
	vector<string> type2 = arguments.at(1);
	return checkArgument(arg1, type1) && checkArgument(arg2, type2);
}

bool RelTable::checkArgument(string arg, vector<string> type) {
	if (std::find(type.begin(), type.end(), arg) != type.end()) {
		return true;
	}
	return false;
}

bool RelTable::checkNumOfArgument(string type, int num) {
	if (relTable.find(type)->second.size() == num) {
		return true;
	}
	return false;
}
