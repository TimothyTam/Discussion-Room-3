#include "Uses.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

Uses::Uses()
{
}

vector<int> Uses::getVarUsedByStmt(int progLineIndex)
{
	vector<int> vectorResult;

	if (!(stmtVarIndexMap.find(progLineIndex) == stmtVarIndexMap.end())) {
		vectorResult = stmtVarIndexMap.at(progLineIndex);
	}

	return vectorResult;
}

vector<int> Uses::getVarUsedByProc(int procIndex)
{
	vector<int> vectorResult;

	if (!(procVarIndexMap.find(procIndex) == procVarIndexMap.end())) {
		vectorResult = procVarIndexMap.at(procIndex);
	}

	return vectorResult;
}

vector<int> Uses::getStmtUsingVar(int varIndex)
{
	vector<int> vectorResult;

	if (!(varStmtIndexMap.find(varIndex) == varStmtIndexMap.end())) {
		vectorResult = varStmtIndexMap.at(varIndex);
	}

	return vectorResult;
}

vector<int> Uses::getProcUsingVar(int procIndex)
{
	vector<int> vectorResult;

	if (!(varProcIndexMap.find(procIndex) == varProcIndexMap.end())) {
		vectorResult = varProcIndexMap.at(procIndex);
	}

	return vectorResult;
}

bool Uses::isStmtUsing(int progLineIndex, int varIndex)
{
	vector<int> vectorIndex;
	vectorIndex = getVarUsedByStmt(progLineIndex);

	if (std::find(vectorIndex.begin(), vectorIndex.end(), varIndex) != vectorIndex.end()) {
		return true;
	}
	else {
		return false;
	}
}

bool Uses::isProcUsing(int procIndex, int varIndex)
{
	vector<int> vectorIndex;
	vectorIndex = getVarUsedByProc(procIndex);	

	if (std::find(vectorIndex.begin(), vectorIndex.end(), varIndex) != vectorIndex.end()) {
		return true;
	}
	else {
		return false;
	}
}