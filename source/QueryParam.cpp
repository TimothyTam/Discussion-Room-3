#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "QueryParam.h"
#include "QueryPair.h"

using namespace std;

QueryParam::QueryParam(ParamType paramType, SynonymType synonymType, string value) {
	_paramType = paramType;
	_synonymType = synonymType;
	_value = value;
}

QueryParam::QueryParam(){}

ParamType QueryParam::getParamType(void) {
	return this->_paramType;
}

SynonymType QueryParam::getSynonymType(void) {
	return this->_synonymType;
}

string QueryParam::getParamValue(void) {
	return this->_value;
}

bool QueryParam::operator==(QueryParam other) {
	return (this->getParamType() == other.getParamType() 
		 && this->getParamValue() == other.getParamValue()
		 && this->getSynonymType() == other.getSynonymType());
}

bool QueryParam::operator!=(QueryParam other) {
	return !(this->getParamType() == other.getParamType()
		  && this->getParamValue() == other.getParamValue()
		  && this->getSynonymType() == other.getSynonymType());
}