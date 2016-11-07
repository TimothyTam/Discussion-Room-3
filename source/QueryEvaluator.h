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
#include "PKB.h"

using namespace std;
typedef std::vector<int> vi;
typedef std::vector<pair<int, int>> vii;
typedef vector<ResultUnit> vUnit;

// not sure if needed

class QueryEvaluator {
	public:
		
		QueryEvaluator();

		bool evaluateConstantClauses();

		bool evaluateGraphEdge(EvaluationGraph * graph, GraphEdge * edge);

		bool evaluateGraph(int graphId);

		Query query;

		void cleanseQuery();

		void evaluate(Query query, list<string>& qresult);

	private:
		vector<bool> isArtiPoint;
		vector<vi> valuesOfAP;
		vector<bool> touchedAP;
		int vertexCount;
		vector<QueryClause> queryClauses;
		EvaluationGraph masterGraph;
		void returnFalse(list<string>& qresult);
		bool selectBoolean;
		vector<EvaluationGraph> allGraphs;
		vector<QueryClause> constantClauses;
		void buildMasterGraph();
		int countConnectedComponents(int removedV);
		void cutTheGraph();

		QueryUtility::SynonymType getTypeOfSynonym(string name);

		NodeType getNodeTypeFromSynType(QueryUtility::SynonymType type);

		void evaluateClause(QueryClause clause, int firstSynId, int secondSynId, vi & resultVi, vii & resultVii, bool & resultBool);


		string removeQuotes(string s);

		
		int getSynonymIndexFromName(string synName);

		string projectSynValueToString(int synValue, int selectIndex);

		string tupleToString(vi * tuple);

		void rebuildValuesOfAP(int graphId);

		void narrowDownAllGraphs(int exceptGraph);

		void expandFinalResultsWith(list<vi>* allTup, list<vi>* results, vector<bool>* gotAPinTup, map<int, int>* idOfSyn, vector<int> synAtResultIndex);

		void combineResults(list<string> & qresult);

		void recursiveAddToFinalResultsFrom(size_t curGraph, vector<list<list<int>>>& allResults, list<list<int>> &finalResults);

		//deprececated

		//filter out from 'results' the unique value of the selected synonym
		

		//add the strings representing the selectedResults to the qresult

		
		
		
		vi loadValuesFromPKB(QueryUtility::SynonymType type);
		
};