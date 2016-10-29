#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
typedef std::vector<std::pair<int, int>> vp_i_i;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{
	TEST_CLASS(IntTestAffectStarOne)
	{
	public:

		TEST_CLASS_INITIALIZE(generateRequireResources) {
			try {
				Parse("..\\IntegrationTesting\\Integration Testing Files\\Affect Star Checker.txt");
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

		/*
		Affect*(1,s1) - 4,5,6,7,8,9
		Affect*(4,s1) - 5,6,7,8,9
		Affect*(5,s1) - 6,9
		Affect*(6,s1) - 9
		Affect*(7,s1) - 5,6,8,9
		*/
		TEST_METHOD(IntFile_Affect_Star)
		{
			PKB& pkb = PKB::getInstance();
			map_i_vi resultsMapVi;
			vp_i_i stmtPairs;
			vp_i_i resultStmtPairs;

			resultsMapVi[1] = { 4,5,6,7,8,9 };
			resultsMapVi[4] = { 5,6,7,8,9 };
			resultsMapVi[5] = { 6,9 };
			resultsMapVi[6] = { 9 };
			resultsMapVi[7] = { 5,6,8,9 };
			for (int i = 0; i < 11; i++) {
				vi stmt = pkb.getTransitiveAffectSpecificGeneric(i);
				//printInt(i);
				printVec(stmt);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmt));
			}

			resultsMapVi.clear();
			pkb.newQuery();
			resultsMapVi[4] = { 1 };
			resultsMapVi[5] = { 4,7 };
			resultsMapVi[6] = { 1,4,5,7 };
			resultsMapVi[7] = { 1,4 };
			resultsMapVi[8] = { 1,4,7 };
			resultsMapVi[9] = { 1,4,5,6,7 };
			
			for (int i = 0; i < 18; i++) {
				vi stmt = pkb.getTransitiveAffectGenericSpecific(i);
				printVec(stmt);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmt));
			}

			Assert::IsFalse(pkb.whetherAffect(0, 4));
			Assert::IsFalse(pkb.whetherAffect(-1, 10));
			Assert::IsFalse(pkb.whetherAffect(1, 0));
			Assert::IsFalse(pkb.whetherAffect(1, -1));
			Assert::IsTrue(pkb.whetherAffect(1, 6));
			Assert::IsFalse(pkb.whetherAffect(6, 1));
			Assert::IsFalse(pkb.whetherAffect(6, 8));
			Assert::IsTrue(pkb.whetherAffect(7, 5));

			//I don't know about this, the order is based on implementation.
			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 4));
			resultStmtPairs.push_back(make_pair(1, 5));
			resultStmtPairs.push_back(make_pair(1, 6));
			resultStmtPairs.push_back(make_pair(1, 7));
			resultStmtPairs.push_back(make_pair(1, 8));
			resultStmtPairs.push_back(make_pair(1, 9));
			resultStmtPairs.push_back(make_pair(4, 5));
			resultStmtPairs.push_back(make_pair(4, 6));
			resultStmtPairs.push_back(make_pair(4, 7));
			resultStmtPairs.push_back(make_pair(4, 8));
			resultStmtPairs.push_back(make_pair(4, 9));

			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 9));

			resultStmtPairs.push_back(make_pair(6, 9));

			resultStmtPairs.push_back(make_pair(7, 5));
			resultStmtPairs.push_back(make_pair(7, 6));
			resultStmtPairs.push_back(make_pair(7, 8));
			resultStmtPairs.push_back(make_pair(7, 9));

			stmtPairs = pkb.getTransitiveAffectGenericGeneric();
			printStmtPairs(stmtPairs);
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

			std::sort(pair1.begin(),pair1.end());
			std::sort(pair2.begin(), pair2.end());

			for (int i = 0; i < pair1.size(); i++) {
				if (pair1[i].first != pair2[i].first || pair1[i].second != pair2[i].second) {
					printStmtPair(pair1[i]);
					printStmtPair(pair2[i]);
					return false;
				}
			}
			return true;
		}

		void printStmtPairs(vp_i_i vp) {
			for (pair<int, int> i : vp) {
				printStmtPair(i);
			}
		}

		void printStmtPair(pair<int, int> pair) {
			string plus = "+ ";
			string msg = to_string(pair.first).c_str() + plus + to_string(pair.second).c_str();
			Logger::WriteMessage(msg.c_str());
		}

		void printStmtPairsSide(vp_i_i stmtPair, vp_i_i stmtPair2) {
			for (int i = 0; i < stmtPair.size(); i++) {
				string plus = "+ ";
				string msg = to_string(stmtPair[i].first).c_str() + plus + to_string(stmtPair[i].second).c_str();
				Logger::WriteMessage(msg.c_str());
				plus = "+ ";
				msg = to_string(stmtPair2[i].first).c_str() + plus + to_string(stmtPair2[i].second).c_str();
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