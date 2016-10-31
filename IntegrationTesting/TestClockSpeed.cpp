#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
#include <time.h>
typedef std::vector<std::pair<int, int>> vp_i_i;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(SpeedTest1)
	{
	public:

		TEST_METHOD(TestNextSpeedLongSource)
		{

			try {
				Parse("..\\IntegrationTesting\\Integration Testing Files\\Long source if while - 1000.txt");
				Logger::WriteMessage("Parse ended");
			}
			catch (std::exception& ex) {
				//Invalid parsing should be done in Unit test/System test. Because I lazy.
				Logger::WriteMessage(ex.what());
				return;
			}
			PKB& pkb = PKB::getInstance();

			//We start measuring here. Remember what was the amount of ticks in the 
			//beginning of the part of code you want to test:
			int start = clock();
			pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			int end = clock();
			int ticks = end - start;
			float time = ((float)end - start) / CLOCKS_PER_SEC;

			Logger::WriteMessage("Next ended");
			printInt(ticks);
			printFloat(time);
			//std::cout << "it took " << end - start << "ticks, or " << ((float)end - start) / CLOCKS_PER_SEC << "seconds." << std::endl;
			
		}

		TEST_METHOD(TestNextSpecificSource)
		{

			try {
				Parse("..\\IntegrationTesting\\Integration Testing Files\\SourceOne.txt");
				Logger::WriteMessage("Parse ended");
			}
			catch (std::exception& ex) {
				//Invalid parsing should be done in Unit test/System test. Because I lazy.
				Logger::WriteMessage(ex.what());
				return;
			}
			PKB& pkb = PKB::getInstance();

			int start = clock();
			pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			int end = clock();
			int ticks = end - start;
			float time = ((float)end - start) / CLOCKS_PER_SEC;

			//vp_i_i stmts = pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);

			//printStmtPairs(stmts);

			vi stmts = pkb.getTransitiveNextGenericSpecific(5, NodeType::StmtLst);
			printVec(stmts);

			//vi stmts = pkb.getUsesSpecificGeneric(5, NodeType::StmtLst);
			//printVec(stmts);

		}

		TEST_METHOD(TestAffectStarSpeed)
		{

			try {
				Parse("..\\IntegrationTesting\\Integration Testing Files\\Affect Star Time Check.txt");
				Logger::WriteMessage("Parse ended");
			}
			catch (std::exception& ex) {
				//Invalid parsing should be done in Unit test/System test. Because I lazy.
				Logger::WriteMessage(ex.what());
				return;
			}
			PKB& pkb = PKB::getInstance();

			int start = clock();
			//pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			pkb.getTransitiveAffectGenericGeneric();
			int end = clock();
			int ticks = end - start;
			float time = ((float)end - start) / CLOCKS_PER_SEC;

			Logger::WriteMessage("Affect ended");
			printInt(ticks);
			printFloat(time);
			std::cout << "it took " << end - start << "ticks, or " << ((float)end - start) / CLOCKS_PER_SEC << "seconds." << std::endl;

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

		void printFloat(float i) {
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