#include "VarTable.h"

using namespace std;

VarTable::VarTable()
{
	this->size = 0;
	this->indexToName = vector<string>();
	this->nameToIndex = map<string, int>();
}

int VarTable::storeVariable(string varName) {
	if (this->nameToIndex.count(varName) > 0) {
		return nameToIndex[varName];
	}
	else {
		this->indexToName.push_back(varName);
		this->nameToIndex[varName] = this->size;
		this->size++;
		return size - 1;
	}

}

string VarTable::getVarNameFromIndex(int varIndex) {
	if (varIndex < 0 || varIndex >= this->size) {
		return "";
	}
	else {
		return this->indexToName[varIndex];
	}
}

int VarTable::getVarIndexFromName(string varName) {
	if (this->nameToIndex.count(varName) == 0) {
		return -1;
	}
	else {
		return this->nameToIndex[varName];
	}
}