#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "QueryClause.h"

using namespace std;

QueryClause::QueryClause()
{
}

QueryClause::QueryClause(QueryUtility::ClauseType clauseType,
						 string synonymValue, 
						 int paramCount, 
						 vector<QueryParam> parametersList) {

	_clauseType = clauseType;
	_synonymValue = synonymValue;
	_paramCount = paramCount;
	_parametersList = parametersList;
}

QueryUtility::ClauseType QueryClause::getClauseType(void) {
	return this->_clauseType;
}

int QueryClause::getParamCount(void) {
	return this->_paramCount;
}

string QueryClause::getSynonymValue(void) {
	return this->_synonymValue;
}

vector<QueryParam> QueryClause::getParametersList(void) {
	return this->_parametersList;
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