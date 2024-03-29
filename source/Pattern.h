#pragma once
#include <vector>
#include <unordered_map>
#include <set>
#include <stack>

#include "TNode.h"
#include "PKB.h"

typedef std::vector<int> vi;
typedef std::set<int> si;
typedef std::set<pair<int, int>> sp_i_i;
typedef std::unordered_map<int, vi> map_i_vi;
typedef std::unordered_map<int, si> map_i_si;
typedef std::unordered_map<int, int> map_i_i;

class Pattern {
private:
	Pattern() {};

	//varIndex | Stmts No. - Facilitate If("X",_,_)
	map_i_vi ifVarStmts;
	map_i_vi whileVarStmts;

	//Stmt No | VarIndex
	map_i_i ifStmtVars;
	map_i_i whileStmtVars;

	// Stmts No. - Facilitate If(s1,_,_)
	//vi ifStmts;
	//vi whileStmts;

	vp_i_i ifPairs;
	vp_i_i whilePairs;


	void error(std::string expected, std::string given);
	std::string getToken();
	void matchTerm();
	void matchOperator();
	void matchExpression();
	void resetExpression(std::string newExpr);

	TNode* createTreeFromExpression(std::string expr);
	void createExpressionTermsFromExpression(std::string expr);

	std::string next_token;
	char buffer;
	int token_index;
	int bracket_term;
	std::string expression_string;
	std::vector<std::string> expression_terms;
	std::stack<int> times_index;
	std::stack<int> bracket_index;
	bool popBracket;

	void generatePatternDataForSingleProcedure(TNode* current, NodeType type);
public:
	static Pattern& getInstance()
	{
		static Pattern instance;
		return instance;
	}

	Pattern(Pattern const&) = delete;
	void operator=(Pattern const&) = delete;

	void generatePatternData(TNode* astRoot);
	vi getPatternAssignGenericSpecific(int varIndex, string expr);
	vi getPatternAssignSpecificGeneric(int stmtNoOfAssign, string expression);
	vp_i_i getPatternAssignGenericGeneric(string expr);

	vi getPatternIfGenericSpecific(int varIndex);
	vi getPatternIfSpecificGeneric(int stmtNoOfIf);
	vp_i_i getPatternIfGenericGeneric();

	vi getPatternWhileGenericSpecific(int varIndex);
	vi getPatternWhileSpecificGeneric(int stmtNoOfWhile);
	vp_i_i getPatternWhileGenericGeneric();

	bool whetherPatternAssign(int assignStmt, int varIndex, string expression);
	bool whetherPatternIf(int ifStmt, int varIndex);
	bool whetherPatternWhile(int whileStmt, int varIndex);

};