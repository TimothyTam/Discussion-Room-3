#pragma once
#include "Call.h"

si Call::generateCallTableForSingleProcedure(TNode* current, int procedure) {
	std::vector<TNode*> childs = current->childs;
	si result;
	try {
		if (current->type == NodeType::Call) {
			result.insert(current->value);
		}
		else if (current->type == NodeType::Program) {
			for (TNode* child : childs) {
				generateCallTableForSingleProcedure(child, procedure);
			}
		}
		else if (current->type == NodeType::Procedure) {
			generateCallTable(childs.at(0));
		}

		else if (current->type == NodeType::StmtLst) {

		}
		else if (current->type == NodeType::While) {
			generateFollowTable(childs.at(1));
			return;
		}
		else if (current->type == NodeType::If) {
			generateFollowTable(childs.at(1)); // Then Stmt
			generateFollowTable(childs.at(2)); // Else Stmt
			return;
		}
	}
	catch (const std::out_of_range& oor) {
		std::cerr << "Out of Range error: " << oor.what() << '\n';
		throw runtime_error("Error Generating Follow Table" + string(oor.what()));
	}
}

void Call::generateCallTable(TNode* current) {
	std::vector<TNode*> childs = current->childs;
	if (current->type == NodeType::Program) {
		for (TNode* child : childs) {
			generateCallTableForSingleProcedure(child, child->value);
		}
	}
}

void Call::addCallValueToCallNodes(TNode* astRoot) {

}
