#pragma once
#include "vector"
#include "map"
#include "set"

#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::map<int, vi> map_i_vi;
typedef std::map<int, si> map_i_si;


class Pattern {
private:
	Pattern() {};

public:
	static Pattern& getInstance()
	{
		static Pattern instance;
		return instance;
	}

	Pattern(Pattern const&) = delete;
	void operator=(Pattern const&) = delete;

	vi getPatternAssign(int varIndex, std::string expr);

};