#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "QueryClause.h"

using namespace std;

QueryClause::QueryClause(ClauseType type, string synonymVal, int paraCount, vector<QueryParam> paramList) {
	clauseType = type;
	synonymValue = synonymVal;
	paramCount = paraCount;
	parametersList = paramList;
}

ClauseType QueryClause::getClauseType(void) {
	return this->clauseType;
}

int QueryClause::getParamCount(void) {
	return this->paramCount;
}

string QueryClause::getSynonymValue(void) {
	return this->synonymValue;
}

vector<QueryParam> QueryClause::getParametersList(void) {
	return this->parametersList;
}

bool QueryClause::operator==(QueryClause other) {
	bool isSameClauseType = false;
	bool isSameParamCount = false;
	bool isSameParamList = true;

	if (this->getClauseType() == other.getClauseType()) {
		isSameClauseType = true;
	}

	if (this->getParamCount() == other.getParamCount()) {
		isSameParamCount = true;
	}
	
	vector<QueryParam> list1 = this->getParametersList();
	vector<QueryParam> list2 = other.getParametersList();

	int max = (list1.size() > list2.size()) ? list1.size() : list2.size();
	
	for (int i = 0; i < max; i++) {
		if (list1.at(i) != list2.at(i)) {
			isSameParamList = false;
			break;
		}
	}
	
	return isSameClauseType && isSameParamCount && isSameParamList;
}