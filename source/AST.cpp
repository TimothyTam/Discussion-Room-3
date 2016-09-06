#include "AST.h"
#include "../source/PKB.h"
#include "../source/TNode.h"
#include "stdio.h"

using namespace std;

AST::AST() {
	AST::rootNode = NULL;
}


TNode* AST::createEntityNode(TNode* parent, NodeType type, string value) {
	if (parent == NULL) {
		// this should be a program node, and will be set to rootNode;
		
		//check for wrong use cases just in case
		if (AST::rootNode != NULL) {
			printf("Adding a node with non-NULL parent to empty AST !!!!!??");
		}
		else if (type != NodeType::Program) {
			printf("Adding non-program node to empty AST ????!!!! ");
		}

		else {
			TNode newNode = TNode(NodeType::Program);
			rootNode = &newNode;
			return &newNode;
		}
		return NULL;
	}

	TNode newNode = TNode(type);
	newNode.parent = parent;

	parent->childs.push_back(&newNode);

	if (type == NodeType::Variable) {
		// insert new variable if not stored yet
		int varIndex = PKB::getInstance().storeVariable(value);
		newNode.value = varIndex;
	}
	else if (type == NodeType::Procedure) {
		int procIndex = PKB::getInstance().addProcedure(value);
		newNode.value = procIndex;
	}

	return &newNode;

}

TNode* AST::createConstantNode(TNode* parent, NodeType type, int value) {
	if (parent == NULL || type != NodeType::Constant) {
		printf("ERROR ! Either parent is Null or its not a constant type");
		return NULL;
	}

	TNode newNode = TNode(NodeType::Constant);
	newNode.value = value;
	newNode.parent = parent;
	parent->childs.push_back(&newNode);
	return &newNode;
}
