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
#include <queue>
#include <list>


using namespace std;


QueryEvaluator::QueryEvaluator() {
	
}


void QueryEvaluator::evaluate(Query query, list<string>& qresult) {
	this->query = query;
	//Debuggin messages


	//First, build the Evaluation Graph for each connected component of the whole Synonym's Graph
	//store the components in vector<EvaluationGraph> allGraphs
	
	//buildEvaluationGraphs();

	//Next, evaluate the constantClauses, if any of them is false, no need to evaluate anymore

	//Then, evaluate each EvaluationGraph.


	//And finally, combine all the results in all the EvaluationGraph












	// then lets do the clauses;
	//vector<QueryClause> allClauses = query.getClauseList();
	//wrong.clear();
	//wrong = vector<bool>(results.size());

	//for (size_t i = 0; i < allClauses.size(); i++) {
	//	if (allClauses[i].getClauseType() == CLAUSETYPE_PATTERN_ASSIGN) {
	//		vector<QueryParam> params = allClauses[i].getParametersList();
	//		try {
	//			vi dummy = PKB::getInstance().getPatternAssign(-1, params[1].getParamValue());
	//		}
	//		catch (exception ex) {
	//			cout << ex.what() << "\n";
	//			return;
	//		}

	//	}
	//}

	//for (size_t j = 0; j < results.size(); j++) {
	//	if (wrong[j]) continue;
	//	vector<ResultUnit> currentTuple = results[j];
	//	//cout << "current tuple: " << currentTuple[0].value << "," << currentTuple[1].value << "\n";

	//	for (size_t i = 0; i < allClauses.size(); i++) {
	//		if (!checkClause(allClauses[i], currentTuple)) {
	//			wrong[j] = true;
	//			break;
	//		}
	//	}
	//}



	//// now just get the vector of selectList[0] out of the result;
	//QueryPair selectedQueryPair = query.getSelectList()[0];
	//vi selectedResults = filterSelectedResults(getIdOfQueryPair(selectedQueryPair));

	//// add/remove from resultsList
	//getResultStrings(selectedResults, selectedQueryPair.getSynonymType(), qresult);
	////cout << " RESULT: \n";
	///*for (list<string>::iterator ii = qresult.begin(); ii != qresult.end(); ii++) {
	//cout << *ii << ",";
	//}*/
}

//
//void QueryEvaluator::buildEvaluationGraphs() {
//	vector<vector<GraphEdge>> adList;
//	
//	int vertexCount = query.getDeclarationList().size();
//	vector<QueryClause> clauses = query.getClauseList();
//
//	adList.reserve(vertexCount);
//	//go through each clause and build the respective graph edges for that relationship
//	for (size_t i = 0; i < clauses.size(); i++) {
//		QueryClause clause = clauses[i];
//		vector<QueryParam> params = clause.getParametersList();
//
//		// lets add the appropriate edge for this relationship
//
//		//first lets find out the synonym id of any involved synonyms
//		int firstSynId = -1;
//		int secondSynId = -1;
//		if (params[0].getParamType() == PARAMTYPE_SYNONYM) {
//			firstSynId = getSynonymIndexFromName(params[0].getParamValue());
//		}
//		if (params[1].getParamType() == PARAMTYPE_SYNONYM) {
//			secondSynId = getSynonymIndexFromName(params[1].getParamValue());
//		}
//
//		if (clause.getClauseType() == CLAUSETYPE_PATTERN_ASSIGN
//			|| clause.getClauseType() == CLAUSETYPE_PATTERN_IF
//			|| clause.getClauseType() == CLAUSETYPE_PATTERN_WHILE) {
//			firstSynId = getSynonymIndexFromName( clause.getSynonymValue() );
//			if (params[0].getParamType() == PARAMTYPE_SYNONYM) {
//				secondSynId = getSynonymIndexFromName(params[0].getParamValue());
//			}
//		}
//
//		//now lets set the forward edge. Eg. if Follows(a,w) we set edge (w,Clause, True)
//		if (firstSynId != -1) // if its not a synonym, the vertex will be -1
//			adList[firstSynId].push_back(GraphEdge(secondSynId, clause, true));
//
//		//now only set the backward edge if the secondSynId is not -1
//		if (secondSynId != -1) // if its not a synonym, the vertex will be -1
//			adList[secondSynId].push_back(GraphEdge(firstSynId, clause, false));
//
//		//we still have the case of two constant params: like Next(1,2)
//		//we store all those clauses in a vector<QueryClause> constantClauses;
//		constantClauses.push_back(clause);
//
//	}
//
//	//done building the forest
//	//now we just need to bfs and find the connected components;
//	vector<bool> done(vertexCount, false);
//	int doneCount = 0;
//	for (int curVertex = 0; curVertex < vertexCount; curVertex++) {
//		if (done[curVertex]) continue;
//		//if not done, this is a new connected component, lets bfs from here;
//		vi verticesList;
//		queue<int> bfsQueue;
//		bfsQueue.push(curVertex);
//		done[curVertex] = true;
//		while (!bfsQueue.empty()) {
//			int bfsVertex = bfsQueue.front();
//			bfsQueue.pop();
//			verticesList.push_back(bfsVertex);
//
//			for (size_t i = 0; i < adList[bfsVertex].size(); i++) {
//				int neighbour = adList[bfsVertex][i].vertex;
//				if (neighbour == -1) continue;
//				if (done[neighbour]) continue;
//				bfsQueue.push(neighbour);
//				done[neighbour] = true;
//			}
//		}
//		//done bfs-ing, we got a new GraphEdge
//		allGraphs.push_back(EvaluationGraph(verticesList, adList));
//	}
//}
//
//
////This function evaluates the clause using specific values for the synonyms if specified,
////Hence the result could be in the form of vector<int>, vector<pair<int,int>> or bool
//void QueryEvaluator::evaluateClause(QueryClause clause, int firstValue, int secondValue, vi & resultVi, vii & resultVii, bool & resultBool) {
//	vector<QueryParam> params = clause.getParametersList();
//	int zeroValue = -1;
//	bool hasFirstSyn = false, hasSecondSyn = false;
//
//	if (clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_ASSIGN
//		|| clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_IF
//		|| clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_WHILE) {
//		//firstSynValue is for a in 'pattern a(v,"vsdf")'
//		//secondSynValue or params[0] will be for v
//		zeroValue = firstValue;
//		firstValue = secondValue;
//	}
//
//	//cout << "params[0] = " << params[0].getParamValue() << " params[1]= " << params[1].getParamValue() << "\n";
//	if (params[0].getParamType() == PARAMTYPE_SYNONYM) {
//		hasFirstSyn = true;
//	}
//
//	if (params[0].getParamType() == PARAMTYPE_ENT_NAME) {
//		//get the firstValue if it is a specific int
//		if (params[0].getParamValue()[0] != '"')
//			firstValue = stoi(params[0].getParamValue());
//	}
//
//
//	if (params[1].getParamType() == PARAMTYPE_SYNONYM) {
//		hasSecondSyn = true;
//	}
//
//	if (params[1].getParamType() == PARAMTYPE_ENT_NAME) {
//		if (params[1].getParamValue()[0] != '"')
//			secondValue = stoi(params[1].getParamValue());
//	}
//
//
//	//cout << "firstID = " << firstId << " secondID =" << secondId << "\n";
//	//cout << "firstVal= " << firstVal << "secondVal" << secondVal << "\n";
//
//	string varname = "";
//	vi tempVector;
//	int varIndex;
//	bool secondCase = true;
//	switch (clause.getClauseType()) {
//	case QueryUtility::CLAUSETYPE_MODIFIES:
//		//modifies(a, v), modifies(a,"_"), modifies(a,"varname")
//
//		//there will be cases of:
//		// (syn,syn) but firstValue/secondValue might be -1/specific
//		// (syn,"v") but firstValue might be -1/specific
//		// (1,syn) but secondValue might be -1/specific
//		// (1,"v")
//		// so I guess lets find out the effective firstValue/secondValue
//		// well firstValue is already effective, just need to settle secondValue
//		if (!hasSecondSyn) {
//			secondValue = PKB::getInstance().getVarIndexFromName(params[1].getParamValue());
//		}
//
//		if (secondId != -1) {
//			//cout << " Modifies (syn,syn)";
//			return PKB::getInstance().whetherStmtModifies(firstVal, tuple[secondId].value);
//
//		}
//		if (params[1].getParamValue()[0] == '_') {
//			return true;
//		}
//		//modifies (a,"varname")
//		varname = QueryEvaluator::removeQuotes(params[1].getParamValue());
//		varIndex = PKB::getInstance().getVarIndexFromName(varname);
//
//		//cout << " Modifies (syn, 'var') , varname=" << varname << "var index=" << varIndex ;
//		//cout << "stmt at hand has value: " << firstVal << "\n";
//		if (varIndex == -1) return false;
//		//cout << PKB::getInstance().whetherStmtModifies(firstVal, varIndex);
//		return PKB::getInstance().whetherStmtModifies(firstVal, varIndex);
//		break;
//
//	case CLAUSETYPE_USES:
//		// uses(a,v)  , uses(a,_),  uses(a,"varname")
//		if (secondId != -1) {
//			return PKB::getInstance().whetherStmtUses(firstVal, tuple[secondId].value);
//		}
//		if (params[1].getParamValue()[0] == '_') {
//			//whether this one uses anything;
//			return !PKB::getInstance().getVarUsedByStmt(firstVal, NodeType::StmtLst).empty();
//		}
//		//uses(a,"varname")
//		varname = QueryEvaluator::removeQuotes(params[1].getParamValue());
//		varIndex = PKB::getInstance().getVarIndexFromName(varname);
//		if (varIndex == -1) return false;
//		return PKB::getInstance().whetherStmtUses(firstVal, varIndex);
//
//		break;
//
//	case CLAUSETYPE_FOLLOWS:
//		secondCase = 0;
//	case CLAUSETYPE_FOLLOWS_STAR:
//		//follows(s1,s2), follows(_,s2), follows(s1,_), follows(_,_)
//		// and the stars
//
//		//follows(_,_) follows*(_,_)
//		if (secondVal == -1 && firstVal == -1) {
//			return PKB::getInstance().getStmtsFollowedByStmt(NodeType::StmtLst, NodeType::StmtLst).size() > 0;
//		}
//
//		//follows(s1,s2);
//		if (secondVal != -1 && firstVal != -1) {
//			if (secondCase) {
//				return PKB::getInstance().whetherTransitivelyFollows(firstVal, secondVal);
//			}
//			else {
//				return PKB::getInstance().whetherFollows(firstVal, secondVal);
//			}
//		}
//
//		//follows(s1,_);
//		if (firstVal != -1 && secondVal == -1) {
//			return PKB::getInstance().getStmtFollowedByStmt(firstVal, NodeType::StmtLst) != 0;
//		}
//
//		//follows(_,s1):
//		if (firstVal == -1 && secondVal != -1) {
//			return PKB::getInstance().getStmtFollowingStmt(secondVal, NodeType::StmtLst) != 0;
//		}
//		break;
//
//	case CLAUSETYPE_PARENT:
//		secondCase = 0;
//	case CLAUSETYPE_PARENT_STAR:
//		// parent(s1,s2), parent(_,s2), parent(s1,_), parent(_,_)
//		// and the stars
//
//		//parent(_,_) and parent*(_,_)
//		if (secondVal == -1 && firstVal == -1) {
//			return PKB::getInstance().getTransitiveParentOfStmt(NodeType::StmtLst, NodeType::StmtLst).size() > 0;
//		}
//
//		//parent(s1,s2);
//		if (secondVal != -1 && firstVal != -1) {
//			if (secondCase) {
//				return PKB::getInstance().whetherTransitiveParent(firstVal, secondVal);
//			}
//			else {
//				return PKB::getInstance().whetherParent(firstVal, secondVal);
//			}
//		}
//
//		//parent(s1,_);
//		if (firstVal != -1 && secondVal == -1) {
//			return PKB::getInstance().getChildOfStmt(firstVal, NodeType::StmtLst).empty() == false;
//		}
//
//		//parent(_,s1):
//		if (firstVal == -1 && secondVal != -1) {
//			return PKB::getInstance().getTransitiveParentOfStmt(secondVal, NodeType::StmtLst).empty() == false;
//		}
//
//		break;
//
//
//	case CLAUSETYPE_PATTERN_ASSIGN:
//		//pattern a(_,_), pattern a(_,"v"), pattern a(x,_),  pattern a(x,"v"), pattern a(x,_"v"_)
//		zeroId = getSynonymIndexFromName(clause.getSynonymValue());
//
//		//cout << "pattern string= '" << params[1].getParamValue() << "'\n";
//		//a(_,...)
//		try {
//			if (firstId == -1) {
//				if (params[0].getParamValue() != "_") {
//					varIndex = PKB::getInstance().getVarIndexFromName(removeQuotes(params[0].getParamValue()));
//					//cout << "varIndex in pattern('var',..) is " << varIndex << "\n";
//					tempVector = PKB::getInstance().getPatternAssign(varIndex, params[1].getParamValue());
//				}
//				else {
//					tempVector = PKB::getInstance().getPatternAssign(-1, params[1].getParamValue());
//				}
//
//				return find(tempVector.begin(), tempVector.end(), tuple[zeroId].value) != tempVector.end();
//			}
//
//			//a(v,...)
//
//			tempVector = PKB::getInstance().getPatternAssign(tuple[firstId].value, params[1].getParamValue());
//			return find(tempVector.begin(), tempVector.end(), tuple[zeroId].value) != tempVector.end();
//		}
//		catch (exception ex) {
//			cout << "\n" << ex.what() << "\n";
//		}
//
//		break;
//
//	default:
//		break;
//	}
//	return false;
//}
//
//
////check if the tuple is correct according to the Clause c
//bool QueryEvaluator::checkClause(QueryClause clause, vector<ResultUnit> tuple) {
//	
//}
//
//string QueryEvaluator::removeQuotes(string s) {
//	return s.substr(1, s.length() - 2);
//}
//
////filter out from 'results' the unique value of the selected synonym
//vector<int> QueryEvaluator::filterSelectedResults(int queryPairId)
//{
//	set<int> selectedResults;
//	for (size_t i = 0; i < results.size(); i++) {
//		if (wrong[i]) continue;
//		vector<ResultUnit>* currentTuple = &results[i];
//		selectedResults.insert(results[i][queryPairId].value);
//
//	}
//	vi selectedResultsVector;
//	selectedResultsVector.assign(selectedResults.begin(), selectedResults.end());
//	return selectedResultsVector;
//}
//
////add the strings representing the selectedResults to the qresult
//void QueryEvaluator::getResultStrings(vector<int> selectedResults, SynonymType type, list<string>& resultStrings)
//{
//	for (size_t i = 0; i < selectedResults.size(); i++) {
//		if (type == SYNONYM_TYPE_VARIABLE) {
//			resultStrings.push_back(PKB::getInstance().getVarNameFromIndex(selectedResults[i]));
//		}
//		else {
//			resultStrings.push_back(to_string(selectedResults[i]));
//		}
//	}
//
//}
//
////get the synonym id of a QueryPair
//int QueryEvaluator::getIdOfQueryPair(QueryPair selectedQueryPair)
//{
//	return getSynonymIndexFromName(selectedQueryPair.getValue());
//}
//
//void QueryEvaluator::populateResults()
//{
//	vector<QueryPair> allSynonyms = query.getDeclarationList();
//	tupleSize = allSynonyms.size();
//	//cout << "tuple size is " << tupleSize << "\n";
//	//cout << "first one is while ?" << ( allSynonyms[0].getSynonymType() == SYNONYM_TYPE_WHILE) << "\n";
//
//	possibleResultUnits.clear();
//
//	for (int i = 0; i < tupleSize; i++) {
//		// lets get all possible values for this synonym
//		vector<ResultUnit> currentSynValues;
//		QueryPair currentSyn = allSynonyms[i];
//		vi valuesAsInt = loadValuesFromPKB(currentSyn.getSynonymType());
//		//cout << "Just loaded stuff from PKB: size = " << valuesAsInt.size();
//		//Helper::printVi(valuesAsInt);
//		for (size_t j = 0; j < valuesAsInt.size(); j++) {
//			// push a new possible resultUnit to the currentSynValues;
//			currentSynValues.push_back(ResultUnit(i, valuesAsInt[j]));
//		}
//		
//		possibleResultUnits.push_back(currentSynValues);
//	}
//	
//	//now lets recursively add all posible tuples of <ResultUnit> < ResultUnit >...
//	vUnit* vectorOfUnits = new vUnit(tupleSize);
//	recursiveAddFrom(0,vectorOfUnits);
//	//now results has vectors of tuples of synonyms;
//
//}
//
////recursively populate the results vector from the values in possibleResultUnits
//void QueryEvaluator::recursiveAddFrom(int synIndex, vUnit* vectorOfUnits) {
//	if (synIndex < tupleSize) {
//		//not the end yet
//		for (size_t i = 0; i < possibleResultUnits[synIndex].size(); i++) {
//			vectorOfUnits->at(synIndex) = possibleResultUnits[synIndex][i];
//			recursiveAddFrom(synIndex + 1, vectorOfUnits);
//		}
//	}
//	else {
//		//at the end;
//		//get a copy of the vector of sequences of units;
//		vUnit newVector = *vectorOfUnits;
//
//		//append to results;
//		results.push_back(newVector);
//	}
//
//}
//
////load vector of all entity index from PKB to populate possibleResultUnits
//vi QueryEvaluator::loadValuesFromPKB(SynonymType type) {
//
//	vi values;
//	int maxStmtNo;
//	switch (type) {
//		case SYNONYM_TYPE_ASSIGN:
//			return PKB::getInstance().getAllEntityIndex(NodeType::Assign);
//			break;
//
//		case SYNONYM_TYPE_WHILE:
//			return PKB::getInstance().getAllEntityIndex(NodeType::While);
//			break;
//
//		case SYNONYM_TYPE_STMT:
//		case SYNONYM_TYPE_PROG_LINE:
//			maxStmtNo = PKB::getInstance().getStmtCount();
//			for (int i = 1; i <= maxStmtNo; i++) values.push_back(i);
//			return values;
//			break;
//		case SYNONYM_TYPE_VARIABLE:
//			return PKB::getInstance().getAllEntityIndex(NodeType::Variable);
//			break;
//		default:
//			break;
//	}
//
//	return vi();
//
//}
//
//
//int QueryEvaluator::getSynonymIndexFromName(string synName) {
//	vector<QueryPair> declareList = query.getDeclarationList() ;
//	for (size_t i = 0; i < declareList.size(); i++) {
//		if (declareList[i].getValue() == synName)
//			return i;
//	}
//	return -1;
//}