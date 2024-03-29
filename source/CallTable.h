#pragma once
#include "PKB.h"

using namespace std;
typedef unordered_map<int, vi> map_i_vi;
typedef set<int> si;
typedef unordered_map<TNode*, string> map_T_str;
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

	vp_i_i callPair;
	vp_i_i callTransitivePair;

	std::vector<std::vector<bool>> procArray;
	std::vector<std::vector<bool>> procTransArray;

	void updateCallValueInCallNodes();

	void buildTransitiveTable();

	void buildReverseCallTable(bool transitive);

	void buildCallPair();

	void buildCallTransitivePair();

	int procTableSize;
	void build2DArrayTable();

	si generateCallTableForSingleProcedure(TNode* current);

public:
	static CallTable& getInstance() {
		static CallTable instance;
		return instance;
	}

	CallTable(CallTable const&) = delete;
	void operator=(CallTable const&) = delete;

	void generateCallTable(TNode* astRoot);

	void addToCallNodeToStringTable(TNode* node, string procName);
	
	vector<int> callsGenericSpecific(int procIndex);    //calls(p, �gpanda�h)
	vector<int> callsSpecificGeneric(int procIndex);    //calls(�gpanda�h, p)

	vector<int> callsTransitiveGenericSpecific(int procIndex); //calls*(p,"panda")
	vector<int> callsTransitiveSpecificGeneric(int procIndex); //calls*("panda",p)

	vp_i_i callsGenericGeneric();
	vp_i_i callsTransitiveGenericGeneric();

	bool whetherCalls(int procIndex1, int procIndex2);
	bool whetherTransitiveCalls(int procIndex1, int procIndex2);

};
