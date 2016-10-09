#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include "Query.h"
#include "QueryClause.h"
#include "QueryUtility.h"
#include "QueryParam.h"
#include "QueryPair.h"

class QueryExtractorTwo {
	public:
		QueryExtractorTwo(void);

		Query extract(unordered_map<string, QueryUtility::SynonymType> decList,
					  string selectString,
					  vector<QueryUtility::ClauseType> clauseEnums,
					  vector<vector<string>> clauseParams);
		vector<QueryPair> getDeclarations(unordered_map<string, QueryUtility::SynonymType> declarationMap);
		vector<QueryPair> getSelects(string str, unordered_map<string, QueryUtility::SynonymType> decList);
		vector<QueryClause> getClauses(vector<QueryUtility::ClauseType> clauseEnums,
									   vector<vector<string>> clauseParams);
		SynonymType QueryExtractorTwo::settleSynonyms(QueryUtility::SynonymType sType);

	private:

};