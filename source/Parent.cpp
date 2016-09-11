#include "Parent.h"
#include "TNode.h"
#include "vector"
#include "PKB.h"


using namespace std;
Parent::Parent()
{
}

void Parent::buildFromNode(TNode* currentNode) {
	vector<TNode*>* childs = &(currentNode->childs);
	

	if (currentNode->type == NodeType::If || currentNode->type == NodeType::While) {

		vector<int> childsOfThisNode = vector<int>();

		for (unsigned i = 1; i < childs->size(); i++) {
			TNode* stmtLst = childs->at(i);
			//now lets add all the childs of this stmtLst as childs of currentNode;
			for (unsigned j = 0; j < stmtLst->childs.size(); j++) {
				TNode* currentChild = stmtLst->childs[j];

				parentOfStmt[currentChild->statementNumber] = currentNode->statementNumber;
				childsOfThisNode.push_back(currentChild->statementNumber);
			}
		}

		childOfStmt[currentNode->statementNumber] = childsOfThisNode;
	}

	for (unsigned i = 0; i < childs->size(); i++) {
		buildFromNode(childs->at(i));
	}
}


vi Parent::getChildOfStmt(int lineNo, NodeType type) {
	if (childOfStmt.count(lineNo) == 0) {
		return vi();
	}
	else if (type == NodeType::StmtLst) {
		return childOfStmt[lineNo];
	} 
	//Loop through childOfStmt[lineNo] and check type.
	vi result;
	PKB& pkb = PKB::getInstance();
	for (int stmt : childOfStmt[lineNo]) {
		if (pkb.getStmt(stmt).second->type == type) {
			result.push_back(stmt);
		}
	}
	return result;
}

int Parent::getParentOfStmt(int lineNo, NodeType type) {
	
	if (parentOfStmt.count(lineNo) == 1) {
		if (type == NodeType::StmtLst ||
			PKB::getInstance().getStmt(parentOfStmt[lineNo]).second->type == type) {
			return parentOfStmt[lineNo];
		}
	}

	return -1;

}

vi Parent::getTransitiveParentOfStmt(int lineNo, NodeType type) {

	if (transitiveParentOfStmt.count(lineNo) == 0) {
		return vi();
	}
	else if (type == NodeType::StmtLst) {
		return transitiveParentOfStmt[lineNo];
	}

	vi result;
	
	for (int parent : transitiveParentOfStmt[lineNo]) {
		if (PKB::getInstance().getStmt(parent).second->type == type) {
			result.push_back(parent);
		}
	}
	return result;
}

bool Parent::whetherTransitiveParent(int lineNo, int lineNo2)
{
	if (transitiveParentOfStmt.count(lineNo2) == 0) return false;
	return find(transitiveParentOfStmt[lineNo2].begin(), transitiveParentOfStmt[lineNo2].end(), lineNo) != transitiveParentOfStmt[lineNo2].end();
}

bool Parent::whetherParent(int lineNo, int lineNo2) {
	if (parentOfStmt.count(lineNo2) == 0) {
		return false;
	}
	return parentOfStmt[lineNo2] == lineNo;
}

vi Parent::getTransitiveChildOfStmt(int lineNo, NodeType type)
{
	if (transitiveChildOfStmt.count(lineNo) == 0) {
		return vi();
	}
	else if (type == NodeType::StmtLst) {
		return transitiveChildOfStmt[lineNo];
	}

	vi result;

	for (int child : transitiveChildOfStmt[lineNo]) {
		if (PKB::getInstance().getStmt(child).second->type == type) {
			result.push_back(child);
		}
	}

	return result;
}

void Parent::generateParentData(TNode* rootNode) {
	buildFromNode(rootNode);
	//buildTransitiveData();
}

vi Parent::buildTransitiveFromStmt(int currentStmt, vector<bool>* done) {
	// if already done building from here, return the result in transitiveChildOf
	if (done->at(currentStmt)) return transitiveChildOfStmt[currentStmt];

	// else, build the transitive childs from here

	//initialise to the list of direct childs;
	vi childsOfCurrentStmt = childOfStmt[currentStmt];
	transitiveChildOfStmt[currentStmt] = vi(childsOfCurrentStmt);
	
	for (unsigned i = 0; i < childsOfCurrentStmt.size(); i++) {
		//for each child of the CurrentStatement, if the child has children also, then add those grandchildren to the club

		if (childOfStmt.count(childsOfCurrentStmt[i]) == 0) continue;
		vi grandChilds = buildTransitiveFromStmt(childsOfCurrentStmt[i], done);

		//add grandchildren
		transitiveChildOfStmt[currentStmt].insert(transitiveChildOfStmt[currentStmt].end(), grandChilds.begin(), grandChilds.end());
	}

	//mark this statement as done
	done->at(currentStmt) = true;
	
	//now we got all the grandchildren, let populate the respective transitiveParentOfStmt entries
	for (unsigned i = 0; i < transitiveChildOfStmt[currentStmt].size(); i++) {
		int grandChild = transitiveChildOfStmt[currentStmt][i];
		if (transitiveParentOfStmt.count(grandChild) == 0) {
			transitiveParentOfStmt[grandChild] = vi();
		}
		transitiveParentOfStmt[grandChild].push_back(currentStmt);
	}

	return transitiveChildOfStmt[currentStmt];
}

void Parent::buildTransitiveData() {
	const int maxStmt = PKB::getInstance().getStmtCount();
	vector<bool> done;
	done.assign(maxStmt + 10, false);

	//iterate through the childOfStmt map, build from each parent node
	for (map_i_vi::iterator it = childOfStmt.begin(); it != childOfStmt.end(); ++it) {
		vi dummy = buildTransitiveFromStmt(it->first, &done);
	}
}
