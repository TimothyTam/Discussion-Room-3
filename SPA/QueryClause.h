#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum ClauseType {
	MODIFIES,
	USES,
	FOLLOWS,
	FOLLOWS_STAR,
	PARENT,
	PARENTS_STAR,

	// insert rest of clauses for > iter 1

};

class QueryClause {

	public:
		ClauseType getClauseType(void);


	private:
		ClauseType type;
		
};

ClauseType QueryClause::getClauseType(void) {
	return type;
}