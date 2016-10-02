#pragma once
#include "PKB.h"

using namespace std;
typedef map<int, vi> map_i_vi;
typedef set<int> si;

class Call {
private:

	Call();
	TNode* rootNode;

	map_i_vi callTable;
	map_i_vi reverseCallTable;

	map_i_vi callTransitiveTable;
	map_i_vi reverseCallTransitiveTable;

public:
	static Call& getInstance() {
		static Call instance;
		return instance;
	}

	Call(Call const&) = delete;
	void operator=(Call const&) = delete;

	si generateCallTableForSingleProcedure(TNode* current, int procedure);
	void generateCallTable(TNode* astRoot);

	void addCallValueToCallNodes(TNode* astRoot);
};
