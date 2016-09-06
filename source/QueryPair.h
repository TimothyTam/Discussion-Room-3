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

};

class QueryPair {
	public:
		SynonymType getSynonymType(void);
		string getValue(void);
		void setSynonymType(string);
		void setValue(string);

	private:
		SynonymType type;
		string value;
};