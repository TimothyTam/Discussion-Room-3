#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
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
	size_t positionOfFullStop = selectString.find(".");
	string value;

	// not a tuple or call.procName
	if (positionOfComma == string::npos && positionOfFullStop == string::npos && selectString != "BOOLEAN") {
		QueryPair qp = QueryPair(decList.at(selectString), selectString);
		list.push_back(qp);
	}
	// Select BOOLEAN
	else if (selectString == "BOOLEAN") {
		QueryPair qp = QueryPair(QueryUtility::SYNONYM_TYPE_BOOLEAN, selectString);
		list.push_back(qp);
	}
	// lone call.procName
	else if (positionOfComma == string::npos && positionOfFullStop != string::npos) {
		string sval = selectString.substr(0, positionOfFullStop);
		QueryPair qp = QueryPair(QueryUtility::SYNONYM_TYPE_CALL_PROCNAME, sval);
		list.push_back(qp);
	}
	// tuple
	else {
	selectString = selectString.substr(1); //removing "<"

		do {
			positionOfComma = selectString.find(",");
			value = selectString.substr(0, positionOfComma);

			positionOfFullStop = value.find(".");

			if (positionOfFullStop != string::npos) {
				string sval = value.substr(0, positionOfFullStop);
				QueryPair qp = QueryPair(QueryUtility::SYNONYM_TYPE_CALL_PROCNAME, sval);
				list.push_back(qp);
			}
			else {
				QueryPair qp = QueryPair(decList.at(value), value);
				list.push_back(qp);
			}
			
			selectString = selectString.substr(value.length() + 1);

			positionOfComma = selectString.find(",");
		} while (positionOfComma != string::npos);

		value = selectString.substr(0, selectString.length()-1);  // removing ">"

		positionOfFullStop = value.find(".");

		if (positionOfFullStop != string::npos) {
			string sval = value.substr(0, positionOfFullStop);
			QueryPair qp = QueryPair(QueryUtility::SYNONYM_TYPE_CALL_PROCNAME, sval);
			list.push_back(qp);
		}
		else {
			QueryPair qp = QueryPair(decList.at(value), value);
			list.push_back(qp);
		}

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
			/*
			#1 v.varName = p.procName
			#2 5 = a.stmt#
			#3 select progline with progline=c.value
			#4 5 = 5
			*/

			string leftHandSide = clauseParams.at(index).at(0);
			string rightHandSide = clauseParams.at(index).at(1);
			QueryParam qpTemp1 = createQueryParamForWith(leftHandSide, decList);
			QueryParam qpTemp2 = createQueryParamForWith(rightHandSide, decList);
			QueryParam qp1;
			QueryParam qp2;

			QueryUtility::ClauseType cType;

			if (qpTemp1.getParamType() == QueryUtility::PARAMTYPE_NULL && qpTemp2.getParamType() == QueryUtility::PARAMTYPE_NULL) {
				// either 5 = 5 or "IDENT" = "IDENT"
				cType = QueryUtility::CLAUSETYPE_WITH;
				qp1 = QueryParam(qpTemp1.getParamType(), qpTemp1.getSynonymType(), qpTemp1.getParamValue());
				qp2 = QueryParam(qpTemp2.getParamType(), qpTemp2.getSynonymType(), qpTemp2.getParamValue());
			}
			else if (qpTemp1.getParamType() == QueryUtility::PARAMTYPE_NULL) {
				qp1 = QueryParam(qpTemp2.getParamType(), qpTemp1.getSynonymType(), qpTemp1.getParamValue());
				qp2 = QueryParam(qpTemp2.getParamType(), qpTemp2.getSynonymType(), qpTemp2.getParamValue());
				cType = determineWithClauseTypeForWith(qp1.getParamType());
			}
			else if (qpTemp2.getParamType() == QueryUtility::PARAMTYPE_NULL) {
				qp2 = QueryParam(qpTemp1.getParamType(), qpTemp2.getSynonymType(), qpTemp2.getParamValue());
				qp1 = QueryParam(qpTemp1.getParamType(), qpTemp1.getSynonymType(), qpTemp1.getParamValue());
				cType = determineWithClauseTypeForWith(qp2.getParamType());
			}
			else {
				qp1 = QueryParam(qpTemp1.getParamType(), qpTemp1.getSynonymType(), qpTemp1.getParamValue());
				qp2 = QueryParam(qpTemp2.getParamType(), qpTemp2.getSynonymType(), qpTemp2.getParamValue());
				cType = determineWithClauseTypeForWith(qp2.getParamType());
			}

			
			int paraCount = 2;

			paramList.push_back(qp1);
			paramList.push_back(qp2);
			
			QueryClause qc = QueryClause(cType, "none", paraCount, paramList);
			
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

QueryParam QueryExtractor::createQueryParamForWith(string input, unordered_map<string, QueryUtility::SynonymType> decList) {
	QueryParam qp;

	unordered_map<string, QueryUtility::SynonymType>::const_iterator exist;

	QueryUtility::ParamType ptype;
	QueryUtility::SynonymType stype = QueryUtility::SYNONYM_TYPE_NULL;
	string paramValue;
	string synonValue = "none";

	size_t positionOfFullStop = input.find(".");

	if (positionOfFullStop == string::npos) {			// progline synonym, IDENT or INT
		if (is_number(input)) {
			ptype = QueryUtility::PARAMTYPE_NULL;		//undetermined at this point
		}
		else {
			exist = decList.find(input);

			if (exist != decList.end()) {		// synonym progline
				stype = decList.at(input);
				synonValue = input;
				ptype = QueryUtility::PARAMTYPE_WITH_PROG_LINE;
			}
			else {								// "IDENT"
				stype = QueryUtility::SYNONYM_TYPE_NULL;
				ptype = QueryUtility::PARAMTYPE_NULL;   //undetermined at this point
			}

		}

		paramValue = input;

	}
	else {
		synonValue = input.substr(0, positionOfFullStop);
		stype = decList.at(synonValue);
		string withString = input.substr(positionOfFullStop + 1);
		ptype = determineWithParamType(withString);
		paramValue = synonValue;
	}

	qp = QueryParam(ptype, stype, paramValue);

	return qp;
}

QueryUtility::ClauseType QueryExtractor::determineWithClauseType(string withString) {
	QueryUtility::ClauseType type = QueryUtility::CLAUSETYPE_NULL;
	
	if (withString == "procName") {
		type = QueryUtility::CLAUSETYPE_WITH_STRING;
	}
	if (withString == "varName") {
		type = QueryUtility::CLAUSETYPE_WITH_STRING;
	}
	if (withString == "stmt#") {
		type = QueryUtility::CLAUSETYPE_WITH_INT;
	}
	if (withString == "value") {
		type = QueryUtility::CLAUSETYPE_WITH_INT;
	}

	return type;
}

QueryUtility::ParamType QueryExtractor::determineWithParamType(string withString) {
	QueryUtility::ParamType type = QueryUtility::PARAMTYPE_NULL;

	if (withString == "procName") {
		type = QueryUtility::PARAMTYPE_WITH_PROCNAME;
	}
	if (withString == "varName") {
		type = QueryUtility::PARAMTYPE_WITH_VARNAME;
	}
	if (withString == "stmt#") {
		type = QueryUtility::PARAMTYPE_WITH_STMTNO;
	}
	if (withString == "value") {
		type = QueryUtility::PARAMTYPE_WITH_VALUE;
	}

	return type;
}

bool QueryExtractor::is_number(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

QueryUtility::ClauseType QueryExtractor::determineWithClauseTypeForWith(QueryUtility::ParamType ptype) {
	QueryUtility::ClauseType type = QueryUtility::CLAUSETYPE_NULL;

	if (ptype == QueryUtility::PARAMTYPE_WITH_STMTNO || ptype == QueryUtility::PARAMTYPE_WITH_VALUE
		|| ptype == QueryUtility::PARAMTYPE_WITH_PROG_LINE) {
		type = QueryUtility::CLAUSETYPE_WITH_INT;
	}
	else {
		type = QueryUtility::CLAUSETYPE_WITH_STRING;
	}

	return type;
}