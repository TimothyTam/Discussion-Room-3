#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "QueryExtractor.h"
#include "Query.h"
#include "QueryPair.h"
#include "QueryClause.h"
#include "QueryParam.h"
#include "ResultUnit.h"
#include "EvaluationGraph.h"
#include <list>

using namespace std;
typedef std::vector<int> vi;
typedef std::vector<pair<int, int>> vii;
typedef vector<ResultUnit> vUnit;

// not sure if needed

class QueryEvaluator {
	public:
		QueryEvaluator();

		Query query;

		void evaluate(Query query, list<string>& qresult);

	//private:
	//	vector<EvaluationGraph> allGraphs;
	//	vector<QueryClause> constantClauses;
	//	void buildEvaluationGraphs();

	//	void evaluateClause(QueryClause clause, int firstSynId, int secondSynId, vi & resultVi, vii & resultVii, bool & resultBool);


	//	string removeQuotes(string s);

	//	int getIdOfQueryPair(QueryPair selectedQueryPair);
	//	int getSynonymIndexFromName(string synName);


	//	//deprececated

	//	//filter out from 'results' the unique value of the selected synonym
	//	vector<int> filterSelectedResults(int queryPairId);

	//	//add the strings representing the selectedResults to the qresult
	//	void getResultStrings(vector<int> selectedResults, SynonymType type, list<string>& qresult);

	//	
	//	
	//	
	//	vi loadValuesFromPKB(SynonymType type);
		
};