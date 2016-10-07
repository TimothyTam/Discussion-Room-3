#include "RelTable.h"

using namespace std;
RelTable::RelTable() {
	vector<string> type1;
	vector<string> type2;
	type1 = { "procedure", "string", "prog_line","stmt","assign","while","if" ,"_" ,"call"};
	type2 = { "variable", "string","_" };
	relTable[QueryUtility::CLAUSETYPE_MODIFIES] = { type1,type2 };

	type1 = { "procedure", "string", "prog_line","stmt","assign","while","if" ,"_","call" };
	type2 = { "variable", "string","_" };
	relTable[QueryUtility::CLAUSETYPE_USES] = { type1,type2 };

	type1 = { "prog_line","stmt","assign","while","if" ,"_","call" };
	type2 = { "prog_line","stmt","assign","while","if" ,"_" ,"call" };
	relTable[QueryUtility::CLAUSETYPE_FOLLOWS] = { type1,type2 };

	type1 = { "prog_line","stmt","assign","while","if" ,"_" ,"call" };
	type2 = { "prog_line","stmt","assign","while","if" ,"_","call" };
	relTable[QueryUtility::CLAUSETYPE_FOLLOWS_STAR] = { type1,type2 };

	type1 = { "prog_line","stmt","while","if" ,"_" };
	type2 = { "prog_line","stmt","assign","while","if" ,"_" ,"call" };
	relTable[QueryUtility::CLAUSETYPE_PARENT] = { type1,type2 };

	type1 = { "prog_line","stmt","while","if" ,"_" };
	type2 = { "prog_line","stmt","assign","while","if" ,"_" ,"call" };
	relTable[QueryUtility::CLAUSETYPE_PARENT_STAR] = { type1,type2 };
	
	type1 = {"procedure","string","_"};
	type2 = { "procedure","string","_" };
	relTable[QueryUtility::CLAUSETYPE_CALLS] = { type1,type2 };
	
	type1 = { "procedure","string","_" };
	type2 = { "procedure","string","_" };
	relTable[QueryUtility::CLAUSETYPE_CALLS_STAR] = { type1,type2 };
	
	type1 = {"prog_line","_","stmt","assign","while","if" ,"call" };
	type2 = { "prog_line","_","stmt","assign","while","if" ,"call" };
	relTable[QueryUtility::CLAUSETYPE_NEXT] = { type1,type2 };

	type1 = { "prog_line","_","stmt","assign","while","if" ,"call" };
	type2 = { "prog_line","_","stmt","assign","while","if" ,"call" };
	relTable[QueryUtility::CLAUSETYPE_NEXT_STAR] = { type1,type2 };

	type1 = { "string","_","variable" };
	type2 = { "string","_" };
	relTable[QueryUtility::CLAUSETYPE_PATTERN_ASSIGN] = { type1,type2 };

	type1 = { "string","_","variable" };
	type2 = { "_" };
	relTable[QueryUtility::CLAUSETYPE_PATTERN_WHILE] = { type1,type2 };
	
	type1 = { "string","_" "variable"};
	type2 = { "_" };
	relTable[QueryUtility::CLAUSETYPE_PATTERN_IF] = { type1,type2 };
	/*NOT DONE*/
	type1 = {};
	type2 = {};
	relTable[QueryUtility::CLAUSETYPE_AFFECTS] = { type1,type2 };
	/*NOT DONE*/
	type1 = {};
	type2 = {};
	relTable[QueryUtility::CLAUSETYPE_AFFECTS_STAR] = { type1,type2 };
}


QueryUtility::ClauseType RelTable::getIndex(string query) {
	std::transform(query.begin(), query.end(), query.begin(), ::tolower);
	if (query == "modifies") return QueryUtility::CLAUSETYPE_MODIFIES;
	else if (query == "uses") return QueryUtility::CLAUSETYPE_USES;
	else if (query == "follows") return QueryUtility::CLAUSETYPE_FOLLOWS;
	else if (query == "follows*") return QueryUtility::CLAUSETYPE_FOLLOWS_STAR;
	else if (query == "parent") return QueryUtility::CLAUSETYPE_PARENT;
	else if (query == "parent*") return QueryUtility::CLAUSETYPE_PARENT_STAR;
	else if (query == "calls") return QueryUtility::CLAUSETYPE_CALLS;
	else if (query == "calls*") return QueryUtility::CLAUSETYPE_CALLS_STAR;
	else if (query == "next") return QueryUtility::CLAUSETYPE_NEXT;
	else if (query == "next*") return QueryUtility::CLAUSETYPE_NEXT_STAR;

	else if (query == "passign") return QueryUtility::CLAUSETYPE_PATTERN_ASSIGN;
	else if (query == "pwhile") return QueryUtility::CLAUSETYPE_PATTERN_WHILE;
	else if (query == "pif*") return QueryUtility::CLAUSETYPE_PATTERN_IF;

	else return QueryUtility::CLAUSETYPE_NULL;
}

//arg1 and arg2 is the data type
bool RelTable::isValidArgument(QueryUtility::ClauseType type, string arg1, string arg2) {
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

bool RelTable::checkNumOfArgument(QueryUtility::ClauseType type, string clause) {
	int num = std::count(clause.begin(), clause.end(), ',') + 1;
	if (type == QueryUtility::QueryUtility::CLAUSETYPE_PATTERN_IF) {
		return (num == 3);
	}
	if (relTable.at(type).size() == num){
		return true;
	}
	return false;
}