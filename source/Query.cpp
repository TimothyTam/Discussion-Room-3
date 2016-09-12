#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryClause.h"

using namespace std;

Query::Query(vector<QueryPair> decList, vector<QueryPair> selList, vector<QueryClause> clauList) {
	declarationList = decList; // does this pass by reference or create new instance?
	selectList = selList;
	clauseList = clauList;
}

Query::Query(void) {

}
/*
Query::~Query(void) {

}*/

vector<QueryPair> Query::getDeclarationList(void) {
	return this->declarationList;
}

vector<QueryPair> Query::getSelectList(void) {
	return this->selectList;
}

vector<QueryClause> Query::getClauseList(void) {
	return this->clauseList;
}