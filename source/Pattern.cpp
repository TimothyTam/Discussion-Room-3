#include "Pattern.h"
#include "Parser.h"
#include "PKB.h"

#include <sstream>
#include <algorithm>


//Split the given string by given delimiter 
//Store parts of the splitted string into given elems parameter
void SplitString(const string &s, char delim, vector<string> &elems) {
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		if (item != "") {
			elems.push_back(item);
		}
	}
}

//Check whether 2 TNode are the same
//This method only checks for binary tree of TNode
//Return a boolean
bool IsSameNode(TNode* root1, TNode* root2) {
	if (root1 == NULL && root2 == NULL) return true;
	if (root1 == NULL || root2 == NULL) return false;
	if (root1->childs.size() == 1 && root2->childs.size() == 1) {
		return (root1->type == root2->type && root1->value == root2->value &&
			IsSameNode(root1->childs.at(0), root2->childs.at(0)));
	} 
	else if (root1->childs.size() == 2 && root2->childs.size() == 2) {
		return (root1->type == root2->type && root1->value == root2->value &&
			IsSameNode(root1->childs.at(0), root2->childs.at(0)) &&
			IsSameNode(root1->childs.at(1), root2->childs.at(1)));
	}
	else if (root1->childs.size() != root2->childs.size()) {
		return false;
	}
	return root1->type == root2->type && root1->value == root2->value;
}

//Check whether 1 TNode is a subtree of another TNode
//This method only checks for binary tree of TNode
//Return a boolean
bool IsSubtree(TNode* tree, TNode* subtree) {
	if (subtree == NULL) return true;
	if (tree == NULL) return false;
	if (IsSameNode(tree, subtree)) return true;
	if (tree->childs.size() == 1) {
		return IsSubtree(tree->childs.at(0), subtree);
	} 
	else if (tree->childs.size() == 2) {
		return IsSubtree(tree->childs.at(0), subtree) ||
			IsSubtree(tree->childs.at(1), subtree);
	}
	return false;
}


void Pattern::error(string expected, string given) {
	throw runtime_error("Invalid pattern: expected '" + expected + "' but given '" + given + "'");
}


string Pattern::getToken() {
	string token = "";
	if (buffer != '\0' && !isspace(buffer)) token += buffer;
	buffer = '\0';
	while (!IsSpecialToken(token) && token_index < expression_string.length()) {
		buffer = expression_string[token_index];
		token_index++;
		if (token.empty() && isspace(buffer)) continue;
		if (!token.empty() && (IsSpecialToken(string(1, buffer)) || isspace(buffer))) break;
		token += buffer;
		buffer = '\0';
	}
	return token;
}


void Pattern::matchTerm() {
	if (IsSpecialToken(next_token)) {
		if (next_token == "(") {
			bracket_term = 0;
			bracket_index.push(expression_terms.size());
			next_token = Pattern::getToken();
			Pattern::matchTerm();
			return;
		}
		else if (next_token == ")") {
			if (bracket_index.empty() || bracket_term == 0) {
				Pattern::error("<var_name> or <constant> or (", next_token);
			}
			else {
				bracket_index.pop();
				if (times_index.size() > bracket_index.size()) {
					times_index.pop();
				}
				next_token = Pattern::getToken();
				return;
			}
		}
		else {
			Pattern::error("<var_name> or <constant> or (", next_token);
		}
	}
	if (next_token == "") return;
	if (!IsValidName(next_token)) {
		int constant = GetConstant(next_token);
		if (constant < 0) {
			Pattern::error("<var_name> or <constant> or (", next_token);
		}
		else {
			bracket_term++;
		}
	}
	else {
		bracket_term++;
	}
	expression_terms.push_back(next_token);
	next_token = Pattern::getToken();
}


void Pattern::matchOperator() {
	if (next_token != "+" && next_token != "-" && next_token != "*") {
		Pattern::error("<operator>", next_token);
	}
	if (next_token == "*") {
		if (times_index.size() <= bracket_index.size()) {
			times_index.push(expression_terms.size() - 1);
		}
		expression_terms.insert(expression_terms.begin() + times_index.top(), next_token);
	}
	else {
		if (!times_index.empty()) {
			times_index.pop();
		}
		if (!bracket_index.empty()) {
			expression_terms.insert(expression_terms.begin() + bracket_index.top(), next_token);
		}
		else {
			expression_terms.insert(expression_terms.begin(), next_token);
		}
	}
	next_token = Pattern::getToken();
}


void Pattern::matchExpression() {
	Pattern::matchTerm();
	if (next_token != "") {
		if (next_token != ")") {
			Pattern::matchOperator();
			bracket_term = 0;
		}
		return Pattern::matchExpression();
	}
	if (!bracket_index.empty()) {
		Pattern::error(")", next_token);
	}
	bracket_term = 0;
}


void Pattern::resetExpression(string newExpr) {
	expression_string = newExpr;
	token_index = 0;
	bracket_term = 0;
	expression_terms.clear();
	times_index.swap(stack<int>());
	bracket_index.swap(stack<int>());
	next_token = Pattern::getToken();
}


TNode* Pattern::createTreeFromExpression(string expr) {
	resetExpression(expr);
	matchExpression();
	stack<TNode*> nodes;
	TNode* result;
	for (auto const& term : expression_terms) {
		if (!nodes.empty() && nodes.top()->childs.size() == 2) {
			result = nodes.top();
			nodes.pop();
		}
		TNode* node;
		if (term == "*") {
			node = new TNode(NodeType::Times);
		} else if (term == "+") {
			node = new TNode(NodeType::Plus);
		} else if (term == "-") {
			node = new TNode(NodeType::Minus);
		} else {
			int number = GetConstant(term);
			if (number >= 0) {
				node = new TNode(NodeType::Constant);
				node->value = number;
			} else {
				node = new TNode(NodeType::Variable);
				node->value = PKB::getInstance().getVarIndexFromName(term);
			}
		}
		if (!nodes.empty()) {
			node->parent = nodes.top();
			nodes.top()->childs.push_back(node);
		}
		nodes.push(node);
	}
	while (!nodes.empty()) {
		result = nodes.top();
		nodes.pop();
	}
	return result;
}


void Pattern::createExpressionTermsFromExpression(string expr) {
	resetExpression(expr);
	matchExpression();
}


vi Pattern::getPatternAssignGenericSpecific(int varIndex, string expression) {
	vi result = {};
	vector<string> tokens;
	bool isSubExpr = false;
	bool isWildCardExpr = false;

	SplitString(expression, '"', tokens);
	string expr = "";
	if (tokens.size() != 1 && tokens.size() != 3) throw std::runtime_error("Invalid pattern");
	if (tokens.size() == 1) {
		expr = tokens[0];
		if (expr == "_") {
			isWildCardExpr = true;
		}
	}
	else if (tokens.size() == 3) {
		if (tokens[0] != "_" || tokens[2] != "_") throw std::runtime_error("Invalid pattern");
		expr = tokens[1];
		isSubExpr = true;
	}

	if (!isWildCardExpr) {	
		createExpressionTermsFromExpression(expr);
	}

	//Go through all Assign Nodes. Check VarIndex. If is sub-expr, check is sub-tree, else check are equal.
	PKB& pkb = PKB::getInstance();

	for (TNode* stmt : pkb.getAllTNodesForStmt(NodeType::Assign)) {
		if (stmt->childs.size() != 2) {
			throw std::runtime_error("One of the assign Nodes does not have 2 child");
		}
		if (stmt->childs[0]->value == varIndex) {
			if (isWildCardExpr) {
				result.push_back(stmt->statementNumber);
			} else if (isSubExpr) {
				vector<string>::iterator pos = search(stmt->expression_terms.begin(), stmt->expression_terms.end(),
					expression_terms.begin(), expression_terms.end());
				if (pos != stmt->expression_terms.end()) {
					result.push_back(stmt->statementNumber);
				}
			} else {
				if (stmt->expression_terms == expression_terms) {
					result.push_back(stmt->statementNumber);
				}
			}
		}
	}

	return result;
}

vi Pattern::getPatternAssignSpecificGeneric(int stmtNo, string expression) {
	vi result = {};
	vector<string> tokens;
	bool isSubExpr = false;
	bool isWildCardExpr = false;

	SplitString(expression, '"', tokens);
	string expr = "";
	if (tokens.size() != 1 && tokens.size() != 3) throw std::runtime_error("Invalid pattern");
	if (tokens.size() == 1) {
		expr = tokens[0];
		if (expr == "_") {
			isWildCardExpr = true;
		}
	}
	else if (tokens.size() == 3) {
		if (tokens[0] != "_" || tokens[2] != "_") throw std::runtime_error("Invalid pattern");
		expr = tokens[1];
		isSubExpr = true;
	}

	if (!isWildCardExpr) {
		createExpressionTermsFromExpression(expr);
	}

	//Go through all Assign Nodes. Check VarIndex. If is sub-expr, check is sub-tree, else check are equal.
	PKB& pkb = PKB::getInstance();

	for (TNode* stmt : pkb.getAllTNodesForStmt(NodeType::Assign)) {
		if (stmt->childs.size() != 2) {
			throw std::runtime_error("One of the assign Nodes does not have 2 child");
		}
		if (stmt->statementNumber == stmtNo) {
			if (isWildCardExpr) {
				result.push_back(stmt->statementNumber);
			}
			else if (isSubExpr) {
				vector<string>::iterator pos = search(stmt->expression_terms.begin(), stmt->expression_terms.end(),
					expression_terms.begin(), expression_terms.end());
				if (pos != stmt->expression_terms.end()) {
					result.push_back(stmt->statementNumber);
				}
			}
			else {
				if (stmt->expression_terms == expression_terms) {
					result.push_back(stmt->statementNumber);
				}
			}
		}
	}

	return result;
}

vp_i_i Pattern::getPatternAssignGenericGeneric(string expression) {
	vp_i_i result;
	vector<string> tokens;
	bool isSubExpr = false;
	bool isWildCardExpr = false;

	SplitString(expression, '"', tokens);
	string expr = "";
	if (tokens.size() != 1 && tokens.size() != 3) throw std::runtime_error("Invalid pattern");
	if (tokens.size() == 1) {
		expr = tokens[0];
		if (expr == "_") {
			isWildCardExpr = true;
		}
	}
	else if (tokens.size() == 3) {
		if (tokens[0] != "_" || tokens[2] != "_") throw std::runtime_error("Invalid pattern");
		expr = tokens[1];
		isSubExpr = true;
	}

	if (!isWildCardExpr) {
		createExpressionTermsFromExpression(expr);
	}

	//Go through all Assign Nodes. If is sub-expr, check is sub-tree, else check are equal.
	PKB& pkb = PKB::getInstance();

	for (TNode* stmt : pkb.getAllTNodesForStmt(NodeType::Assign)) {
		if (stmt->childs.size() != 2) {
			throw std::runtime_error("One of the assign Nodes does not have 2 child");
		}

		if (isWildCardExpr) {
			result.push_back(make_pair(stmt->statementNumber, stmt->childs[0]->value));
		}
		else if (isSubExpr) {
			vector<string>::iterator pos = search(stmt->expression_terms.begin(), stmt->expression_terms.end(),
				expression_terms.begin(), expression_terms.end());
			if (pos != stmt->expression_terms.end()) {
				result.push_back(make_pair(stmt->statementNumber,stmt->childs[0]->value));
			}
		}
		else {
			if (stmt->expression_terms == expression_terms) {
				result.push_back(make_pair(stmt->statementNumber, stmt->childs[0]->value));
			}
		}
		
	}

	return result;
}

void Pattern::generatePatternData(TNode* astRoot) {
	if (astRoot->type != NodeType::Program) {
		throw runtime_error("Generate Pattern data failed. Only accepts AST Root");
		return;
	}
	for (TNode* proc : astRoot->childs) {
		generatePatternDataForSingleProcedure(proc, NodeType::If);
		generatePatternDataForSingleProcedure(proc, NodeType::While);
	}
}

void Pattern::generatePatternDataForSingleProcedure(TNode* current, NodeType type) {
	if (current->type == type) {
		generatePatternDataForSingleProcedure(current->childs.at(1), type);
		if (type == NodeType::If) {
			generatePatternDataForSingleProcedure(current->childs.at(2), type);
			ifVarStmts[current->childs[0]->value].push_back(current->statementNumber);
			ifPairs.push_back(make_pair(current->statementNumber, current->childs[0]->value));
			ifStmtVars[current->statementNumber] = current->childs[0]->value;
		}
		else if (type == NodeType::While) {
			whileVarStmts[current->childs[0]->value].push_back(current->statementNumber);
			whilePairs.push_back(make_pair(current->statementNumber, current->childs[0]->value));
			whileStmtVars[current->statementNumber] = current->childs[0]->value;
		}
			
	}
	else {
		//Go to each child and carry on.
		for (TNode* child : current->childs) {
			generatePatternDataForSingleProcedure(child, type);
		}
	}
	return;
}

vi Pattern::getPatternIfGenericSpecific(int varIndex) {
	//Intended brackets.
	return ifVarStmts[varIndex];
}

vi Pattern::getPatternWhileGenericSpecific(int varIndex) {
	//Intended brackets.
	return whileVarStmts[varIndex];
}

vp_i_i Pattern::getPatternIfGenericGeneric() {
	return ifPairs;
}

vp_i_i Pattern::getPatternWhileGenericGeneric() {
	return whilePairs;
}

bool Pattern::whetherPatternAssign(int assignStmt, int varIndex, string expression) {
	vector<string> tokens;
	bool isSubExpr = false;
	bool isWildCardExpr = false;

	SplitString(expression, '"', tokens);
	string expr = "";
	if (tokens.size() != 1 && tokens.size() != 3) throw std::runtime_error("Invalid pattern");
	if (tokens.size() == 1) {
		expr = tokens[0];
		if (expr == "_") {
			isWildCardExpr = true;
		}
	}
	else if (tokens.size() == 3) {
		if (tokens[0] != "_" || tokens[2] != "_") throw std::runtime_error("Invalid pattern");
		expr = tokens[1];
		isSubExpr = true;
	}

	if (!isWildCardExpr) {
		createExpressionTermsFromExpression(expr);
	}

	//Go to the Assign Nodes Check VarIndex. If is sub-expr, check is sub-tree, else check are equal.
	PKB& pkb = PKB::getInstance();
	TNode* stmt = pkb.getStmt(assignStmt).second;


	if (stmt->type != NodeType::Assign) {
		return false;
	}

	if (stmt->childs.size() != 2) {
		throw std::runtime_error("One of the assign Nodes does not have 2 child");
	}

	if (isWildCardExpr) {
		return true;
	}
	else if (isSubExpr) {
		vector<string>::iterator pos = search(stmt->expression_terms.begin(), stmt->expression_terms.end(),
			expression_terms.begin(), expression_terms.end());
		if (pos != stmt->expression_terms.end()) {
			return true;
		}
	}
	else {
		if (stmt->expression_terms == expression_terms) {
			return true;
		}
	}
	return false;
}

vi Pattern::getPatternIfSpecificGeneric(int stmtNoOfIf) {
	vi result;
	if (ifStmtVars.count(stmtNoOfIf) == 1) result.push_back(ifStmtVars[stmtNoOfIf]);
	return result;
}

vi Pattern::getPatternWhileSpecificGeneric(int stmtNoOfWhile) {
	vi result;
	if (whileStmtVars.count(stmtNoOfWhile) == 1) result.push_back(whileStmtVars[stmtNoOfWhile]);
	return result;
}

bool Pattern::whetherPatternIf(int ifStmt, int varIndex) {
	return ifStmtVars.count(ifStmt) == 1 ? ifStmtVars[ifStmt] == varIndex : false;
}
bool Pattern::whetherPatternWhile(int whileStmt, int varIndex) {
	return whileStmtVars.count(whileStmt) == 1 ? whileStmtVars[whileStmt] == varIndex : false;
}