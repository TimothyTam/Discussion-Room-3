#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryClause.h"

using namespace std;

Query::Query(vector<QueryPair> declarationList, vector<QueryPair> selectList, vector<QueryClause> clauseList) {
	_declarationList = declarationList; // does this pass by reference or create new instance?
	_selectList = selectList;
	_clauseList = clauseList;
}

Query::Query(void) {

}

vector<QueryPair> Query::getDeclarationList(void) {
	return this->_declarationList;
}

vector<QueryPair> Query::getSelectList(void) {
	return this->_selectList;
}

vector<QueryClause> Query::getClauseList(void) {
	return this->_clauseList;
}