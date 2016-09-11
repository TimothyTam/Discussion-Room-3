#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "QueryExtractor.h"
#include "Query.h"
#include "QueryPair.h"
#include "QueryClause.h"
#include "QueryParam.h"
#include "QueryEvaluator.h"

using namespace std;

QueryEvaluator::QueryEvaluator() {
	
}

vector<string> QueryEvaluator::evaluate(Query query) {
	this->query = query;


	// add/remove from resultsList
	return this->currentResultsList;
}