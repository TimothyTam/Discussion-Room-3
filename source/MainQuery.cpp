#include "MainQuery.h"
#include "QueryExtractor.h"

MainQuery::MainQuery() {
}

vector<string> MainQuery::processQuery(string query) {
	validation check = validation();
	QueryExtractor extractor = QueryExtractor();
	Query queryObj = Query();
	vector<string> result;

	if (check.isValidQuery(query)) {
		queryObj = extractor.extract(check.getDeclaration(), query);
		//getresults
		//projector
	}
	return result; //error
}
