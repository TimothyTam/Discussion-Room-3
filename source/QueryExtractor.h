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

class QueryExtractor {
	public:
		QueryExtractor(void);

		Query extract(unordered_map<string, QueryUtility::SynonymType> decList,
					  string selectString,
					  vector<QueryUtility::ClauseType> clauseEnums,
					  vector<vector<string>> clauseParams);
		vector<QueryPair> getDeclarations(unordered_map<string, QueryUtility::SynonymType> declarationMap);
		vector<QueryPair> getSelects(string str, unordered_map<string, QueryUtility::SynonymType> decList);
		vector<QueryClause> getClauses(vector<QueryUtility::ClauseType> clauseEnums,
									   vector<vector<string>> clauseParams,
									   unordered_map<string, QueryUtility::SynonymType> decList);
		QueryParam createQueryParam(string input, unordered_map<string, QueryUtility::SynonymType> decList);
		QueryParam createQueryParamForPatternAssign(string input, unordered_map<string, QueryUtility::SynonymType> decList);
		QueryParam createQueryParamForWith(string input, unordered_map<string, QueryUtility::SynonymType> decList);
		QueryUtility::ClauseType determineWithClauseType(string withString);
		QueryUtility::ParamType determineWithParamType(string withString);
		QueryUtility::ClauseType determineWithClauseTypeForWith(QueryUtility::ParamType ptype);
		bool is_number(const std::string& s);


	private:

};