#include "MainQuery.h"
#include "QueryExtractor.h"
#include "QueryEvaluator.h"
#include "PKB.h"

MainQuery::MainQuery() {
}

void MainQuery::processQuery(string query, list<string>& qresult) {

	cout << "In MainQuery now\n";
	QueryValidation check = QueryValidation();
	QueryExtractor extractor = QueryExtractor();
	QueryEvaluator evaluator = QueryEvaluator();
	Query queryObj = Query();
	

	if (check.isValidQuery(query)) {
		// pass in QueryValidation object(check)
		cout << "before extracting query object\n";
		queryObj = extractor.extract(check.getDeclaration(), check.getSelect(), check.getClauseEnum(), check.getClauseParam());
		// Printing query object
		// For debuggin purposes
		cout << "done extracting query objects \n";
		cout << "Declaration list: \n";
		for (size_t i = 0; i < queryObj.getDeclarationList().size(); i++) {
			cout << queryObj.getDeclarationList()[i].getValue() << ",";
		}
		cout << "Select list: \n";
		for (size_t i = 0; i < queryObj.getSelectList().size(); i++) {
			cout << queryObj.getSelectList()[i].getValue() << ",";
		}

		cout << "Query clauses: \n";
		for (size_t i = 0; i < queryObj.getClauseList().size(); i++) {
			cout << queryObj.getClauseList()[i].getParametersList()[0].getParamValue() << ",";
			cout << queryObj.getClauseList()[i].getParametersList()[1].getParamValue() << "\n" ;
		}
		


		//getresult 
	}
	else {
		cout << "\nInvalid query\n";
		return;
	}
	evaluator.evaluate(queryObj, qresult); //error
}
