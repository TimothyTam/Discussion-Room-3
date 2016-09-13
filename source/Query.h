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
		Query(vector<QueryPair> decList, vector<QueryPair> selList, vector<QueryClause> clauList);
		Query();
		vector<QueryPair> getDeclarationList();
		vector<QueryPair> getSelectList();
		vector<QueryClause> getClauseList();

	private:
		vector<QueryPair> _declarationList;
		vector<QueryPair> _selectList;
		vector<QueryClause> _clauseList;
};