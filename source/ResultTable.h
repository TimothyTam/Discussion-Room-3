#pragma once
#include <vector>
#include <list>
#include <map>
#include "ResultUnit.h"
typedef std::vector<int> vi;

const int ANY = -3;

class ResultTable
{
public:
	ResultTable(vi synonymList);
	ResultTable();
	std::list<std::vector<int>> allTuples;
	std::map<int, int> indexOfSynonym;
	vi synonymList;
};

