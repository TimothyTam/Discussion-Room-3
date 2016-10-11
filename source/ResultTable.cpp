#include "ResultTable.h"

using namespace std;


ResultTable::ResultTable(vi synList) {
	synonymList = synList;
	indexOfSynonym = map<int, int>();
	for (size_t i = 0; i < synList.size(); i++) {
		indexOfSynonym[synList[i]] = i;
	}
}

ResultTable::ResultTable()
{
}
