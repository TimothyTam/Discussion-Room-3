#pragma once
#include <vector>
#include <map>
using namespace std;

typedef int varIndex;
typedef int progLineIndex;
typedef int procIndex;
typedef std::vector<int> progLineIndexes;
typedef std::vector<int> varIndexes;
typedef std::vector<int> procIndexes;

class Uses {
private:
	map<int, vector<int>> stmtVarIndexMap;
	map<int, vector<int>> varStmtIndexMap;
	map<int, vector<int>> procVarIndexMap;
	map<int, vector<int>> varProcIndexMap;

public:
	Uses();
	vector<int> getVarUsedByStmt(int progLineIndex);
	vector<int> getVarUsedByProc(int procIndex);

	vector<int> getStmtUsingVar(int varIndex);
	vector<int> getProcUsingVar(int procIndex);
	
	bool isStmtUsing(int progLineIndex, int varIndex);
	bool isProcUsing(int procIndex, int varIndex);
};

