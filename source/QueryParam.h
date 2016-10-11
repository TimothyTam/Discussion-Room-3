#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include"QueryPair.h"

using namespace std;

enum ParamType {
	PARAMTYPE_SYNONYM,
	PARAMTYPE_ENT_NAME,
	PARAMTYPE_PATTERN_STRING_EXACT,
	PARAMTYPE_PATTERN_STRING_LEFT_OPEN,
	PARAMTYPE_PATTERN_STRING_RIGHT_OPEN,
	PARAMTYPE_PATTERN_STRING_BOTH_OPEN,
	PARAMTYPE_PLACEHOLDER,

	PARAMTYPE_NULL
};

// This class represents an argument/parameter in a clause.
// It stores the what type it is, what synonym type it is (if
// it is a synonym), as well as the value.

class QueryParam {
	public:
		QueryParam(QueryUtility::ParamType paramType,
				   QueryUtility::SynonymType synonymType,
				   string value);
		QueryParam();

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