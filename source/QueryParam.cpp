#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"

using namespace std;

QueryParam::QueryParam(ParamType type, string val) {

}

ParamType QueryParam::getParamType(void) {
	return this->type;
}

string QueryParam::getParamValue(void) {
	return this->value;
}
