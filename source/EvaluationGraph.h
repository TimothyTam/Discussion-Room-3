#pragma once
#include "ResultTable.h"
#include "GraphEdge.h"


class EvaluationGraph
{
public:
	EvaluationGraph();
	EvaluationGraph(vi verticesList, std::vector<std::vector<GraphEdge>> adList);
	vi vertices;
	std::vector<std::vector<GraphEdge>> adjacencyList;

private:
	ResultTable resultTable;

};

