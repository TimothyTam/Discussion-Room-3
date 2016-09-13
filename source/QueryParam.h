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

class QueryParam {
	public:
		QueryParam(ParamType paramType, SynonymType synonymType, string value);
		QueryParam();

		ParamType getParamType(void);
		SynonymType getSynonymType(void);
		string getParamValue(void);
		
		bool QueryParam::operator==(QueryParam other);
		bool QueryParam::operator!=(QueryParam other);

	private:
		ParamType _paramType;
		SynonymType _synonymType;
		string _value;
};