#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include"QueryPair.h";

using namespace std;

enum ParamType {
	PARAMTYPE_SYNONYM,
	PARAMTYPE_ENT_NAME,
	PARAMTYPE_PATTERN_STRING_EXACT,
	PARAMTYPE_PATTERN_STRING_LEFT_OPEN,
	PARAMTYPE_PATTERN_STRING_RIGHT_OPEN,
	PARAMTYPE_PATTERN_STRING_BOTH_OPEN,
	PARAMTYPE_PLACEHOLDER
};

class QueryParam {
	public:
		QueryParam(ParamType pType, SynonymType sType, string val);
		QueryParam();
		ParamType getParamType(void);
		string getParamValue(void);

	private:
		ParamType paramType;
		string value;
};