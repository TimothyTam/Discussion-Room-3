#pragma once

#include <string>

int Parse(std::string filename);
bool IsSpecialToken(std::string token);
bool IsValidName(std::string name);
bool IsValidNumber(std::string number);
int GetConstant(std::string constant);
std::string GetToken();
void Error(std::string message);
void Error(std::string expected, std::string given);
int Match(std::string token);
int MatchProcName();
int MatchVarName();
int MatchTerm();
int MatchOperator();
int MatchExpression();
int Program();
int Procedure();
int StatementList(std::string stmtLstName);
int Statement();
int StatementIf();
int StatementWhile();
int StatementCall();
int StatementAssign();