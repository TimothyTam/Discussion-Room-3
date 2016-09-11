#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "QueryExtractor.h"
#include "Query.h"
#include "QueryPair.h"
#include "QueryClause.h"
#include "QueryParam.h"

using namespace std;

class QueryEvaluator {
	public:
		QueryEvaluator();

		Query query;
		vector<string> currentResultsList;

		vector<string> evaluate(Query query);


	private:


};