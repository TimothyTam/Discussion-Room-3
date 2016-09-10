#pragma once

#include <stdio.h>
#include <iostream>
#include <string>

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

class QueryPair {
	public:
		QueryPair(SynonymType synType, string val);

		SynonymType getSynonymType(void);
		string getValue(void);
		bool QueryPair::operator==(QueryPair other);

	private:
		SynonymType synonymType;
		string value;
};