#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Query.h"
#include "QueryClause.h"
#include "QueryParam.h"
#include "QueryPair.h"

class QueryExtractor {
	public:
		QueryExtractor(void);
		Query query;

		Query extract(unordered_map<string, string> declarationMap, string query);
		vector<QueryPair> getDeclarations(unordered_map<string, string> declarationMap);
		SynonymType determineSynonymType(string input);
		ClauseType determineClauseType(unordered_map<string, string> decMap, string input, string next);
		string removeDeclarations(string input);
		vector<QueryPair> getSelects(unordered_map<string, string> map, string input);
		vector<string> sanitiseForSelects(string input);
		vector<QueryClause> getClauses(unordered_map<string, string> map, string input);
		string removeSpaces(string input);
		QueryParam determineParamType(unordered_map<string, string> map, string input);


	private:
		

};