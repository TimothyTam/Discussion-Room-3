#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"
#include "QueryUtility.h"

using namespace std;

// This class represents a clause entered by the user, e.g.
// Parent(s1, s2) and holds the type of clause it is, any synonym
// attribute it should have (applies to pattern clauses), the no.
// of parameters it holds (pattern-if will hold 3, the rest 2) as well
// as the vector list of parameters.

class QueryClause {
	public:
		QueryClause();
		QueryClause(QueryUtility::ClauseType type,
					string synonymVal, 
					int paraCount, 
					vector<QueryParam> paramList);

		QueryUtility::ClauseType getClauseType(void);
		int getParamCount(void);
		string getSynonymValue(void);
		vector<QueryParam> getParametersList(void);
		bool QueryClause::operator==(QueryClause other);

	private:
		QueryUtility::ClauseType _clauseType;
		int _paramCount;
		string _synonymValue; // for pattern queries
		vector<QueryParam> _parametersList;

};