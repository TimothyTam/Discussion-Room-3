#include "EvaluationGraph.h"



EvaluationGraph::EvaluationGraph()
{
}

EvaluationGraph::EvaluationGraph(vi verticesList, std::vector<std::vector<GraphEdge>> adList)
{
	vertices = verticesList;
	adjacencyList = adList;
}
