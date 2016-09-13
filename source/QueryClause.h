#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"

using namespace std;

enum ClauseType {
	CLAUSETYPE_MODIFIES,
	CLAUSETYPE_USES,
	CLAUSETYPE_FOLLOWS,
	CLAUSETYPE_FOLLOWS_STAR,
	CLAUSETYPE_PARENT,
	CLAUSETYPE_PARENT_STAR,

	CLAUSETYPE_PATTERN_ASSIGN,
	CLAUSETYPE_PATTERN_IF,
	CLAUSETYPE_PATTERN_WHILE,


	// insert rest of clauses for > iter 1


	CLAUSETYPE_NULL

};

class QueryClause {
	public:
		QueryClause(ClauseType type, string synonymVal, int paraCount, vector<QueryParam> paramList);

		ClauseType getClauseType(void);
		int getParamCount(void);
		string getSynonymValue(void);
		vector<QueryParam> getParametersList(void);
		bool QueryClause::operator==(QueryClause other);

	private:
		ClauseType _clauseType;
		int _paramCount;
		string _synonymValue; // for pattern queries
		vector<QueryParam> _parametersList;

};