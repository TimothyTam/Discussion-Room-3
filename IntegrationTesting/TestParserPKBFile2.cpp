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

		TEST_CLASS_INITIALIZE(generateRequireResources) {
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

			Logger::WriteMessage(string("VarTable").c_str());
			for (int i = 0; i < 11; i++) {
				string temp = pkb.getVarNameFromIndex(i);
				if (temp != "") {
					std::string s = std::to_string(i);
					s = s + " " + temp;
					Logger::WriteMessage(s.c_str());
				}
			}
		}

		//I think I need to separate this.
		TEST_METHOD(IntFile2_Next)
		{
			PKB& pkb = PKB::getInstance();
			vp_i_i stmtPairs = pkb.getModifyGenericGeneric(NodeType::Procedure);

			//printStmtPairs(stmtPairs);


			/* Next
			for (int i = 0; i < 12; i++) {
				vi stmts = pkb.getNextSpecificGeneric(i, NodeType::StmtLst);
				//printVec(stmts);
			}

			for (int i = 0; i < 11; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::StmtLst);

				printVec(stmts);
			}

			vp_i_i stmtPair = pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			printStmtPairs(stmtPair);
			*/

		}

		TEST_METHOD(IntFile2_Modify) {
			Logger::WriteMessage("Testing Modify Methods");
			PKB& pkb = PKB::getInstance();
			map_i_vi resultsMapVi;
			vp_i_i stmtPairs;
			vp_i_i resultStmtPairs;
			int i;

			resultsMapVi.clear();
			resultsMapVi[1] = { 1,2,4,5,6,7 };
			resultsMapVi[2] = { 1};
			resultsMapVi[3] = { 2,4,5,6,7 };
			resultsMapVi[4] = { 1 };
			resultsMapVi[5] = { 2,4,5,6,7 };
			resultsMapVi[6] = { 4 };
			resultsMapVi[7] = { 5 };
			resultsMapVi[8] = { 5 };
			resultsMapVi[9] = { 2,6,7 };
			resultsMapVi[10] = { 6 };
			resultsMapVi[11] = { 7 };
			resultsMapVi[12] = { 2 };
			resultsMapVi[13] = { 2 };
			resultsMapVi[14] = { 2 };
			resultsMapVi[15] = { 2 };
			resultsMapVi[16] = { 2 };
			resultsMapVi[17] = { 2,4,5,6,7 };

			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 1,2,4,5,6,7 };
			resultsMapVi[2] = { 1 };
			resultsMapVi[4] = { 1 };
			resultsMapVi[6] = { 4 };
			resultsMapVi[7] = { 5 };
			resultsMapVi[8] = { 5 };
			resultsMapVi[10] = { 6 };
			resultsMapVi[11] = { 7 };
			resultsMapVi[12] = { 2 };
			resultsMapVi[13] = { 2 };
			resultsMapVi[14] = { 2 };
			resultsMapVi[15] = { 2 };
			resultsMapVi[16] = { 2 };

			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::Assign);
				printVec(stmts);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 2,4 };
			resultsMapVi[8] = { 3 };
			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 1,2,4 };
			resultsMapVi[1] = { 2,3 };
			resultsMapVi[2] = { 2,5,6 };
			resultsMapVi[3] = { 2,8,9 };
			resultsMapVi[4] = { 2,5,7 };
			resultsMapVi[6] = { 10 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 1,4 };
			resultsMapVi[1] = { 3 };
			resultsMapVi[2] = { 6 };
			resultsMapVi[3] = { 9 };
			resultsMapVi[4] = { 7 };
			resultsMapVi[6] = { 10 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 2 };
			resultsMapVi[1] = { 2 };
			resultsMapVi[2] = { 2 };
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0,1,2,3,4,6 };
			for (i = 0; i < 1; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0 };
			resultsMapVi[1] = { 0 };
			resultsMapVi[2] = { 0 };
			resultsMapVi[3] = { 0 };
			resultsMapVi[4] = { 0 };
			resultsMapVi[6] = { 0 };

			for (i = 0; i < 7; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			Assert::IsTrue(pkb.whetherProcModifies(0, 0));
			Assert::IsTrue(pkb.whetherProcModifies(0, 1));
			Assert::IsTrue(pkb.whetherProcModifies(0, 2));
			Assert::IsTrue(pkb.whetherProcModifies(0, 3));
			Assert::IsTrue(pkb.whetherProcModifies(0, 4));
			Assert::IsFalse(pkb.whetherProcModifies(0, 5));
			Assert::IsTrue(pkb.whetherProcModifies(0, 6));

			Assert::IsFalse(pkb.whetherStmtModifies(0, 0));
			Assert::IsTrue(pkb.whetherStmtModifies(1, 0));
			Assert::IsFalse(pkb.whetherStmtModifies(1, 1));
			Assert::IsFalse(pkb.whetherStmtModifies(1, 2));
			Assert::IsTrue(pkb.whetherStmtModifies(5, 2));
			Assert::IsTrue(pkb.whetherStmtModifies(5, 4));
			Assert::IsTrue(pkb.whetherStmtModifies(10, 6));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(9, 3));
			resultStmtPairs.push_back(make_pair(1, 0));
			resultStmtPairs.push_back(make_pair(3, 1));
			resultStmtPairs.push_back(make_pair(4, 0));
			resultStmtPairs.push_back(make_pair(6, 2));
			resultStmtPairs.push_back(make_pair(7, 4));
			resultStmtPairs.push_back(make_pair(10, 6));
			resultStmtPairs.push_back(make_pair(5, 2));
			resultStmtPairs.push_back(make_pair(5, 4));
			resultStmtPairs.push_back(make_pair(8, 3));
			resultStmtPairs.push_back(make_pair(2, 0));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(2, 2));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));

			stmtPairs = pkb.getModifyGenericGeneric(NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(9, 3));
			resultStmtPairs.push_back(make_pair(1, 0));
			resultStmtPairs.push_back(make_pair(3, 1));
			resultStmtPairs.push_back(make_pair(4, 0));
			resultStmtPairs.push_back(make_pair(6, 2));
			resultStmtPairs.push_back(make_pair(7, 4));
			resultStmtPairs.push_back(make_pair(10, 6));

			stmtPairs = pkb.getModifyGenericGeneric(NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));


			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(2, 0));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(2, 2));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));

			stmtPairs = pkb.getModifyGenericGeneric(NodeType::If);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 2));
			resultStmtPairs.push_back(make_pair(5, 4));
			resultStmtPairs.push_back(make_pair(8, 3));

			stmtPairs = pkb.getModifyGenericGeneric(NodeType::While);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

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
				string plus = "+ ";
				string msg = to_string(i.first).c_str() + plus + to_string(i.second).c_str();
				Logger::WriteMessage(msg.c_str());
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