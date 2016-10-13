#pragma once
#include "QueryClause.h"
class GraphEdge
{
public:
	GraphEdge();
	GraphEdge(int u, int v, QueryClause clause, bool isOutward);
	GraphEdge* backEdge;
	int fromVertex; // will be -1 if there is no synonym related in this Clause
	int toVertex;
	QueryClause clause;
	bool outward;
	bool isDone;
};

