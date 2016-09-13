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
	// first get all the entities of the declaration clauses and populate the results Vector
	populateResults();

	// then lets do the clauses;
	vector<QueryClause> allClauses = query.getClauseList();
	for (size_t i = 0; i < allClauses.size(); i++) {
		applyClause(allClauses[i]);
	}
	
	// now just get the vector of selectList[0] out of the result;
	QueryPair selectedQueryPair = query.getSelectList()[0];
	vector<ResultUnit> selectedResults = filterSelectedResults(getIdOfQueryPair(selectedQueryPair));

	// add/remove from resultsList
	return projectResults(selectedResults);
}

void QueryEvaluator::applyClause(QueryClause c)
{
}

vector<ResultUnit> QueryEvaluator::filterSelectedResults(int queryPairId)
{
	return vector<ResultUnit>();
}

vector<string> QueryEvaluator::projectResults(vector<ResultUnit> selectedResults)
{
	return vector<string>();
}

int QueryEvaluator::getIdOfQueryPair(QueryPair selectedQueryPair)
{
	return 0;
}

void QueryEvaluator::populateResults()
{
	vector<QueryPair> allSynonyms = query.getDeclarationList();
	int tupleSize = allSynonyms.size();
	vector< vector<ResultUnit> > possibleResultUnits;
	for (int i = 0; i < tupleSize; i++) {
		// lets get all possible values for this synonym

	}
}
