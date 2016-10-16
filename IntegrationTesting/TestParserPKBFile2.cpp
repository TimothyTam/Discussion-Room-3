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
			map_i_vi resultsMapVi;
			vp_i_i stmtPairs;
			vp_i_i resultStmtPairs;


			resultsMapVi.clear();
			resultsMapVi[1] = { 2,4 };
			resultsMapVi[2] = { 3 };
			resultsMapVi[3] = { 1 };
			resultsMapVi[5] = { 6,13 };
			resultsMapVi[6] = { 7 };
			resultsMapVi[7] = { 8,9 };
			resultsMapVi[8] = { 7};
			resultsMapVi[9] = { 10,11 };
			resultsMapVi[11] = { 12 };
			resultsMapVi[13] = { 14 };
			resultsMapVi[14] = { 13 };
			for (int i = 0; i < 18; i++) {
				vi stmts = pkb.getNextSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}


			resultsMapVi.clear();
			resultsMapVi[1] = { 1,2,3,4 };
			resultsMapVi[2] = { 1,2,3,4 };
			resultsMapVi[3] = { 1,2,3,4 };
			resultsMapVi[5] = { 6,7,8,9,10,11,12,13,14 };
			resultsMapVi[6] = { 7,8,9,10,11,12 };
			resultsMapVi[7] = { 7,8,9,10,11,12 };
			resultsMapVi[8] = { 7,8,9,10,11,12 };
			resultsMapVi[9] = { 10,11,12 };
			resultsMapVi[11] = { 12 };
			resultsMapVi[13] = { 13,14 };
			resultsMapVi[14] = { 13,14 };
			for (int i = 1; i < 18; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 9 };
			resultsMapVi[6] = { 9 };
			resultsMapVi[7] = { 9 };
			resultsMapVi[8] = { 9 };
			
			for (int i = 1; i < 18; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 3};
			resultsMapVi[2] = { 3 };
			resultsMapVi[3] = { 3 };
			resultsMapVi[5] = { 12,14 };
			resultsMapVi[6] = { 12 };
			resultsMapVi[7] = { 12 };
			resultsMapVi[8] = { 12 };
			resultsMapVi[9] = { 12 };
			resultsMapVi[11] = { 12 };
			resultsMapVi[13] = { 14 };
			resultsMapVi[14] = { 14 };
			for (int i = 1; i < 18; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::Call);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 2,4 };
			resultsMapVi[2] = { 2,4 };
			resultsMapVi[3] = { 2,4 };
			resultsMapVi[5] = { 6,8,10,11 };
			resultsMapVi[6] = { 8,10,11 };
			resultsMapVi[7] = { 8,10,11 };
			resultsMapVi[8] = { 8,10,11 };
			resultsMapVi[9] = { 10,11};

			for (int i = 1; i < 18; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 1 };
			resultsMapVi[2] = { 1 };
			resultsMapVi[3] = { 1 };
			resultsMapVi[5] = { 7,13 };
			resultsMapVi[6] = { 7 };
			resultsMapVi[7] = { 7 };
			resultsMapVi[8] = { 7 };
			resultsMapVi[13] = { 13 };
			resultsMapVi[14] = { 13 };

			for (int i = 1; i < 18; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(2, 2));
			resultStmtPairs.push_back(make_pair(2, 4));
			resultStmtPairs.push_back(make_pair(6, 8));
			resultStmtPairs.push_back(make_pair(6, 10));
			resultStmtPairs.push_back(make_pair(6, 11));
			resultStmtPairs.push_back(make_pair(8, 8));
			resultStmtPairs.push_back(make_pair(8, 10));
			resultStmtPairs.push_back(make_pair(8, 11));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(6, 7));
			resultStmtPairs.push_back(make_pair(8, 7));
			resultStmtPairs.push_back(make_pair(6, 9));
			resultStmtPairs.push_back(make_pair(8, 9));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(6, 12));
			resultStmtPairs.push_back(make_pair(8, 12));
			resultStmtPairs.push_back(make_pair(11, 12));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 4));
			resultStmtPairs.push_back(make_pair(7, 8));
			resultStmtPairs.push_back(make_pair(7, 10));
			resultStmtPairs.push_back(make_pair(7, 11));
			resultStmtPairs.push_back(make_pair(1, 1));
			resultStmtPairs.push_back(make_pair(7, 7));
			resultStmtPairs.push_back(make_pair(13, 13));
			resultStmtPairs.push_back(make_pair(7, 9));
			resultStmtPairs.push_back(make_pair(1, 3));
			resultStmtPairs.push_back(make_pair(7, 12));
			resultStmtPairs.push_back(make_pair(13, 14));
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 8));
			resultStmtPairs.push_back(make_pair(5, 10));
			resultStmtPairs.push_back(make_pair(5, 11));
			resultStmtPairs.push_back(make_pair(9, 10));
			resultStmtPairs.push_back(make_pair(9, 11));
			resultStmtPairs.push_back(make_pair(5, 7));
			resultStmtPairs.push_back(make_pair(5, 13));
			resultStmtPairs.push_back(make_pair(5, 9));
			resultStmtPairs.push_back(make_pair(5, 12));
			resultStmtPairs.push_back(make_pair(5, 14));
			resultStmtPairs.push_back(make_pair(9, 12));
			resultStmtPairs.push_back(make_pair(3, 2));
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(3, 1));
			resultStmtPairs.push_back(make_pair(14, 13));
			resultStmtPairs.push_back(make_pair(3, 3));
			resultStmtPairs.push_back(make_pair(14, 14));

			stmtPairs = pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));


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
			resultsMapVi[2] = { 1 };
			resultsMapVi[4] = { 1 };
			resultsMapVi[6] = { 4 };
			resultsMapVi[8] = { 5 };
			resultsMapVi[10] = { 6 };
			resultsMapVi[11] = { 7 };
			resultsMapVi[16] = { 2 };

			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 1,2,4,5,6,7 };
			resultsMapVi[7] = { 5 };
			resultsMapVi[13] = { 2 };
			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[3] = { 2,4,5,6,7 };
			resultsMapVi[12] = { 2 };
			resultsMapVi[14] = { 2 };
			resultsMapVi[15] = { 2 };
			resultsMapVi[17] = { 2,4,5,6,7 };
			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::Call);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 1,2,4 };
			resultsMapVi[2] = { 1,3,5,9,12,13,14,15,16,17 };
			resultsMapVi[4] = { 1,3,5,6,17 };
			resultsMapVi[5] = { 1,3,5,7,8,17 };
			resultsMapVi[6] = { 1,3,5,9,10,17 };
			resultsMapVi[7] = { 1,3,5,9,11,17 };
			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 2,4 };
			resultsMapVi[2] = { 16 };
			resultsMapVi[4] = { 6 };
			resultsMapVi[5] = { 8 };
			resultsMapVi[6] = { 10 };
			resultsMapVi[7] = { 11 };
			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 5,9 };
			resultsMapVi[4] = { 5 };
			resultsMapVi[5] = { 5 };
			resultsMapVi[6] = { 5,9 };
			resultsMapVi[7] = { 5,9 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 3,12,14,15,17 };
			resultsMapVi[4] = { 3,17 };
			resultsMapVi[5] = { 3,17 };
			resultsMapVi[6] = { 3,17 };
			resultsMapVi[7] = { 3,17 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::Call);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 1,2,4,5,6,7 };
			resultsMapVi[1] = { 2,4,5,6,7 };
			resultsMapVi[2] = { 2 };
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2,4,5,6,7 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 0 };
			resultsMapVi[2] = { 0,1,2,3,4 };
			resultsMapVi[4] = { 0,1,4 };
			resultsMapVi[5] = { 0,1,4 };
			resultsMapVi[6] = { 0,1,4 };
			resultsMapVi[7] = { 0,1,4 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getModifyGenericSpecific(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			Assert::IsFalse(pkb.whetherProcModifies(0, 0));
			Assert::IsTrue(pkb.whetherProcModifies(0, 1));
			Assert::IsTrue(pkb.whetherProcModifies(0, 2));
			Assert::IsFalse(pkb.whetherProcModifies(0, 3));
			Assert::IsTrue(pkb.whetherProcModifies(0, 4));
			Assert::IsTrue(pkb.whetherProcModifies(0, 5));
			Assert::IsTrue(pkb.whetherProcModifies(0, 6));
			Assert::IsTrue(pkb.whetherProcModifies(3, 2));
			Assert::IsFalse(pkb.whetherProcModifies(3, -1));
			Assert::IsTrue(pkb.whetherProcModifies(3, 2));
			Assert::IsFalse(pkb.whetherProcModifies(-1, 2));


			Assert::IsFalse(pkb.whetherStmtModifies(0, 0));
			Assert::IsTrue(pkb.whetherStmtModifies(1, 1));
			Assert::IsTrue(pkb.whetherStmtModifies(2, 1));
			Assert::IsTrue(pkb.whetherStmtModifies(4, 1));
			Assert::IsFalse(pkb.whetherStmtModifies(1, 0));
			Assert::IsTrue(pkb.whetherStmtModifies(5, 2));
			Assert::IsTrue(pkb.whetherStmtModifies(5, 4));
			Assert::IsTrue(pkb.whetherStmtModifies(10, 6));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(10, 6));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(4, 1));
			resultStmtPairs.push_back(make_pair(6, 4));
			resultStmtPairs.push_back(make_pair(8, 5));
			resultStmtPairs.push_back(make_pair(11, 7));
			resultStmtPairs.push_back(make_pair(16, 2));
			resultStmtPairs.push_back(make_pair(1, 1));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 4));
			resultStmtPairs.push_back(make_pair(1, 5));
			resultStmtPairs.push_back(make_pair(1, 6));
			resultStmtPairs.push_back(make_pair(1, 7));
			resultStmtPairs.push_back(make_pair(7, 5));
			resultStmtPairs.push_back(make_pair(13, 2));
			resultStmtPairs.push_back(make_pair(9, 2));
			resultStmtPairs.push_back(make_pair(9, 6));
			resultStmtPairs.push_back(make_pair(9, 7));
			resultStmtPairs.push_back(make_pair(5, 2));
			resultStmtPairs.push_back(make_pair(5, 4));
			resultStmtPairs.push_back(make_pair(5, 5));
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 7));
			resultStmtPairs.push_back(make_pair(3, 2));
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(3, 5));
			resultStmtPairs.push_back(make_pair(3, 6));
			resultStmtPairs.push_back(make_pair(3, 7));
			resultStmtPairs.push_back(make_pair(12, 2));
			resultStmtPairs.push_back(make_pair(14, 2));
			resultStmtPairs.push_back(make_pair(15, 2));
			resultStmtPairs.push_back(make_pair(17, 2));
			resultStmtPairs.push_back(make_pair(17, 4));
			resultStmtPairs.push_back(make_pair(17, 5));
			resultStmtPairs.push_back(make_pair(17, 6));
			resultStmtPairs.push_back(make_pair(17, 7));

			stmtPairs = pkb.getModifyGenericGeneric(NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(10, 6));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(4, 1));
			resultStmtPairs.push_back(make_pair(6, 4));
			resultStmtPairs.push_back(make_pair(8, 5));
			resultStmtPairs.push_back(make_pair(11, 7));
			resultStmtPairs.push_back(make_pair(16, 2));
			stmtPairs = pkb.getModifyGenericGeneric(NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));


			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(9, 2));
			resultStmtPairs.push_back(make_pair(9, 6));
			resultStmtPairs.push_back(make_pair(9, 7));
			resultStmtPairs.push_back(make_pair(5, 2));
			resultStmtPairs.push_back(make_pair(5, 4));
			resultStmtPairs.push_back(make_pair(5, 5));
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 7));
			stmtPairs = pkb.getModifyGenericGeneric(NodeType::If);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 1));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 4));
			resultStmtPairs.push_back(make_pair(1, 5));
			resultStmtPairs.push_back(make_pair(1, 6));
			resultStmtPairs.push_back(make_pair(1, 7));
			resultStmtPairs.push_back(make_pair(7, 5));
			resultStmtPairs.push_back(make_pair(13, 2));

			stmtPairs = pkb.getModifyGenericGeneric(NodeType::While);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(3, 2));
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(3, 5));
			resultStmtPairs.push_back(make_pair(3, 6));
			resultStmtPairs.push_back(make_pair(3, 7));
			resultStmtPairs.push_back(make_pair(12, 2));
			resultStmtPairs.push_back(make_pair(14, 2));
			resultStmtPairs.push_back(make_pair(15, 2));
			resultStmtPairs.push_back(make_pair(17, 2));
			resultStmtPairs.push_back(make_pair(17, 4));
			resultStmtPairs.push_back(make_pair(17, 5));
			resultStmtPairs.push_back(make_pair(17, 6));
			resultStmtPairs.push_back(make_pair(17, 7));
			stmtPairs = pkb.getModifyGenericGeneric(NodeType::Call);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

		}

		TEST_METHOD(IntFile2_Uses) {
			Logger::WriteMessage("Testing Uses Methods");
			PKB& pkb = PKB::getInstance();
			map_i_vi resultsMapVi;
			vp_i_i stmtPairs;
			vp_i_i resultStmtPairs;
			int i;

			resultsMapVi.clear();
			resultsMapVi[1] = { 0,2,3 };
			resultsMapVi[2] = { 2 };
			resultsMapVi[3] = { 0,3 };
			resultsMapVi[4] = { 3 };
			resultsMapVi[5] = { 0,3 };
			resultsMapVi[6] = { 3 };
			resultsMapVi[7] = { 0,3 };
			resultsMapVi[8] = { 3 };
			resultsMapVi[9] = { 0,3 };
			resultsMapVi[10] = { 3 };
			resultsMapVi[11] = { 3 };
			resultsMapVi[12] = { 3 };
			resultsMapVi[13] = { 0,3 };
			resultsMapVi[14] = { 3 };
			resultsMapVi[15] = { 3 };
			resultsMapVi[16] = { 3 };
			resultsMapVi[17] = { 0,3 };

			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 2 };
			resultsMapVi[4] = { 3 };
			resultsMapVi[6] = { 3 };
			resultsMapVi[8] = { 3 };
			resultsMapVi[10] = { 3 };
			resultsMapVi[11] = { 3 };
			resultsMapVi[16] = { 3 };
			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 0,2,3 };
			resultsMapVi[7] = { 0,3 };
			resultsMapVi[13] = { 0,3 };
			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[3] = { 0,3 };
			resultsMapVi[12] = { 3 };
			resultsMapVi[14] = { 3 };
			resultsMapVi[15] = { 3 };
			resultsMapVi[17] = { 0,3 };
			for (i = 1; i <= 18; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::Call);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 1,3,5,7,9,13,17 };
			resultsMapVi[2] = { 1,2 }; 
			resultsMapVi[3] = { 1,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17 };
			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 2 };
			resultsMapVi[3] = { 4,6,8,10,11,16 };
			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 5,9 };
			resultsMapVi[3] = { 5,9 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear(); 
			resultsMapVi[0] = { 3,17 };
			resultsMapVi[3] = { 3,12,14,15,17 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::Call);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0,2,3 };
			resultsMapVi[1] = { 0,3 };
			resultsMapVi[2] = { 3 };
			resultsMapVi[3] = { 3 };
			resultsMapVi[4] = { 0,3 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0,1,4 };
			resultsMapVi[2] = { 0 };
			resultsMapVi[3] = { 0,1,2,3,4 };


			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			Assert::IsTrue(pkb.whetherProcUses(0, 0));
			Assert::IsFalse(pkb.whetherProcUses(0, 1));
			Assert::IsTrue(pkb.whetherProcUses(0, 2));
			Assert::IsTrue(pkb.whetherProcUses(0, 3));
			Assert::IsTrue(pkb.whetherProcUses(1, 0));
			Assert::IsTrue(pkb.whetherProcUses(1, 3));
			Assert::IsTrue(pkb.whetherProcUses(2, 3));
			Assert::IsTrue(pkb.whetherProcUses(3, 3));
			Assert::IsFalse(pkb.whetherProcUses(3, -1));
			Assert::IsTrue(pkb.whetherProcUses(4, 3));
			Assert::IsFalse(pkb.whetherProcUses(-1, 2));

			Assert::IsFalse(pkb.whetherStmtUses(0, 0));
			Assert::IsTrue(pkb.whetherStmtUses(1, 0));
			Assert::IsTrue(pkb.whetherStmtUses(2, 2));
			Assert::IsTrue(pkb.whetherStmtUses(4, 3));
			Assert::IsFalse(pkb.whetherStmtUses(1, -1));
			Assert::IsTrue(pkb.whetherStmtUses(5, 3));
			Assert::IsTrue(pkb.whetherStmtUses(5, 0));
			Assert::IsTrue(pkb.whetherStmtUses(10, 3));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(10, 3));
			resultStmtPairs.push_back(make_pair(2, 2));
			resultStmtPairs.push_back(make_pair(4, 3));
			resultStmtPairs.push_back(make_pair(6, 3));
			resultStmtPairs.push_back(make_pair(8, 3));
			resultStmtPairs.push_back(make_pair(11, 3));
			resultStmtPairs.push_back(make_pair(16, 3));
			resultStmtPairs.push_back(make_pair(1, 0));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 3));
			resultStmtPairs.push_back(make_pair(7, 0));
			resultStmtPairs.push_back(make_pair(7, 3));
			resultStmtPairs.push_back(make_pair(13, 0));
			resultStmtPairs.push_back(make_pair(13, 3));
			resultStmtPairs.push_back(make_pair(9, 0));
			resultStmtPairs.push_back(make_pair(9, 3));
			resultStmtPairs.push_back(make_pair(5, 0));
			resultStmtPairs.push_back(make_pair(5, 3));
			resultStmtPairs.push_back(make_pair(3, 0));
			resultStmtPairs.push_back(make_pair(3, 3));
			resultStmtPairs.push_back(make_pair(12, 3));
			resultStmtPairs.push_back(make_pair(14, 3));
			resultStmtPairs.push_back(make_pair(15, 3));
			resultStmtPairs.push_back(make_pair(17, 0));
			resultStmtPairs.push_back(make_pair(17, 3));

			stmtPairs = pkb.getUsesGenericGeneric(NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(10, 3));
			resultStmtPairs.push_back(make_pair(2, 2));
			resultStmtPairs.push_back(make_pair(4, 3));
			resultStmtPairs.push_back(make_pair(6, 3));
			resultStmtPairs.push_back(make_pair(8, 3));
			resultStmtPairs.push_back(make_pair(11, 3));
			resultStmtPairs.push_back(make_pair(16, 3));
			stmtPairs = pkb.getUsesGenericGeneric(NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));


			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(9, 0));
			resultStmtPairs.push_back(make_pair(9, 3));
			resultStmtPairs.push_back(make_pair(5, 0));
			resultStmtPairs.push_back(make_pair(5, 3));
			stmtPairs = pkb.getUsesGenericGeneric(NodeType::If);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 0));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 3));
			resultStmtPairs.push_back(make_pair(7, 0));
			resultStmtPairs.push_back(make_pair(7, 3));
			resultStmtPairs.push_back(make_pair(13, 0));
			resultStmtPairs.push_back(make_pair(13, 3));

			stmtPairs = pkb.getUsesGenericGeneric(NodeType::While);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(3, 0));
			resultStmtPairs.push_back(make_pair(3, 3));
			resultStmtPairs.push_back(make_pair(12, 3));
			resultStmtPairs.push_back(make_pair(14, 3));
			resultStmtPairs.push_back(make_pair(15, 3));
			resultStmtPairs.push_back(make_pair(17, 0));
			resultStmtPairs.push_back(make_pair(17, 3));
			stmtPairs = pkb.getUsesGenericGeneric(NodeType::Call);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

		}

		TEST_METHOD(IntFile2_Calls) {
			Logger::WriteMessage("Testing Call Methods");
			PKB& pkb = PKB::getInstance();
			map_i_vi resultsMapVi;
			vp_i_i stmtPairs;
			vp_i_i resultStmtPairs;
			int i;

			resultsMapVi.clear();
			resultsMapVi[1] = { 0, 4 };
			resultsMapVi[2] = { 1 };
			resultsMapVi[3] = { 1,2 };
			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.callsGenericSpecific(i);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 1 };
			resultsMapVi[1] = { 2,3 };
			resultsMapVi[2] = { 3 };
			resultsMapVi[4] = { 1 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.callsSpecificGeneric(i);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 0, 4 };
			resultsMapVi[2] = { 0,1,4 };
			resultsMapVi[3] = { 0,1,2,4 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.callsTransitiveGenericSpecific(i);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 1,2,3 };
			resultsMapVi[1] = { 2,3 };
			resultsMapVi[2] = { 3 };
			resultsMapVi[4] = { 1,2,3 };

			for (i = 0; i <= 7; i++) {
				vi stmts = pkb.callsTransitiveSpecificGeneric(i);
				printVec(stmts);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}


			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(0, 1));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 3));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(4, 1));

			stmtPairs = pkb.callsGenericGeneric();
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));


			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(0, 1));
			resultStmtPairs.push_back(make_pair(0, 2));
			resultStmtPairs.push_back(make_pair(0, 3));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(1, 3));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(4, 1));
			resultStmtPairs.push_back(make_pair(4, 2));
			resultStmtPairs.push_back(make_pair(4, 3));

			stmtPairs = pkb.callsTransitiveGenericGeneric();
			printStmtPairs(stmtPairs);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			Assert::IsTrue(pkb.whetherCalls(0, 1));
			Assert::IsFalse(pkb.whetherCalls(0, 2));
			Assert::IsTrue(pkb.whetherCalls(2, 3));
			Assert::IsTrue(pkb.whetherTransitiveCalls(0, 1));
			Assert::IsTrue(pkb.whetherTransitiveCalls(0, 3));
			Assert::IsFalse(pkb.whetherTransitiveCalls(0, -1));
			Assert::IsFalse(pkb.whetherTransitiveCalls(-1, 1));
			Assert::IsFalse(pkb.whetherTransitiveCalls(4, 0));
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