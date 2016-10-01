#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include "QueryPair.h"


QueryPair::QueryPair(SynonymType synType, string val) {
	synonymType = synType;
	value = val;
}

SynonymType QueryPair::getSynonymType(void) {
	return this->synonymType;
}

string QueryPair::getValue(void) {
	return this->value;
}

bool QueryPair::operator==(QueryPair other) {
	return this->getSynonymType() == other.getSynonymType() && this->getValue() == other.getValue();
}