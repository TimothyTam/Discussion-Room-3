#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryClause.h"

using namespace std;

Query::Query(int numQ, int numS, vector<QueryPair> decList, vector<QueryPair> selList, vector<QueryClause> clauList) {
	numQueryClause = numQ;
	numSelects = numS;
	declarationList = decList; // does this pass by reference or create new instance?
	selectList = selList;
	clauseList = clauList;
}

Query::~Query(void) {

}