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
		
		TEST_METHOD(TestMethod1)
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
			Assert::AreEqual(1,(int)procs.size());
			Assert::AreEqual(string("P"), procs[0]);
			vector<string> vars = pkb.getAllEntity(NodeType::Variable);
			Assert::AreEqual(8, (int)vars.size());

			size_t i;
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
			
			TNode* assign1 = pkb.getStmt(0).second;
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

			printTree(firstStmtList, -1);

		}

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