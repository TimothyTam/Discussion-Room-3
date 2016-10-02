#pragma once

#include <string>
#include "TNode.h"
#include "CFGNode.h"

void Parse(std::string filename);
bool IsSpecialToken(std::string token);
bool IsValidName(std::string name);
bool IsValidNumber(std::string number);
int GetConstant(std::string constant);
std::string GetToken();
void Error(std::string message);
void Error(std::string expected, std::string given);
void Match(std::string token);
void MatchProcName();
void MatchVarName();
void MatchTerm();
void MatchOperator();
void MatchExpression();
void Program();
void Procedure();
void StatementList(std::string stmtLstName);
void Statement();
void StatementIf();
void StatementWhile();
void StatementCall();
void StatementAssign();
CFGNode* AddStatementToCFG(int statementNumber, NodeType type);