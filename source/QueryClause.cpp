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
	bool isSameSynonymValue = false;
	bool isSameParamList = true;

	if (this->getClauseType() == other.getClauseType()) {
		isSameClauseType = true;
	}

	if (this->getParamCount() == other.getParamCount()) {
		isSameParamCount = true;
	}

	if (this->getSynonymValue() == other.getSynonymValue()) {
		isSameSynonymValue = true;
	}
	
	vector<QueryParam> list1 = this->getParametersList();
	vector<QueryParam> list2 = other.getParametersList();

	if (list1.size() != list2.size()) {
		return false;
	}
	
	for (int i = 0; i < list1.size(); i++) {
		if (list1.at(i) != list2.at(i)) {
			isSameParamList = false;
			break;
		}
	}
	
	return isSameClauseType && isSameSynonymValue && isSameParamCount && isSameParamList;
}