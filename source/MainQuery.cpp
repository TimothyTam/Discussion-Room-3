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
		cout << "\nSelect list: \n";
		for (size_t i = 0; i < queryObj.getSelectList().size(); i++) {
			cout << queryObj.getSelectList()[i].getValue() << ",";
		}

		cout << "\nQuery clauses: \n";
		for (size_t i = 0; i < queryObj.getClauseList().size(); i++) {
			cout << queryObj.getClauseList()[i].getParametersList()[0].getParamValue() << ",";
			cout << queryObj.getClauseList()[i].getParametersList()[1].getParamValue() << "\n" ;
		}
		
		QueryPair pair1 = QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a1");
		QueryPair pair2 = QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a1");
		vector<QueryPair> decList = { QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a1"), 
						QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a1") };
		vector<QueryPair> selectList = { QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a1") };
		vector<QueryClause> clauses = {
			QueryClause(QueryUtility::CLAUSETYPE_NEXT,"a1",2,
			{
				QueryParam(QueryUtility::PARAMTYPE_SYNONYM,QueryUtility::SYNONYM_TYPE_ASSIGN,"a1"),
				QueryParam(QueryUtility::PARAMTYPE_SYNONYM,QueryUtility::SYNONYM_TYPE_ASSIGN,"a1")
			}
			)
		};

		Query q = Query(decList, selectList, clauses);


		//getresult 
	}
	else {
		cout << "\nQueryValidation:";
		cout << "\nInvalid query\n";
		return;
	}
	evaluator.evaluate(queryObj, qresult); //error
}
