#pragma once
#include "iostream"
#include "vector"
#include "map"

class ProcTable
{
public:
	ProcTable();
	std::vector<std::string> indexToName;
	std::map<std::string, int> nameToIndex;
	int size;

	static ProcTable& getInstance() {
		static ProcTable instance;
		return instance;
	}

	int addProc(std::string procName);
	std::string getProcNameFromIndex(int procIndex);
	int getProcIndexFromName(std::string procName);

};
