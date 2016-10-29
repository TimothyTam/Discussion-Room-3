#pragma once
#include "set"
#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::unordered_map<int, vi> map_i_vi;
typedef std::unordered_map<int, si> map_i_si;
typedef std::unordered_map<int, int> map_i_i;
typedef std::vector<std::pair<int, int>> vp_i_i;

typedef std::unordered_map<int, std::unordered_map<int, int>> map_map;

class Affect {
private:
	// key	| Value
	map_map affect;				// 1	| 2,3 //Max infinite
	map_map affectReverse;		// 2	| 1 //Max infinite
	map_i_i affectReverseCalculated;

	map_map affectTrans;			// 1	| 2,3 //Max infinite
	map_map affectTransReverse;		// 2	| 1 //Max infinite

	bool allAffectCalculated;

	Affect() {
		allAffectCalculated = false;
	};

	void depthFirstSearchAffectSpecificGeneric(CFGNode* current, unordered_map<int, int>& stmtsUsedMap, int modified, unordered_map<CFGNode*, int>& visited, int lineNo);
	
	void calculateAffectSpecificGeneric(CFGNode* node);

public:
	static Affect& getInstance()
	{
		static Affect instance;
		return instance;
	}

	Affect(Affect const&) = delete;
	void operator=(Affect const&) = delete;

	vi getAffectSpecificGeneric(int lineNo);
	vi getAffectGenericSpecific(int lineNo);

	//Hoang, getAffectGenericGeneric is Affect(s1,s2). This returns <1,2>,<1,3> etc. Current impl is naive method, go and change it to the fast method.
	vp_i_i getAffectGenericGeneric();
	bool whetherAffect(int lineNo, int lineNo2);

	//Affect*(1,s1)
	vi getTransitiveAffectSpecificGeneric(int lineNo);
	//Affect*(s1,1)
	vi getTransitiveAffectGenericSpecific(int lineNo);
	//Affect*(s1,s2)
	vp_i_i getTransitiveAffectGenericGeneric();
	//Affect(1,2)
	bool whetherTransitiveAffect(int lineNo, int lineNo2);

	void newQuery();
};