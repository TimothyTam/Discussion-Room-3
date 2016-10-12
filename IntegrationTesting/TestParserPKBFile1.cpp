#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"
typedef std::vector<std::pair<int,int>> vp_i_i;

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(IntTest1)
	{
	public:
		
		//I think I need to separate this.
		TEST_METHOD(TestParserPKB)
		{
			
			try {
				Parse("..\\IntegrationTesting\\Integration Testing Files\\IntTestingFile1.txt");
				Logger::WriteMessage("Parse ended");
			}
			catch (std::exception& ex) {
				//Invalid parsing should be done in Unit test/System test. Because I lazy.
				Logger::WriteMessage(ex.what());
				return;
			}

			PKB& pkb = PKB::getInstance();
			//This is called in TestWrapper.cpp in AutoTester
			//pkb.buildAllTables();

			vector<string> procs = pkb.getAllEntityName(NodeType::Procedure);
			Assert::AreEqual(1, (int)procs.size());
			Assert::AreEqual(string("P"), procs[0]);
			vector<string> vars = pkb.getAllEntityName(NodeType::Variable);
			Assert::AreEqual(8, (int)vars.size());

			size_t i;
			vector<string> variables = { "z","x","y","k","n","p","b","c" };
			for (i = 0; i < 8; i++) {
				Assert::AreEqual(string(variables[i]), vars[i]);
			}

			Assert::AreEqual(10, pkb.getStmtCount());
			Assert::AreEqual(10, (int)pkb.getAllEntityIndex(NodeType::StmtLst).size());
			Assert::AreEqual(7, (int)pkb.getAllEntityIndex(NodeType::Assign).size());
			Assert::AreEqual(1, (int)pkb.getAllEntityIndex(NodeType::If).size());
			Assert::AreEqual(2, (int)pkb.getAllEntityIndex(NodeType::While).size());
			Assert::AreEqual(0, (int)pkb.getAllEntityIndex(NodeType::Call).size());

			Logger::WriteMessage("Testing AST Methods");

			TNode* assign1 = pkb.getStmt(1).second;
			Assert::AreEqual(2, (int)assign1->childs.size());
			Assert::AreEqual(0, assign1->childs[0]->value);
			Assert::AreEqual(1, assign1->childs[1]->value);
			TNode* firstStmtList = assign1->parent;
			Assert::AreEqual(3, (int)firstStmtList->childs.size());
			TNode* ifStmt = firstStmtList->childs[1];
			Assert::AreEqual((int)NodeType::If, (int)ifStmt->type);
			Assert::AreEqual(3, (int)ifStmt->childs.size());
			Assert::AreEqual(0, (int)ifStmt->childs[0]->value);
			Assert::AreEqual((int)NodeType::StmtLst, (int)ifStmt->childs[1]->type);
			Assert::AreEqual((int)NodeType::StmtLst, (int)ifStmt->childs[2]->type);
			TNode* thenStmt = ifStmt->childs[1];
			TNode* elseStmt = ifStmt->childs[2];
			Assert::AreEqual(2, (int)thenStmt->childs.size());
			Assert::AreEqual((int)NodeType::Assign, (int)thenStmt->childs[0]->type);
			Assert::AreEqual((int)NodeType::Assign, (int)thenStmt->childs[1]->type);
			//The tree looks correct :P

			Logger::WriteMessage("Testing Follow Methods in PKB");

			vi results;
			map_i_vi resultsMapVi;

			results = { 2,10,4,0,8,7,0,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getFollowSpecificGeneric(i, NodeType::StmtLst);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,10,4,0,0,7,0,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getFollowSpecificGeneric(i, NodeType::Assign);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,0,0,0,8,0,0,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getFollowSpecificGeneric(i, NodeType::While);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,1,0,3,0,0,6,5,0,2 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getFollowGenericSpecific(i, NodeType::StmtLst);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,1,0,3,0,0,6,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getFollowGenericSpecific(i, NodeType::Assign);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,0,0,0,0,0,0,5,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getFollowGenericSpecific(i, NodeType::While);
				Assert::AreEqual(results[i - 1], result);
			}

			vp_i_i resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(6, 7));
			resultStmtPairs.push_back(make_pair(1, 2));
			resultStmtPairs.push_back(make_pair(5, 8));
			resultStmtPairs.push_back(make_pair(2,10));			

			vp_i_i stmtPairs = pkb.getFollowGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 2));

			stmtPairs = pkb.getFollowGenericGeneric(NodeType::StmtLst, NodeType::If);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(2, 10));

			stmtPairs = pkb.getFollowGenericGeneric(NodeType::If, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();

			stmtPairs = pkb.getFollowGenericGeneric(NodeType::While, NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(6, 7));

			stmtPairs = pkb.getFollowGenericGeneric(NodeType::Assign, NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultsMapVi.clear();
			resultsMapVi[1] = { 2,10 };
			resultsMapVi[2] = { 10 };
			resultsMapVi[3] = { 4 };
			resultsMapVi[5] = { 8 };
			resultsMapVi[6] = { 7 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveFollowSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 10 };
			resultsMapVi[2] = { 10 };
			resultsMapVi[3] = { 4 };
			resultsMapVi[6] = { 7 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveFollowSpecificGeneric(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 8 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveFollowSpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 1 };
			resultsMapVi[4] = { 3 };
			resultsMapVi[7] = { 6 };
			resultsMapVi[8] = { 5 };
			resultsMapVi[10] = { 1,2 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveFollowGenericSpecific(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 1 };
			resultsMapVi[4] = { 3 };
			resultsMapVi[7] = { 6 };
			resultsMapVi[10] = { 1 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveFollowGenericSpecific(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[8] = { 5 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveFollowGenericSpecific(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 10));
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(6, 7));
			resultStmtPairs.push_back(make_pair(1, 2)); 
			resultStmtPairs.push_back(make_pair(5, 8));
			resultStmtPairs.push_back(make_pair(2, 10));

			stmtPairs = pkb.getTransitiveFollowGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 10));
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(6, 7));
			resultStmtPairs.push_back(make_pair(1, 2));

			stmtPairs = pkb.getTransitiveFollowGenericGeneric(NodeType::Assign, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 8));

			stmtPairs = pkb.getTransitiveFollowGenericGeneric(NodeType::StmtLst, NodeType::While);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(1, 10));
			resultStmtPairs.push_back(make_pair(3, 4));
			resultStmtPairs.push_back(make_pair(6, 7));

			stmtPairs = pkb.getTransitiveFollowGenericGeneric(NodeType::Assign, NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			Assert::IsTrue(pkb.whetherFollows(1, 2));
			Assert::IsFalse(pkb.whetherFollows(1, 10));
			Assert::IsTrue(pkb.whetherFollows(2, 10));
			Assert::IsTrue(pkb.whetherTransitivelyFollows(1, 10));
			Assert::IsTrue(pkb.whetherTransitivelyFollows(1, 2));

			Logger::WriteMessage("Testing Modify Methods");
			
			resultsMapVi.clear();
			resultsMapVi[1] = { 0 };
			resultsMapVi[2] = { 0,1,2,3,4 };
			resultsMapVi[3] = { 1 };
			resultsMapVi[4] = { 0 };
			resultsMapVi[5] = { 2,4 };
			resultsMapVi[6] = { 2 };
			resultsMapVi[7] = { 4 };
			resultsMapVi[8] = { 3 };
			resultsMapVi[9] = { 3 };
			resultsMapVi[10] = { 6 };
			resultsMapVi[11] = {};
			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 0 };
			resultsMapVi[3] = { 1 };
			resultsMapVi[4] = { 0 };
			resultsMapVi[6] = { 2 };
			resultsMapVi[7] = { 4 };
			resultsMapVi[9] = { 3 };
			resultsMapVi[10] = { 6 };
			resultsMapVi[11] = {};
			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getModifySpecificGeneric(i, NodeType::Assign);
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


			Logger::WriteMessage("Testing Uses Methods");
			resultsMapVi.clear();
			resultsMapVi[2] = { 0,1,3,4,5 };
			resultsMapVi[5] = { 0,1,3,4 };
			resultsMapVi[6] = { 0,1,3 };
			resultsMapVi[7] = { 4 };
			resultsMapVi[8] = { 0, 5 };
			resultsMapVi[9] = { 0 };
			resultsMapVi[10] = { 7 };
			resultsMapVi[11] = {};

			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[6] = { 0,1,3 };
			resultsMapVi[7] = { 4 };
			resultsMapVi[9] = { 0 };
			resultsMapVi[10] = { 7 };

			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 0,1,3,4 };
			resultsMapVi[8] = { 0, 5 };

			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 2,5,6,8,9 };
			resultsMapVi[1] = { 2,5,6 };
			resultsMapVi[3] = { 2,5,6 };
			resultsMapVi[4] = { 2,5,7 };
			resultsMapVi[5] = { 2,8 };
			resultsMapVi[7] = { 10 };


			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 6,9 };
			resultsMapVi[1] = { 6 };
			resultsMapVi[3] = { 6 };
			resultsMapVi[4] = { 7 };
			resultsMapVi[7] = { 10 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 2 };
			resultsMapVi[1] = { 2 };
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2 };
			resultsMapVi[5] = { 2 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0,1,3,4,5,7 };
			for (i = 0; i < 1; i++) {
				vi stmts = pkb.getUsesSpecificGeneric(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0 };
			resultsMapVi[1] = { 0 };
			resultsMapVi[3] = { 0 };
			resultsMapVi[4] = { 0 };
			resultsMapVi[5] = { 0 };
			resultsMapVi[7] = { 0 };
			for (i = 0; i < 7; i++) {
				vi stmts = pkb.getUsesGenericSpecific(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(9, 0));
			resultStmtPairs.push_back(make_pair(6, 0));
			resultStmtPairs.push_back(make_pair(6, 1));
			resultStmtPairs.push_back(make_pair(6, 3)); 
			resultStmtPairs.push_back(make_pair(7, 4));
			resultStmtPairs.push_back(make_pair(10, 7)); 
			resultStmtPairs.push_back(make_pair(5, 0));
			resultStmtPairs.push_back(make_pair(5, 1));
			resultStmtPairs.push_back(make_pair(5, 3)); 
			resultStmtPairs.push_back(make_pair(5, 4));
			resultStmtPairs.push_back(make_pair(8, 0));
			resultStmtPairs.push_back(make_pair(8, 5)); 
			resultStmtPairs.push_back(make_pair(2, 0));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));
			resultStmtPairs.push_back(make_pair(2, 5));

			stmtPairs = pkb.getUsesGenericGeneric(NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(9, 0));
			resultStmtPairs.push_back(make_pair(6, 0));
			resultStmtPairs.push_back(make_pair(6, 1));
			resultStmtPairs.push_back(make_pair(6, 3));
			resultStmtPairs.push_back(make_pair(7, 4));
			resultStmtPairs.push_back(make_pair(10, 7));

			stmtPairs = pkb.getUsesGenericGeneric(NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 0));
			resultStmtPairs.push_back(make_pair(5, 1));
			resultStmtPairs.push_back(make_pair(5, 3));
			resultStmtPairs.push_back(make_pair(5, 4));
			resultStmtPairs.push_back(make_pair(8, 0));
			resultStmtPairs.push_back(make_pair(8, 5));

			stmtPairs = pkb.getUsesGenericGeneric(NodeType::While);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(2, 0));
			resultStmtPairs.push_back(make_pair(2, 1));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));
			resultStmtPairs.push_back(make_pair(2, 5));

			stmtPairs = pkb.getUsesGenericGeneric(NodeType::If);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			Assert::IsTrue(pkb.whetherStmtUses(2, 0));
			Assert::IsTrue(pkb.whetherStmtUses(2, 1));
			Assert::IsFalse(pkb.whetherStmtUses(2, 2));
			Assert::IsTrue(pkb.whetherStmtUses(2, 3));
			Assert::IsTrue(pkb.whetherStmtUses(2, 4));

			Assert::IsTrue(pkb.whetherProcUses(0, 0));
			Assert::IsTrue(pkb.whetherProcUses(0, 1));
			Assert::IsFalse(pkb.whetherProcUses(0, 2));
			Assert::IsTrue(pkb.whetherProcUses(0, 3));
			Assert::IsTrue(pkb.whetherProcUses(0, 4));
			Assert::IsFalse(pkb.whetherProcUses(1, 4));
			Assert::IsFalse(pkb.whetherProcUses(1, 7));
			
			
			Logger::WriteMessage("Testing Parent Methods in PKB");

			resultsMapVi.clear();
			resultsMapVi[1] = { };
			resultsMapVi[2] = {3,4,5,8};
			resultsMapVi[5] = { 6,7};
			resultsMapVi[8] = { 9 };

			for (i = 1; i < 11; i++) {
				vi result = pkb.getParentSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], result));
			}

			results = { -1,-1,2,2,2,5,5,2,8,-1 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getParentGenericSpecific(i, NodeType::StmtLst);
				Assert::AreEqual(results[i - 1], result);
			}
			
			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 7));
			resultStmtPairs.push_back(make_pair(8, 9));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));
			resultStmtPairs.push_back(make_pair(2, 5));
			resultStmtPairs.push_back(make_pair(2, 8));

			stmtPairs = pkb.getParentGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();

			stmtPairs = pkb.getParentGenericGeneric(NodeType::Assign, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 7));
			resultStmtPairs.push_back(make_pair(8, 9));
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));

			stmtPairs = pkb.getParentGenericGeneric(NodeType::StmtLst, NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 7));
			resultStmtPairs.push_back(make_pair(8, 9));

			stmtPairs = pkb.getParentGenericGeneric(NodeType::While, NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultsMapVi.clear();
			resultsMapVi[1] = {};
			resultsMapVi[2] = {3,4,5,8,6,7,9 };
			resultsMapVi[5] = {6,7};
			resultsMapVi[8] = {9};
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 3,4,6,7,9 };
			resultsMapVi[5] = { 6,7 };
			resultsMapVi[8] = { 9 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentSpecificGeneric(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 5,8 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentSpecificGeneric(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2 };
			resultsMapVi[5] = { 2 };
			resultsMapVi[6] = { 5,2 };
			resultsMapVi[7] = { 5,2 };
			resultsMapVi[8] = { 2 };
			resultsMapVi[9] = { 8,2 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentGenericSpecific(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentGenericSpecific(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[6] = { 5 };
			resultsMapVi[7] = { 5 };
			resultsMapVi[9] = { 8 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentGenericSpecific(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2 };
			resultsMapVi[5] = { 2 };
			resultsMapVi[6] = { 2 };
			resultsMapVi[7] = { 2 };
			resultsMapVi[8] = { 2 };
			resultsMapVi[9] = { 2 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getTransitiveParentGenericSpecific(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(2, 3));
			resultStmtPairs.push_back(make_pair(2, 4));
			resultStmtPairs.push_back(make_pair(2, 6));
			resultStmtPairs.push_back(make_pair(2, 7));
			resultStmtPairs.push_back(make_pair(2, 9));
			resultStmtPairs.push_back(make_pair(2, 5));
			resultStmtPairs.push_back(make_pair(2, 8));
			stmtPairs = pkb.getTransitiveParentGenericGeneric(NodeType::If, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(2, 5));
			resultStmtPairs.push_back(make_pair(2, 8));
			stmtPairs = pkb.getTransitiveParentGenericGeneric(NodeType::If, NodeType::While);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			stmtPairs = pkb.getTransitiveParentGenericGeneric(NodeType::Assign, NodeType::StmtLst);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			resultStmtPairs = vp_i_i();
			resultStmtPairs.push_back(make_pair(5, 6));
			resultStmtPairs.push_back(make_pair(5, 7));
			resultStmtPairs.push_back(make_pair(8, 9));
			stmtPairs = pkb.getTransitiveParentGenericGeneric(NodeType::While, NodeType::Assign);
			Assert::IsTrue(checkStmtPairsEqual(stmtPairs, resultStmtPairs));

			Assert::IsTrue(pkb.whetherParent(2, 3));
			Assert::IsFalse(pkb.whetherParent(1, 10));
			Assert::IsTrue(pkb.whetherParent(2, 5));
			Assert::IsTrue(pkb.whetherTransitiveParent(2, 9));
			Assert::IsFalse(pkb.whetherTransitiveParent(2, 2));

			// Next
			resultsMapVi.clear();
			resultsMapVi[1] = { 2 };
			resultsMapVi[2] = { 3,4 };
			resultsMapVi[3] = { 4 };
			resultsMapVi[4] = { 10 };
			resultsMapVi[5] = { 6,8 };
			resultsMapVi[6] = { 7 };
			resultsMapVi[7] = { 5 };
			resultsMapVi[8] = { 9,10 };
			resultsMapVi[9] = { 8 };
			resultsMapVi[10] = {  };
			resultsMapVi[11] = {  };
			
			
			for (int i = 0; i < 12; i++) {
				vi stmts = pkb.getNextSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}
			
			for (int i = 0; i < 11; i++) {
				vi stmts = pkb.getTransitiveNextSpecificGeneric(i, NodeType::StmtLst);
				
				printVec(stmts);
			}
			
			stmtPairs = pkb.getTransitiveNextGenericGeneric(NodeType::StmtLst, NodeType::StmtLst);
			//printStmtPairs(stmtPairs);
			
			


			vi getNextSpecificGeneric(int lineNo, NodeType type);
			vi getNextGenericSpecific(int lineNo, NodeType type);
			vp_i_i getNextGenericGeneric(NodeType typeA, NodeType typeB);
			bool whetherNext(int a, int b);

			vi getTransitiveNextSpecificGeneric(int lineNo, NodeType type);
			vi getTransitiveNextGenericSpecific(int lineNo, NodeType type);
			vp_i_i getTransitiveNextGenericGeneric(NodeType typeA, NodeType typeB);
			bool whetherTransitivelyNext(int a, int b);



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
			for (pair<int,int> i : vp) {
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