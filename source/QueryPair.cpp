#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "QueryPair.h"
#include "QueryUtility.h"


QueryPair::QueryPair(QueryUtility::SynonymType synType, string val) {
	synonymType = synType;
	value = val;
}

QueryUtility::SynonymType QueryPair::getSynonymType(void) {
	return this->synonymType;
}

string QueryPair::getValue(void) {
	return this->value;
}

bool QueryPair::operator==(QueryPair other) {
	return this->getSynonymType() == other.getSynonymType() && this->getValue() == other.getValue();
}