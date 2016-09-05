#pragma once

#include <stdio.h>
#include <fstream>
#include <string>
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

string next_token;
char buffer;
ifstream source;
int bracket_count;
int bracket_term;

int Parse(string filename);
bool IsSpecialToken(string token);
bool IsValidName(string name);
bool IsValidNumber(string number);
int GetConstant(string constant);
string GetToken();
void Error(string expected, string given);
int Match(string token);
int MatchProcName();
int MatchVarName();
int MatchTerm();
int MatchOperator();
int MatchExpression();
int Program();
int Procedure();
int StatementList();
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
			next_token = GetToken();
			return MatchTerm();
		} else if (next_token == kRB) {
			if (bracket_count == 0 || bracket_term == 0) {
				Error("<var_name> or <constant> or (", next_token);
				return 1;
			} else {
				bracket_count--;
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
	next_token = GetToken();
	return 0;
}

int MatchOperator() {
	if (next_token != kPlus && next_token != kMinus && next_token != kTimes) {
		Error("<operator>", next_token);
		return 1;
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
	next_token = GetToken();
	return Procedure();
}

int Procedure() {
	if (Match(kProcedure) != 0) return 1;
	if (MatchProcName() != 0) return 1;
	if (Match(kSB) != 0) return 1;
	if (StatementList() != 0) return 1;
	if (Match(kEB) != 0) return 1;
	return 0;
}

int StatementList() {
	if (Statement() != 0) return 1;
	if (Match(kEOS) != 0) return 1;
	if (next_token != kEB) {
		return StatementList();
	}
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
	if (MatchVarName() != 0) return 1;
	if (Match(kThen) != 0) return 1;
	if (Match(kSB) != 0) return 1;
	if (StatementList() != 0) return 1;
	if (Match(kEB) != 0) return 1;
	if (Match(kElse) != 0) return 1;
	if (Match(kSB) != 0) return 1;
	if (StatementList() != 0) return 1;
	if (Match(kEB) != 0) return 1;
	return 0;
}

int StatementWhile() {
	if (Match(kWhile) != 0) return 1;
	if (MatchVarName() != 0) return 1;
	if (Match(kSB) != 0) return 1;
	if (StatementList() != 0) return 1;
	if (Match(kEB) != 0) return 1;
	return 0;
}

int StatementCall() {
	if (Match(kCall) != 0) return 1;
	if (MatchProcName() != 0) return 1;
	return 0;
}

int StatementAssign() {
	if (MatchVarName() != 0) return 1;
	if (Match(kEQL) != 0) return 1;
	if (MatchExpression() != 0) return 1;
	return 0;
}
