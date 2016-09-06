#include "stdafx.h"
#include "CppUnitTest.h"
#include "Validation.h"
#include "RelTable.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestValidation) {
public:
	TEST_METHOD(Synonym_Invalid) {
		string query = "Cluster c;assign a; procedure p1; Select a";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Synonym_Valid) {
		string query = "assign a; procedure p1;while w; Select a";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Select_Valid) {
		string query = "assign a; Select a ";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Select_Invalid) {
		string query = "assign a; Select s ";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectTuple_Valid) {
		string query = "assign a1,a2; Select <a1,a2> ";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectTuple_Invalid) {
		string query = "assign a1,a2; Select <a,a2> ";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(invalid_noDeclaration) {
		string query = "assign a; procedure p1; Select p1 Modifies(p,\"a\")";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Entity_Invalid) {
		string query = "assign a; procedure p1; Select a such that Modifes(p1,a)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}

	TEST_METHOD(ArgumentNo_Invalid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,a,3)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(ArgumentType_Invalid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,a)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Argument_Valid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,\"a\")";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Valid) {
		string query = "assign a; procedure p1; Select a such that pattern a(\"x\",_)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Valid_2) {
		string query = "assign a; procedure p; Select a such that pattern a(\"x\",_\"x\"_)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Invalid) {
		string query = "assign a; procedure p1; Select a such that pattern a(\"x\",_\"x\")";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Invalid_2) {
		string query = "assign a; procedure p1; Select a such that a(\"x\",_\"x_\")";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternWhile_Valid) {
		string query = "assign a;while w; Select a such that w(\"x\",_)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternWhile_Invalid) {
		string query = "assign a;while w; Select a such that w(\"x\",x)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Valid) {
		string query = "assign a; procedure p ; Select a with p.procName = \"Second\"";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Valid_2) {
		string query = "assign a; procedure p ; Select a with a.stmt# =12";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Invalid) {
		string query = "assign a; procedure p ; Select a with a.stmt# =\"Second\"";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Invalid_2) {
		string query = "assign a; constant c;prog_line n ; Select a with a.stmt# =c.value and n=\"Second\"";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(MeaninglessQueries_Valid) {
		string query = "stmt s; Select s such that Follows(4,5)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(MeaninglessQueries_Invalid) {
		string query = "stmt s1; Select s such that Follows(4,5)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectBOOLEAN_Valid) {
		string query = "Select BOOLEAN such that Follows(4,5)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(FollowStar_Valid) {
		string query = "stmt s;Select s such that Follows*(4,5)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(ParentStar_Valid) {
		string query = "stmt s;Select s such that Parent*(4,5)";
		validation check = validation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	};
}