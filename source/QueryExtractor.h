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
		unordered_map<string, SynonymType> decHashMap;

		Query extract(unordered_map<string, string> declarationMap, string query);
		vector<QueryPair> getDeclarations(unordered_map<string, string> declarationMap);
		SynonymType determineSynonymType(string input);
		ClauseType determineClauseType(string input, string next);
		string removeDeclarations(string input);
		vector<QueryPair> getSelects(unordered_map<string, string> map, string input);
		vector<string> sanitiseForSelects(string input);
		vector<QueryClause> getClauses(string input);
		string removeSpaces(string input);
		QueryParam createQueryParam(string input);
		QueryParam createQueryParamForPatternAssign(string input);


	private:
		

};