#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#include "PKB.h"
#include "TNode.h"

int PKB::setProcToAST(PROC p, TNode* r) {
	return NULL;
}

TNode* PKB::getRootAST(PROC p) {
	return NULL;
}

TNode* PKB::createEntityNode(TNode* parent, NodeType type, string value) {
	return NULL;
}

TNode* PKB::createConstantNode(TNode* parent, NodeType type, int value) {
	return NULL;
}

int PKB::addStatement(string statement, TNode* node) {
	return 0;
}
