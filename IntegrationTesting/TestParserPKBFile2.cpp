#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
typedef std::vector<std::pair<int, int>> vp_i_i;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(IntTest2)
	{
	public:

		//I think I need to separate this.
		TEST_METHOD(TestParserPKBFile2)
		{
			try {
				Parse("..\\IntegrationTesting\\Integration Testing Files\\IntTestingFile2.txt");
				Logger::WriteMessage("Parse ended");
			}
			catch (std::exception& ex) {
				//Invalid parsing should be done in Unit test/System test. Because I lazy.
				Logger::WriteMessage(ex.what());
				return;
			}

			PKB& pkb = PKB::getInstance();


			for (int i = 0; i < 12; i++) {
				vi stmts = pkb.getNextSpecificGeneric(i, NodeType::StmtLst);
				printVec(stmts);
			}

			for (int i = 0; i < 11; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::StmtLst);

				//printVec(stmts);
			}

		}

		bool checkVectorEqual(vi v1, vi v2) {
			if (v1.size() != v2.size()) return false;
			size_t i;
			for (i = 0; i < v1.size(); i++) {
				if (v1[0] != v2[0]) return false;
			}
			return true;
		}

		bool checkStmtPairsEqual(vp_i_i pair1, vp_i_i pair2) {
			if (pair1.size() != pair2.size()) return false;
			for (int i = 0; i < pair1.size(); i++) {
				if (pair1[i].first != pair2[i].first || pair1[i].second != pair2[i].second) {
					return false;
				}
			}
			return true;
		}

		void printStmtPairs(vp_i_i vp) {
			for (pair<int, int> i : vp) {
				Logger::WriteMessage(to_string(i.first).c_str());
				Logger::WriteMessage("+");
				Logger::WriteMessage(to_string(i.second).c_str());
				Logger::WriteMessage("_");
			}

		}

		void printInt(int i) {
			std::string s = std::to_string(i);
			Logger::WriteMessage(s.c_str());
		}

		void printVec(vi v) {
			for (int i : v) {
				Logger::WriteMessage(to_string(i).c_str());
			}
			Logger::WriteMessage("_");
		}

		//For Printing the AST.
		vector<string> nodeType = { "Program","Procedure", "StmtLst", "Assign", "Variable", "Plus", "Minus", "Times", "Constant", "While", "If", "Call" };

		void printTree(TNode* root, int nestLevel) {
			nestLevel++;
			string level = "Nexting Level : " + to_string(nestLevel);
			string type = "Type:" + nodeType[(int)root->type];
			string value = "Value:" + to_string((int)root->value);
			string stmtNo = "Stmt No:" + to_string((int)root->statementNumber);
			string parent = "Parent Type:" + nodeType[(int)root->parent->type];

			Logger::WriteMessage(level.c_str());
			Logger::WriteMessage(type.c_str());
			Logger::WriteMessage(value.c_str());
			Logger::WriteMessage(stmtNo.c_str());
			Logger::WriteMessage(parent.c_str());
			Logger::WriteMessage("_");


			int i = 0;
			for (TNode* child : root->childs) {
				string childNo = "Child" + to_string(i);
				Logger::WriteMessage(childNo.c_str());
				printTree(child, nestLevel);
				i++;
			}
		}

	};
}