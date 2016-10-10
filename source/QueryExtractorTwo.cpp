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
#include "QueryExtractorTwo.h"

QueryExtractorTwo::QueryExtractorTwo(void) {

}

Query QueryExtractorTwo::extract(unordered_map<string, QueryUtility::SynonymType> decList,
								 string selectString,
								 vector<QueryUtility::ClauseType> clauseEnums,
								 vector<vector<string>> clauseParams) {
	Query q;

	vector<QueryPair> queryPairList = getDeclarations(decList);
	vector<QueryPair> selectList = getSelects(selectString, decList);
	vector<QueryClause> clauseList = getClauses(clauseEnums, clauseParams, decList);

	q = Query(queryPairList, selectList, clauseList);
	return q;
}


vector<QueryPair> QueryExtractorTwo::getDeclarations(unordered_map<string, QueryUtility::SynonymType> decList) {
	vector<QueryPair> list;

	for (auto it = decList.begin(); it != decList.end(); ++it) {
		QueryPair pair = QueryPair(settleSynonyms(it->second), it->first);
	}

	return list;
}

vector<QueryPair> QueryExtractorTwo::getSelects(string selectString, unordered_map<string, QueryUtility::SynonymType> decList) {
	vector<QueryPair> list;
	size_t positionOfComma = selectString.find(",");
	string value;

	//not a tuple
	if (positionOfComma == string::npos) {
		QueryPair qp = QueryPair(settleSynonyms(decList.at(selectString)), selectString);
		list.push_back(qp);
	}
	else {
	selectString = selectString.substr(1); //removing "<"

		do {
			positionOfComma = selectString.find(",");
			value = selectString.substr(0, positionOfComma);
			QueryPair qp = QueryPair(settleSynonyms(decList.at(value)), value);
			selectString = selectString.substr(value.length() + 1);

			list.push_back(qp);
			positionOfComma = selectString.find(",");
		} while (positionOfComma != string::npos);

		value = selectString.substr(0, selectString.length()-1);
		QueryPair qp = QueryPair(settleSynonyms(decList.at(value)), value);
		list.push_back(qp);
	}

	return list;
}

vector<QueryClause> QueryExtractorTwo::getClauses(vector<QueryUtility::ClauseType> clauseEnums,
												  vector<vector<string>> clauseParams,
												  unordered_map<string, QueryUtility::SynonymType> decList) {
	vector<QueryClause> list;

	return list;
}



//temporary function to be removed
SynonymType QueryExtractorTwo::settleSynonyms(QueryUtility::SynonymType sType) {
	if (sType == QueryUtility::SYNONYM_TYPE_ASSIGN) return SYNONYM_TYPE_ASSIGN;
	if (sType == QueryUtility::SYNONYM_TYPE_BOOLEAN) return SYNONYM_TYPE_BOOLEAN;
	if (sType == QueryUtility::SYNONYM_TYPE_CALL) return SYNONYM_TYPE_CALL;
	if (sType == QueryUtility::SYNONYM_TYPE_CONSTANT) return SYNONYM_TYPE_CONSTANT;
	if (sType == QueryUtility::SYNONYM_TYPE_IF) return SYNONYM_TYPE_IF;
	if (sType == QueryUtility::SYNONYM_TYPE_PROCEDURE) return SYNONYM_TYPE_PROCEDURE;
	if (sType == QueryUtility::SYNONYM_TYPE_PROG_LINE) return SYNONYM_TYPE_PROG_LINE;
	if (sType == QueryUtility::SYNONYM_TYPE_STMT) return SYNONYM_TYPE_STMT;
	if (sType == QueryUtility::SYNONYM_TYPE_VARIABLE) return SYNONYM_TYPE_VARIABLE;
	if (sType == QueryUtility::SYNONYM_TYPE_WHILE) return SYNONYM_TYPE_WHILE;

	else
		return SYNONYM_TYPE_NULL;
}