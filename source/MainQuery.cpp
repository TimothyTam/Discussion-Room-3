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
	
	cout << "In MainQuery, query=_" << query << "_";
	if (check.isValidQuery(query)) {
		queryObj = extractor.extract(check.getDeclaration(), query);
		
		// Printing query object
		cout << "done extracting query objects \n";
		cout << "Declaration list: \n";
		for (size_t i = 0; i < queryObj.getDeclarationList().size(); i++) {
			cout << queryObj.getDeclarationList()[i].getValue() << ",";
		}
		cout << "\nQuery select:" << queryObj.getSelectList()[0].getValue() << "\n";
		cout << "Query clauses: \n";
		for (size_t i = 0; i < queryObj.getClauseList().size(); i++) {
			cout << queryObj.getClauseList()[i].getParametersList()[0].getParamValue() << ",";
			cout << queryObj.getClauseList()[i].getParametersList()[1].getParamValue() << ",";
		}



		//getresult 
	}
	else {
		cout << "Invalid query\n";
	}

	qresult.push_back("dummy");
	//evaluator.evaluate(queryObj, qresult); //error
}
