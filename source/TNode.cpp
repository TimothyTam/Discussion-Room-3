#include "TNode.h"
using namespace std;

const char *type_names[] = { "program", "procedure", "stmtlst", "assign", "variable", "plus", "minus", "times", "constant", "while", "if", "call" };

TNode::TNode() {

}

TNode::TNode(NodeType t) {
	this->type = t;
}