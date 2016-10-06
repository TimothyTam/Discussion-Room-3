#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"
#include "TNode.h"
#include <stdio.h>
#include "PKB.h"
#include <algorithm>
#include "Parent.h"
#include "MainQuery.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestAST)
	{
	public:

		/* Sample SIMPLE program:

		procedure p1{
		1	x = y + z;
		2	while z{
		3		y = x + 2;
		4		if y{
		5			m = 2;
		}
		}
		6	y = z;

		p1:Program        -root
		|
		p1:Procedure
		|
		stmtlst-------------------assignAt6
		|          \                  |  \
		assignAt1  whileAt2           y   z
		|   |     /   |
		|  Plus  z  stmtlstAt2
		x:  | \      |        \
		y  z  assignAt3   ifAt4
		/   |       /  |
		y   Plus    y  then:stmtlstAt4
		/ |          |
		x  2       assignAt5
		/   |
		m    2
		}*/

		TEST_CLASS_INITIALIZE(TestCreateAST)
		{

			AST& ast = AST::getInstance();
			TNode* root = ast.createEntityNode(NULL, NodeType::Program, "Not necessary");
			TNode* p1 = ast.createEntityNode(root, NodeType::Procedure, "p1");
			TNode* stmtlst = ast.createEntityNode(p1, NodeType::StmtLst, "");

			//stmt#1
			TNode* assignAt1 = ast.createEntityNode(stmtlst, NodeType::Assign, "");
			TNode* xAt1 = ast.createEntityNode(assignAt1, NodeType::Variable, "x");
			TNode* plusAt1 = ast.createEntityNode(assignAt1, NodeType::Plus, "");
			TNode* yAt1 = ast.createEntityNode(plusAt1, NodeType::Variable, "y");
			TNode* zAt1 = ast.createEntityNode(plusAt1, NodeType::Variable, "z");

			//stmt#2
			TNode* whileAt2 = ast.createEntityNode(stmtlst, NodeType::While, "");
			TNode* zAt2 = ast.createEntityNode(whileAt2, NodeType::Variable, "z");
			TNode* stmtlstAt2 = ast.createEntityNode(whileAt2, NodeType::StmtLst, "");

			//stmt#3
			TNode* assignAt3 = ast.createEntityNode(stmtlstAt2, NodeType::Assign, "");
			TNode* yAt3 = ast.createEntityNode(assignAt3, NodeType::Variable, "y");
			TNode* plusAt3 = ast.createEntityNode(assignAt3, NodeType::Plus, "");
			TNode* xAt3 = ast.createEntityNode(plusAt3, NodeType::Variable, "x");
			TNode* twoAt3 = ast.createConstantNode(plusAt3, NodeType::Constant, 2);

			//stmt#4
			TNode* ifAt4 = ast.createEntityNode(stmtlstAt2, NodeType::If, "");
			TNode* yAt4 = ast.createEntityNode(ifAt4, NodeType::Variable, "y");
			TNode* stmtlstAt4 = ast.createEntityNode(ifAt4, NodeType::StmtLst, "then");

			//stmt#5
			TNode* assignAt5 = ast.createEntityNode(stmtlstAt4, NodeType::Assign, "");
			TNode* mAt5 = ast.createEntityNode(assignAt5, NodeType::Variable, "m");
			TNode* twoAt5 = ast.createConstantNode(assignAt5, NodeType::Constant, 2);

			//stmt#6
			TNode* assignAt6 = ast.createEntityNode(stmtlst, NodeType::Assign, "");
			TNode* yAt6 = ast.createEntityNode(assignAt6, NodeType::Variable, "y");
			TNode* zAt6 = ast.createEntityNode(assignAt6, NodeType::Variable, "z");

			TNode* astRoot = ast.getRootNode();

			PKB::getInstance().addStatement("", assignAt1);
			PKB::getInstance().addStatement("", whileAt2);
			PKB::getInstance().addStatement("", assignAt3);
			PKB::getInstance().addStatement("", ifAt4);
			PKB::getInstance().addStatement("", assignAt5);
			PKB::getInstance().addStatement("", assignAt6);







			// varIndex of x is one
			Assert::AreEqual(astRoot->childs[0]->childs[0]->childs[0]->childs[0]->value, 0);

			// varIndex of m must be 3
			Assert::AreEqual(astRoot->childs[0]->childs[0]->childs[1]->childs[1]->childs[1]->childs[1]->childs[0]->childs[0]->value, 3);

		}

		TEST_METHOD(TestStmtLst) {
			// select w such that parent(w,a)
			// check stmtNumbers
			for (int i = 1; i <= 6; i++) {
				Assert::AreEqual(PKB::getInstance().getStmt(i).second->statementNumber, i);
			}

			Assert::AreEqual(PKB::getInstance().getNodeTypeOfStmt(1) == NodeType::Assign, true);
			Assert::AreEqual(PKB::getInstance().getStmt(2).second->type == NodeType::While, true);
			Assert::AreEqual(PKB::getInstance().getStmt(3).second->type == NodeType::Assign, true);
			Assert::AreEqual(PKB::getInstance().getStmt(4).second->type == NodeType::If, true);
			Assert::AreEqual(PKB::getInstance().getStmt(5).second->type == NodeType::Assign, true);
			Assert::AreEqual(PKB::getInstance().getStmt(6).second->type == NodeType::Assign, true);
		}
		
		TEST_METHOD(TestParentGenericTypes) {
			// select w such that parent(w,a)
			Parent& parent = Parent::getInstance();
			parent.generateParentData(AST::getInstance().getRootNode());

			map_i_vi resultsMapVi;
			resultsMapVi.clear();

			// Check Child*
			resultsMapVi[1] = {};
			resultsMapVi[2] = { 3,4,5 };
			resultsMapVi[3] = {};
			resultsMapVi[4] = { 5 };
			resultsMapVi[5] = {};
			resultsMapVi[6] = {};


			for (size_t i = 1; i <= 6; i++) {
				vi stmts = parent.getTransitiveParentSpecificGeneric(i, NodeType::StmtLst);
				Assert::IsTrue(checkVectorContentEqual(resultsMapVi[i], stmts));
			}

			//select s1 such that parent(s1,s2)
			vi results = { 2,4 };
			vi stmts = parent.getParentOfStmt(NodeType::StmtLst, NodeType::StmtLst);
			Assert::IsTrue(checkVectorContentEqual(stmts, results));

			//select s2 such that parent*(w,s2)
			results = { 3,4,5 };
			stmts = parent.getTransitiveChildOfStmt(NodeType::While, NodeType::StmtLst);
			Assert::IsTrue(checkVectorContentEqual(stmts, results));


		}

		/*TEST_METHOD(TestEvaluator) {
			MainQuery mQuery = MainQuery();
			list<string>qresult;
			mQuery.processQuery("assign a; Select a such that Modifies(a,\"y\")", qresult);
			string s = "meo";
			Assert::AreEqual(qresult.size(), (unsigned int)1);
			Assert::AreEqual(qresult.front(), s);
		}
*/
		bool checkVectorContentEqual(vi v1, vi v2) {
			sort(v1.begin(), v1.end());
			sort(v2.begin(), v2.end());
			return checkVectorEqual(v1, v2);
		}

		bool checkVectorEqual(vi v1, vi v2) {
			if (v1.size() != v2.size()) return false;
			size_t i;
			for (i = 0; i < v1.size(); i++) {
				if (v1[0] != v2[0]) return false;
			}
			return true;
		}
	};
}