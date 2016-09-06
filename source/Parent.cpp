#include "Parent.h"
#include "TNode.h"
#include "vector"
#include "PKB.h"


using namespace std;
Parent::Parent()
{
}

void Parent::buildFromNode(TNode* currentNode) {
	vector<TNode*> childs = currentNode->childs;
	

	if (currentNode->type == NodeType::If || currentNode->type == NodeType::While) {

		vector<int> childsOfThisNode = vector<int>();

		for (int i = 1; i < childs.size(); i++) {
			TNode* stmtLst = childs[i];
			//now lets add all the childs of this stmtLst as childs of currentNode;
			for (int j = 0; j < stmtLst->childs.size(); j++) {
				TNode* currentChild = stmtLst->childs[j];

				this->parentOfStmt[currentChild->statementNumber] = currentNode->statementNumber;
				childsOfThisNode.push_back(currentChild->statementNumber);
			}
		}

		this->childOfStmt[currentNode->statementNumber] = childsOfThisNode;
	}

	for (int i = 0; i < childs.size(); i++) {
		buildFromNode(childs[i]);
	}
}


void Parent::generateParentData(TNode* rootNode) {
	buildFromNode(rootNode);
	buildFromNode();
}

vi Parent::buildTransitiveFromStmt(int currentStmt, vector<bool>* done) {
	// if already done building from here, return the result in transitiveChildOf
	if (done->at(currentStmt)) return this->transitiveChildOfStmt[currentStmt];

	// else, build the transitive childs from here

	//initialise to the list of direct childs;
	vi childsOfCurrentStmt = this->childOfStmt[currentStmt];
	this->transitiveChildOfStmt[currentStmt] = vi(childsOfCurrentStmt);
	
	for (int i = 0; i < childsOfCurrentStmt.size(); i++) {
		if (this->childOfStmt.count(childsOfCurrentStmt[i]) == 0) continue;
		vi newChilds = this->buildTransitiveFromStmt(childsOfCurrentStmt[i], done);
		this->childOfStmt[currentStmt].insert(this->childOfStmt[currentStmt].end(), newChilds.begin(), newChilds.end());
	}

	done->at(currentStmt) = true;
	

}

void Parent::buildTransitiveData() {
	const int maxStmt = PKB::getInstance().getStmtCount();
	vector<bool> done;
	done.assign(maxStmt + 10, false);

	//iterate through the childOfStmt map, build from each parent node
	for (map_i_vi::iterator it = this->childOfStmt.begin(); it != this->childOfStmt.end(); ++it) {
		this->buildTransitiveFromStmt(it->first, &done);
	}
	


}
