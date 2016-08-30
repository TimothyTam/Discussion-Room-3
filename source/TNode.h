#include <vector>

using namespace std;

enum NodeType { PROGRAM, Procedure, StmtLst, Assign, Variable, Plus, Minus, Times, Constant, While, If, Call };
const char *type_names[] = { "program", "procedure", "stmtlst", "assign", "variable", "plus", "minus", "times", "constant", "while", "if", "call" };

class TNode {
public:
	TNode* parent;
	vector<TNode> childs;
	NodeType type;
};

class EntityNode : public TNode {
public:
	string value;
};

class ConstantNode : public TNode {
public:
	int value;
};