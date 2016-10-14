#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "QueryUtility.h"

using namespace std;

// This class is used mainly for the user's declarations
// as well as selects. The information is stored as a pair
// of values: the type and value of the synonym.

class QueryPair {
	public:
		QueryPair();
		QueryPair(QueryUtility::SynonymType synType, string val);

		QueryUtility::SynonymType getSynonymType(void);
		string getValue(void);
		bool QueryPair::operator==(QueryPair other);

	private:
		QueryUtility::SynonymType synonymType;
		string value;
};