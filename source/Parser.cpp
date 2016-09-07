#pragma once

#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <set>
#include <vector>

using namespace std;

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

int Parse(string filename);
bool IsSpecialToken(string token);
bool IsValidName(string name);
bool IsValidNumber(string number);
int GetConstant(string constant);
string GetToken();
void Error(string message);
void Error(string expected, string given);
int Match(string token);
int MatchProcName();
int MatchVarName();
int MatchTerm();
int MatchOperator();
int MatchExpression();
int Program();
int Procedure();
int StatementList(string stmtLstName);
int Statement();
int StatementIf();
int StatementWhile();
int StatementCall();
int StatementAssign();

int Parse(string filename) {
	source.open(filename);
	return Program();
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
	return !name.empty() && !isdigit(name[0]);
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
	cout << "Error parsing Simple source: " << message << "\n";
}

void Error(string expected, string given) {
	cout << "Error parsing Simple source: expected '" << expected << "' but given '" << given << "'\n";
}

int Match(string token) {
	if (next_token != token) {
		Error(token, next_token);
		return 1;
	}
	next_token = GetToken();
	return 0;
}

int MatchProcName() {
	if (IsSpecialToken(next_token) || !IsValidName(next_token)) {
		Error("<proc_name>", next_token);
		return 1;
	}
	next_token = GetToken();
	return 0;
}

int MatchVarName() {
	if (IsSpecialToken(next_token) || !IsValidName(next_token)) {
		Error("<var_name>", next_token);
		return 1;
	}
	next_token = GetToken();
	return 0;
}

int MatchTerm() {
	if (IsSpecialToken(next_token)) {
		if (next_token == kLB) {
			bracket_count++;
			bracket_term = 0;
			bracket_index.push(expression_terms.size());
			next_token = GetToken();
			return MatchTerm();
		} else if (next_token == kRB) {
			if (bracket_count == 0 || bracket_term == 0) {
				Error("<var_name> or <constant> or (", next_token);
				return 1;
			} else {
				bracket_count--;
				bracket_index.pop();
				if (times_index.size() > bracket_index.size()) {
					times_index.pop();
				}
				next_token = GetToken();
				return 0;
			}
		} else {
			Error("<var_name> or <constant> or (", next_token);
			return 1;
		}
	}
	if (!IsValidName(next_token)) {
		int constant = GetConstant(next_token);
		if (constant < 0) {
			Error("<var_name> or <constant> or (", next_token);
			return 1;
		} else {
			bracket_term++;
		}
	} else {
		bracket_term++;
	}
	expression_terms.push_back(next_token);
	next_token = GetToken();
	return 0;
}

int MatchOperator() {
	if (next_token != kPlus && next_token != kMinus && next_token != kTimes) {
		Error("<operator>", next_token);
		return 1;
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
	return 0;
}

int MatchExpression() {
	if (MatchTerm() != 0) return 1;
	if (next_token != kEOS) {
		if (next_token != kRB) {
			if (MatchOperator() != 0) return 1;
			bracket_term = 0;
		}
		return MatchExpression();
	}
	if (bracket_count > 0) {
		Error(")", next_token);
		return 1;
	}
	bracket_term = 0;
	return 0;
}

int Program() {
	nodes.push(PKB::getInstance().createEntityNode(NULL, NodeType::Program, ""));
	next_token = GetToken();
	return Procedure();
}

int Procedure() {
	if (Match(kProcedure) != 0) return 1;
	string procName = next_token;
	if (proc_names.find(procName) != proc_names.end()) {
		Error("Duplicate procedure name " + procName);
		return 1;
	}
	if (MatchProcName() != 0) return 1;
	proc_names.insert(procName);
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Procedure, procName));
	if (Match(kSB) != 0) return 1;
	if (StatementList("") != 0) return 1;
	if (Match(kEB) != 0) return 1;
	nodes.pop();
	return 0;
}

int StatementList(string stmtLstName) {
	if (nodes.top()->type != NodeType::StmtLst) {
		nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::StmtLst, stmtLstName));
	}
	if (Statement() != 0) return 1;
	if (Match(kEOS) != 0) return 1;
	if (next_token != kEB) {
		return StatementList(stmtLstName);
	}
	nodes.pop();
	return 0;
}

int Statement() {
	if (next_token == kIf) return StatementIf();
	if (next_token == kWhile) return StatementWhile();
	if (next_token == kCall) return StatementCall();
	return StatementAssign();
}

int StatementIf() {
	if (Match(kIf) != 0) return 1;
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::If, ""));
	string varName = next_token;
	if (MatchVarName() != 0) return 1;
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, varName);
	if (Match(kThen) != 0) return 1;
	if (Match(kSB) != 0) return 1;
	if (StatementList("then") != 0) return 1;
	if (Match(kEB) != 0) return 1;
	if (Match(kElse) != 0) return 1;
	if (Match(kSB) != 0) return 1;
	if (StatementList("else") != 0) return 1;
	if (Match(kEB) != 0) return 1;
	nodes.pop();
	return 0;
}

int StatementWhile() {
	if (Match(kWhile) != 0) return 1;
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::While, ""));
	string varName = next_token;
	if (MatchVarName() != 0) return 1;
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, varName);
	if (Match(kSB) != 0) return 1;
	if (StatementList("") != 0) return 1;
	if (Match(kEB) != 0) return 1;
	nodes.pop();
	return 0;
}

int StatementCall() {
	if (Match(kCall) != 0) return 1;
	string procName = next_token;
	if (MatchProcName() != 0) return 1;
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Call, procName);
	return 0;
}

int StatementAssign() {
	string varName = next_token;
	if (MatchVarName() != 0) return 1;
	if (Match(kEQL) != 0) return 1;
	nodes.push(PKB::getInstance().createEntityNode(nodes.top(), NodeType::Assign, ""));
	PKB::getInstance().createEntityNode(nodes.top(), NodeType::Variable, varName);
	expression_terms.clear();
	if (MatchExpression() != 0) return 1;
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
	return 0;
}
