#include "GraphEdge.h"


GraphEdge::GraphEdge() {

}

GraphEdge::GraphEdge(int u, int v, QueryClause c, bool isOutward)
{
	fromVertex = u;
	toVertex = v;
	backEdge = NULL;
	clause = c;
	outward = isOutward;
	isDone = false;
}



