#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include "Query.h"
#include "QueryClause.h"
#include "QueryExtractor.h"
#include "QueryParam.h"
#include "QueryPair.h"

using namespace std;

QueryExtractor::QueryExtractor(void) {

}

Query QueryExtractor::extract(unordered_map<string, string> declarationMap, string query) {
	Query q;
	vector<QueryPair> queryPairList = getDeclarations(declarationMap);
	string declarationsRemoved = removeDeclarations(query);
	vector<QueryPair> selectList = getSelects(declarationMap, declarationsRemoved);


	return q;
}

vector<QueryPair> QueryExtractor::getDeclarations(unordered_map<string, string> declarationMap) {
	vector<QueryPair> list;
	
	for (auto it = declarationMap.begin(); it != declarationMap.end(); ++it) {
		QueryPair pair = QueryPair(determineSynonymType(it->first), it->second);
		list.push_back(pair);
	}

	return list;
}

SynonymType QueryExtractor::determineSynonymType(string str) {
	//needs major cleanup later on; find out how to do str const || std::string const&  

	if (str == "assign") return SYNONYM_TYPE_ASSIGN;
	if (str == "BOOLEAN") return SYNONYM_TYPE_BOOLEAN;
	if (str == "call") return SYNONYM_TYPE_CALL;
	if (str == "constant") return SYNONYM_TYPE_CONSTANT;
	if (str == "if") return SYNONYM_TYPE_IF;
	if (str == "procedure") return SYNONYM_TYPE_PROCEDURE;
	if (str == "prog_line") return SYNONYM_TYPE_PROG_LINE;
	if (str == "stmt") return SYNONYM_TYPE_STMT;
	if (str == "variable") return SYNONYM_TYPE_VARIABLE;
	if (str == "while") return SYNONYM_TYPE_WHILE;

	else
		return SYNONYM_TYPE_NULL;
}

string QueryExtractor::removeDeclarations(string input) {
	string str;

	std::size_t pos = input.find("Select");
	str = input.substr(pos);

	return str;
}

vector<QueryPair> QueryExtractor::getSelects(unordered_map<string, string> map, string input) {
	string str;
	vector<string> selectStringList = sanitiseForSelects(input);

	

	vector<QueryPair> outputList;

	
	return outputList;
}

vector<string> QueryExtractor::sanitiseForSelects(string input) {
	
	istringstream iss(input);
	vector<string> selects;

	do {
		string substr;
		iss >> substr;

		if (substr.length() == 0 || substr == "such" || substr == "pattern") {
			break;
		}

		if (substr == "Select") {
			continue;
		}

		selects.push_back(substr);

	} while (iss);

	vector<string> sanitised;

	//removing non alphanumeric characters
	for (string s : selects) {
		size_t len = s.length();
		size_t index = 0;

		while (index < len) {
			if (!isalnum(s[index])) {
				s = s.erase(index, 1);
				len--;
			}
			else {
				index++;
			}
		}

		sanitised.push_back(s);
	}

	return sanitised;
}