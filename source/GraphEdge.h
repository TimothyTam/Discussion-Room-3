#pragma once
#include "QueryClause.h"
class GraphEdge
{
public:
	GraphEdge(int v, QueryClause clause, bool isOutward);
	int vertex; // will be -1 if there is no synonym related in this Clause
	QueryClause clause;
	bool outward;
};

