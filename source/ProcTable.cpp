#include "ProcTable.h"

using namespace std;

ProcTable::ProcTable()
{
	this->size = 0;
	this->indexToName = vector<string>();
	this->nameToIndex = map<string, int>();
}

int ProcTable::addProc(string procName) {
	this->indexToName.push_back(procName);
	this->nameToIndex[procName] = this->size;
	this->size++;
	return size - 1;
}

string ProcTable::getProcNameFromIndex(int procIndex) {
	if (procIndex < 0 || procIndex >= this->size) {
		return NULL;
	}
	else {
		return this->indexToName[procIndex];
	}
}

int ProcTable::getProcIndexFromName(string procName) {
	if (this->nameToIndex.count(procName) == 0) {
		return -1;
	}
	else {
		return this->nameToIndex[procName];
	}
}

int ProcTable::getSize() {
	return this->size;
}