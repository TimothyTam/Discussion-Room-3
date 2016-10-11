#include "CFGNode.h"

using namespace std;

CFGNode::CFGNode(int sN, NodeType t) {
	CFGNode::statementNumber = sN;
	CFGNode::type = t;
	CFGNode::isEnd = false;
}