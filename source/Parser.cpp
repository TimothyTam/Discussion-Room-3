#pragma once

#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <vector>

using namespace std;

#include "Parser.h"
#include "PKB.h"
#include "TNode.h"

const string kProcedure = "procedure";
const string kCall = "call";
const string kIf = "if";
const string kElse = "else";
const string kThen = "then";
const string kWhile = "while";

const string kSB = "{";
const string kEB = "}";
const string kLB = "(";
const string kRB = ")";
const string kEQL = "=";
const string kPlus = "+";
const string kMinus = "-";
const string kTimes = "*";
const string kEOS = ";";

// for reading source
string next_token;
char buffer;
ifstream source;

// for checking duplicate procedure
set<string> proc_names;

// for tracking AST nodes
stack<TNode*> nodes;

// for parsing expression
int bracket_count;
int bracket_term;

// for creating expression AST
vector<string> expression_terms;
stack<int> times_index;
stack<int> bracket_index;

void Parse(string filename) {
	source.open(filename);
	Program();
}

bool IsSpecialToken(string symbol) {
	return symbol == kSB
		|| symbol == kEB
		|| symbol == kLB
		|| symbol == kRB
		|| symbol == kEQL
		|| symbol == kPlus
		|| symbol == kMinus
		|| symbol == kTimes
		|| symbol == kEOS;
}

bool IsValidName(string name) {
	if (name.empty() || isdigit(name[0])) return false;
	string::const_iterator it = name.begin();
	while (it != name.end() && isalnum(*it)) ++it;
	return it == name.end();
}

bool IsValidNumber(string number) {
	if (number.length() > 1 && number[0] == '0') return false;
	string::const_iterator it = number.begin();
	while (it != number.end() && isdigit(*it)) ++it;
	return !number.empty() && it == number.end();
}

int GetConstant(string constant) {
	if (!IsValidNumber(constant)) return -1;
	try {
		return stoi(constant);
	} catch (...) {
		return -1;
	}
}

string GetToken() {
	string token = "";
	if (buffer != '\0' && !isspace(buffer)) token += buffer;
	buffer = '\0';
	while (!IsSpecialToken(token) && source.get(buffer)) {
		if (token.empty() && isspace(buffer)) continue;
		if (!token.empty() && (IsSpecialToken(string(1, buffer)) || isspace(buffer))) break;
		token += buffer;
		buffer = '\0';
	}
	cout << token << "\n";
	return token;
}

void Error(string message) {
	throw runtime_error("Error parsing Simple source: " + message);
}

void Error(string expected, string given) {
	throw runtime_error("Error parsing Simple source: expected '" + expected + "' but given '" + given + "'");
}

void Match(string token) {
	if (next_token != token) {
		Error(token, next_token);
	}
	next_token = GetToken();
}

void MatchProcName() {
	if (IsSpecialToken(next_token) || !IsValidName(next_token)) {
		Error("<proc_name>", next_token);
	}
	next_token = GetToken();
}

void MatchVarName() {
	if (IsSpecialToken(next_token) || !IsValidName(next_token)) {
		Error("<var_name>", next_token);
	}
	next_token = GetToken();
}

void MatchTerm() {
	if (IsSpecialToken(next_token)) {
		if (next_token == kLB) {
			bracket_count++;
			bracket_term = 0;
			bracket_index.push(expression_terms.size());
			next_token = GetToken();
			MatchTerm();
			return;
		} else if (next_token == kRB) {
			if (bracket_count == 0 || bracket_term == 0) {
				Error("<var_name> or <constant> or (", next_token);
			} else {
				bracket_count--;
				bracket_index.pop();
				if (times_index.size() > bracket_index.size()) {
					times_index.pop();
				}
				next_token = GetToken();
				return;
			}
		} else {
			Error("<var_name> or <constant> or (", next_token);
		}
	}
	if (!IsValidName(next_token)) {
		int constant = GetConstant(next_token);
		if (constant < 0) {
			Error("<var_name> or <constant> or (", next_token);
		} else {
			bracket_term++;
		}
	} else {
		bracket_term++;
	}
	expression_terms.push_back(next_token);
	next_token = GetToken();
}

void MatchOperator() {
	if (next_token != kPlus && next_token != kMinus && next_token != kTimes) {
		Error("<operator>", next_token);
	}
	if (next_token == kTimes) {
		if (times_index.size() <= bracket_index.size()) {
			times_index.push(expression_terms.size() - 1);
		}
		expression_terms.insert(expression_terms.begin() + times_index.top(), next_token);
	} else {
		if (!times_index.empty()) {
			times_index.pop();
		}
		if (!bracket_index.empty()) {
			expression_terms.insert(expression_terms.begin() + bracket_index.top(), next_token);
		} else {
			expression_terms.insert(expression_terms.begin(), next_token);
		}
	}
	next_token = GetToken();
}

void MatchExpression() {
	MatchTerm();
	if (next_token != kEOS) {
		if (next_token != kRB) {
			MatchOperator();
			bracket_term = 0;
		}
		MatchExpression();
		return;
	}
	if (bracket_count > 0) {
		Error(")", next_token);
	}
	bracket_term = 0;
}

void Program() {
	nodes.push(PKB::getInstance().createEntityNode(NULL, NodeType::Program, ""));
	next_token = GetToken();
	Procedure();
}

void Procedure() {
	Match(kProcedure);
	string procName = next_token;
	if (proc_names.find(procName) != proc_names.end()) {
		Error("Duplicate procedure name " + procName);
	}
	MatchProcName();
	proc_names.insert(procName);
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Procedure, procName));
	Match(kSB);
	StatementList("");
	Match(kEB);
	nodes.pop();
}

void StatementList(string stmtLstName) {
	if (nodes.top()->type != NodeType::StmtLst) {
		nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::StmtLst, stmtLstName));
	}
	Statement();
	Match(kEOS);
	if (next_token != kEB) {
		StatementList(stmtLstName);
	}
	nodes.pop();
}

void Statement() {
	if (next_token == kIf) StatementIf();
	else if (next_token == kWhile) StatementWhile();
	else if (next_token == kCall) StatementCall();
	else StatementAssign();
}

void StatementIf() {
	Match(kIf);
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::If, ""));
	string varName = next_token;
	MatchVarName();
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, varName);
	Match(kThen);
	Match(kSB);
	StatementList("then");
	Match(kEB);
	Match(kElse);
	Match(kSB);
	StatementList("else");
	Match(kEB);
	nodes.pop();
}

void StatementWhile() {
	Match(kWhile);
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::While, ""));
	string varName = next_token;
	MatchVarName();
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, varName);
	Match(kSB);
	StatementList("");
	Match(kEB);
	nodes.pop();
}

void StatementCall() {
	Match(kCall);
	string procName = next_token;
	MatchProcName();
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Call, procName);
}

void StatementAssign() {
	string varName = next_token;
	MatchVarName();
	Match(kEQL);
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Assign, ""));
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, varName);
	expression_terms.clear();
	MatchExpression();
	for (auto const& term : expression_terms) {
		if (nodes.top()->childs.size() == 2) nodes.pop();
		if (term == kTimes) {
			nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Times, ""));
		} else if (term == kPlus) {
			nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Plus, ""));
		} else if (term == kMinus) {
			nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Minus, ""));
		} else {
			int number = GetConstant(term);
			if (number >= 0) {
				PKB::getInstance().createConstantNode(nodes.top(), NodeType::Constant, number);
			} else {
				PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, term);
			}
		}
	}
	if (nodes.top()->type != NodeType::Assign) nodes.pop();
	expression_terms.clear();
	nodes.pop();
}
