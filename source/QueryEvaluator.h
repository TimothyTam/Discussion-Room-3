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
#include <list>

using namespace std;
typedef std::vector<int> vi;
typedef vector<ResultUnit> vUnit;

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

		void evaluate(Query query, list<string>& qresult);
		// Pattern2ndParameterType secondParamType(string value);

	private:
		vector< vector<ResultUnit> > results;
		vector< vector<ResultUnit> > possibleResultUnits;
		int tupleSize;
		vector<bool> wrong;

		bool checkClause(QueryClause c, vector<ResultUnit> tuple);
		string removeQuotes(string s);
		vector<int> filterSelectedResults(int queryPairId);
		void getResultStrings(vector<int> selectedResults, SynonymType type, list<string>& qresult);
		int getIdOfQueryPair(QueryPair selectedQueryPair);
		void populateResults();
		void recursiveAddFrom(int synIndex, vUnit * vectorOfUnits);
		vi loadValuesFromPKB(SynonymType type);
		int getSynonymIndexFromName(string synName);
};