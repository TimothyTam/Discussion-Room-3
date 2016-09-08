#include "MainQuery.h"

MainQuery::MainQuery() {
}

vector<string> MainQuery::processQuery(string query) {
	validation check = validation();
	vector<string> result;
	if (check.isValidQuery(query)) {
		// check.getDeclaration(); //-> gets the declaration list
		//parse string to list
		//getresults
		//projector
	}
	return result; //error
}
