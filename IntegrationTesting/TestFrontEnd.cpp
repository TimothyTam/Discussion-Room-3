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
			//Nowhere is calling this build all tables yet. Where do we want to call this?
			pkb.buildAllTables();

			vector<string> procs = pkb.getAllEntity(NodeType::Procedure);
			Assert::AreEqual(1, (int)procs.size());
			Assert::AreEqual(string("P"), procs[0]);
			vector<string> vars = pkb.getAllEntity(NodeType::Variable);
			Assert::AreEqual(8, (int)vars.size());

			size_t i,j;
			vector<string> variables = { "z","x","y","k","n","p","b","c" };
			for (i = 0; i < 8; i++) {
				Assert::AreEqual(string(variables[i]), vars[i]);
			}

			Assert::AreEqual(10, pkb.getStmtCount());
			Assert::AreEqual(10, (int)pkb.getAllEntityForStmt(NodeType::StmtLst).size());
			Assert::AreEqual(7, (int)pkb.getAllEntityForStmt(NodeType::Assign).size());
			Assert::AreEqual(1, (int)pkb.getAllEntityForStmt(NodeType::If).size());
			Assert::AreEqual(2, (int)pkb.getAllEntityForStmt(NodeType::While).size());
			Assert::AreEqual(0, (int)pkb.getAllEntityForStmt(NodeType::Call).size());

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
			printVec(stmts);
			Assert::IsTrue(checkVectorEqual(results, stmts));
			

			Assert::IsTrue(pkb.whetherFollows(1, 2));
			Assert::IsFalse(pkb.whetherFollows(1, 10));
			Assert::IsTrue(pkb.whetherFollows(2, 10));
			Assert::IsTrue(pkb.whetherTransitivelyFollows(1, 10));
			Assert::IsTrue(pkb.whetherTransitivelyFollows(1, 2));
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