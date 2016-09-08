#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "QueryPair.h"


QueryPair::QueryPair(SynonymType synType, string val) {
	synonymType = synType;
	value = val;
}
/*
QueryPair::~QueryPair(void) {

}*/
/*
void QueryPair::setSynonymType(string str) {
	//needs major cleanup later on; find out how to do str const || std::string const&  

	if (str == "assign") this->synonymType = SYNONYM_TYPE_ASSIGN;
	if (str == "BOOLEAN") this->synonymType = SYNONYM_TYPE_BOOLEAN;
	if (str == "call") this->synonymType = SYNONYM_TYPE_CALL;
	if (str == "constant") this->synonymType = SYNONYM_TYPE_CONSTANT;
	if (str == "if") this->synonymType = SYNONYM_TYPE_IF;
	if (str == "procedure") this->synonymType = SYNONYM_TYPE_PROCEDURE;
	if (str == "prog_line") this->synonymType = SYNONYM_TYPE_PROG_LINE;
	if (str == "stmt") this->synonymType = SYNONYM_TYPE_STMT;
	if (str == "variable") this->synonymType = SYNONYM_TYPE_VARIABLE;
	if (str == "while") this->synonymType = SYNONYM_TYPE_WHILE;

}*/

SynonymType QueryPair::getSynonymType(void) {
	return this->synonymType;
}

string QueryPair::getValue(void) {
	return this->value;
}