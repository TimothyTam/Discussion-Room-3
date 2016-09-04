#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum ParamType {
	PARAMTYPE_SYNONYM,
	PARAMTYPE_ENT_NAME,
	PARAMTYPE_PATTERN_STRING,
	PARAMTYPE_PLACEHOLDER
};

class QueryParam {
	public:
		ParamType getParamType(void);
		string getParamValue(void);

	private:
		ParamType type;
		string value;
};

ParamType QueryParam::getParamType(void) {
	return this->type;
}

string QueryParam::getParamValue(void) {
	return this->value;
}