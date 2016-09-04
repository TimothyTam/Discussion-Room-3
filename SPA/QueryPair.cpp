#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "QueryPair.h"

QueryPair::QueryPair(void) {

}

QueryPair::~QueryPair(void) {

}

void QueryPair::setSynonymType(string str) {
	/* needs major cleanup later on; find out how to do str const || std::string const&  */

	if (str == "assign") this->type == SYNONYM_TYPE_ASSIGN;
	if (str == "BOOLEAN") this->type == SYNONYM_TYPE_BOOLEAN;
	if (str == "call") this->type == SYNONYM_TYPE_CALL;
	if (str == "constant") this->type == SYNONYM_TYPE_CONSTANT;
	if (str == "if") this->type == SYNONYM_TYPE_IF;
	if (str == "procedure") this->type == SYNONYM_TYPE_PROCEDURE;
	if (str == "prog_line") this->type == SYNONYM_TYPE_PROG_LINE;
	if (str == "stmt") this->type == SYNONYM_TYPE_STMT;
	if (str == "variable") this->type == SYNONYM_TYPE_VARIABLE;
	if (str == "while") this->type == SYNONYM_TYPE_WHILE;

}

void QueryPair::setValue(string input) {
	this->value == input;
}

SynonymType QueryPair::getSynonymType(void) {
	return this->type;
}

string QueryPair::getValue(void) {
	return this->value;
}