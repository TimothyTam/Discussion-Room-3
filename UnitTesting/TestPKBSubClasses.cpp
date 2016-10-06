#include "stdafx.h"
#include "CppUnitTest.h"
#include "Follow.h"
#include "Modify.h"
#include "Uses.h"
#include "Parent.h"
#include "PKB.h"
#include <algorithm>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;



namespace UnitTesting
{
	//Call before each TEST_MODULE. Should not have anything special to put here.
	TEST_MODULE_INITIALIZE(methodName) {
		Logger::WriteMessage("In Module Initialize");
	}

	TEST_CLASS(TestPKB) {
public:
	//Did not work as intended. All Nodes are unallocated after method ends.
	TEST_CLASS_INITIALIZE(generateRequireResources) {
		Logger::WriteMessage("In Class Initialize");

		//Generate AST MANUALLY. :(
		TNode root(NodeType::Program);
		TNode proc(NodeType::Procedure), stmtLst1(NodeType::StmtLst);
		TNode a1(NodeType::Assign), a1Var(NodeType::Variable), c1(NodeType::Constant);
		TNode if1(NodeType::If), if1Var(NodeType::Variable), stmtLstThen(NodeType::StmtLst), stmtLstElse(NodeType::StmtLst);
		TNode a2(NodeType::Assign), a2Var(NodeType::Variable), c2(NodeType::Constant);
		TNode a3(NodeType::Assign), a3Var(NodeType::Variable), c3(NodeType::Constant);
		TNode w1(NodeType::While), w1Var(NodeType::Variable), stmtLstW1(NodeType::StmtLst);
		TNode a4(NodeType::Assign), a4Var(NodeType::Variable), a4k(NodeType::Variable), a4z(NodeType::Variable);
		TNode a4x(NodeType::Variable), a4Plus(NodeType::Plus), a4Times(NodeType::Times);
		TNode a5(NodeType::Assign), a5Var(NodeType::Variable), a5n(NodeType::Variable);
		TNode w2(NodeType::While), w2Var(NodeType::Variable), stmtLstW2(NodeType::StmtLst);
		TNode a6(NodeType::Assign), a6Var(NodeType::Variable), a6z(NodeType::Variable);
		TNode a7(NodeType::Assign), a7Var(NodeType::Variable), a7c(NodeType::Variable);

		PKB::getInstance().addStatement("",&a1);
		PKB::getInstance().addStatement("",&if1);
		PKB::getInstance().addStatement("", &a2);
		PKB::getInstance().addStatement("", &a3);
		PKB::getInstance().addStatement("", &w1);
		PKB::getInstance().addStatement("", &a4);
		PKB::getInstance().addStatement("", &a5);
		PKB::getInstance().addStatement("", &w2);
		PKB::getInstance().addStatement("", &a6);
		PKB::getInstance().addStatement("", &a7);

		//Set Values
		proc.value = 0;

		a1Var.value = 0; c1.value = 1;
		a2Var.value = 1; c2.value = 0;
		a3Var.value = 0; c3.value = 8;
		a4Var.value = 2; a4k.value = 3; a4z.value = 0; a4x.value = 1;
		a5Var.value = 4; a5n.value = 4;
		a6Var.value = 3; a6z.value = 0;
		a7Var.value = 6; a7c.value = 7;

		if1Var.value = 0; w1Var.value = 1; w2Var.value = 5;

		//Set Parents
		proc.parent = &root;
		stmtLst1.parent = &proc;
		a1.parent = &stmtLst1;
		a1Var.parent = &a1;
		c1.parent = &a1;
		a2.parent = &stmtLstThen; a2Var.parent = &a2; c2.parent = &a2;
		a3.parent = &stmtLstThen; a3Var.parent = &a3; c3.parent = &a3;

		w1.parent = &stmtLstElse; w1Var.parent = &w1; stmtLstW1.parent = &w1;
		a4.parent = &stmtLstW1; a4Var.parent = &a4; a4k.parent = &a4Plus;
		a4x.parent = &a4Times; a4z.parent = &a4Times; a4Times.parent = &a4Plus; a4Plus.parent = &a4;
		a5.parent = &stmtLstW1; a5Var.parent = &a5; a5n.parent = &a5;

		w2.parent = &stmtLstElse; w2Var.parent = &w2; stmtLstW2.parent = &w2;
		a6.parent = &stmtLstW2; a6Var.parent = &a6; a6z.parent = &a6;

		if1.parent = &stmtLst1;
		if1Var.parent = &if1; stmtLstThen.parent = &if1; stmtLstElse.parent = &if1;

		a7.parent = &stmtLst1; a7Var.parent = &a7; a7c.parent = &a7;

		//Set Statment Number
		a1.statementNumber = 1; if1.statementNumber = 2; a2.statementNumber = 3; a3.statementNumber = 4; w1.statementNumber = 5;
		a4.statementNumber = 6; a5.statementNumber = 7; w2.statementNumber = 8; a6.statementNumber = 9; a7.statementNumber = 10;

		//Push_back to form tree
		a7.childs.push_back(&a7Var); a7.childs.push_back(&a7c);
		a6.childs.push_back(&a6Var); a6.childs.push_back(&a6z);
		a5.childs.push_back(&a5Var); a5.childs.push_back(&a5n);
		a4Times.childs.push_back(&a4z); a4Times.childs.push_back(&a4x); a4Plus.childs.push_back(&a4k); a4Plus.childs.push_back(&a4Times);
		a4.childs.push_back(&a4Var); a4.childs.push_back(&a4Plus);
		a3.childs.push_back(&a3Var); a3.childs.push_back(&c3);
		a2.childs.push_back(&a2Var); a2.childs.push_back(&c2);
		a1.childs.push_back(&a1Var); a1.childs.push_back(&c1);

		stmtLstThen.childs.push_back(&a2); stmtLstThen.childs.push_back(&a3);

		stmtLstW1.childs.push_back(&a4); stmtLstW1.childs.push_back(&a5);
		stmtLstW2.childs.push_back(&a6);

		w1.childs.push_back(&w1Var); w1.childs.push_back(&stmtLstW1);
		w2.childs.push_back(&w2Var); w2.childs.push_back(&stmtLstW2);

		stmtLstElse.childs.push_back(&w1); stmtLstElse.childs.push_back(&w2);

		if1.childs.push_back(&if1Var); if1.childs.push_back(&stmtLstThen); if1.childs.push_back(&stmtLstElse);

		stmtLst1.childs.push_back(&a1); stmtLst1.childs.push_back(&if1); stmtLst1.childs.push_back(&a7);
		proc.childs.push_back(&stmtLst1);
		root.childs.push_back(&proc);

		//Generate the tables here. I can't seem to keep all the TNodes across different tests.
		Follow::getInstance().generateFollowTable(&root);
		Modify::getInstance().generateModifyTable(&root);
		Use::getInstance().generateUseTable(&root);
		Parent::getInstance().generateParentData(&root);

		//I don't think this is the correct way to test. But it is okay, I have manually generated an AST for the 10 line code below.
		/*
		procedure P{
		1.	z = 1;
		2.	if z then{
		3.    x = 0;
		4.    z = 8;
			} else {
		5.		while x {
		6.			y = k + z * x;
		7.			n = n;
				}
		8.		while p{
		9.			k = z;
				}
			}
		10. b = c;
		}

		VarTable
		0 z
		1 x
		2 y
		3 k
		4 n
		5 p
		6 b
		7 c
		*/
	}

	//These are the test methods that will be run. I hope it works.		

	/*	FOLLOW TABLE
		follows(1, 2)-- 1 | 2			-- 2 | 1
		follows(2, 10)-- 2 | 10...etc
		follows(3, 4)
		follows(5, 8)
		follows(6, 7)

		Such that							-- Follow* Table	-- FollowedBy* Table
		follows*(1, 10) and all the above	-- 1 | 2, 10		-- 1 |
											-- 2 | 10			-- 2 | 1
											-- 3 | 4			-- 3 |
											-- 4 |				-- 4 | 3
											-- 5 | 8			-- 5 |
											-- 6 | 7			-- 6 |
											-- 7 |				-- 7 | 6
											-- 8 |				-- 8 | 5
											-- 9 |				-- 9 |
											-- 10 |				-- 10 | 1, 2
		*/
	TEST_METHOD(TestFollow_Valid) {
		Logger::WriteMessage("In TestFollow");
		Follow& inst = Follow::getInstance();
		size_t i;

		vi results;
		map_i_vi resultsMapVi;

		results = { 2,10,4,0,8,7,0,0,0,0,0 };
		for (i = 1; i < 11; i++) {
			int result = inst.getFollowSpecificGeneric(i, NodeType::StmtLst);
			Assert::AreEqual(results[i - 1], result);
		}

		results = { 0,1,0,3,0,0,6,5,0,2 };
		for (i = 1; i < 11; i++) {
			int result = inst.getFollowGenericSpecific(i, NodeType::StmtLst);
			Assert::AreEqual(results[i - 1], result);
		}

		resultsMapVi.clear();
		resultsMapVi[1] = { 2,10 };
		resultsMapVi[2] = { 10 };
		resultsMapVi[3] = { 4 };
		resultsMapVi[5] = { 8 };
		resultsMapVi[6] = { 7 };
		for (i = 1; i < 11; i++) {
			vi stmts = inst.getTransitiveFollowSpecificGeneric(i, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		resultsMapVi.clear();
		resultsMapVi[2] = { 1 };
		resultsMapVi[4] = { 3 };
		resultsMapVi[7] = { 6 };
		resultsMapVi[8] = { 5 };
		resultsMapVi[10] = { 1,2 };
		for (i = 1; i < 11; i++) {
			vi stmts = inst.getTransitiveFollowGenericSpecific(i, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		Assert::IsTrue(inst.whetherFollows(1,2));
		Assert::IsFalse(inst.whetherFollows(1, 10));
		Assert::IsTrue(inst.whetherFollows(2, 10));
		Assert::IsTrue(inst.whetherTransitivelyFollows(1, 10));
		Assert::IsTrue(inst.whetherTransitivelyFollows(1, 2));
	}

	/*
		Such that				-- Modify Table
		modifies(1, z)			-- 1 | 0
		modifies(2, z|x|y|n|k)	-- 2 | 0,1,2,3,4
		modifies(3, x)			-- 3 | 1
		modifies(4, z)			-- 4 | 0
		modifies(5, y|n)		-- 5 | 2,4
		modifies(6 ,y)			-- 6 | 2
		modifies(7, n)			-- 7 | 4
		modifies(8, k)			-- 8 | 3
		modifies(9, k)			-- 9 | 3
		modifies(10,b)			-- 10| 6
		procModifies(z|x|y|n|k|b) -- proc 0 | 0,1,2,3,4,6
	*/
	TEST_METHOD(TestModify_Valid) {
		Logger::WriteMessage("In TestModify");
		Modify& inst = Modify::getInstance();
		size_t i;
		map_i_vi resultsMapVi;

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
			vi stmts = inst.getModifySpecificGeneric(i, NodeType::StmtLst);
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
			vi stmts = inst.getModifyGenericSpecific(i, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		resultsMapVi.clear();
		resultsMapVi[0] = { 0,1,2,3,4,6 };
		for (i = 0; i < 1; i++) {
			vi stmts = inst.getModifySpecificGeneric(i, NodeType::Procedure);
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
			vi stmts = inst.getModifyGenericSpecific(i, NodeType::Procedure);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		Assert::IsTrue(inst.whetherProcModifies(0, 0));
		Assert::IsTrue(inst.whetherProcModifies(0, 1));
		Assert::IsTrue(inst.whetherProcModifies(0, 2));
		Assert::IsTrue(inst.whetherProcModifies(0, 3));
		Assert::IsTrue(inst.whetherProcModifies(0, 4));
		Assert::IsFalse(inst.whetherProcModifies(0, 5));
		Assert::IsTrue(inst.whetherProcModifies(0, 6));

		Assert::IsFalse(inst.whetherStmtModifies(0, 0));
		Assert::IsTrue(inst.whetherStmtModifies(1, 0));
		Assert::IsFalse(inst.whetherStmtModifies(1, 1));
		Assert::IsFalse(inst.whetherStmtModifies(1, 2));
		Assert::IsTrue(inst.whetherStmtModifies(5, 2));
		Assert::IsTrue(inst.whetherStmtModifies(5, 4));
		Assert::IsTrue(inst.whetherStmtModifies(10, 6));



	}

	/*	Such that				-- Uses Table
		uses(2, k|z|x|n)		-- 2 | 0, 1, 3, 4, 5
		uses(5, k|z|x|n)		-- 5 | 0, 1, 3, 4
		uses(6, k|z|x)			-- 6 | 0, 1, 3
		uses(7, n)				-- 7 | 4
		uses(8, z)				-- 8 | 0, 5
		uses(9, z)				-- 9 | 0
		uses(10, c)				-- 10 | 7
		procUses(k|z|x|n|c)		--proc 0 | 0, 1, 3, 4, 5, 7
	*/
	TEST_METHOD(TestUses_Valid) {
		// TODO: Your test code here
		Logger::WriteMessage("In TestUses");
		Use& inst = Use::getInstance();
		size_t i;
		map_i_vi resultsMapVi;

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
			vi stmts = inst.getUsesSpecificGeneric(i, NodeType::StmtLst);
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
			vi stmts = inst.getUsesGenericSpecific(i, NodeType::StmtLst);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		resultsMapVi.clear();
		resultsMapVi[0] = { 0,1,3,4,5,7 };
		for (i = 0; i < 1; i++) {
			vi stmts = inst.getUsesSpecificGeneric(i, NodeType::Procedure);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		resultsMapVi.clear();
		resultsMapVi[0] = { 0 };
		resultsMapVi[1] = { 0 };
		resultsMapVi[3] = { 0 };
		resultsMapVi[4] = { 0 };
		resultsMapVi[5] = { 0 };
		resultsMapVi[7] = { 0 };

		for (i = 0; i < 1; i++) {
			vi stmts = inst.getUsesGenericSpecific(i, NodeType::Procedure);
			Assert::IsTrue(checkVectorEqual(resultsMapVi[i], stmts));
		}

		Assert::IsTrue(inst.whetherStmtUses(2, 0));
		Assert::IsTrue(inst.whetherStmtUses(2, 1));
		Assert::IsFalse(inst.whetherStmtUses(2, 2));
		Assert::IsTrue(inst.whetherStmtUses(2, 3));
		Assert::IsTrue(inst.whetherStmtUses(2, 4));

		Assert::IsTrue(inst.whetherProcUses(0, 0));
		Assert::IsTrue(inst.whetherProcUses(0, 1));
		Assert::IsFalse(inst.whetherProcUses(0, 2));
		Assert::IsTrue(inst.whetherProcUses(0, 3));
		Assert::IsTrue(inst.whetherProcUses(0, 4));
		Assert::IsFalse(inst.whetherProcUses(1, 4));
		Assert::IsFalse(inst.whetherProcUses(1, 7));
	}
	
	/*	Stmt		-- Transitive Child		--Transitive Parents
	1				-- {}					-- {}
	2				-- { 3,4,5,6,7,8,9 }	-- {}
	3				-- {}					-- {2}
	4				-- {}					-- {2}
	5				-- {6,7}				-- {2}
	6				-- {}					-- {2,5}
	7				-- {}					-- {2,5}
	8				-- {9}					-- {2}
	9				-- {}					-- {2,8}
	10				-- {}					-- {}
	
	*/
	TEST_METHOD(TestParentWithSpecificStmt) {
		// TODO: Your test code here
		Logger::WriteMessage("In TestParent");
		Parent& parent = Parent::getInstance();

		map_i_vi resultsMapVi;
		resultsMapVi.clear();

		// Check Child*
		resultsMapVi[1] = {};
		resultsMapVi[2] = { 3,4,5,6,7,8,9 };
		resultsMapVi[3] = {};
		resultsMapVi[4] = {};
		resultsMapVi[5] = {6,7 };
		resultsMapVi[6] = { };
		resultsMapVi[7] = { };
		resultsMapVi[8] = { 9 };
		resultsMapVi[9] = { };
		resultsMapVi[10] = { };
		
		for (size_t i = 1; i <= 10; i++) {
			vi stmts = parent.getTransitiveParentSpecificGeneric(i,NodeType::StmtLst);
			printVec(stmts);
			Assert::IsTrue(checkVectorContentEqual(resultsMapVi[i], stmts));
		}

		// Check Parent*
		resultsMapVi[1] = {};
		resultsMapVi[2] = {};
		resultsMapVi[3] = {2};
		resultsMapVi[4] = {2};
		resultsMapVi[5] = {2};
		resultsMapVi[6] = {2,5};
		resultsMapVi[7] = {2,5};
		resultsMapVi[8] = {2};
		resultsMapVi[9] = {2,8};
		resultsMapVi[10] = {};

		for (size_t i = 1; i <= 10; i++) {
			vi stmts = parent.getTransitiveParentGenericSpecific(i, NodeType::StmtLst);
			Assert::IsTrue(checkVectorContentEqual(resultsMapVi[i], stmts));
		}


	}

	TEST_METHOD(TestFollow_Invalid) {
		Follow& inst = Follow::getInstance();
		vi stmt = inst.getStmtsFollowedByStmt(NodeType::Invalid, NodeType::Assign);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getStmtsFollowedByStmt(NodeType::Invalid, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		Assert::AreEqual(0, inst.getFollowSpecificGeneric(-1, NodeType::StmtLst));
		Assert::AreEqual(0, inst.getFollowSpecificGeneric(-2, NodeType::StmtLst));
		Assert::AreEqual(0, inst.getFollowSpecificGeneric(11, NodeType::StmtLst));

		stmt = inst.getStmtsFollowingStmt(NodeType::StmtLst, NodeType::Invalid);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getStmtsFollowingStmt(NodeType::While, NodeType::Invalid);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		Assert::AreEqual(0, inst.getFollowGenericSpecific(-1, NodeType::StmtLst));
		Assert::AreEqual(0, inst.getFollowGenericSpecific(-2, NodeType::StmtLst));
		Assert::AreEqual(0, inst.getFollowGenericSpecific(11, NodeType::StmtLst));

		stmt = inst.getStmtsTransitivelyFollowingStmt(NodeType::StmtLst, NodeType::Invalid);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getStmtsTransitivelyFollowingStmt(NodeType::Invalid, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getTransitiveFollowGenericSpecific(-1, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getTransitiveFollowGenericSpecific(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getTransitiveFollowGenericSpecific(11, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));

		stmt = inst.getStmtsTransitivelyFollowedByStmt(NodeType::StmtLst, NodeType::Invalid);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getStmtsTransitivelyFollowedByStmt(NodeType::Invalid, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getTransitiveFollowSpecificGeneric(-1, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getTransitiveFollowSpecificGeneric(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getTransitiveFollowSpecificGeneric(11, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
	}

	TEST_METHOD(TestModify_Invalid) {
		Modify& inst = Modify::getInstance();
		vi stmt = inst.getModifySpecificGeneric(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getModifySpecificGeneric(11, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getModifySpecificGeneric(5, NodeType::Invalid);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getModifyGenericSpecific(9, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getModifyGenericSpecific(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
	}

	TEST_METHOD(TestUse_Invalid) {
		Use& inst = Use::getInstance();
		vi stmt = inst.getUsesSpecificGeneric(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getUsesSpecificGeneric(11, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getUsesSpecificGeneric(5, NodeType::Invalid);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getUsesGenericSpecific(9, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getUsesGenericSpecific(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
	}

	TEST_METHOD(TestParent_Invalid) {
		Parent& inst = Parent::getInstance();
		vi stmt = inst.getParentSpecificGeneric(-1, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getParentSpecificGeneric(-2, NodeType::StmtLst);
		Assert::IsTrue(checkVectorEqual(stmt, vi()));
		stmt = inst.getParentSpecificGeneric(11, NodeType::StmtLst);
		//Assert::IsTrue(checkVectorEqual(stmt, vi()));
		Assert::AreEqual(-1, inst.getParentGenericSpecific(-1, NodeType::StmtLst));
		Assert::AreEqual(-1, inst.getParentGenericSpecific(-2, NodeType::StmtLst));
		Assert::AreEqual(-1, inst.getParentGenericSpecific(11, NodeType::StmtLst));

	}

	void printVec(vi v) {
		for (int i : v) {
			Logger::WriteMessage(to_string(i).c_str());
		}
		Logger::WriteMessage("_");
	}



	// Call after each TEST_CLASS
	TEST_CLASS_CLEANUP(methodName) {
		// Unlink the AST? Nah, end the program :P
		Logger::WriteMessage("In Module Cleanup");
	}

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