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

// not sure if needed
enum Pattern2ndParameterType {
	PATTERN_SECOND_PARAMTYPE_VARIABLE,
	PATTERN_SECOND_PARAMTYPE_CONSTANT,
	PATTERN_SECOND_PARAMTYPE_EXPRESSION
};

class QueryEvaluator {
	public:
		QueryEvaluator();

		Query query;
		vector<string> currentResultsList;

		vector<string> evaluate(Query query);
		Pattern2ndParameterType secondParamType(string value);


	private:


};