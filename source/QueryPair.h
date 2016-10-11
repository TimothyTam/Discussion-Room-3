#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "QueryUtility.h"

using namespace std;

enum SynonymType {
	SYNONYM_TYPE_ASSIGN,
	SYNONYM_TYPE_BOOLEAN,
	SYNONYM_TYPE_CALL,
	SYNONYM_TYPE_CONSTANT,
	SYNONYM_TYPE_IF,
	SYNONYM_TYPE_PROCEDURE,
	SYNONYM_TYPE_PROG_LINE,
	SYNONYM_TYPE_STMT,
	SYNONYM_TYPE_VARIABLE,
	SYNONYM_TYPE_WHILE,

	SYNONYM_TYPE_NULL

};

// This class is used mainly for the user's declarations
// as well as selects. The information is stored as a pair
// of values: the type and value of the synonym.

class QueryPair {
	public:
		QueryPair(QueryUtility::SynonymType synType, string val);

		QueryUtility::SynonymType getSynonymType(void);
		string getValue(void);
		bool QueryPair::operator==(QueryPair other);

	private:
		QueryUtility::SynonymType synonymType;
		string value;
};