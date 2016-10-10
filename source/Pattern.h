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
typedef std::map<int, int> map_i_i;

class Pattern {
private:
	Pattern() {};

	//varIndex | Stmts No. - Facilitate If("X",_,_)
	map_i_vi ifControlVars;
	map_i_vi whileControlVars;

	// Stmts No. - Facilitate If(s1,_,_)
	vi ifStmts;
	vi whileStmts;

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

	si generatePatternDataForSingleProcedure(TNode* current, NodeType type);
public:
	static Pattern& getInstance()
	{
		static Pattern instance;
		return instance;
	}

	Pattern(Pattern const&) = delete;
	void operator=(Pattern const&) = delete;

	void generatePatternData(TNode* astRoot);
	vi getPatternAssign(int varIndex, std::string expr);
	vi getPatternIf(int varIndex);
	vi getPatternWhile(int varIndex);
};