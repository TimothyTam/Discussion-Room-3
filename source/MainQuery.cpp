#include "MainQuery.h"
#include "QueryExtractor.h"
#include "QueryEvaluator.h"

MainQuery::MainQuery() {
}

void MainQuery::processQuery(string query, list<string>& qresult) {
	validation check = validation();
	QueryExtractor extractor = QueryExtractor();
	QueryEvaluator evaluator = QueryEvaluator();
	Query queryObj = Query();
	list<string> result;

	if (check.isValidQuery(query)) {
		queryObj = extractor.extract(check.getDeclaration(), query);
		//getresult 
	}

	evaluator.evaluate(queryObj, qresult); //error
}
