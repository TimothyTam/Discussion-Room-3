#include "TestWrapper.h"
#include <string>
#include "Parser.h"
#include "PKB.h"
#include "MainQuery.h"

// implementation code of WrapperFactory - do NOT modify the next 5 lines
AbstractWrapper* WrapperFactory::wrapper = 0;
AbstractWrapper* WrapperFactory::createWrapper() {
  if (wrapper == 0) wrapper = new TestWrapper;
  return wrapper;
}
// Do not modify the following line
volatile bool TestWrapper::GlobalStop = false;

// a default constructor
TestWrapper::TestWrapper() {
  // create any objects here as instance variables of this class
  // as well as any initialization required for your spa program
}

// method for parsing the SIMPLE source
void TestWrapper::parse(std::string filename) {
	try {
		Parse(filename);

		PKB::getInstance().buildAllTables();
		cout << "PKB done building all tables" << '\n';
	}
	catch (std::exception& ex) {
		std::cout << ex.what() << "\n";
		return;
	}
}

// method to evaluating a query
void TestWrapper::evaluate(std::string query,std::list<std::string> & results){
	//results.push_back("hello");
// call your evaluator to evaluate the query here
  // ...code to evaluate query...
	cout << "In TestWrapper, query=_" << query << "_\n";
	MainQuery mainQuery = MainQuery();
	mainQuery.processQuery(query, results);
  // store the answers to the query in the results list (it is initially empty)
  // each result must be a string.
}
