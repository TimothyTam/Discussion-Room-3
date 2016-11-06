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
#include <set>
#include <iterator>


using namespace std;
typedef pair<int, GraphEdge*> pIG;

bool printDetails = false;
bool printG = false;
bool printMoreDetails = false;


void QueryEvaluator::returnFalse(list<string>& qresult) {
	if (selectBoolean) {
		qresult.push_back("false");
	}
	return;
}

QueryEvaluator::QueryEvaluator() {
	
}
void printVi(vi v) {
	for (size_t i = 0; i < v.size(); i++) {
		cout << v[i] << ",";
	}
	cout << "\n";
}

void printLLI(list<list<int>> l) {
	for (list<list<int>>::iterator i = l.begin(); i != l.end(); i++) {
		cout << "<";
		for (list<int>::iterator ii = i->begin(); ii != i->end(); ii++) {
			cout << *ii << ",";
		}
		cout << ">\n";
	}
	cout << "\n";
}

void printLVI(list<vector<int>> l) {
	for (list<vector<int>>::iterator i = l.begin(); i != l.end(); i++) {
		cout << "<";
		for (vector<int>::iterator ii = i->begin(); ii != i->end(); ii++) {
			cout << *ii << ",";
		}
		cout << ">\n";
	}
	cout << "\n";
}


bool QueryEvaluator::evaluateConstantClauses() {
	vi dummyVi = vi();
	vp_i_i dummyVii = vp_i_i();
	//cout << "There are " << constantClauses.size() << " constant clauses\n";
	for (size_t i = 0; i < constantClauses.size(); i++) {
		bool result;
		evaluateClause(constantClauses[i], -2, -2, dummyVi, dummyVii, result);
		if (!result) return false;
	}
	return true;
}

void expandTupleWithVi(list<vector<int>>::iterator ii, vi* values, int position, list<vector<int>>* allTuples) {
	if (printMoreDetails) cout << "Expanding tuples with Vi\n";
	if (printMoreDetails) cout << "values to be expanded to index = " << position << ":\n";
	if (printMoreDetails) printVi(*values);

	for (size_t i = 0; i < values->size() - 1; i++) {
		allTuples->insert(ii, *ii);
	}
	list<vector<int>>::iterator iiback = ii;
	//then lets run an iterator back to add the appropriate values
	for (size_t i = 0; i < values->size(); i++) {
		iiback->at(position) = values->at(i);
		if (i < values->size()-1) iiback--;
	}
	return;
}

bool QueryEvaluator::evaluateGraphEdge(EvaluationGraph* graph, GraphEdge* edge) {
	if (edge->isDone) return true;

	int tupleSize = graph->vertices.size();
	map<int, int>* idOfSyn = &graph->resultTable.indexOfSynonym;
	list<vector<int>>* allTuples = &graph->resultTable.allTuples;
	vp_i_i resultVii;
	vi resultVi;
	bool resultBool;

	//so we have fromSyn, toSyn, and the direction
	//lets find out the firstSyn, secondSyn for the clause first
	int firstSyn = edge->fromVertex;
	int secondSyn = edge->toVertex;
	vector<int> firstTuple = allTuples->front();


	//now we establish that the relationship is clause(firstSyn,secondSyn)
	//oh wait, what if one of the two is -1 ,meaning constant ? Hmm then we only need to update 1 syn
	if (firstSyn == -1 || secondSyn == -1) {
		int tupleId;
		if (firstSyn == -1) {
			tupleId = (*idOfSyn)[secondSyn];
		}
		else {
			tupleId = (*idOfSyn)[firstSyn];
		}

		if (printMoreDetails) cout << " so its an edge from syn-constant, tupleId = " << tupleId << "\n";

		if (firstTuple[tupleId] == ANY) {
			if (printMoreDetails) cout << "syn = ANY\n";
			//then we just need to get the resultVi for this clause and then expand vi;
			evaluateClause(edge->clause, -1, -1, resultVi, resultVii, resultBool);
			
			//if resultVi is empty, then there is no result;
			if (resultVi.empty()) return false;

			for (list<vector<int>>::iterator ii = allTuples->begin(); ii != allTuples->end(); ii++) {
				//expand this tuple by adding two
				if (printMoreDetails) cout << "Going through the tuple ";
				if (printMoreDetails) printVi(*ii);
				expandTupleWithVi(ii, &resultVi, tupleId, allTuples);
			}
		}
		else {
			//then we just go through each of the tuples and check, discarding stuff that does not satisfy
			list<vector<int>>::iterator ii = allTuples->begin();
			while (ii != allTuples->end()) {
				//check the value in the tupleId. Since the other param is constant, we can just put anything
				//so we can put both as the id of the synonym here
				if (printMoreDetails) cout << "Going through this tuple :";
				if (printMoreDetails) printVi(*ii);

				evaluateClause(edge->clause, ii->at(tupleId), ii->at(tupleId), resultVi, resultVii, resultBool);
				//if wrong, remove them
				if (!resultBool) {
					//cout << "this tuple is wrong\n";
					allTuples->erase(ii++);
				}
				else {
					++ii;
				}
			}
		}

		if (allTuples->empty()) return false;
		return true;
	}


	//From here on out, THERE ARE TWO SYNONYMS INVOLVED here
	//and if they are two legitimate synonyms, then I guess we need to check if those two synonyms are any/specific
	//and then proceed from there
	
	int tupleIdOfFirst = (*idOfSyn)[firstSyn];
	int tupleIdOfSecond = (*idOfSyn)[secondSyn];
	bool firstIsAny = firstTuple[tupleIdOfFirst] == ANY;
	bool secondIsAny = firstTuple[tupleIdOfSecond] == ANY;
	if (printMoreDetails) cout << "TupId of firstSyn=" << tupleIdOfFirst << ", of secondSyn=" << tupleIdOfSecond << "\n";

	if (firstIsAny && secondIsAny) {
		//we will get the getGenericGeneric of the clause here I guess;
		cout << "about the evaluate any any\n";
		evaluateClause(edge->clause, -1, -1, resultVi, resultVii, resultBool);
		//cout << "Evaluated any any, resultVii.size = " << resultVii.size() << "\n";

		if (resultVii.empty()) return false;

		//so now we just need to go through each of the tuple and expand;
		for (list<vector<int>>::iterator ii = allTuples->begin(); ii != allTuples->end(); ii++) {
			//expand this tuple by adding two
			//cout << "Going through the tuple ";
			//printVi(*ii);

			for (size_t i = 0; i < resultVii.size() - 1; i++) {
				allTuples->insert(ii, *ii);
			}
			list<vector<int>>::iterator iiback = ii;
			//then lets run an iterator back to add the appropriate values
			for (size_t i = 0; i < resultVii.size();i++) {
				iiback->at(tupleIdOfFirst) = resultVii[i].first;
				iiback->at(tupleIdOfSecond) = resultVii[i].second;
				if (i<resultVii.size()-1) iiback--;
			}
			//cout << "after 'expanding', allTuples.size = " << allTuples->size() << "\n";
		}
		
	}
	else if (!firstIsAny && !secondIsAny) {
		//so both of them already exists in the results table;
		//we have to go through each of the tuple, check and discard any that doesnt satisfy
		list<vector<int>>::iterator ii = allTuples->begin();
		while ( ii!=allTuples->end() ) {
			//check the pair of value at this tuple
			evaluateClause(edge->clause, ii->at(tupleIdOfFirst), ii->at(tupleIdOfSecond), resultVi, resultVii, resultBool);
			//if wrong, remove them
			if (!resultBool) {
				allTuples->erase(ii++);
			}
			else {
				++ii;
			}
		}
		
	}
	else {
		//so one of them is specific, one is Any. Here we need to get the results of the Any for each unique values of the specific
		//and then expand the table with values of the Any
		//I guess lets collect a unique list of values for the specific ones first;
		
		int specificId = firstIsAny ? tupleIdOfSecond : tupleIdOfFirst;
		int insertId = firstIsAny ? tupleIdOfFirst : tupleIdOfSecond;
		if (printMoreDetails) cout << "about to evaluate an edge with 1 specific, 1 any, specific Id = " << specificId << ", insertId = " << "\n";
		map<int, bool> gotValue;

		for (list<vector<int>>::iterator ii = allTuples->begin(); ii != allTuples->end(); ii++) {
			gotValue[ii->at(specificId)] = true;
		}

		//now we query for each of those values and store in a vi
		map<int, vi> resultVis;
		for (map<int, bool>::iterator ii = gotValue.begin(); ii != gotValue.end(); ii++) {
			int currentValue = ii->first;
			if (firstIsAny) {
				evaluateClause(edge->clause, -1, currentValue, resultVi, resultVii, resultBool);
			}
			else {
				evaluateClause(edge->clause, currentValue, -1, resultVi, resultVii, resultBool);
			}
			resultVis[currentValue] = resultVi;
		}

		//now we just need to go through each of the tuples and expand accordingly;
		list<vector<int>>::iterator ii = allTuples->begin();
		while (ii != allTuples->end()) {
			int currentValue = ii->at(specificId);
			
			//check if the resultVi for this specific value is empty, then discard this tuple
			if (resultVis[currentValue].size()==0) {
				allTuples->erase(ii++);
			}
			else {
				//lets expand this one
				expandTupleWithVi(ii, &resultVis[currentValue], insertId,allTuples);
				++ii;
			}
		}
	}

	
//	if (edge->backEdge != NULL) edge->backEdge->isDone = true;
	
	if (allTuples->empty()) return false;
	return true;
}

bool QueryEvaluator::evaluateGraph(EvaluationGraph* graph) {
	//how to evaluate graph ?
	//lets just go through each edge and evaluate
	int tupleSize = graph->vertices.size();
	vector<int> firstVector;
	list<vector<int>>* allTuples = &graph->resultTable.allTuples;
	for (int i = 0; i < tupleSize; i++) firstVector.push_back(ANY);

	allTuples->push_back(firstVector);
	//wait, for each of the vertices, if they are touched, then expand those values first;
	for (int i = 0; i < tupleSize; i++) {
		if (isArtiPoint[graph->vertices[i]] && touchedAP[graph->vertices[i]]) {
			//if its a touched articulation point, then we load the current values first
			for (list<vector<int>>::iterator ii = allTuples->begin(); ii != allTuples->end();ii++) {
				expandTupleWithVi(ii, &valuesOfAP[graph->vertices[i]], i, allTuples);
			}
		}
	}

	for (size_t i = 0; i < graph->allEdges.size(); i++) {
		if (!evaluateGraphEdge(graph, graph->allEdges[i]) ) return false;
		if (printMoreDetails) cout << "after evaluating edge " << i << ", sizeOf allTuples = " << graph->resultTable.allTuples.size() << ":\n";
		if (printMoreDetails) printLVI(graph->resultTable.allTuples);
	}
	allTuples->sort();
	allTuples->unique();

	//after being done with this, we can narrow down the values of articulation points back;
	for (int i = 0; i < tupleSize; i++) {
		if (isArtiPoint[graph->vertices[i]]) {
			int curV = graph->vertices[i];
			touchedAP[curV] = true;
			//find the unique values of this point here;
			set<int> uValues;
			for (list<vector<int>>::iterator ii = allTuples->begin(); ii != allTuples->end(); ii++) {
				uValues.insert(ii->at(i));
			}
			valuesOfAP[curV] = vi();
			for (set<int>::iterator ii = uValues.begin(); ii != uValues.end(); ii++) {
				valuesOfAP[curV].push_back(*ii);
			}
		}
	}


	return allTuples->size() > 0;
}

void printGraph(EvaluationGraph graph) {
	vector<vector<pIG>> adList = graph.adjacencyList;
	vector<int> vertices = graph.vertices;
	cout << "Vertices are: ";
	for (size_t i = 0; i < vertices.size(); i++) {
		cout << vertices[i] << " ,";
	}
	cout << "\nEdges are: \n";
	for (size_t j = 0; j < graph.allEdges.size(); j++) {
		GraphEdge* edge = graph.allEdges[j];
		cout << "From " << edge->fromVertex << " to " << edge->toVertex << ", clause has params[0]="
			<< edge->clause.getParametersList()[0].getParamValue() << " params[1]=" <<
			edge->clause.getParametersList()[1].getParamValue() << "\n";
	}
}

void QueryEvaluator::evaluate(Query query, list<string>& qresult) {
	this->query = query;
	

	
	//First, build the whole Evaluation Graph
	buildMasterGraph();

	for (int i = 0; i < vertexCount; i++) {
		cout << "Synonym id" <<i<< " = " << query.getDeclarationList()[i].getValue() << "\n";
	}
	//then, find the articulation points of the graph and break it down into EvaluationGraphs
	//as such, we have global data:		
	// vector<bool> isArtiPoint[];
	// vector< vi > valuesOfAP;
	// vector<bool> touchedAP[];
	// vector<EvaluationGraph> allGraphs;
	cutTheGraph();

	for (size_t i = 0; i < allGraphs.size(); i++) {
		cout << "\nGraph number " << (i + 1) << "\n";
		printGraph(allGraphs[i]);
	}
	 

	//cout << "Done building Evaluation Graphs\n";
	
	//keep track if it is selectBoolean
	selectBoolean = query.getSelectList()[0].getSynonymType() == QueryUtility::SYNONYM_TYPE_BOOLEAN;

	//Next, evaluate the constantClauses, if any of them is false, no need to evaluate anymore

	if (!evaluateConstantClauses()) {
		returnFalse(qresult);
		return;
	}
	
	if (printDetails) cout << "Done Evaluating True/False \n";


	// Then, evaluate each EvaluationGraph.
	for (size_t i = 0; i < allGraphs.size(); i++) {
		if (printDetails) cout << "Evaluating graph " << i << "\n";
		// if there are no results then no need to continue, return None
		if (!evaluateGraph(&allGraphs[i])) {
			returnFalse(qresult);
			return;
		}
	}

	if (printDetails) cout << "Done evaluating all graphs\n";

	//Now then we get all the values of the articulation points in valuesOfAP

	//cout << "Done evaluating all graphs\n";
	
	if (selectBoolean) { //its definitely true
		qresult.push_back("true");
		return;
	}

	//And finally, combine all the results in all the EvaluationGraph
	combineResults(qresult);
	return;


	
	////cout << " RESULT: \n";
	//for (list<string>::iterator ii = qresult.begin(); ii != qresult.end(); ii++) {
	//cout << *ii << ",";
	//}
}



void QueryEvaluator::buildMasterGraph() {
	//cout << "about to build evaluation graphs   nasdf\n";
	vector< vector<pair<int,GraphEdge*>> > adList;
	vertexCount = query.getDeclarationList().size();
	vector<QueryClause> clauses = query.getClauseList();
	vector<GraphEdge*> allEdges;

	for (int i = 0; i < vertexCount;i++) adList.push_back(vector<pIG>());

	//go through each clause and build the respective graph edges for that relationship
	for (size_t i = 0; i < clauses.size(); i++) {
		QueryClause clause = clauses[i];
		vector<QueryParam> params = clause.getParametersList();

		// lets add the appropriate edge for this relationship

		//first lets find out the synonym id of any involved synonyms
		int firstSynId = -1;
		int secondSynId = -1;
		if (params[0].getSynonymType() != QueryUtility::SYNONYM_TYPE_NULL) {
			firstSynId = getSynonymIndexFromName(params[0].getParamValue());
		}
		if (params[1].getSynonymType() != QueryUtility::SYNONYM_TYPE_NULL) {
			secondSynId = getSynonymIndexFromName(params[1].getParamValue());
		}

		if (clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_ASSIGN
			|| clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_IF
			|| clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_WHILE) {
			firstSynId = getSynonymIndexFromName( clause.getSynonymValue() );
			if (params[0].getSynonymType() != QueryUtility::SYNONYM_TYPE_NULL) {
				secondSynId = getSynonymIndexFromName(params[0].getParamValue());
			}
		}
		//cout << "firstSynId = " << firstSynId << "secondSynId = " << secondSynId << "\n";

		GraphEdge* newEdge = new GraphEdge(firstSynId, secondSynId, clause);
		if (firstSynId + secondSynId != -2) allEdges.push_back(newEdge);
		
		//now lets set the forward edge. Eg. if Follows(a,w) we set edge (w,Clause, True)
		if (firstSynId != -1) { // if its not a synonym, the vertex will be -1
			adList[firstSynId].push_back(pIG(secondSynId,newEdge));
		}

		//now only set the backward edge if the secondSynId is not -1
		if (secondSynId != -1) { // if its not a synonym, the vertex will be -1
			adList[secondSynId].push_back(pIG(firstSynId,newEdge));
		}

		//we still have the case of two constant params: like Next(1,2)
		//we store all those clauses in a vector<QueryClause> constantClauses;
		if (firstSynId + secondSynId == -2) constantClauses.push_back(clause);
		//cout << "Done adding a clause\n";
	}



	vector<int> testAllVertices;
	for (int i = 0; i < vertexCount; i++) testAllVertices.push_back(i);

	masterGraph = EvaluationGraph(testAllVertices, adList, allEdges);


	//cout << "Graph after building forest:\n";
	//printGraph(adList, testAllVertices);

	//done building the forest
	//now we just need to bfs and find the connected components;
	//vector<bool> done(vertexCount, false);
	//int doneCount = 0;
	//for (int curVertex = 0; curVertex < vertexCount; curVertex++) {
	//	if (done[curVertex]) continue;
	//	//if not done, this is a new connected component, lets bfs from here;
	//	vi verticesList;
	//	vector<GraphEdge*> alledges;
	//	alledges.clear();

	//	vector<int> parent;
	//	parent.assign(vertexCount, -2);
	//	queue<int> bfsQueue;
	//	bfsQueue.push(curVertex);
	//	done[curVertex] = true;
	//	while (!bfsQueue.empty()) {
	//		int bfsVertex = bfsQueue.front();
	//		bfsQueue.pop();
	//		verticesList.push_back(bfsVertex);

	//		for (size_t i = 0; i < adList[bfsVertex].size(); i++) {
	//			GraphEdge* edge = adList[bfsVertex][i];
	//			int neighbour = edge->toVertex;
	//			if (parent[bfsVertex] == neighbour) continue;
	//			alledges.push_back(edge);
	//			if (edge->backEdge != NULL) alledges.push_back(edge->backEdge);

	//			if (neighbour == -1) continue;
	//			if (done[neighbour]) continue;
	//			bfsQueue.push(neighbour);
	//			parent[neighbour] = bfsVertex;
	//			done[neighbour] = true;
	//		}
	//	}
	//	//done bfs-ing, we got a new GraphEdge
	//	//unless it is just a synonym with no relationships at all, then we skip it
	//	
	//	if (alledges.size() == 0) continue;
	//	if (printDetails) cout << "Got a new sub-graph with alledges.size=" << alledges.size() << ":\n";
	//	if (printDetails) printGraph(adList, verticesList);

	//	allGraphs.push_back(EvaluationGraph(verticesList, adList, alledges));
	//}
}


int QueryEvaluator::countConnectedComponents(int removedV) {
	vector< vector<pIG> > adList = masterGraph.adjacencyList;

	int countConnected = 0;
	vector<bool> doneV;
	doneV.assign(vertexCount,false);
	for (int curV = 0; curV < vertexCount; curV++) {
		if (doneV[curV] || curV == removedV) continue;
		countConnected++;
		//bfs to traverse the connected component;
		queue<int> q = queue<int>();
		q.push(curV); doneV[curV] = true;
		while (!q.empty()) {
			int bfsV = q.front(); q.pop();
			for (size_t i = 0; i < adList[bfsV].size(); i++) {
				int nextV = adList[bfsV][i].first;
				if (nextV < 0) continue;
				if (nextV == removedV || doneV[nextV]) continue;
				doneV[nextV] = true;
				q.push(nextV);
			}
		}
	}
	
	return countConnected;
}

void QueryEvaluator::cutTheGraph() {
	//well easy, first initilise stuff
	// vector<bool> isArtiPoint[];
	// vector< vi > valuesOfAP;
	// vector<bool> touchedAP[];
	
	allGraphs = vector<EvaluationGraph>();
	isArtiPoint.assign(vertexCount, false);
	touchedAP.assign(vertexCount, false);
	valuesOfAP = vector<vi>();
	for (int i = 0; i < vertexCount; i++) valuesOfAP.push_back(vi());
	
	vector< vector<pIG> > adList = masterGraph.adjacencyList;
	int totalComponents = countConnectedComponents(-1);

	for (int vertex = 0; vertex < vertexCount; vertex++) {
		//try removing this one and see if the graph is still connected;
		if (countConnectedComponents(vertex) > totalComponents) {
			//bingo, here is another articulation point;
			cout << "got articulation point : " << vertex << "\n";
			isArtiPoint[vertex] = true;
		}
	}

	//now we can do a final bfs to find the "components divided by articulation points";
	vector<bool> doneV;
	doneV.assign(vertexCount, false);
	for (int curV = 0; curV < vertexCount; curV++) {
		if (printG) cout << " We are starting discoring at vertex " << curV << "\n";
		if (doneV[curV]) continue;
		if (!isArtiPoint[curV]) doneV[curV] = true; // articulation points are never done

		vector<int> graphVertices;
		vector<GraphEdge*> graphEdges;
		map<int, bool> doneBFS;

		
		if (isArtiPoint[curV]) {
			//we can just add the constant edges for the articulation points here;
			if (printG) cout << "Discovering from a AP\n";
			graphVertices.push_back(curV);
			for (int i = 0; i < adList[curV].size(); i++) {
				int nextV = adList[curV][i].first;
				if (nextV >= 0) continue;
				//only add the constant edges;
				graphEdges.push_back(adList[curV][i].second);
				adList[curV][i].second->isDone = true;
			}

			if (graphEdges.size()>0) allGraphs.push_back(EvaluationGraph(graphVertices, adList, graphEdges));

			continue;
		}

		//bfs to traverse the "semi-connected" component;
		queue<int> q = queue<int>();
		q.push(curV);

		while (!q.empty()) {
			int bfsV = q.front(); q.pop();
			cout << "poped " << bfsV << "from queue\n";
			if (doneBFS[bfsV]) continue;
			
			graphVertices.push_back(bfsV);
			doneBFS[bfsV] = true;
			if (!isArtiPoint[bfsV]) doneV[bfsV] = true;
			//doneV[bfsV] = true;
			
			//if (isArtiPoint[bfsV]) continue;  // if this is an articulation point, dont need to bfs

			for (size_t i = 0; i < adList[bfsV].size(); i++) {
				int nextV = adList[bfsV][i].first;
				//if bfsV is AP, we have to treat this differently,
				if (isArtiPoint[bfsV]) {
					if (nextV < 0) continue;
					if (isArtiPoint[nextV] && doneBFS[nextV]) {
						graphEdges.push_back(adList[bfsV][i].second);
						if (bfsV==nextV) graphEdges.push_back(adList[bfsV][i].second);
						adList[bfsV][i].second->isDone = true;
					}
					continue;
				}

				//firstly add this graph edge
				if (nextV >= 0) {
					if (bfsV == nextV) {
						graphEdges.push_back(adList[bfsV][i].second);
						graphEdges.push_back(adList[bfsV][i].second);
					}
					if (doneBFS[nextV]) continue;
				}
					
				if (bfsV == nextV) graphEdges.push_back(adList[bfsV][i].second);
				graphEdges.push_back(adList[bfsV][i].second);
				adList[bfsV][i].second->isDone = true; // mark this edge as done
				if (nextV < 0) continue;
				

				 // only set done if not an articulation point
				
				cout << "Pushing " << nextV << " to the queue\n";
				q.push(nextV);
			}
		}

		//just add a graph
		if (graphEdges.size()>0) allGraphs.push_back(EvaluationGraph(graphVertices, adList, graphEdges));
	}

	//now, if we still have some edges left, then it has got to be between two APs;
	for (size_t i = 0; i < masterGraph.allEdges.size(); i++) {
		GraphEdge* edge = masterGraph.allEdges[i];
		if (edge->isDone) {
			edge->isDone = false;
			continue;
		}
		edge->isDone = false;

		if (printG) cout << "This edge not done: " << edge->fromVertex << " - " << edge->toVertex << "\n";
		if (edge->fromVertex == edge->toVertex) continue;

		vi graphVertices = vi();
		graphVertices.push_back(edge->fromVertex);
		graphVertices.push_back(edge->toVertex);
		vector<GraphEdge*> graphEdges;
		graphEdges.push_back(edge);
		
		allGraphs.push_back(EvaluationGraph(graphVertices, adList, graphEdges));
	}
	
}

QueryUtility::SynonymType QueryEvaluator::getTypeOfSynonym(string name) {
	for (size_t i = 0; i < query.getDeclarationList().size(); i++) {
		QueryPair pair = query.getDeclarationList().at(i);
		if (pair.getValue() == name) {
			return pair.getSynonymType();
		}
	}
	return QueryUtility::SYNONYM_TYPE_NULL;
}

NodeType QueryEvaluator::getNodeTypeFromSynType(QueryUtility::SynonymType type) {
	switch (type) {
		case QueryUtility::SYNONYM_TYPE_CONSTANT:
			return NodeType::Constant;
			break;
		case QueryUtility::SYNONYM_TYPE_ASSIGN:
			return NodeType::Assign;
			break;
		case QueryUtility::SYNONYM_TYPE_IF:
			return NodeType::If;
			break;
		case QueryUtility::SYNONYM_TYPE_WHILE:
			return NodeType::While;
			break;
		case QueryUtility::SYNONYM_TYPE_STMT:
			return NodeType::StmtLst;
			break;
		case QueryUtility::SYNONYM_TYPE_STMTLST:
			return NodeType::StmtLst;
			break;
		case QueryUtility::SYNONYM_TYPE_PROCEDURE:
			return NodeType::Procedure;
			break;
		case QueryUtility::SYNONYM_TYPE_VARIABLE:
			return NodeType::Variable;
			break;
		case QueryUtility::SYNONYM_TYPE_CALL:
		case QueryUtility::SYNONYM_TYPE_CALL_PROCNAME:
			return NodeType::Call;
			break;
		case QueryUtility::SYNONYM_TYPE_PROG_LINE:
			return NodeType::StmtLst;
			break;


		default:
			return NodeType::Invalid;
	}
}

string getWithStringFromParam(QueryParam param, int synValue) {
	switch (param.getParamType()) {
		case QueryUtility::PARAMTYPE_WITH_PROG_LINE:
		case QueryUtility::PARAMTYPE_WITH_STMTNO:
		case QueryUtility::PARAMTYPE_WITH_VALUE:
			return to_string(synValue);
			break;
		case QueryUtility::PARAMTYPE_WITH_VARNAME:
			return PKB::getInstance().getVarNameFromIndex(synValue);
			break;
		case QueryUtility::PARAMTYPE_WITH_PROCNAME:
			if (param.getSynonymType() == QueryUtility::SYNONYM_TYPE_CALL) {
				return PKB::getInstance().getCalledValue(synValue);
			}
			else {
				return PKB::getInstance().getProcNameFromIndex(synValue);
			}
			break;
		default:
			break;
	}
	return "";
}

//This function evaluates the clause using specific values for the synonyms if specified,
//Hence the result could be in the form of vector<int>, vector<pair<int,int>> or bool

vi filterFirstValues(vp_i_i resultVii, bool takeFirst) {
	vi result;
	set<int> s;
	for (size_t i = 0; i < resultVii.size(); i++) {
		if (takeFirst) s.insert(resultVii[i].first);
		else s.insert(resultVii[i].second);
	}
	for (set<int>::iterator ii = s.begin(); ii != s.end(); ii++) {
		result.push_back(*ii);
	}
	return result;
}

bool validStatement(int s) {
	return (s > 0 && s <= PKB::getInstance().getStmtCount() );
}

void QueryEvaluator::evaluateClause(QueryClause clause, int firstValue, int secondValue, vi & resultVi, vii & resultVii, bool & resultBool) {
	vector<QueryParam> params = clause.getParametersList();
	int zeroValue = -1;
	bool hasFirstSyn = false, hasSecondSyn = false;
	bool firstIs_, secondIs_;
	firstIs_ = params[0].getParamValue()[0] == '_';
	secondIs_ = params[1].getParamValue()[0] == '_';

	//resultVii = PKB::getInstance().getFollowGenericGeneric(NodeType::Assign, NodeType::While);
	//resultVii = PKB::getInstance().getNextGenericGeneric(NodeType::Assign, NodeType::While);
	//cout << "tesed followGenGe(assign,while) and NextGenGen\n";
	//resultVii = PKB::getInstance().callsGenericGeneric();
	//cout << "Before testing Next\n";
	//resultVii = PKB::getInstance().getNextGenericGeneric(NodeType::Assign,NodeType::Assign);
	//cout << " Test next gen gen(assign,assign): size = " << resultVii.size() << "\n";

	//resultVii = PKB::getInstance().getPatternAssignGenericGeneric("_\"x\"_");
	//resultVi = PKB::getInstance().getPatternAssignSpecificGeneric(34, "_\"x\"_");
	//cout << "assign tested = " << PKB::getInstance().whetherPatternAssign(24,9,"_") << "\n";
	//cout << "assign tested = " << PKB::getInstance().whetherPatternAssign(25, 9, "_") << "\n";

	//cout << "assign tested = " << PKB::getInstance().whetherPatternAssign(30, 9, "_") << "\n";
	//cout << "Whether Next*(2,3): " << PKB::getInstance().whetherTransitiveNext(2, 3) << "\n";
	//cout << "GetFollowGeneric.." << PKB::getInstance().getFollowGenericSpecific(2222, NodeType::StmtLst) << "\n";
	//cout << "Get Pattern 3(generic,df)" << PKB::getInstance().getVarNameFromIndex(PKB::getInstance().getPatternAssignSpecificGeneric(3, "_")[0]);
	//cout << "Get Pattern 5(generic,df)" << PKB::getInstance().getVarNameFromIndex(PKB::getInstance().getPatternAssignSpecificGeneric(5, "_")[0]);
	//cout << "Get Pattern 8(generic,df)" << PKB::getInstance().getVarNameFromIndex(PKB::getInstance().getPatternAssignSpecificGeneric(8, "_")[0]);
	//Note: firstSynType and secondSynType are defaulted to be stmts
	QueryUtility::SynonymType firstSynType = QueryUtility::SYNONYM_TYPE_STMT
		, secondSynType = QueryUtility::SYNONYM_TYPE_STMT;

	if (clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_ASSIGN
		|| clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_IF
		|| clause.getClauseType() == QueryUtility::CLAUSETYPE_PATTERN_WHILE) {
		//firstSynValue is for a in 'pattern a(v,"vsdf")'
		//secondSynValue or params[0] will be for v
		zeroValue = firstValue;
		firstValue = secondValue;
	}

	//cout << "params[0] = " << params[0].getParamValue() << " params[1]= " << params[1].getParamValue() << "\n";
	if (params[0].getParamType() == QueryUtility::PARAMTYPE_SYNONYM) {
		hasFirstSyn = true;
		firstSynType = getTypeOfSynonym(params[0].getParamValue());
	}

	if (params[0].getParamType() == QueryUtility::PARAMTYPE_ENT_NAME) {
		//get the firstValue if it is a specific int
		if (params[0].getParamValue()[0] != '"' && params[0].getParamValue()[0] != '_')
			firstValue = stoi(params[0].getParamValue());
	}


	if (params[1].getParamType() == QueryUtility::PARAMTYPE_SYNONYM) {
		hasSecondSyn = true;
		secondSynType = getTypeOfSynonym(params[1].getParamValue());
	}

	if (params[1].getParamType() == QueryUtility::PARAMTYPE_ENT_NAME) {
		if (params[1].getParamValue()[0] != '"' && params[1].getParamValue()[0] != '_')
			secondValue = stoi(params[1].getParamValue());
	}

	string expression = params[1].getParamValue();
	if (printDetails) cout << "\n\nhasFirstSyn = " << hasFirstSyn << " hasSecondSyn =" << hasSecondSyn << "\n";
	if (printDetails) cout << "zeroValue=" << zeroValue << " firstValue = " << firstValue << "secondValue" << secondValue << " expression =" << expression << "\n";

	string varname = "";
	string withString1 = "$", withString2 = "$";
	int tempi;
	vi tempVi1,tempVi2;
	//int varIndex;
	bool secondCase = true;
	bool secondStar = true;
	bool nextOrParent = true;
	bool parent = true;
	bool thirdStar = true;
	switch (clause.getClauseType()) {
		case QueryUtility::CLAUSETYPE_MODIFIES:
			secondCase = false;
		case QueryUtility::CLAUSETYPE_USES:
			//modifies(a, v), modifies(a,"_"), modifies(a,"varname")

			//there will be cases of:
			// (syn,syn) but firstValue/secondValue might be -1/specific
			// (syn,"v") but firstValue might be -1/specific
			// (1,syn) but secondValue might be -1/specific
			// (1,"v")
			// so I guess lets find out the effective firstValue/secondValue
			// well firstValue is already effective, just need to settle secondValue
			
			if (params[0].getParamValue()[0] == '"') {
				//it must be a proc
				firstValue = PKB::getInstance().getProcIndexFromName(removeQuotes(params[0].getParamValue()));
				if (firstValue == -1) {
					resultBool = false;
					resultVi = vi();
					return;
				}
				if (printDetails) cout << "Uses/Modifies('procnam',blah blah), procIndex = " << firstValue << "\n";
				firstSynType = QueryUtility::SYNONYM_TYPE_PROCEDURE;
			}

			if (params[1].getParamValue()[0] == '_') {
				if (secondCase) {
					//Involving _: Uses(a,_), Uses(_,_), Uses(2,_)
					
					//Uses(_,_)
					if (params[0].getParamValue()[0] == '_') {
						//Uses(_,_)
						resultBool = !PKB::getInstance().getUsesGenericGeneric(NodeType::StmtLst).empty()
							&& !PKB::getInstance().getUsesGenericGeneric(NodeType::Procedure).empty();
						return;
					}
					else if (firstValue == -1) {
						//Uses(a/p,_)
						resultVi = filterFirstValues(PKB::getInstance().getUsesGenericGeneric(getNodeTypeFromSynType(firstSynType)),true);
						return;
					}
					else {
						//Uses(2,_)
						resultBool = !PKB::getInstance().getUsesSpecificGeneric(firstValue, getNodeTypeFromSynType(firstSynType)).empty();
						return;
					}

				}
				else if (firstValue != -1 || params[0].getParamValue()[0] == '_') {
					// modifies(2,_) or modifies(_,_)
					// assuming procedure always modifies something
					if (firstValue != -1) {
						resultBool = validStatement(firstValue);
						return;
					}
					resultBool = true;
					return;
				}
				else {
					// modifies(p/a,_)
					// assumes modifies(p,_) = all procedures
					resultVi = loadValuesFromPKB(firstSynType);
					return;
				}
			}


			if (!hasSecondSyn) {
				secondValue = PKB::getInstance().getVarIndexFromName(QueryEvaluator::removeQuotes(params[1].getParamValue()));
				cout << "Specified a variable, varIndex = " << secondValue << "\n";
				//if the variable doesnt exist, then return empty I guess
				if (secondValue == -1) {
					resultVi = vi();
					resultBool = false;
					return;
				}
			}
			if (firstValue != -1 && secondValue != -1) {
				cout << " Modifies/Uses (1,2)\n";
				if (firstSynType == QueryUtility::SYNONYM_TYPE_PROCEDURE) {
					resultBool = secondCase ? PKB::getInstance().whetherProcUses(firstValue, secondValue) :
						PKB::getInstance().whetherProcModifies(firstValue, secondValue);
				} 
				else {
					resultBool = secondCase ? PKB::getInstance().whetherStmtUses(firstValue, secondValue):
						PKB::getInstance().whetherStmtModifies(firstValue, secondValue);
				}
				return;
			}
			else if (firstValue == -1 && secondValue != -1) {
				cout << " Modifies/Uses (a/p,'v') \n";
				resultVi = secondCase? PKB::getInstance().getUsesGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType))
					: PKB::getInstance().getModifyGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType));
				return;
			}
			else if (firstValue != -1 && secondValue == -1) {
				cout << " Modifies/Uses (1,v) \n";
				resultVi = secondCase ? PKB::getInstance().getUsesSpecificGeneric(firstValue, getNodeTypeFromSynType(firstSynType))
					: PKB::getInstance().getModifySpecificGeneric(firstValue, getNodeTypeFromSynType(firstSynType));
				return;
			}
			else {
				cout << " Modifies/Uses (p,v)\n";
				resultVii = secondCase ? PKB::getInstance().getUsesGenericGeneric(getNodeTypeFromSynType(firstSynType))
					: PKB::getInstance().getModifyGenericGeneric(getNodeTypeFromSynType(firstSynType));
				return;
			}

			break;

		
		case QueryUtility::CLAUSETYPE_FOLLOWS:
			secondCase = 0;
			//cout << "in ::Follows, next is set to : " << next << "\n";
		case QueryUtility::CLAUSETYPE_FOLLOWS_STAR:
			nextOrParent = false;
			//cout << "in ::Follows*, next is set to : " << next << "\n";
		case QueryUtility::CLAUSETYPE_NEXT:
			secondStar = false;
		case QueryUtility::CLAUSETYPE_NEXT_STAR:
			parent = false;
		case QueryUtility::CLAUSETYPE_PARENT:
			thirdStar = false;
		case QueryUtility::CLAUSETYPE_PARENT_STAR:
			// cases:
			// involving _: Follows(a,_), Follows(_,a), Follows(_,_), Follows(2,_), Follows(_,3)
			// otherwise: Follows(a,2), Follows(a,b), Follows(2,b), Follows(a,b);
			if (printDetails) cout << "Processing Follows/Nexts/Parent\n\n";
			//involving _:
			
			if (firstIs_ || secondIs_) {
				if (firstIs_ && secondIs_) {
					//Follows/Next(_,_); as long as there is something following something
					resultBool = nextOrParent ?
						parent ? !PKB::getInstance().getParentGenericGeneric(NodeType::StmtLst, NodeType::StmtLst).empty()
						:!PKB::getInstance().getNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst).empty()
						: !PKB::getInstance().getFollowGenericGeneric(NodeType::StmtLst, NodeType::StmtLst).empty();
					return;
				}
				if (hasFirstSyn && firstValue == -1) {
					//Follows/Next(a,_);
					//filter all statement of type a that follows something;
					resultVi = nextOrParent ? 
						parent ? filterFirstValues(PKB::getInstance().getParentGenericGeneric(getNodeTypeFromSynType(firstSynType), NodeType::StmtLst), true)
						: filterFirstValues(PKB::getInstance().getNextGenericGeneric(getNodeTypeFromSynType(firstSynType), NodeType::StmtLst), true)
						: filterFirstValues(PKB::getInstance().getFollowGenericGeneric(getNodeTypeFromSynType(firstSynType), NodeType::StmtLst),true);
					return;
				}
				if (hasSecondSyn && secondValue == -1) {
					//Follows/Next(_,a);
					//filter all statements of type a that is followed by something;
					resultVi = nextOrParent ? 
						parent ? filterFirstValues(PKB::getInstance().getParentGenericGeneric(NodeType::StmtLst, getNodeTypeFromSynType(secondSynType)), false)
						: filterFirstValues(PKB::getInstance().getNextGenericGeneric(NodeType::StmtLst, getNodeTypeFromSynType(secondSynType)), false)
						: filterFirstValues(PKB::getInstance().getFollowGenericGeneric(NodeType::StmtLst, getNodeTypeFromSynType(secondSynType)), false);
					return;
				}
				if (firstIs_) {
					//Follows/Next(_,2);
					resultBool = nextOrParent ?
						parent ? PKB::getInstance().getParentGenericSpecific(secondValue, NodeType::StmtLst) != -1
						: !PKB::getInstance().getNextGenericSpecific(secondValue, NodeType::StmtLst).empty()
						: PKB::getInstance().getFollowGenericSpecific(secondValue, NodeType::StmtLst) != 0;
					return;
				}
				if (secondIs_) {
					//Follows/Next(2,_);
					resultBool = nextOrParent ? 
						parent ? !PKB::getInstance().getParentSpecificGeneric(firstValue, NodeType::StmtLst).empty()
						: !PKB::getInstance().getNextSpecificGeneric(firstValue, NodeType::StmtLst).empty()
						: PKB::getInstance().getFollowSpecificGeneric(firstValue, NodeType::StmtLst) != 0;
					return;
				}
				//not supposed to go here;
				cout << " WHATTTTTTTTTTTTTTTTTTTT?  Exception ! In Case:Follows/Follows*\n";
			}

			// otherwise: Follows(a,2), Follows(a,b), Follows(2,b), Follows(a,b);
			//from here, both synonyms are defined;
			if (firstValue != -1 && secondValue != -1) {
				if (printDetails) cout << "Follows/Next/Parent(1,2) \n" ;
				resultBool = 
					nextOrParent ? 
					parent ?
					thirdStar ?
					PKB::getInstance().whetherTransitiveParent(firstValue, secondValue)
					: PKB::getInstance().whetherParent(firstValue, secondValue)

					: secondStar ?
					PKB::getInstance().whetherTransitiveNext(firstValue, secondValue)
					: PKB::getInstance().whetherNext(firstValue, secondValue) 
					
					: secondCase ? PKB::getInstance().whetherTransitivelyFollows(firstValue, secondValue)
					: PKB::getInstance().whetherFollows(firstValue, secondValue);
				return;
			}
			else if (firstValue != -1 && secondValue == -1) {
				if (printDetails) cout << " Follows/Next/Parent(1,a) \n";
				resultVi = vi();
				if (nextOrParent) {
					resultVi = parent ?
						thirdStar ? PKB::getInstance().getTransitiveParentSpecificGeneric(firstValue, getNodeTypeFromSynType(secondSynType))
						: PKB::getInstance().getParentSpecificGeneric(firstValue, getNodeTypeFromSynType(secondSynType))

						:secondStar ? PKB::getInstance().getTransitiveNextSpecificGeneric(firstValue, getNodeTypeFromSynType(secondSynType))
						: PKB::getInstance().getNextSpecificGeneric(firstValue, getNodeTypeFromSynType(secondSynType));
					return;
				}
				if (secondCase) { //Follows*
					resultVi = PKB::getInstance().getTransitiveFollowSpecificGeneric(firstValue, getNodeTypeFromSynType(secondSynType));
					return;
				}
				//Follows
				zeroValue = PKB::getInstance().getFollowSpecificGeneric(firstValue, getNodeTypeFromSynType(secondSynType));
				if (zeroValue != 0) resultVi.push_back(zeroValue);
				return;
			}
			else if (firstValue == -1 && secondValue != -1) {
				//Follows(a,1)
				if (printDetails) cout << " Follows/Next/Parent(a,1) \n";

				
				if (nextOrParent && !parent) {
					resultVi = secondStar ? PKB::getInstance().getTransitiveNextGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType))
						: PKB::getInstance().getNextGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType));
					return;
				}
				if (secondCase && !parent) { //Follows*
					resultVi = PKB::getInstance().getTransitiveFollowGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType));
					return;
				}
				if (thirdStar) { // parent*
					if (printDetails) cout << "Parent* gen spec\n";

					resultVi = PKB::getInstance().getTransitiveParentGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType));
					if (printDetails) cout << "results:\n";
					if (printDetails) printVi(resultVi);

					return;
				}
				//Follows or parent 
				resultVi = vi();
				zeroValue = parent ? PKB::getInstance().getParentGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType))
					: PKB::getInstance().getFollowGenericSpecific(secondValue, getNodeTypeFromSynType(firstSynType));
				if (zeroValue > 0) resultVi.push_back(zeroValue);
				return;
			}
			else {
				//Follows(a,w)
				if (printDetails) cout << " Follows/Next/parent(a,w) \n";
				//cout << "next? " << nextOrParent << " secondStar? " << secondStar << " secondCase? " << secondCase << "\n";
				resultVii = nextOrParent ?
					parent ?
					thirdStar ? PKB::getInstance().getTransitiveParentGenericGeneric(getNodeTypeFromSynType(firstSynType), getNodeTypeFromSynType(secondSynType))
					: PKB::getInstance().getParentGenericGeneric(getNodeTypeFromSynType(firstSynType), getNodeTypeFromSynType(secondSynType))

					: secondStar ? PKB::getInstance().getTransitiveNextGenericGeneric(getNodeTypeFromSynType(firstSynType), getNodeTypeFromSynType(secondSynType))
					: PKB::getInstance().getNextGenericGeneric(getNodeTypeFromSynType(firstSynType), getNodeTypeFromSynType(secondSynType))
					
					: secondCase ? PKB::getInstance().getTransitiveFollowGenericGeneric(getNodeTypeFromSynType(firstSynType), getNodeTypeFromSynType(secondSynType))
					: PKB::getInstance().getFollowGenericGeneric(getNodeTypeFromSynType(firstSynType), getNodeTypeFromSynType(secondSynType));
				if (printDetails) cout << " Got from PKB\n";
				return;
			}

			break;

		case QueryUtility::CLAUSETYPE_PATTERN_ASSIGN:
			secondStar = false;
		case QueryUtility::CLAUSETYPE_PATTERN_IF:
			thirdStar = false;
		case QueryUtility::CLAUSETYPE_PATTERN_WHILE:
			//pattern a(_,_), pattern a(_,"v"), pattern a(x,_),  pattern a(x,"v"), pattern a(x,_"v"_)
			//so we are working with zeroValue and firstValue
			try {
				if (firstIs_) {
					if (printDetails) cout << " pattern a(_,_), a(_,'v') \n";
					if (zeroValue == -1) {
						//pattern a(_,exp)
						resultVi = secondStar ?
							thirdStar ? filterFirstValues(PKB::getInstance().getPatternWhileGenericGeneric(), true)
							: filterFirstValues(PKB::getInstance().getPatternIfGenericGeneric(), true)
							: filterFirstValues(PKB::getInstance().getPatternAssignGenericGeneric(expression), true);
						return;
					}
					else {
						resultBool = secondStar ?
							thirdStar ? !PKB::getInstance().getPatternWhileSpecificGeneric(zeroValue).empty()
							: !PKB::getInstance().getPatternIfSpecificGeneric(zeroValue).empty()
							: !PKB::getInstance().getPatternAssignSpecificGeneric(zeroValue, expression).empty();
						//cout << "resultBool = " << resultBool << "\n";
						return;
					}
				}

				//pattern a/w/if(v,"x") or a/w/if("v","x")
				if (!hasFirstSyn) {
					if (printDetails) cout << " clause has form pattern a(\"variable\",\"x\") \n";

					//pattern a("v","x");, lets get the value of "v";
					firstValue = PKB::getInstance().getVarIndexFromName(removeQuotes(params[0].getParamValue()));
					if (printDetails) cout << "varIndex of the variable = " << firstValue << "\n";
					//if that variable doesn't exist -> 
					if (firstValue == -1) {
						resultVi = vi();
						resultBool = false;
						return;
					}
				}

				if (zeroValue == -1 && firstValue == -1) {
					//pattern a/w(v,"x")
					resultVii = secondStar ?
						thirdStar ? PKB::getInstance().getPatternWhileGenericGeneric()
						: PKB::getInstance().getPatternIfGenericGeneric()
						: PKB::getInstance().getPatternAssignGenericGeneric(expression);
					return;
				}
				else if (zeroValue == -1 && firstValue != -1) {
					//pattern a/w(1,"x")
					if (printDetails) cout << "querying pattern a/w(" <<firstValue<<"," << expression << ") \n Results=";


					resultVi = secondStar ?
						thirdStar ? PKB::getInstance().getPatternWhileGenericSpecific(firstValue)
						: PKB::getInstance().getPatternIfGenericSpecific(firstValue)
						: PKB::getInstance().getPatternAssignGenericSpecific(firstValue, expression);
					printVi(resultVi);
					return;
				}
				else if (zeroValue != -1 && firstValue == -1) {
					//pattern 1(v,"x")
					if (printDetails) cout << "querying pattern " << zeroValue <<  "(" << "<generic>" << "," << expression << ") \n Results=";
					
					//if (printDetails) cout << "pattern 1(v,'x') \n";
					resultVi = secondStar ?
						thirdStar ? PKB::getInstance().getPatternWhileSpecificGeneric(zeroValue)
						: PKB::getInstance().getPatternIfSpecificGeneric(zeroValue)
						: PKB::getInstance().getPatternAssignSpecificGeneric(zeroValue, expression);
					
					if (printDetails) printVi(resultVi);
					return;
				}
				else {
					//pattern 1(1,"x")
					if (printDetails) cout << "pattern " << zeroValue<< "(" << firstValue << "," << expression<< ")\n";
					resultBool = secondStar ?
						thirdStar ? PKB::getInstance().whetherPatternWhile(zeroValue, firstValue)
						: PKB::getInstance().whetherPatternIf(zeroValue, firstValue)
						: PKB::getInstance().whetherPatternAssign(zeroValue, firstValue, expression);
					if (printDetails) cout << "result = " << resultBool << "\n";
					return;
				}
			}
			catch (exception ex) {
				cout << "\n" << ex.what() << "\n";
			}

			break;

		case QueryUtility::CLAUSETYPE_CALLS:
			secondStar = false;
		case QueryUtility::CLAUSETYPE_CALLS_STAR:
			if (printDetails) cout << "processing call\n";
			//if the proc_name is specified, change them to the proc index
			if (params[0].getParamValue()[0] == '"') {
				firstValue = PKB::getInstance().getProcIndexFromName(removeQuotes(params[0].getParamValue()));
				if (firstValue == -1) {
					resultVi = vi();
					resultBool = false;
					return;
				}
			}
			if (params[1].getParamValue()[0] == '"') {
				secondValue = PKB::getInstance().getProcIndexFromName(removeQuotes(params[1].getParamValue()));
				if (secondValue == -1) {
					resultVi = vi();
					resultBool = false;
					return;
				}
			}
			if (printDetails) cout << " in calls, firstValue = " << firstValue << "secondValue=" << secondValue << "\n";

			if (firstIs_ || secondIs_) {
				if (printDetails) cout << "firstIs_ or secondIs_ \n";
				if (firstIs_ && secondIs_) {
					resultBool = !PKB::getInstance().callsGenericGeneric().empty();
					return;
				}
				if (hasFirstSyn && firstValue == -1) {
					//Call(p,_); any proc that calls some other
					resultVi = filterFirstValues(PKB::getInstance().callsGenericGeneric(), true);
					return;
				}
				if (hasSecondSyn && secondValue == -1) {
					//Call(_,a);  any proc that is called;
					resultVi = filterFirstValues(PKB::getInstance().callsGenericGeneric(), false);
					return;
				}
				if (firstIs_) {
					//Call(_,2);  any proc call 2 ?
					resultBool = !PKB::getInstance().callsGenericSpecific(secondValue).empty();
					return;
				}
				if (secondIs_) {
					//Call(2,_);
					resultBool = !PKB::getInstance().callsSpecificGeneric(firstValue).empty();
					return;
				}
			}
			
			if (firstValue != -1 && secondValue != -1) {
				if (printDetails) cout << "Call(1,2) \n";
				resultBool = secondStar? PKB::getInstance().whetherTransitiveCalls(firstValue, secondValue)
					: PKB::getInstance().whetherCalls(firstValue, secondValue);
				return;
			}
			else if (firstValue != -1 && secondValue == -1) {
				if (printDetails) cout << " Call(1,a) \n";
				resultVi = secondStar ? PKB::getInstance().callsTransitiveSpecificGeneric(firstValue)
					: PKB::getInstance().callsSpecificGeneric(firstValue);
				return;
			}
			else if (firstValue == -1 && secondValue != -1) {
				//Call(a,1)
				if (printDetails) cout << "call(a,2)\n";
				resultVi = secondStar ? PKB::getInstance().callsTransitiveGenericSpecific(secondValue)
					: PKB::getInstance().callsGenericSpecific(secondValue);
				return;
			}
			else {
				//Call(a,w)
				if (printDetails) cout << "Call(p1,p2) \n";
				//cout << "next? " << nextOrParent << " secondStar? " << secondStar << " secondCase? " << secondCase << "\n";
				resultVii = secondStar ? PKB::getInstance().callsTransitiveGenericGeneric()
					: PKB::getInstance().callsGenericGeneric();
				//cout << " Got from PKB\n";
				return;
			}

			break;

		case QueryUtility::CLAUSETYPE_WITH:
			resultBool = params[0].getParamValue() == params[1].getParamValue();
			return;

		case QueryUtility::CLAUSETYPE_WITH_STRING:
		case QueryUtility::CLAUSETYPE_WITH_INT:
			//it can involve two synonyms or 1 synonyms
			hasFirstSyn = params[0].getSynonymType() != QueryUtility::SYNONYM_TYPE_NULL;
			hasSecondSyn = params[1].getSynonymType() != QueryUtility::SYNONYM_TYPE_NULL;
			//so firstValue and secondValue are values of the involved synonyms;
			//but we need something else, which is the value of the actual string
			if (!hasFirstSyn) withString1 = params[0].getParamValue();
			if (!hasSecondSyn) withString2 = params[1].getParamValue();

			if (withString1[0] == '"') withString1 = removeQuotes(withString1);
			if (withString2[0] == '"') withString2 = removeQuotes(withString2);

			if (hasFirstSyn && firstValue != -1) {
				//get the actual string from the value of the synonym
				withString1 = getWithStringFromParam(params[0], firstValue);
			}
			if (hasSecondSyn && secondValue != -1) {
				//get the actual string from the value of the synonym
				withString2 = getWithStringFromParam(params[1], secondValue);
			}
			if (printDetails) cout << "Processing with clause, withString1=" << withString1 << " withString2 = " << withString2 << "\n";
			//now we break it into 4 cases I guess;
			if (withString1 == "$" && withString2 == "$") {
				//well we will have to get all the pairs of synonyms satisfying
				//gg
				if (printDetails) cout << "Any any\n";
				resultVii = vii();
				tempVi1 = loadValuesFromPKB(params[0].getSynonymType());
				tempVi2 = loadValuesFromPKB(params[1].getSynonymType());
				for (size_t i = 0; i < tempVi1.size(); i++)
					for (size_t j = 0; j < tempVi2.size(); j++) {
						if (getWithStringFromParam(params[0], tempVi1[i]) == getWithStringFromParam(params[1], tempVi2[j])) {
							resultVii.push_back(make_pair(tempVi1[i], tempVi2[j]));
						}
					}

				return;
			}
			else if (withString1 != "$" && withString2 != "$") {
				if (printDetails) cout << "specific specific\n";
				resultBool = withString1 == withString2;
				return;
			}
			else {
				//one of the values is specific while the other is a synonym;
				// how about we just do the same, get all the entities and compare ?
				if (printDetails) cout << "1 specific 1 gen\n";
				resultVi = vi();
				tempi = 1;
				if (withString1 == "$") {
					tempi = 0;
					withString1 = withString2;
				}
				if (printDetails) cout << "Gen param= " << params[tempi].getParamValue() << " Spec string = " << withString1 <<"SynType of gen = "
					<< params[tempi].getSynonymType() << "\n";
				//now tempi is the index of the params with syn = ANY;
				//withString1 is the value of the other params;

				tempVi1 = loadValuesFromPKB(params[tempi].getSynonymType());
				if (printDetails) cout << "after loading from PKB, possible values = ";
				if (printDetails) printVi(tempVi1);

				for (size_t i = 0; i < tempVi1.size(); i++)
					if (getWithStringFromParam(params[tempi], tempVi1[i]) == withString1) {
						resultVi.push_back(tempVi1[i]);
					}

				return;
			}
			break;

		case QueryUtility::CLAUSETYPE_AFFECTS:
			secondStar = false;
		case QueryUtility::CLAUSETYPE_AFFECTS_STAR:
			if (printDetails) cout << "Affect, secondStar = " << secondStar << "\n";
			if (firstIs_ || secondIs_) {
				if (firstIs_ && secondIs_) {
					//Affect(*)(_,_); as long as there is something affecting something
					resultBool = !PKB::getInstance().getAffectGenericGeneric().empty();
					return;
				}
				if (hasFirstSyn && firstValue == -1) {
					//Affect(a,_);
					//filter all statements that affects something;
					resultVi = filterFirstValues(PKB::getInstance().getAffectGenericGeneric(), true);
					return;
				}
				if (hasSecondSyn && secondValue == -1) {
					//Affect(_,a);
					//filter all statements that is affecting something;
					resultVi = filterFirstValues(PKB::getInstance().getAffectGenericGeneric(), false);
					return;
				}
				if (firstIs_) {
					//Affect(_,2); whether anything affecting 2 ?
					resultBool = !PKB::getInstance().getAffectGenericSpecific(secondValue).empty();
					return;
				}
				if (secondIs_) {
					//Affect(2,_);
					resultBool = !PKB::getInstance().getAffectSpecificGeneric(firstValue).empty();
					return;
				}
				//not supposed to go here;
				cout << " WHATTTTTTTTTTTTTTTTTTTT?  Exception ! In Case:Follows/Follows*\n";
			}

			// otherwise: Affect(a,2), Affect(a,b), Affect(2,b), Affect(a,b);
			//from here, both synonyms are defined;
			if (firstValue != -1 && secondValue != -1) {
				if (printDetails) cout << "Affect(" << firstValue << "," << secondValue<< ") \n";
				resultBool = secondStar ?
					PKB::getInstance().whetherTransitiveAffect(firstValue, secondValue)
					: PKB::getInstance().whetherAffect(firstValue, secondValue);
				if (printDetails) cout << "Results=" << resultBool << "\n";
				return;
			}
			else if (firstValue != -1 && secondValue == -1) {
				//affect(1,a);
				if (printDetails) cout << " Affect(" << firstValue << ",generic), results = \n";
				resultVi = secondStar ? 
					PKB::getInstance().getTransitiveAffectSpecificGeneric(firstValue)
					: PKB::getInstance().getAffectSpecificGeneric(firstValue);
				if (printDetails) printVi(resultVi);
				return;
			}
			else if (firstValue == -1 && secondValue != -1) {
				//Affect(a,1)
				if (printDetails) cout << " Affect(a,1) \n";
				resultVi = secondStar ?
					PKB::getInstance().getTransitiveAffectGenericSpecific(secondValue)
					: PKB::getInstance().getAffectGenericSpecific(secondValue);
				return;
			}
			else {
				//Affect(a,w)
				if (printDetails) cout << " Affect(a,w) \n";
				//cout << "next? " << nextOrParent << " secondStar? " << secondStar << " secondCase? " << secondCase << "\n";
				resultVii = secondStar ?
					PKB::getInstance().getTransitiveAffectGenericGeneric()
					: PKB::getInstance().getAffectGenericGeneric();

				//if (printDetails) cout << " Got from PKB\n";
				return;
			}

			break;

		default:
			break;
		}
	return;
}


string QueryEvaluator::removeQuotes(string s) {
	return s.substr(1, s.length() - 2);
}

//load vector of all entity index from PKB to populate possibleResultUnits
vi QueryEvaluator::loadValuesFromPKB(QueryUtility::SynonymType type) {

	vi values;
	int maxStmtNo;
	vector<string> stringV;
	switch (type) {
		case QueryUtility::SYNONYM_TYPE_PROG_LINE:
			maxStmtNo = PKB::getInstance().getStmtCount();
			for (int i = 1; i <= maxStmtNo; i++) values.push_back(i);
			return values;
			break;
		case QueryUtility::SYNONYM_TYPE_CONSTANT:
		case QueryUtility::SYNONYM_TYPE_STMTLST:
			stringV = PKB::getInstance().getAllEntityName(getNodeTypeFromSynType(type));
			for (size_t i = 0; i < stringV.size(); i++)
				values.push_back(stoi(stringV[i]));
			return values;
		default:
			return PKB::getInstance().getAllEntityIndex(getNodeTypeFromSynType(type));
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


string QueryEvaluator::projectSynValueToString(int synValue, int selectIndex) {
	//cout << "projecting syn value " << synValue << " at select Index " << selectIndex << "\n";
	switch (query.getSelectList()[selectIndex].getSynonymType()) {
		case QueryUtility::SYNONYM_TYPE_VARIABLE:
			return PKB::getInstance().getVarNameFromIndex(synValue);
			break;
		case QueryUtility::SYNONYM_TYPE_PROCEDURE:
			return PKB::getInstance().getProcNameFromIndex(synValue);
			break;
		case QueryUtility::SYNONYM_TYPE_CALL_PROCNAME:
			if (printDetails) cout << "procname of the call statement = " << PKB::getInstance().getCalledValue(synValue) << "\n";
			return PKB::getInstance().getCalledValue(synValue);
			break;
		default:
			return to_string(synValue);
			break;
	}
	//cout << "done\n";
}

string QueryEvaluator::tupleToString(vi* tuple) {
	string thisResult = "";
	if (tuple->size() == 1) {
		thisResult = projectSynValueToString(tuple->at(0),0);
	}
	else {
		for (size_t i = 0; i < tuple->size(); i++) {
			if (i != 0) thisResult += " ";
			thisResult += projectSynValueToString(tuple->at(i),i);
		}
	}
	return thisResult;
}

bool same_list(list<int> l1, list<int> l2) {
	if (l1.size() != l2.size()) return false;
	list<int>::iterator i1, i2;
	i1 = l1.begin(); i2 = l2.begin();
	for (int i = 0; i < l1.size(); i++) {
		if (*i1 != *i2) return false;
		i1++;
		i2++;
	}
	return true;
}

void QueryEvaluator::narrowDownAllGraphs() {
	vector<unordered_map<int, bool>> possibleValuesOfAP;
	
	for (int v = 0; v < vertexCount; v++) {
		possibleValuesOfAP.push_back(unordered_map<int, bool>());
		if (!isArtiPoint[v]) continue;

		if (printG) {
			cout << "Possible results for AP " << v << "\n";
			printVi(valuesOfAP[v]);
		}

		for (size_t i = 0; i < valuesOfAP[v].size(); i++) {
			int thisValue = valuesOfAP[v][i];
			possibleValuesOfAP[v][thisValue] = true;
		}
	}

	

	for (size_t i = 0; i < allGraphs.size(); i++) {
		//for each graph, first find out the APs in it;
		vi APs;
		EvaluationGraph* graph = &allGraphs[i];
		ResultTable* table = &graph->resultTable;
		list<vi>* allTup = &graph->resultTable.allTuples;

		for (int j = 0; j < graph->vertices.size(); j++) {
			int thisSyn = table->synonymList[j];
			if (isArtiPoint[thisSyn]) {
				APs.push_back(thisSyn);
			}
		}
		if (printG) {
			cout << "For graph number " << i << ", Articulation points are: \n";
			printVi(APs);
			cout << "Before tuning, all tuples are:\n";
			printLVI(*allTup);
		}

		list<vi>::iterator ii = allTup->begin();

		while ( ii != allTup->end() ) {
			bool correct = true;
			for (size_t j = 0; j < APs.size(); j++) {
				//check this vi (*ii) against the AP values;
				int thisSyn = APs[j];
				int indexOfSyn = table->indexOfSynonym[thisSyn];
				if (!possibleValuesOfAP[thisSyn][ii->at(indexOfSyn)]) {
					correct = false;
					break;
				}
			}

			if (!correct) {
				allTup->erase(ii++);
			}
			else {
				++ii;
			}
		}

		if (printG) {
			cout << "\nAfter tuning, all tuples are: \n";
			printLVI(*allTup);
		}

	}

}

class tupleCmp {
	vi indexes;
public:
	tupleCmp(vi in) : indexes(in) {}

	bool operator()(vi tup1, vi tup2) {
		// return true if tup1 <= tup2
		//bool tup1smallerOrEqual = true;
		for (size_t i = 0; i < indexes.size(); i++) {
			if (tup1[indexes[i]] > tup2[indexes[i]]) return false;
			if (tup1[indexes[i]] < tup2[indexes[i]]) return true;
		}
		//tup1 = tup2;
		return true;
	}
};

int compareTup(vi tup1, vi tup2, vi indexes1, vi indexes2) {

	for (size_t i = 0; i < indexes1.size(); i++) {
		if (tup1[indexes1[i]] > tup2[indexes2[i]]) return 1;
		if (tup1[indexes1[i]] < tup2[indexes2[i]]) return -1;
	}
	//tup1 = tup2;
	return 0;
}

void QueryEvaluator::expandFinalResultsWith(list<vi>* results, list<vi> *allTup, vector<bool>* gotAPinTup, map<int,int>* idOfSyn,vector<int> synAtResultIndex) {
	//idOfSyn is the index of a synonym in a tuple in allTup
	//synAtResultIndex[i] = the synonym at index i of a tuple in results;
	//well first, find out the common APoints;
	vi commonAPs;
	map<int, int> resultIdOfSyn;
	
	for (size_t i = 0; i < synAtResultIndex.size(); i++) {
		int curSyn = synAtResultIndex[i];
		resultIdOfSyn[curSyn] = i;
		
		if (isArtiPoint[curSyn] && gotAPinTup->at(curSyn)) {
			//if its an AP and already gotAPinTup  then add it to commonAPs
			commonAPs.push_back(curSyn);
		}
		(*gotAPinTup)[curSyn] = true;
	}

	//I guess we found all commonAPs;
	//now must sort *allTup and *results based on these commonAPs;
	vi indexes1 = vi();
	vi indexes2 = vi();
	for (size_t i = 0; i < commonAPs.size(); i++) {
		int synId = commonAPs[i];
		indexes1.push_back(idOfSyn->at(synId));
		indexes2.push_back(resultIdOfSyn[synId]);
	}
	allTup->sort(tupleCmp(indexes1));
	results->sort(tupleCmp(indexes2));

	//sorted already, now go through each of the tuples in allTup, 
	list<vi>::iterator it1 = allTup->begin();
	list<vi>::iterator it2 = results->begin();
	while (true) {
		//well we have the commonAPs values in it1 tuples and it2 tuples;
		//I guess we increase it1 and it2 until got the same values in commonAPs;
		while (true) {
			int it1vsit2 = compareTup(*it1, *it2, indexes1, indexes2);
			if (it1vsit2 == 0) break;
			if (it1vsit2 < 0) allTup->erase(it1++);
			else it2++;
			if (it1 == allTup->end() || it2 == results->end()) break;
		}

		//now, either it1 = it2 or we need to break;
		if (it1 == allTup->end() || it2 == results->end()) break;

		//now, it1=it2;
		list<vi>::iterator startIt = it2;
		while (it2 != results->end() && compareTup(*startIt, *it2, indexes2, indexes2) == 0) {
			it2++;
		}

		//now all the tuples from startIt to it2-1 has the same AP values;
		//go through each of the tuples starting from it1 and expand accordingly with all the values from
		//startIt to it2-1
		while (compareTup(*it1, *startIt, indexes1, indexes2) == 0) {
			//we need to expand specifically this it1.
			list<vi>::iterator ii = startIt;
			while (true) {
				//copy the values at ii to it1
				for (int id = 0; id < synAtResultIndex.size(); id++) {
					int curSynId = synAtResultIndex[id];
					int indexAtTup = idOfSyn->at(curSynId);
					(*it1)[indexAtTup] = (*ii)[id];
				}

				ii++;
				if (ii == it2) break;
				else allTup->insert(it1, *it1);// insert a copy before it1, then modify this it1 again
			}
			

			it1++;
			if (it1 == allTup->end()) break;
		}

		if (it2 == results->end()) {
			while (it1 != allTup->end()) {
				allTup->erase(it1++);
			}
		}

		if (it1 == allTup->end()) break;
	}


}

void QueryEvaluator::combineResults(list<string>& qresult)
{
	vector<QueryPair> selectedPairs = query.getSelectList();
	map<int, vi> synToSelectedId;
	for (size_t i = 0; i < selectedPairs.size(); i++) {
		int synId = getSynonymIndexFromName(selectedPairs[i].getValue());
		if (synToSelectedId.find(synId) == synToSelectedId.end()) {
			synToSelectedId[synId] = vi();
			synToSelectedId[synId].push_back(i);
		}
		else {
			synToSelectedId[synId].push_back(i);
		}
	}

	//well now we have got the possible values of APs in valuesOfAP[];
	//we can "narrow down" each of the Evaluation Graph;
	narrowDownAllGraphs();

	//now the results of each graph should be all correct;
	//Next, we find the relevant Graphs, adding the relevant result lists to the allGraphsResults;
	
	vector<int> selectedSynsUnsorted;
	map<int, int> idOfSyn;
	vector< list<vi> > allGraphResults;
	vector< vi > synsInResults;
	
	int tupleSize = 0;
	for (size_t i = 0; i < allGraphs.size(); i++) {
		bool relevant = false;
		for (size_t j = 0; j < allGraphs[i].vertices.size(); j++) {
			int curSyn = allGraphs[i].vertices[j];
			if (synToSelectedId.find(curSyn) != synToSelectedId.end()) {
				relevant = true;
			}
		}
		if (!relevant) continue;
		//now if its relevant, we should add the tuple size accordingly, 
		//as well as storing all results in allGraphResults;
		vi tupleIndexesToAdd;
		ResultTable* table = &allGraphs[i].resultTable;
		list<vi>* curAllTup = &table->allTuples;
		list<vi> currentResults;
		vi synInResult;

		for (size_t j = 0; j < allGraphs[i].vertices.size(); j++) {
			int curSyn = allGraphs[i].vertices[j];
			if (synToSelectedId.find(curSyn) != synToSelectedId.end() || isArtiPoint[curSyn]) {
				tupleIndexesToAdd.push_back(table->indexOfSynonym[curSyn]);
				synInResult.push_back(curSyn);

				if (idOfSyn.find(curSyn) != idOfSyn.end()) continue; // if this curSyn is already counted in overallTuple
				idOfSyn[curSyn] = tupleSize;
				tupleSize++;
				selectedSynsUnsorted.push_back(curSyn);
				
			}
		}

		//now, run through all the tuples and add results to new Results
		for (list<vi>::iterator ii = curAllTup->begin(); ii != curAllTup->end(); ii++) {
			vi newTup;
			for (size_t j = 0; j < tupleIndexesToAdd.size(); j++) {
				int curTupIndex = tupleIndexesToAdd[j];
				newTup.push_back(ii->at(curTupIndex));
			}
			currentResults.push_back(newTup);
		}
		//now actually we should make currentResults unique;
		currentResults.sort();
		currentResults.unique();
		// and finally add to allGraphResults;
		allGraphResults.push_back(currentResults);
		synsInResults.push_back(synInResult);
	}
	
	// now we have to take into accounts synonyms that dont appear in the clauses;
	for (size_t i = 0; i < selectedPairs.size(); i++) {
		int synId = getSynonymIndexFromName(selectedPairs[i].getValue());
		if (find(selectedSynsUnsorted.begin(), selectedSynsUnsorted.end(), synId) == selectedSynsUnsorted.end()) {
			//well we will have to add some results;
			list<vi> currentResults;
			vector<int> allEntitiesResults = loadValuesFromPKB(selectedPairs[i].getSynonymType());
			for (size_t j = 0; j < allEntitiesResults.size(); j++) {
				vi newTuple;
				newTuple.push_back(allEntitiesResults[j]);
				currentResults.push_back(newTuple);
			}
			allGraphResults.push_back(currentResults);
			vi newSynInResult;
			newSynInResult.push_back(synId);
			synsInResults.push_back(newSynInResult);
			idOfSyn[synId] = tupleSize;
			selectedSynsUnsorted.push_back(synId);
			tupleSize++;
		}
	}

	if (printDetails) cout << "done settling selected synonyms not in clauses\n";
	
	//then, we just slowly add the results of each relevant table, one at a time.
	
	if (printG) {
		cout << "$$$$$$$$$$$$$$    Results Tables " << "<size = " << allGraphResults.size() << "): \n";
		for (size_t i = 0; i < allGraphResults.size(); i++) {
			cout << "For result table number " << i<< ":\nSynonyms:";
			printVi(synsInResults[i]);

			printLVI(allGraphResults[i]);
			cout << "\n";
		}
	}

	vector<bool> gotAPinTup;
	gotAPinTup.assign(vertexCount, false);
	list<vi> finalResults;
	vi firstTup;
	firstTup.assign(tupleSize, ANY);
	finalResults.push_back(firstTup);

	//now we have to "expand tuple is a more advanced way"
	for (size_t i = 0; i < allGraphResults.size(); i++) {
		
		expandFinalResultsWith(&allGraphResults[i], &finalResults, &gotAPinTup, &idOfSyn, synsInResults[i]);
		if (printG) {
			cout << "After combining the table id " << i << ":\nSynonyms:";
			printVi(selectedSynsUnsorted);
			cout << "\n";
			printLVI(finalResults);
			cout << "\n";
		}
	}


	//got the list of final results, now just need to rearrange for actual results;
	
	int selectedTupleSize = selectedPairs.size();

	list<vi> finalFinalResults;

	for (list<vi>::iterator ii = finalResults.begin(); ii != finalResults.end(); ii++) {
		vector<int> currentTuple = vector<int>(selectedTupleSize);
		//cout << "size of currentTuple = " << currentTuple.size();

		
		for (int index = 0; index < tupleSize; index++) {
			if (synToSelectedId.find(selectedSynsUnsorted[index]) == synToSelectedId.end())
				continue;
			vi selectedIdsOfSyn = synToSelectedId[selectedSynsUnsorted[index]];
			for (size_t i = 0; i < selectedIdsOfSyn.size();i++)
				currentTuple[selectedIdsOfSyn[i]] = ii->at(index);
			//cout << "index = " << index << " selectedIdOfSyn = " << selectedIdOfSyn << " value = " << *syn << "\n";
		}
		//cout << "currentTuple: ";
		//printVi(currentTuple);
		finalFinalResults.push_back(currentTuple);		
	}
	finalFinalResults.sort();
	finalFinalResults.unique();
	for (list<vi>::iterator ii = finalFinalResults.begin(); ii != finalFinalResults.end(); ii++) {
		qresult.push_back(tupleToString(&(*ii)));
	}
	return;
}

void QueryEvaluator::recursiveAddToFinalResultsFrom(size_t curGraph, vector<list<list<int>>> &allResults, list<list<int>> &finalResults) {
	//cout << "curGraph = " << curGraph << "\n finalResults is :\n";
	//printLLI(finalResults);

	if (curGraph == allResults.size()) return;
	for (list<list<int>>::iterator existingList = finalResults.begin(); existingList != finalResults.end(); existingList++) {
		//for each existing list, well lets duplicate it first
		for (size_t i = 0; i < allResults[curGraph].size()-1; i++) {
			finalResults.insert(existingList, *existingList);
		}
		list<list<int>>::iterator ii = existingList;
		//then lets run an iterator from this Current result list back to append the appropriate lists.
		for (list<list<int>>::iterator i = allResults[curGraph].begin(); i != allResults[curGraph].end(); i++) {
			//append to the list at ii;
			ii->insert(ii->end(), i->begin(), i->end());
			if (next(i,1) != allResults[curGraph].end()) ii--;
		}
	}
	recursiveAddToFinalResultsFrom(curGraph + 1,allResults,finalResults);
}
