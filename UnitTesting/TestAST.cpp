#include "stdafx.h"
#include "CppUnitTest.h"
#include "AST.h"
#include "TNode.h"
#include <stdio.h>

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

		TEST_METHOD(TestCreateAST)
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

			TNode* astRoot = ast.rootNode;
			// varIndex of x is one
			Assert::AreEqual(astRoot->childs[0]->childs[0]->childs[0]->childs[0]->value, 0);
			
			// varIndex of m must be 3
			Assert::AreEqual(astRoot->childs[0]->childs[0]->childs[1]->childs[1]->childs[1]->childs[1]->childs[0]->childs[0]->value, 3);
		
		}

	};
}