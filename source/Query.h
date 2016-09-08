#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryClause.h"
#include "QueryPair.h"

using namespace std;

class Query {
	public:
		Query(int numQ, int numS, vector<QueryPair> decList, vector<QueryPair> selList, vector<QueryClause> clauList);
		int getNumQueryClause;
		int getNumSelects;
		vector<QueryPair> getDeclarationList();
		vector<QueryPair> getSelectList();
		vector<QueryClause> getClauseList();

	private:
		int numQueryClause;
		int numSelects;
		vector<QueryPair> declarationList;
		vector<QueryPair> selectList;
		vector<QueryClause> clauseList;
};