#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryUtility.h"

using namespace std;

// This class represents an argument/parameter in a clause.
// It stores the what type it is, what synonym type it is (if
// it is a synonym), as well as the value.

class QueryParam {
	public:
		QueryParam();
		QueryParam(QueryUtility::ParamType paramType,
				   QueryUtility::SynonymType synonymType,
				   string value);

		QueryUtility::ParamType getParamType(void);
		QueryUtility::SynonymType getSynonymType(void);
		string getParamValue(void);
		
		bool QueryParam::operator==(QueryParam other);
		bool QueryParam::operator!=(QueryParam other);

	private:
		QueryUtility::ParamType _paramType;
		QueryUtility::SynonymType _synonymType;
		string _value;
};