#pragma once
#include "iostream"
#include "vector"
#include "map"

class VarTable
{
private: 
	VarTable();
public:
	
	std::vector<std::string> indexToName;
	std::map<std::string, int> nameToIndex;
	int size;

	static VarTable& getInstance() {
		static VarTable instance;
		return instance;
	}

	VarTable(VarTable const&) = delete;
	void operator=(VarTable const&) = delete;

	int storeVariable(std::string varName);
	int getVarIndexFromName(std::string varName);

	std::string getVarNameFromIndex(int varIndex);
};

