#pragma once
#include "CallTable.h"

si CallTable::generateCallTableForSingleProcedure(TNode* current) {
	std::vector<TNode*> childs = current->childs;
	si result;

	if (current->type == NodeType::Call) {
		result.insert(current->value);
	} else {
		for (TNode* child : childs) {
			si temp = generateCallTableForSingleProcedure(child);
			result.insert(temp.begin(), temp.end());
		}
	}
	return result;
}

void CallTable::generateCallTable(TNode* astRoot) {

	updateCallValueInCallNodes();

	std::vector<TNode*> childs = astRoot->childs;
	if (astRoot->type == NodeType::Program) {
		for (TNode* child : childs) {
			si result = generateCallTableForSingleProcedure(child);
			callTable[child->value].assign(result.begin(), result.end());
		}
	}

	buildTransitiveTable();

	buildReverseCallTable(false);
	buildReverseCallTable(true);
}

void CallTable::buildReverseCallTable(bool transitive) {
	map_i_si calledBySet;
	map_i_vi::iterator it;
	map_i_si::iterator itSet;
	map_i_vi *xCall;
	map_i_vi *calledByX;

	if (transitive) {
		xCall = &callTable;
		calledByX = &reverseCallTable;
	}
	else {
		xCall = &callTransitiveTable;
		calledByX = &reverseCallTransitiveTable;
	}

	for (it = xCall->begin(); it != xCall->end(); it++) {
		for (int j : it->second) {
			calledBySet[j].insert(it->first);
		}
	}

	for (itSet = calledBySet.begin(); itSet != calledBySet.end(); itSet++) {
		(*calledByX)[itSet->first].assign(itSet->second.begin(),
			itSet->second.end());
	}
}

void CallTable::buildTransitiveTable() {
	int tableSize = PKB::getInstance().getProcTableSize();

	int** arr = new int*[tableSize];
	for (int i = 0; i < tableSize; i++) {
		arr[i] = new int[tableSize];
	}
		
	for (int i = 0; i < tableSize; i++) {
		vi to = callTable[i];
		for (int j = 0; j < to.size(); j++) {
			arr[i][to.at(j)] = 1;
		}
	}

	// Run Floyd Warshall Algorithm
	for (int k = 0; k < tableSize; k++)	{
		for (int i = 0; i < tableSize; i++)	{
			for (int j = 0; j < tableSize; j++)	{
				arr[i][j] = arr[i][j] || (arr[i][k] && arr[k][j]);
			}
		}
	}

	for (int i = 0; i < tableSize; i++) {
		if (arr[i][i] == 1) {
			//Cyclic
			string error_msg = "Cyclic Call: Procedure " + PKB::getInstance().getProcNameFromIndex(i) + " have a cyclic call";
			throw runtime_error(error_msg);
		}
	}

	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			if (arr[i][j] == 1) {
				callTransitiveTable[i].push_back(j);
			}
		}
	}

	//deallocate the array
	for (int i = 0; i < tableSize; i++)
		delete[] arr[i];
	delete[] arr;
}

//Call Nodes value are all 0, update their value to their respective procIndex
void CallTable::updateCallValueInCallNodes() {
	for (map_T_str_it iterator = callNodeToStringTable.begin(); iterator != callNodeToStringTable.end(); iterator++) {
		iterator->first->value = PKB::getInstance().getProcIndexFromName(iterator->second);
	}
}

void CallTable::addToCallNodeToStringTable(TNode* node, string procName) {
	callNodeToStringTable.insert(make_pair(node, procName));
}

vi CallTable::callsGenericSpecific(int procIndex) {
	return (reverseCallTable.count(procIndex) == 1) ? reverseCallTable[procIndex] : vi();
}

vi CallTable::callsSpecificGeneric(int procIndex) {
	return (callTable.count(procIndex) == 1) ? callTable[procIndex] : vi();
}

vi CallTable::callsTransitiveGenericSpecific(int procIndex) {
	return (reverseCallTransitiveTable.count(procIndex) == 1) ? reverseCallTransitiveTable[procIndex] : vi();
}

vi CallTable::callsTransitiveSpecificGeneric(int procIndex) {
	return (callTransitiveTable.count(procIndex) == 1) ? callTransitiveTable[procIndex] : vi();
}

vp_i_i CallTable::callsGenericGeneric() {
	return callPair;
}

vp_i_i CallTable::callsTransitiveGenericGeneric() {
	return callTransitivePair;
}

bool CallTable::whetherCalls(int procIndex1, int procIndex2) {
	vi stmts;
	if (reverseCallTable.count(procIndex2) == 1) {
		stmts = reverseCallTable[procIndex2];
		return (std::find(stmts.begin(), stmts.end(), procIndex1) != stmts.end());
	}
	return false;
}
bool CallTable::whetherTransitiveCalls(int procIndex1, int procIndex2) {
	vi stmts;
	if (reverseCallTransitiveTable.count(procIndex2) == 1) {
		stmts = reverseCallTransitiveTable[procIndex2];
		return (std::find(stmts.begin(), stmts.end(), procIndex1) != stmts.end());
	}
	return false;

}

void CallTable::buildCallPair() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (it = callTable.begin(); it != callTable.end(); it++) {
		for (int i : it->second) {
			callPair.push_back(make_pair(it->first, i));
		}
	}
}


void CallTable::buildCallTransitivePair() {
	map_i_vi::iterator it;
	PKB& inst = PKB::getInstance();

	for (it = callTransitiveTable.begin(); it != callTransitiveTable.end(); it++) {
		for (int i : it->second) {
			callTransitivePair.push_back(make_pair(it->first, i));
		}
	}
}

