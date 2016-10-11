#include "GraphEdge.h"



GraphEdge::GraphEdge(int v, QueryClause c, bool isOutward)
{
	vertex = v;
	clause = c;
	outward = isOutward;
}



