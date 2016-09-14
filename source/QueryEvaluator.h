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

		

		void evaluate(Query query, list<string>& qresult);

	private:
		//vector of all the tuples of possible combinations of declared synonyms
		vector< vector<ResultUnit> > results;
		
		// possibleResultUnits[i] stores all the possible values for 
		// synonym with id = i
		vector< vector<ResultUnit> > possibleResultUnits;

		int tupleSize;

		// a vector keeping track of which tuples have been marked wrong
		vector<bool> wrong;
		
		//check if the tuple is correct according to the Clause c
		bool checkClause(QueryClause c, vector<ResultUnit> tuple);

		string removeQuotes(string s);
		//filter out from 'results' the unique value of the selected synonym
		vector<int> filterSelectedResults(int queryPairId);

		//add the strings representing the selectedResults to the qresult
		void getResultStrings(vector<int> selectedResults, SynonymType type, list<string>& qresult);

		int getIdOfQueryPair(QueryPair selectedQueryPair);
		void populateResults();
		void recursiveAddFrom(int synIndex, vUnit * vectorOfUnits);
		vi loadValuesFromPKB(SynonymType type);
		int getSynonymIndexFromName(string synName);
};