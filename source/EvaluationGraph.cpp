#include "EvaluationGraph.h"



EvaluationGraph::EvaluationGraph()
{
}

EvaluationGraph::EvaluationGraph(vi verticesList, std::vector<std::vector<GraphEdge*>> adList, std::vector<GraphEdge*> edges)
{
	vertices = verticesList;
	adjacencyList = adList;
	allEdges = edges;
	resultTable = ResultTable(verticesList);
}

bool EvaluationGraph::evaluate()
{
	vector<int> d = vector<int>();
	d.push_back(2);
	resultTable.allTuples.push_back(d);
	d[0] = 1;
	resultTable.allTuples.push_back(d);
	return true;
}
