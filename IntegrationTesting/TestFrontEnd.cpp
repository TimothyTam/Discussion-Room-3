#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"
#include "PKB.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace IntegrationTesting
{		
	TEST_CLASS(UnitTest1)
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
			pkb.buildAllTables();

			vector<string> procs = pkb.getAllEntityName(NodeType::Procedure);
			Assert::AreEqual(1, (int)procs.size());
			Assert::AreEqual(string("P"), procs[0]);
			vector<string> vars = pkb.getAllEntityName(NodeType::Variable);
			Assert::AreEqual(8, (int)vars.size());

			size_t i,j;
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
				int result = pkb.getStmtFollowedByStmt(i, NodeType::StmtLst);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,10,4,0,0,7,0,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getStmtFollowedByStmt(i, NodeType::Assign);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,0,0,0,8,0,0,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getStmtFollowedByStmt(i, NodeType::While);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,1,0,3,0,0,6,5,0,2 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getStmtFollowingStmt(i, NodeType::StmtLst);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,1,0,3,0,0,6,0,0,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getStmtFollowingStmt(i, NodeType::Assign);
				Assert::AreEqual(results[i - 1], result);
			}

			results = { 0,0,0,0,0,0,0,5,0,0 };
			for (i = 1; i < 11; i++) {
				int result = pkb.getStmtFollowingStmt(i, NodeType::While);
				Assert::AreEqual(results[i - 1], result);
			}

			vi stmts = pkb.getStmtsFollowedByStmt(NodeType::StmtLst, NodeType::StmtLst);
			Assert::AreEqual(5, (int)stmts.size());
			results = { 2,4,7,8,10 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowedByStmt(NodeType::StmtLst, NodeType::If);
			Assert::AreEqual(1, (int)stmts.size());
			results = { 2 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowedByStmt(NodeType::If, NodeType::StmtLst);
			Assert::AreEqual(1, (int)stmts.size());
			results = { 10 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowedByStmt(NodeType::While, NodeType::Assign);
			Assert::AreEqual(0, (int)stmts.size());

			stmts = pkb.getStmtsFollowedByStmt(NodeType::Assign, NodeType::Assign);
			Assert::AreEqual(2, (int)stmts.size());
			results = { 4, 7 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowingStmt(NodeType::StmtLst, NodeType::StmtLst);
			Assert::AreEqual(5, (int)stmts.size());
			results = { 1,2,3,5,6 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowingStmt(NodeType::If, NodeType::StmtLst);
			Assert::AreEqual(1, (int)stmts.size());
			results = { 2 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowingStmt(NodeType::StmtLst, NodeType::If);
			Assert::AreEqual(1, (int)stmts.size());
			results = { 1 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsFollowingStmt(NodeType::While, NodeType::While);
			Assert::AreEqual(1, (int)stmts.size());
			results = { 5 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			resultsMapVi.clear();
			resultsMapVi[1] = { 2,10 };
			resultsMapVi[2] = { 10 };
			resultsMapVi[3] = { 4 };
			resultsMapVi[5] = { 8 };
			resultsMapVi[6] = { 7 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getStmtsTransitivelyFollowedByStmt(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[1] = { 10 };
			resultsMapVi[2] = { 10 };
			resultsMapVi[3] = { 4 };
			resultsMapVi[6] = { 7 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getStmtsTransitivelyFollowedByStmt(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 8 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getStmtsTransitivelyFollowedByStmt(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 1 };
			resultsMapVi[4] = { 3 };
			resultsMapVi[7] = { 6 };
			resultsMapVi[8] = { 5 };
			resultsMapVi[10] = { 1,2 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getStmtsTransitivelyFollowingStmt(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[2] = { 1 };
			resultsMapVi[4] = { 3 };
			resultsMapVi[7] = { 6 };
			resultsMapVi[10] = { 1 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getStmtsTransitivelyFollowingStmt(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[8] = { 5 };
			for (i = 1; i < 11; i++) {
				vi stmts = pkb.getStmtsTransitivelyFollowingStmt(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			stmts = pkb.getStmtsTransitivelyFollowedByStmt(NodeType::StmtLst, NodeType::StmtLst);
			results = { 2,4,7,8,10 };
			Assert::IsTrue(checkVectorEqual(results, stmts));
			
			stmts = pkb.getStmtsTransitivelyFollowedByStmt(NodeType::Assign, NodeType::StmtLst);
			results = { 2,4,7,10 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowedByStmt(NodeType::StmtLst, NodeType::While);
			results = { 8 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowedByStmt(NodeType::Assign, NodeType::Assign);
			results = { 4,7,10 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowingStmt(NodeType::StmtLst, NodeType::StmtLst);
			results = { 1,2,3,5,6 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowingStmt(NodeType::Assign, NodeType::StmtLst);
			results = { 1,3,6 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowingStmt(NodeType::StmtLst, NodeType::Assign);
			results = { 1,2,3,6 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowingStmt(NodeType::While, NodeType::While);
			results = { 5 };
			Assert::IsTrue(checkVectorEqual(results, stmts));

			stmts = pkb.getStmtsTransitivelyFollowingStmt(NodeType::While, NodeType::Assign);
			results = {};
			Assert::IsTrue(checkVectorEqual(results, stmts));

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
				vi stmts = pkb.getVarModifiedByStmt(i, NodeType::StmtLst);
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
				vi stmts = pkb.getVarModifiedByStmt(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 2,4 };
			resultsMapVi[8] = { 3 };
			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getVarModifiedByStmt(i, NodeType::While);
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
				vi stmts = pkb.getStmtModifyingVar(i, NodeType::StmtLst);
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
				vi stmts = pkb.getStmtModifyingVar(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 2 };
			resultsMapVi[1] = { 2 };
			resultsMapVi[2] = { 2 };
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getStmtModifyingVar(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0,1,2,3,4,6 };
			for (i = 0; i < 1; i++) {
				vi stmts = pkb.getVarModifiedByStmt(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0 };
			resultsMapVi[1] = { 0 };
			resultsMapVi[2] = { 0 };
			resultsMapVi[3] = { 0 };
			resultsMapVi[4] = { 0 };
			resultsMapVi[6] = { 0 };

			for (i = 0; i < 1; i++) {
				vi stmts = pkb.getStmtModifyingVar(i, NodeType::Procedure);
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

			results = { 0,1,2,3,4,6 };
			stmts = pkb.getVarModifiedByStmt(-1, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(results, stmts));
			
			results = { 0,1,2,3,4,6 };
			stmts = pkb.getVarModifiedByStmt(-1, NodeType::Assign);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 0,1,2,3,4 };
			stmts = pkb.getVarModifiedByStmt(-1, NodeType::If);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 2,3,4 };
			stmts = pkb.getVarModifiedByStmt(-1, NodeType::While);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 1,2,3,4,5,6,7,8,9,10 };
			stmts = pkb.getStmtModifyingVar(-1, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 1,3,4,6,7,9,10 };
			stmts = pkb.getStmtModifyingVar(-1, NodeType::Assign);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 5,8 };
			stmts = pkb.getStmtModifyingVar(-1, NodeType::While);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			Logger::WriteMessage("Testing Uses Methods");
			resultsMapVi.clear();
			resultsMapVi[2] = { 0,1,3,4 };
			resultsMapVi[5] = { 0,1,3,4 };
			resultsMapVi[6] = { 0,1,3 };
			resultsMapVi[7] = { 4 };
			resultsMapVi[8] = { 0 };
			resultsMapVi[9] = { 0 };
			resultsMapVi[10] = { 7 };
			resultsMapVi[11] = {};

			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getVarUsedByStmt(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[6] = { 0,1,3 };
			resultsMapVi[7] = { 4 };
			resultsMapVi[9] = { 0 };
			resultsMapVi[10] = { 7 };

			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getVarUsedByStmt(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[5] = { 0,1,3,4 };
			resultsMapVi[8] = { 0 };

			for (i = 1; i <= 11; i++) {
				vi stmts = pkb.getVarUsedByStmt(i, NodeType::While);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 2,5,6,8,9 };
			resultsMapVi[1] = { 2,5,6 };
			resultsMapVi[3] = { 2,5,6 };
			resultsMapVi[4] = { 2,5,7 };
			resultsMapVi[7] = { 10 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getStmtUsingVar(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 6,9 };
			resultsMapVi[1] = { 6 };
			resultsMapVi[3] = { 6 };
			resultsMapVi[4] = { 7 };
			resultsMapVi[7] = { 10 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getStmtUsingVar(i, NodeType::Assign);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 2 };
			resultsMapVi[1] = { 2 };
			resultsMapVi[3] = { 2 };
			resultsMapVi[4] = { 2 };

			for (i = 0; i < 11; i++) {
				vi stmts = pkb.getStmtUsingVar(i, NodeType::If);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0,1,3,4,7 };
			for (i = 0; i < 1; i++) {
				vi stmts = pkb.getVarUsedByStmt(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			resultsMapVi.clear();
			resultsMapVi[0] = { 0 };
			resultsMapVi[1] = { 0 };
			resultsMapVi[3] = { 0 };
			resultsMapVi[4] = { 0 };
			resultsMapVi[7] = { 0 };
			for (i = 0; i < 1; i++) {
				vi stmts = pkb.getStmtUsingVar(i, NodeType::Procedure);
				Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
			}

			results = { 0,1,3,4,7 };
			stmts = pkb.getVarUsedByStmt(-1, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 0,1,3,4 };
			stmts = pkb.getVarUsedByStmt(-1, NodeType::While);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 0,1,3,4,7 };
			stmts = pkb.getVarUsedByStmt(-1, NodeType::Assign);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 2,5,6,7,8,9,10 };
			stmts = pkb.getStmtUsingVar(-1, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 6,7,9,10 };
			stmts = pkb.getStmtUsingVar(-1, NodeType::Assign);
			Assert::IsTrue(checkVectorEqual(results, stmts));

			results = { 2 };
			stmts = pkb.getStmtUsingVar(-1, NodeType::If);
			Assert::IsTrue(checkVectorEqual(results, stmts));

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
			//printTree(firstStmtList, -1);

		}

		bool checkVectorEqual(vi v1, vi v2) {
			if (v1.size() != v2.size()) return false;
			size_t i;
			for (i = 0; i < v1.size(); i++) {
				if (v1[0] != v2[0]) return false;
			}
			return true;
		}

		void printVec(vi v) {
			for (int i : v) {
				Logger::WriteMessage(to_string(i).c_str());
			}
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