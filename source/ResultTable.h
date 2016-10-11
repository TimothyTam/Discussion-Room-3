#pragma once
#include <vector>
#include <map>
#include "ResultUnit.h"
typedef std::vector<int> vi;

class ResultTable
{
public:
	ResultTable(vi synonymList);
	ResultTable();
	std::vector<std::vector<ResultUnit>> allTuples;
private:
	vi synonymList;
	std::map<int, int> indexOfSynonym;
};

