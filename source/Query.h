#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryClause.h"
#include "QueryPair.h"

using namespace std;

// This class represents the user-entered query as a whole. It stores
// declarations, selects and clauses made by the user in separate vectors,
// for organised retrieval of relevant information.

class Query {
	public:
		Query(vector<QueryPair> declarationList,
			  vector<QueryPair> selectList,
			  vector<QueryClause> clauseList);
		Query();

		vector<QueryPair> getDeclarationList();
		vector<QueryPair> getSelectList();
		vector<QueryClause> getClauseList();

	private:
		vector<QueryPair> _declarationList;
		vector<QueryPair> _selectList;
		vector<QueryClause> _clauseList;
};