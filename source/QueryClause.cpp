#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryClause.h"

using namespace std;

QueryClause::QueryClause(ClauseType type, int paraCount, vector<QueryParam> paramList) {
	clauseType = type;
	paramCount = paraCount;
	parametersList = paramList;
}

ClauseType QueryClause::getClauseType(void) {
	return this->clauseType;
}

int QueryClause::getParamCount(void) {
	return this->paramCount;
}

vector<QueryParam> QueryClause::getParametersList(void) {
	return this->parametersList;
}