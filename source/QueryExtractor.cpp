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
#include "QueryExtractor.h"

QueryExtractor::QueryExtractor(void) {

}

Query QueryExtractor::extract(unordered_map<string, QueryUtility::SynonymType> decList,
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


vector<QueryPair> QueryExtractor::getDeclarations(unordered_map<string, QueryUtility::SynonymType> decList) {
	vector<QueryPair> list;

	for (auto it = decList.begin(); it != decList.end(); ++it) {
		QueryPair pair = QueryPair(it->second, it->first);
		list.push_back(pair);
	}

	return list;
}

vector<QueryPair> QueryExtractor::getSelects(string selectString, unordered_map<string, QueryUtility::SynonymType> decList) {
	vector<QueryPair> list;
	size_t positionOfComma = selectString.find(",");
	string value;

	// not a tuple
	if (positionOfComma == string::npos) {
		QueryPair qp = QueryPair(decList.at(selectString), selectString);
		list.push_back(qp);
	}
	// Select BOOLEAN
	else if (selectString == "BOOLEAN") {
		QueryPair qp = QueryPair(QueryUtility::SYNONYM_TYPE_BOOLEAN, selectString);
		list.push_back(qp);
	}
	else {
	selectString = selectString.substr(1); //removing "<"

		do {
			positionOfComma = selectString.find(",");
			value = selectString.substr(0, positionOfComma);
			QueryPair qp = QueryPair(decList.at(value), value);
			selectString = selectString.substr(value.length() + 1);

			list.push_back(qp);
			positionOfComma = selectString.find(",");
		} while (positionOfComma != string::npos);

		value = selectString.substr(0, selectString.length()-1);
		QueryPair qp = QueryPair(decList.at(value), value);
		list.push_back(qp);
	}

	return list;
}

vector<QueryClause> QueryExtractor::getClauses(vector<QueryUtility::ClauseType> clauseEnums,
												  vector<vector<string>> clauseParams,
												  unordered_map<string, QueryUtility::SynonymType> decList) {
	vector<QueryClause> list;

	for (int index = 0; index < clauseEnums.size(); index++) {
		vector<QueryParam> paramList;

		if (clauseEnums.at(index) != QueryUtility::CLAUSETYPE_PATTERN_ASSIGN
		 && clauseEnums.at(index) != QueryUtility::CLAUSETYPE_PATTERN_IF
	  	 && clauseEnums.at(index) != QueryUtility::CLAUSETYPE_PATTERN_WHILE
		 && clauseEnums.at(index) != QueryUtility::CLAUSETYPE_WITH) {

			QueryUtility::ClauseType cType = clauseEnums.at(index);
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
			QueryUtility::ClauseType cType = clauseEnums.at(index);
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
			QueryUtility::ClauseType cType = clauseEnums.at(index);
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
			QueryUtility::ClauseType cType = clauseEnums.at(index);
			string sVal = clauseParams.at(index).at(0);
			int paraCount = 2;
			QueryParam qp1 = createQueryParam(clauseParams.at(index).at(1), decList);
			QueryParam qp2 = createQueryParam("_", decList);
			paramList.push_back(qp1);
			paramList.push_back(qp2);

			QueryClause qc = QueryClause(cType, sVal, paraCount, paramList);

			list.push_back(qc);
		}
		else {	// a 'with' clause
			size_t positionOfFullStop = clauseParams.at(index).at(0).find(".");
			string sVal = clauseParams.at(index).at(0).substr(0, positionOfFullStop);
			QueryUtility::SynonymType sType = decList.at(sVal);
			string withString = clauseParams.at(index).at(0).substr(positionOfFullStop + 1);
			QueryUtility::ClauseType cType = determineWithClauseType(withString);
			int paraCount = 2;
			string equalVal = clauseParams.at(index).at(1);
			QueryParam qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH, sType, sVal);
			QueryParam qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH, sType, equalVal);

			paramList.push_back(qp1);
			paramList.push_back(qp2);
			
			QueryClause qc = QueryClause(cType, sVal, paraCount, paramList);

			list.push_back(qc);
		}


	}

	return list;
}

QueryParam QueryExtractor::createQueryParam(string input, unordered_map<string, QueryUtility::SynonymType> decList) {
	QueryParam qp;
	unordered_map<string, QueryUtility::SynonymType>::const_iterator exist = decList.find(input);
	// if string can be found in decList, means it is a synonym
	if (exist != decList.end()) {
		QueryUtility::ParamType paramtype = QueryUtility::PARAMTYPE_SYNONYM;
		QueryUtility::SynonymType synontype = exist->second;
		string value = exist->first;
		qp = QueryParam(paramtype, synontype, value);

	}
	else {
		// wild card
		if (input == "_") {
			QueryUtility::ParamType paramtype = QueryUtility::PARAMTYPE_PLACEHOLDER;
			QueryUtility::SynonymType synontype = QueryUtility::SYNONYM_TYPE_NULL;
			string value = input;
			qp = QueryParam(paramtype, synontype, value);
		}
		// entity
		else {
			QueryUtility::ParamType paramtype = QueryUtility::PARAMTYPE_ENT_NAME;
			QueryUtility::SynonymType synontype = QueryUtility::SYNONYM_TYPE_NULL;
			string value = input;
			qp = QueryParam(paramtype, synontype, value);
		}
	}

	return qp;
}

// This function only applies to the 2nd parameter/argument
// of a pattern-assign clause as it is a special case
// as compared to other types of parameters.
QueryParam QueryExtractor::createQueryParamForPatternAssign(string input, unordered_map<string, QueryUtility::SynonymType> decList) {
	QueryParam qp;
	QueryUtility::ParamType paramtype;
	QueryUtility::SynonymType synontype = QueryUtility::SYNONYM_TYPE_NULL;

	unordered_map<string, QueryUtility::SynonymType>::const_iterator exist = decList.find(input);

	if (input == "_") {
		paramtype = QueryUtility::PARAMTYPE_PLACEHOLDER;
	}
	// 2nd parameter is a synonym that was declared
	else if (exist != decList.end()) {
		paramtype = QueryUtility::PARAMTYPE_SYNONYM;
		synontype = exist->second;
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
				paramtype = QueryUtility::PARAMTYPE_PATTERN_STRING_BOTH_OPEN;
			}
			// Underscore exists only on left side
			else {
				paramtype = QueryUtility::PARAMTYPE_PATTERN_STRING_LEFT_OPEN;
			}

		}

		// Underscore exists only on right side
		else if (positionOfPlaceHolder == input.length() - 1) {
			paramtype = QueryUtility::PARAMTYPE_PATTERN_STRING_RIGHT_OPEN;
		}

		// Underscore does not exist
		else {
			paramtype = QueryUtility::PARAMTYPE_PATTERN_STRING_EXACT;
		}
	}

	qp = QueryParam(paramtype, synontype, input);

	return qp;
}

QueryUtility::ClauseType QueryExtractor::determineWithClauseType(string withString) {
	QueryUtility::ClauseType type = QueryUtility::CLAUSETYPE_NULL;
	
	if (withString == "procName") {
		type = QueryUtility::CLAUSETYPE_WITH_PROCNAME;
	}
	if (withString == "varName") {
		type = QueryUtility::CLAUSETYPE_WITH_VARNAME;
	}
	if (withString == "stmt#") {
		type = QueryUtility::CLAUSETYPE_WITH_STMTNO;
	}
	if (withString == "value") {
		type = QueryUtility::CLAUSETYPE_WITH_VALUE;
	}

	return type;
}