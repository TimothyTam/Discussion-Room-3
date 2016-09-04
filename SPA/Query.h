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
		int numQueryClause;
		vector<QueryPair> declarationsList;
		vector<QueryPair> selectList;
		vector<QueryClause> clauseList;
};