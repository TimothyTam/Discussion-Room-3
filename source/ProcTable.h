#pragma once
#include "iostream"
#include "vector"
#include "unordered_map"

class ProcTable
{
public:
	ProcTable();
	std::vector<std::string> indexToName;
	std::unordered_map<std::string, int> nameToIndex;
	int size;

	static ProcTable& getInstance() {
		static ProcTable instance;
		return instance;
	}

	int addProc(std::string procName);
	std::string getProcNameFromIndex(int procIndex);
	int getProcIndexFromName(std::string procName);
	int getSize();
};
