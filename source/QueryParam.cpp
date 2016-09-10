#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"
#include "QueryPair.h"

using namespace std;

QueryParam::QueryParam(ParamType ptype, SynonymType stype, string val) {
	paramType = ptype;
	value = val;
}

QueryParam::QueryParam(){}

ParamType QueryParam::getParamType(void) {
	return this->paramType;
}

string QueryParam::getParamValue(void) {
	return this->value;
}

bool QueryParam::operator==(QueryParam other) {
	return (this->getParamType() == other.getParamType() && this->getParamValue() == other.getParamValue());
}

bool QueryParam::operator!=(QueryParam other) {
	return !(this->getParamType() == other.getParamType() && this->getParamValue() == other.getParamValue());
}