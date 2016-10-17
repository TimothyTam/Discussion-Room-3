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


class Affect {
private:
	// key	| Value
	map_i_vi affect;			// 1	| 2,3 //Max infinite
	map_i_vi affectReverse;		// 2	| 1 //Max infinite

	map_i_si affectTrans;			// 1	| 2,3 //Max infinite
	map_i_si affectTransReverse;	// 2	| 1 //Max infinite

	Affect() {};

public:
	static Affect& getInstance()
	{
		static Affect instance;
		return instance;
	}

	Affect(Affect const&) = delete;
	void operator=(Affect const&) = delete;

	void generateAffectTable();

	void newQuery();
};