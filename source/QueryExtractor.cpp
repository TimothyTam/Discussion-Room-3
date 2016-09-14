#pragma once

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <unordered_map>
#include "Query.h"
#include "QueryClause.h"
#include "QueryExtractor.h"
#include "QueryParam.h"
#include "QueryPair.h"

using namespace std;

QueryExtractor::QueryExtractor(void) {
}

// "Main" function that is called by the MainQuery for preprocessing
// the query that the user has entered.
Query QueryExtractor::extract(unordered_map<string, string> declarationMap, string query) {
	Query q;

	string declarationsRemoved = removeDeclarations(query);

	vector<QueryPair> queryPairList = getDeclarations(declarationMap);
	vector<QueryPair> selectList = getSelects(declarationMap, declarationsRemoved);
	vector<QueryClause> clauseList = getClauses(declarationsRemoved);

	q = Query(queryPairList, selectList, clauseList);

	return q;
}

// This function extracts the declarations from the unordered_map
// and returns the information in the form of a vector, containing
// QueryPairs.
vector<QueryPair> QueryExtractor::getDeclarations(unordered_map<string, string> declarationMap) {
	vector<QueryPair> list;
	//cout << " about to get declarations "; 

	for (auto it = declarationMap.begin(); it != declarationMap.end(); ++it) {
		//cout << "\n map: " << it->first << " ," << it->second << "\n" ;
		SynonymType sType = determineSynonymType(it->second);
		QueryPair pair = QueryPair(sType, it->first);
		this->decHashMap.insert(std::pair<string, SynonymType>(it->first, sType));
		list.push_back(pair);
	}

	return list;
}

// This function checks the string and assigns an enum
// of SynonymType, based on what string was entered.
SynonymType QueryExtractor::determineSynonymType(string str) {

	if (str == "assign") return SYNONYM_TYPE_ASSIGN;
	if (str == "BOOLEAN") return SYNONYM_TYPE_BOOLEAN;
	if (str == "call") return SYNONYM_TYPE_CALL;
	if (str == "constant") return SYNONYM_TYPE_CONSTANT;
	if (str == "if") return SYNONYM_TYPE_IF;
	if (str == "procedure") return SYNONYM_TYPE_PROCEDURE;
	if (str == "prog_line") return SYNONYM_TYPE_PROG_LINE;
	if (str == "stmt") return SYNONYM_TYPE_STMT;
	if (str == "variable") return SYNONYM_TYPE_VARIABLE;
	if (str == "while") return SYNONYM_TYPE_WHILE;

	else
		return SYNONYM_TYPE_NULL;
}

// This function chops away the declarations that have
// been processed in an earlier called function, returning
// the string that begins with "Select"
string QueryExtractor::removeDeclarations(string input) {
	string str;

	std::size_t pos = input.find("Select");
	str = input.substr(pos);

	return str;
}

// This function processes and returns a vector of QueryPairs
// that correspond to the user's desired selects after making use
// of the declarations inside the unordered_map.
vector<QueryPair> QueryExtractor::getSelects(unordered_map<string, string> map, string input) {

	vector<QueryPair> outputList;
	vector<string> selectStringList = sanitiseForSelects(input);

	for (string s : selectStringList) {
		SynonymType type = determineSynonymType(map.at(s));
		QueryPair pair = QueryPair(type, s);
		
		outputList.push_back(pair);
	}
	
	return outputList;
}

// This function cleans up the string of selects and removes
// non-alphanumeric characters so that the synonym value can
// be extracted and its type determined.
vector<string> QueryExtractor::sanitiseForSelects(string input) {
	
	istringstream iss(input);
	vector<string> selects;

	do {
		string substr;
		iss >> substr;

		if (substr.length() == 0 || substr == "such" || substr == "pattern") {
			break;
		}

		if (substr == "Select") {
			continue;
		}

		selects.push_back(substr);

	} while (iss);

	vector<string> sanitised;

	//removing non alphanumeric characters
	for (string s : selects) {
		size_t len = s.length();
		size_t index = 0;

		while (index < len) {
			if (!isalnum(s[index])) {
				s = s.erase(index, 1);
				len--;
			}
			else {
				index++;
			}
		}

		sanitised.push_back(s);
	}

	return sanitised;
}

// This function extracts and returns the clauses entered
// in a vector containing >= 1 QueryClauses.
vector<QueryClause> QueryExtractor::getClauses(string input) {

	vector<QueryClause> clauses;
	string clausesOnward = removeSpaces(input);

	size_t positionOfSuchThat;
	size_t positionOfPattern;
	//size_t positionOfWith = clausesOnward.find("with");
	size_t positionOfAnd;
	size_t firstToAppear;

	size_t positionOfOpenBracket;
	size_t positionOfComma;
	size_t positionOfCloseBracket;

	while (clausesOnward.find("suchthat") != string::npos
			|| clausesOnward.find("pattern") != string::npos
			|| clausesOnward.find("and") != string::npos) {

		positionOfSuchThat = clausesOnward.find("suchthat");
		positionOfPattern = clausesOnward.find("pattern");
		positionOfAnd = clausesOnward.find("and");

		// if a suchthat appears first
		if (positionOfSuchThat < positionOfPattern && positionOfSuchThat < positionOfAnd) {
			firstToAppear = positionOfSuchThat;
		}
		// if a pattern appears first
		else if (positionOfPattern < positionOfSuchThat && positionOfPattern < positionOfAnd) {
			firstToAppear = positionOfPattern;
		}
		// if an and appears first
		else {
			firstToAppear = positionOfAnd;
			
			if (positionOfAnd + 3 == positionOfPattern) {
				firstToAppear = positionOfPattern;
			}
			else if (positionOfAnd + 3 == positionOfSuchThat) {
				firstToAppear = positionOfSuchThat;
			}
		}

		clausesOnward = clausesOnward.substr(firstToAppear);

		positionOfOpenBracket = clausesOnward.find("(");
		positionOfComma = clausesOnward.find(",");
		positionOfCloseBracket = clausesOnward.find(")");

		string clause;
		string next = "none";

		if (firstToAppear == positionOfPattern) {
			clause = clausesOnward.substr(0, 7);
			next = clausesOnward.substr(7, positionOfOpenBracket-7);
		}
		else if (firstToAppear == positionOfSuchThat) { // suchthat is 8 letters
			clause = clausesOnward.substr(8, positionOfOpenBracket-8);
		}
		else {
			clause = clausesOnward.substr(3, positionOfOpenBracket-3);
		}
		
		ClauseType clauseType = determineClauseType(clause, next);

		// ** IMPORANT **
		// does not yet account for clauses with > 2 parameters, i.e. pattern if
		string parameter1 = clausesOnward.substr(positionOfOpenBracket + 1, positionOfComma - positionOfOpenBracket - 1);
		string parameter2 = clausesOnward.substr(positionOfComma + 1, positionOfCloseBracket - positionOfComma - 1);

		QueryParam param1 = createQueryParam(parameter1);
		QueryParam param2;
		string synonymValue = "none";
		if (clauseType == CLAUSETYPE_PATTERN_ASSIGN) {
			synonymValue = next;
			param2 = createQueryParamForPatternAssign(parameter2);
		}
		else {
			param2 = createQueryParam(parameter2);
		}

		// To change next time to account for pattern if, which has 3 parameters
		int paramCount = 2;

		vector<QueryParam> paraList;
		paraList.push_back(param1);
		paraList.push_back(param2);

		QueryClause qc = QueryClause(clauseType, synonymValue, 2, paraList);
		clauses.push_back(qc);

		clausesOnward = clausesOnward.substr(positionOfCloseBracket+1);
	}
	
	return clauses;
}

// This function removes whitespaces from the query so
// that it does not matter if the user inputs spaces
// accidentally or intentionally
string QueryExtractor::removeSpaces(string input) {
	istringstream iss(input);
	vector<string> splitString;

	do {
		string substr;
		iss >> substr;

		if (substr.length() == 0) {
			break;
		}

		splitString.push_back(substr);

	} while (iss);

	string output;

	for (string s : splitString) {
		output.append(s);
	}

	return output;
}

// This function takes in a string (and the next token
// as well, for pattern clauses) and runs a check to
// determine what type of clause it is, then returning
// the corresponding enum.
ClauseType QueryExtractor::determineClauseType(string input, string next) {
	if (input == "Modifies") return CLAUSETYPE_MODIFIES;
	if (input == "Uses") return CLAUSETYPE_USES;
	if (input == "Follows") return CLAUSETYPE_FOLLOWS;
	if (input == "Follows*") return CLAUSETYPE_FOLLOWS_STAR;
	if (input == "Parent") return CLAUSETYPE_PARENT;
	if (input == "Parent*") return CLAUSETYPE_PARENT_STAR;

	if (input == "pattern") {
		if (this->decHashMap.at(next) == SYNONYM_TYPE_ASSIGN) {
			return CLAUSETYPE_PATTERN_ASSIGN;
		}
		else if (this->decHashMap.at(next) == SYNONYM_TYPE_IF) {
			return CLAUSETYPE_PATTERN_IF;
		}
		else if (this->decHashMap.at(next) == SYNONYM_TYPE_WHILE) {
			return CLAUSETYPE_PATTERN_WHILE;
		}

		// works for interation 1
		return CLAUSETYPE_PATTERN_ASSIGN;
	}

	return CLAUSETYPE_NULL;

}

// This method builds a QueryParam object using the
// entered string, after checking it against the map
// of declarations for what type of parameter/argument
// it is.
QueryParam QueryExtractor::createQueryParam(string input) {
	QueryParam qp;

	unordered_map<string, SynonymType>::const_iterator exist = this->decHashMap.find(input);

	// if string can be found in decHashMap, means it is a synonym
	if (exist != this->decHashMap.end()) {
		ParamType paramtype = PARAMTYPE_SYNONYM;
		SynonymType synontype = exist->second;
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
QueryParam QueryExtractor::createQueryParamForPatternAssign(string input) {
	QueryParam qp;
	ParamType paramtype;
	SynonymType synontype = SYNONYM_TYPE_NULL;

	unordered_map<string, SynonymType>::const_iterator exist = this->decHashMap.find(input);

	if (input == "_") {
		paramtype = PARAMTYPE_PLACEHOLDER;
	}
	// 2nd parameter is a synonym that was declared
	else if (exist != this->decHashMap.end()) {
		paramtype = PARAMTYPE_SYNONYM;
		synontype = exist->second;
	}
	else {

		size_t positionOfPlaceHolder = input.find("_");
		size_t positionOfNextPlaceHolder;

		// Underscore exists on left side
		if (positionOfPlaceHolder == 0) {
			string chopped = input.substr(positionOfPlaceHolder+1);
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