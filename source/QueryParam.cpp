#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"

using namespace std;

QueryParam::QueryParam(ParamType type, string val) {
	paramType = type;
	value = val;
}

ParamType QueryParam::getParamType(void) {
	return this->paramType;
}

string QueryParam::getParamValue(void) {
	return this->value;
}
