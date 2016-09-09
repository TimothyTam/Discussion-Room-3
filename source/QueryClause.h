#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"

using namespace std;

enum ClauseType {
	MODIFIES,
	USES,
	FOLLOWS,
	FOLLOWS_STAR,
	PARENT,
	PARENT_STAR,

	PATTERN_ASSIGN,
	PATTERN_IF,
	PATTERN_WHILE

	// insert rest of clauses for > iter 1

};

class QueryClause {
	public:
		QueryClause(ClauseType type, string synonymVal, int paraCount, vector<QueryParam> paramList);
		ClauseType getClauseType(void);
		int getParamCount(void);
		vector<QueryParam> getParametersList(void);

	private:
		ClauseType clauseType;
		int paramCount;
		string synonymValue; // for pattern queries
		vector<QueryParam> parametersList;

};