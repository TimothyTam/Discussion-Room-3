#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryClause.h"
#include "QueryParam.h"
#include "QueryPair.h"

class QueryExtractor {
	public:
		QueryExtractor(void);
		Query query;

		Query extract(string query);

	private:
		

};