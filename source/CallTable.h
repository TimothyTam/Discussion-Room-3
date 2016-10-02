#pragma once
#include "PKB.h"

using namespace std;
typedef map<int, vi> map_i_vi;
typedef set<int> si;
typedef map<TNode*, string> map_T_str;
typedef map_T_str::iterator map_T_str_it;


class CallTable {
private:
	CallTable() {};
	TNode* rootNode;

	map_i_vi callTable;
	map_i_vi reverseCallTable;

	map_i_vi callTransitiveTable;
	map_i_vi reverseCallTransitiveTable;

	map_T_str callNodeToStringTable;

	void updateCallValueInCallNodes();

	void buildTransitiveTable();

	void buildReverseCallTable(bool transitive);

public:
	static CallTable& getInstance() {
		static CallTable instance;
		return instance;
	}

	CallTable(CallTable const&) = delete;
	void operator=(CallTable const&) = delete;

	si generateCallTableForSingleProcedure(TNode* current);
	void generateCallTable(TNode* astRoot);

	void addToCallNodeToStringTable(TNode* node, string procName);
	
};
