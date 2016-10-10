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

	// not a tuple
	if (positionOfComma == string::npos) {
		QueryPair qp = QueryPair(settleSynonyms(decList.at(selectString)), selectString);
		list.push_back(qp);
	}
	// Select BOOLEAN
	else if (selectString == "BOOLEAN") {
		QueryPair qp = QueryPair(SYNONYM_TYPE_BOOLEAN, selectString);
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

	for (int index = 0; index < clauseEnums.size(); index++) {
		vector<QueryParam> paramList;

		if (clauseEnums.at(index) != QueryUtility::CLAUSETYPE_PATTERN_ASSIGN
		 && clauseEnums.at(index) != QueryUtility::CLAUSETYPE_PATTERN_IF
	  	 && clauseEnums.at(index) != QueryUtility::CLAUSETYPE_PATTERN_WHILE) {

			ClauseType cType = settleClauses(clauseEnums.at(index));
			string sVal = "none";
			int paraCount = 2;
			
			QueryParam qp1 = createQueryParam(clauseParams.at(index).at(0), decList);
			QueryParam qp2 = createQueryParam(clauseParams.at(index).at(1), decList);
			paramList.push_back(qp1);
			paramList.push_back(qp2);

			QueryClause qc = QueryClause(cType, sVal, paraCount, paramList);

			list.push_back(qc);
		}
		else if (clauseEnums.at(index) == QueryUtility::CLAUSETYPE_PATTERN_ASSIGN) {
			ClauseType cType = settleClauses(clauseEnums.at(index));
			string sVal = clauseParams.at(index).at(0);
			int paraCount = 2;
			QueryParam qp1 = createQueryParam(clauseParams.at(index).at(1), decList);
			QueryParam qp2 = createQueryParamForPatternAssign(clauseParams.at(index).at(2), decList);
			paramList.push_back(qp1);
			paramList.push_back(qp2);

			QueryClause qc = QueryClause(cType, sVal, paraCount, paramList);

			list.push_back(qc);
		}
		else if (clauseEnums.at(index) == QueryUtility::CLAUSETYPE_PATTERN_IF) {
			ClauseType cType = settleClauses(clauseEnums.at(index));
			string sVal = clauseParams.at(index).at(0);
			int paraCount = 3;
			QueryParam qp1 = createQueryParam(clauseParams.at(index).at(1), decList);
			QueryParam qp2 = createQueryParam("_", decList);
			QueryParam qp3 = createQueryParam("_", decList);
			paramList.push_back(qp1);
			paramList.push_back(qp2);
			paramList.push_back(qp3);

			QueryClause qc = QueryClause(cType, sVal, paraCount, paramList);

			list.push_back(qc);
		}
		else if (clauseEnums.at(index) == QueryUtility::CLAUSETYPE_PATTERN_WHILE) {
			ClauseType cType = settleClauses(clauseEnums.at(index));
			string sVal = clauseParams.at(index).at(0);
			int paraCount = 2;
			QueryParam qp1 = createQueryParam(clauseParams.at(index).at(1), decList);
			QueryParam qp2 = createQueryParam("_", decList);
			paramList.push_back(qp1);
			paramList.push_back(qp2);

			QueryClause qc = QueryClause(cType, sVal, paraCount, paramList);

			list.push_back(qc);
		}



	}

	return list;
}

QueryParam QueryExtractorTwo::createQueryParam(string input, unordered_map<string, QueryUtility::SynonymType> decList) {
	QueryParam qp;
	unordered_map<string, QueryUtility::SynonymType>::const_iterator exist = decList.find(input);
	// if string can be found in decHashMap, means it is a synonym
	if (exist != decList.end()) {
		ParamType paramtype = PARAMTYPE_SYNONYM;
		SynonymType synontype = settleSynonyms(exist->second);
		string value = exist->first;
		qp = QueryParam(paramtype, synontype, value);

	}
	else {
		// wild card
		if (input == "_") {
			ParamType paramtype = PARAMTYPE_PLACEHOLDER;
			SynonymType synontype = SYNONYM_TYPE_NULL;
			string value = input;
			qp = QueryParam(paramtype, synontype, value);
		}
		// entity
		else {
			ParamType paramtype = PARAMTYPE_ENT_NAME;
			SynonymType synontype = SYNONYM_TYPE_NULL;
			string value = input;
			qp = QueryParam(paramtype, synontype, value);
		}
	}

	return qp;
}

// This function only applies to the 2nd parameter/argument
// of a pattern-assign clause as it is a special case
// as compared to other types of parameters.
QueryParam QueryExtractorTwo::createQueryParamForPatternAssign(string input, unordered_map<string, QueryUtility::SynonymType> decList) {
	QueryParam qp;
	ParamType paramtype;
	SynonymType synontype = SYNONYM_TYPE_NULL;

	unordered_map<string, QueryUtility::SynonymType>::const_iterator exist = decList.find(input);

	if (input == "_") {
		paramtype = PARAMTYPE_PLACEHOLDER;
	}
	// 2nd parameter is a synonym that was declared
	else if (exist != decList.end()) {
		paramtype = PARAMTYPE_SYNONYM;
		synontype = settleSynonyms(exist->second);
	}
	else {

		size_t positionOfPlaceHolder = input.find("_");
		size_t positionOfNextPlaceHolder;

		// Underscore exists on left side
		if (positionOfPlaceHolder == 0) {
			string chopped = input.substr(positionOfPlaceHolder + 1);
			positionOfNextPlaceHolder = chopped.find("_");

			// Underscore exists on left + right side
			if (positionOfNextPlaceHolder != string::npos) {
				paramtype = PARAMTYPE_PATTERN_STRING_BOTH_OPEN;
			}
			// Underscore exists only on left side
			else {
				paramtype = PARAMTYPE_PATTERN_STRING_LEFT_OPEN;
			}

		}

		// Underscore exists only on right side
		else if (positionOfPlaceHolder == input.length() - 1) {
			paramtype = PARAMTYPE_PATTERN_STRING_RIGHT_OPEN;
		}

		// Underscore does not exist
		else {
			paramtype = PARAMTYPE_PATTERN_STRING_EXACT;
		}
	}

	qp = QueryParam(paramtype, synontype, input);

	return qp;
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

ClauseType QueryExtractorTwo::settleClauses(QueryUtility::ClauseType cType) {
	if (cType == QueryUtility::CLAUSETYPE_PATTERN_ASSIGN) return CLAUSETYPE_PATTERN_ASSIGN;
	if (cType == QueryUtility::CLAUSETYPE_PATTERN_IF) return CLAUSETYPE_PATTERN_WHILE;
	if (cType == QueryUtility::CLAUSETYPE_PATTERN_WHILE) return CLAUSETYPE_PATTERN_WHILE;
	if (cType == QueryUtility::CLAUSETYPE_MODIFIES) return CLAUSETYPE_MODIFIES;
	if (cType == QueryUtility::CLAUSETYPE_USES) return CLAUSETYPE_USES;
	if (cType == QueryUtility::CLAUSETYPE_FOLLOWS) return CLAUSETYPE_FOLLOWS;
	if (cType == QueryUtility::CLAUSETYPE_FOLLOWS_STAR) return CLAUSETYPE_FOLLOWS_STAR;
	if (cType == QueryUtility::CLAUSETYPE_PARENT) return CLAUSETYPE_PARENT;
	if (cType == QueryUtility::CLAUSETYPE_PARENT_STAR) return CLAUSETYPE_PARENT_STAR;
	if (cType == QueryUtility::CLAUSETYPE_CALLS) return CLAUSETYPE_CALLS;
	if (cType == QueryUtility::CLAUSETYPE_CALLS_STAR) return CLAUSETYPE_CALLS_STAR;
	if (cType == QueryUtility::CLAUSETYPE_NEXT) return CLAUSETYPE_NEXT;
	if (cType == QueryUtility::CLAUSETYPE_NEXT_STAR) return CLAUSETYPE_NEXT_STAR;
	if (cType == QueryUtility::CLAUSETYPE_AFFECTS) return CLAUSETYPE_AFFECTS;
	if (cType == QueryUtility::CLAUSETYPE_AFFECTS_STAR) return CLAUSETYPE_AFFECTS_STAR;

	else
		return CLAUSETYPE_NULL;
}