#pragma once

#include <vector>

enum NodeType { PROGRAM, Procedure, StmtLst, Assign, Variable, Plus, Minus, Times, Constant, While, If, Call };
const char *type_names[] = { "program", "procedure", "stmtlst", "assign", "variable", "plus", "minus", "times", "constant", "while", "if", "call" };

class TNode {
public:
	TNode* parent;
	std::vector<TNode> childs;
	NodeType type;
	int value;
};