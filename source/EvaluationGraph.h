#pragma once
#include "ResultTable.h"
#include "GraphEdge.h"
#include "QueryUtility.h"


class EvaluationGraph
{
public:
	EvaluationGraph();
	EvaluationGraph(vi verticesList, std::vector<std::vector<pair<int,GraphEdge*>>> adList, std::vector<GraphEdge*> edges);
	vi vertices;
	vector<GraphEdge*> allEdges;
	std::vector<std::vector<pair<int,GraphEdge*>>> adjacencyList;
	ResultTable resultTable;
	ResultTable finalResults;
	bool evaluate();
private:
	

};

