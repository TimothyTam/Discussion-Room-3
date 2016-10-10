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

	CLAUSETYPE_NEXT,
	CLAUSETYPE_NEXT_STAR,
	CLAUSETYPE_CALLS,
	CLAUSETYPE_CALLS_STAR,
	CLAUSETYPE_AFFECTS,
	CLAUSETYPE_AFFECTS_STAR,

	CLAUSETYPE_PATTERN_ASSIGN,
	CLAUSETYPE_PATTERN_IF,
	CLAUSETYPE_PATTERN_WHILE,
	CLAUSETYPE_WITH,

	CLAUSETYPE_NULL

};

// This class represents a clause entered by the user, e.g.
// Parent(s1, s2) and holds the type of clause it is, any synonym
// attribute it should have (applies to pattern clauses), the no.
// of parameters it holds (pattern-if will hold 3, the rest 2) as well
// as the vector list of parameters.

class QueryClause {
	public:
		QueryClause(ClauseType type,
					string synonymVal, 
					int paraCount, 
					vector<QueryParam> paramList);

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