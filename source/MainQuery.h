#pragma once

#include <string>
#include <vector>

#include "QueryValidation.h"
using namespace std;

class MainQuery {

public:
	MainQuery();
	void processQuery(string query, list<string>& qresults);
};

