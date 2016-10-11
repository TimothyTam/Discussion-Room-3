#pragma once
#include "PKB.h"

using namespace std;
typedef std::pair<std::string, TNode*> vpair;

class DesignExtractor {
private:
	
public:
	static void buildAllTables(TNode* astRoot);
	static void extractStmtBasedOnType(vector<vpair> stmtList);
};