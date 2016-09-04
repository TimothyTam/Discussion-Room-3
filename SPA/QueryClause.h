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
		ClauseType getClauseType(void);
		int getParamCount(void);
		vector<QueryParam> getParametersList(void);

	private:
		ClauseType type;
		int paramCount;
		vector<QueryParam> parametersList;

};

ClauseType QueryClause::getClauseType(void) {
	return this->type;
}

int QueryClause::getParamCount(void) {
	return this->paramCount;
}

vector<QueryParam> QueryClause::getParametersList(void) {
	return this->parametersList;
}