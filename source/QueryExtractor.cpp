#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Query.h"
#include "QueryClause.h"
#include "QueryExtractor.h"
#include "QueryParam.h"
#include "QueryPair.h"

using namespace std;

QueryExtractor::QueryExtractor(void) {

}

Query QueryExtractor::extract(unordered_map<string, string> declarationMap, string query) {
	Query q;
	vector<QueryPair> queryPairList = getDeclarations(declarationMap);





	return q;
}

vector<QueryPair> QueryExtractor::getDeclarations(unordered_map<string, string> declarationMap) {
	vector<QueryPair> list;
	
	for (auto it = declarationMap.begin(); it != declarationMap.end(); ++it) {

	}

	return list;
}

