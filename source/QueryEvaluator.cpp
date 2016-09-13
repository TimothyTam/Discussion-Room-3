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
#include "PKB.h"
#include "TNode.h"
#include <list>

using namespace std;


QueryEvaluator::QueryEvaluator() {
	
}

void QueryEvaluator::evaluate(Query query, list<string>& qresult) {
	this->query = query;
	// first get all the entities of the declaration clauses and populate the results Vector
	populateResults();

	// then lets do the clauses;
	vector<QueryClause> allClauses = query.getClauseList();
	wrong.clear();
	wrong = vector<bool>(results.size());

	for (size_t j = 0; j < results.size(); j++) {
		if (wrong[j]) continue;
		vector<ResultUnit> currentTuple = results[j];

		for (size_t i = 0; i < allClauses.size(); i++) {
			if ( !checkClause(allClauses[i],currentTuple) ) {
				wrong[j] = true;
				break;
			}
		}
	}
	


	// now just get the vector of selectList[0] out of the result;
	QueryPair selectedQueryPair = query.getSelectList()[0];
	vi selectedResults = filterSelectedResults(getIdOfQueryPair(selectedQueryPair));
	
	// add/remove from resultsList
	getResultStrings(selectedResults,selectedQueryPair.getSynonymType(), qresult);
}

bool QueryEvaluator::checkClause(QueryClause clause, vector<ResultUnit> tuple) {
	vector<QueryParam> params = clause.getParametersList();
	int firstId = -1, secondId = -1;

	if (params[0].getParamType() == PARAMTYPE_SYNONYM) {
		firstId = getSynonymIndexFromName(params[0].getParamValue());
	}
	if (params[1].getParamType() == PARAMTYPE_SYNONYM) {
		secondId = getSynonymIndexFromName(params[1].getParamValue());
	}
	string varname="";
	int varIndex;
	switch (clause.getClauseType()) {
		case CLAUSETYPE_MODIFIES: 
			// modifies(a, v), modifies(a,"_"), modifies(a,"varname")
			if (secondId != -1) {
				return PKB::getInstance().whetherStmtModifies(tuple[firstId].value, tuple[secondId].value);
			}
			if (params[1].getParamValue()[0] == '_') {
				return true;
			}

			//modifies (a,"varname")
			varname = QueryEvaluator::removeQuotes(params[1].getParamValue());
			varIndex = PKB::getInstance().getVarIndexFromName(varname);
			if (varIndex == -1) return false;
			return PKB::getInstance().whetherStmtModifies(tuple[firstId].value, varIndex);
			break;

		case CLAUSETYPE_USES:

			// uses(a,v)  , uses(a,_),  uses(a,"varname")

			

			break;

		case CLAUSETYPE_FOLLOWS:
		case CLAUSETYPE_FOLLOWS_STAR:
			//follows(s1,s2), follows(_,s2), follows(s1,_), follows(_,_)
			// and the stars

			break;
		
		case CLAUSETYPE_PARENT:
		case CLAUSETYPE_PARENT_STAR:
			// parent(s1,s2), parent(_,s2), parent(s1,_), parent(_,_)
			// and the stars

		case CLAUSETYPE_PATTERN_ASSIGN:
			// pattern a(_,_), pattern a(_,"v"), pattern a(x,_),  pattern a(x,"v")
			


			break;
			
		default:
			break;
	}
	return false;
}

string QueryEvaluator::removeQuotes(string s) {
	return s.substr(1, s.length() - 2);
}

vector<int> QueryEvaluator::filterSelectedResults(int queryPairId)
{
	set<int> selectedResults;
	for (size_t i = 0; i < results.size(); i++) {
		vector<ResultUnit>* currentTuple = &results[i];
		selectedResults.insert(results[i][queryPairId].value);

	}
	vi selectedResultsVector;
	selectedResultsVector.assign(selectedResults.begin(), selectedResults.end());
	return selectedResultsVector;
}

void QueryEvaluator::getResultStrings(vector<int> selectedResults, SynonymType type, list<string>& resultStrings)
{
	for (size_t i = 0; i < selectedResults.size(); i++) {
		if (type == SYNONYM_TYPE_VARIABLE) {
			resultStrings.push_back(PKB::getInstance().getVarNameFromIndex(selectedResults[i]));
		}
		else {
			resultStrings.push_back(to_string(selectedResults[i]));
		}
	}

}

int QueryEvaluator::getIdOfQueryPair(QueryPair selectedQueryPair)
{
	return getSynonymIndexFromName(selectedQueryPair.getValue());
}

void QueryEvaluator::populateResults()
{
	vector<QueryPair> allSynonyms = query.getDeclarationList();
	tupleSize = allSynonyms.size();
	possibleResultUnits.clear();

	for (int i = 0; i < tupleSize; i++) {
		// lets get all possible values for this synonym
		vector<ResultUnit> currentSynValues;
		QueryPair currentSyn = allSynonyms[i];
		vi valuesAsInt = loadValuesFromPKB(currentSyn.getSynonymType());
		for (size_t j = 0; j < valuesAsInt.size(); j++) {
			// push a new possible resultUnit to the currentSynValues;
			currentSynValues.push_back(ResultUnit(i, valuesAsInt[j]));
		}
		
		possibleResultUnits.push_back(currentSynValues);
	}
	
	//now lets recursively add all posible tuples of <ResultUnit> < ResultUnit >...
	vUnit* vectorOfUnits = new vUnit(tupleSize);
	recursiveAddFrom(0,vectorOfUnits);
	//now results has vectors of tuples of synonyms;

}

void QueryEvaluator::recursiveAddFrom(int synIndex, vUnit* vectorOfUnits) {
	if (synIndex < tupleSize) {
		//not the end yet
		for (size_t i = 0; i < possibleResultUnits[synIndex].size(); i++) {
			vectorOfUnits->at(synIndex) = possibleResultUnits[synIndex][i];
			recursiveAddFrom(synIndex + 1, vectorOfUnits);
		}
	}
	else {
		//at the end;
		//get a copy of the vector of sequences of units;
		vUnit newVector = *vectorOfUnits;

		//append to results;
		results.push_back(newVector);
	}

}

vi QueryEvaluator::loadValuesFromPKB(SynonymType type) {

	vi values;
	int maxStmtNo;
	switch (type) {
		case SYNONYM_TYPE_ASSIGN:
			return PKB::getInstance().getAllEntityIndex(NodeType::Assign);
			break;

		case SYNONYM_TYPE_WHILE:
			return PKB::getInstance().getAllEntityIndex(NodeType::While);
			break;

		case SYNONYM_TYPE_STMT:
		case SYNONYM_TYPE_PROG_LINE:
			maxStmtNo = PKB::getInstance().getStmtCount();
			for (int i = 1; i <= maxStmtNo; i++) values.push_back(i);
			return values;
			break;
		case SYNONYM_TYPE_VARIABLE:
			//hmmm the api doesnt quite match
			//
			//
			break;
		default:
			break;
	}

	return vi();

}

int QueryEvaluator::getSynonymIndexFromName(string synName) {
	vector<QueryPair> declareList = query.getDeclarationList() ;
	for (size_t i = 0; i < declareList.size(); i++) {
		if (declareList[i].getValue() == synName)
			return i;
	}
	return -1;
}