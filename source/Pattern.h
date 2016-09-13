#pragma once
#include <vector>
#include <map>
#include <set>
#include <stack>

#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::map<int, vi> map_i_vi;
typedef std::map<int, si> map_i_si;


class Pattern {
private:
	Pattern() {};

	void error(std::string expected, std::string given);
	std::string getToken();
	void matchTerm();
	void matchOperator();
	void matchExpression();
	void resetExpression(std::string newExpr);

	TNode* createTreeFromExpression(std::string expr);

	std::string next_token;
	char buffer;
	int token_index;
	int bracket_term;
	std::string expression_string;
	std::vector<std::string> expression_terms;
	std::stack<int> times_index;
	std::stack<int> bracket_index;

	bool isSubTree(TNode* assignRoot, TNode* patternRoot);
	bool areEqual(TNode* t1, TNode* t2);
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