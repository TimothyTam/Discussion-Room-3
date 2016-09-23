#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryValidation.h"
#include "RelTable.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(TestValidation) {

public:
	TEST_METHOD(Synonym_Invalid) {
		string query = "Cluster c;assign a; procedure p1; Select a";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Synonym_Valid) {
		string query = "assign a; procedure p1;while w; Select a";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Select_Valid) {
		string query = "assign a; Select a";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Select_Invalid) {
		string query = "assign a; Select s ";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	
	TEST_METHOD(SelectTuple_Valid) {
		string query = "assign a1,a2; Select <a1,a2> ";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectTuple_Invalid) {
		string query = "assign a1,a2; Select <a,a2> ";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(invalid_noDeclaration) {
		string query = "assign a; procedure p1; Select p1 such that Modifies(p,\"a\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Entity_Invalid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,a)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}

	TEST_METHOD(ArgumentNo_Invalid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,a,3)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(ArgumentType_Invalid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,a)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Argument_Valid) {
		string query = "assign a; procedure p1; Select a such that Modifies(p1,\"a\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Valid) {
		string query = "assign a; procedure p1; Select a pattern a(\"x\",_)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Valid_2) {
		string query = "assign a; procedure p; Select a pattern a(\"x\",_\"x\"_)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Invalid) {
		string query = "assign a; procedure p1; Select a pattern a(\"x\",_\"x\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Invalid_2) {
		string query = "assign a; procedure p1; Select a pattern a(\"x\",_\"x_\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	
	TEST_METHOD(PatternWhile_Valid) {
		string query = "assign a;while w; Select a pattern w(\"x\",_)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternWhile_Invalid) {
		string query = "assign a;while w; Select a pattern w(\"x\",x)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Valid) {
		string query = "assign a; procedure p ; Select a with p.procName = \"Second\"";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Valid_2) {
		string query = "assign a; procedure p ; Select a with a.stmt# =12";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Invalid) {
		string query = "assign a; procedure p ; Select a with a.stmt# =\"Second\"";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Invalid_2) {
		string query = "assign a; constant c;prog_line n ; Select a with a.stmt# =c.value and n=\"Second\"";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(MeaninglessQueries_Valid) {
		string query = "stmt s; Select s such that Follows(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(MeaninglessQueries_Invalid) {
		string query = "stmt s1; Select s such that Follows(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	
	TEST_METHOD(SelectBOOLEAN_Valid) {
		string query = "Select BOOLEAN such that Follows(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(FollowStar_Valid) {
		string query = "stmt s;Select s such that Follows*(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(ParentStar_Valid) {
		string query = "stmt s;Select s such that Parent*(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(DeclarationList_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		unordered_map<string,string> decl = check.getDeclaration();
		unordered_map<string, string> table = { {"p1", "procedure"},{ "s", "stmt" },{ "a", "assign" } };
		Assert::IsTrue(decl == table);
	}
	TEST_METHOD(SelectList_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		string decl = check.getSelect();
		string select ="s";
		Assert::IsTrue(decl == select);
	}
	TEST_METHOD(SelectList_Valid2) {
		string query = "procedure p1; stmt s; assign a;Select <p1,    s>";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		string decl = check.getSelect();
		string select = "<p1,s>";
		Assert::IsTrue(decl == select);
	}
	TEST_METHOD(ClauseEnum_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s such that Modifies(1,\"x\") pattern a(\"x\",_)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		vector<QueryUtility::ClauseType> result = check.getClauseEnum();
		vector<QueryUtility::ClauseType> enums = {QueryUtility::CLAUSETYPE_MODIFIES,QueryUtility::CLAUSETYPE_PATTERN_ASSIGN};
		Assert::IsTrue(result == enums);
	}
	TEST_METHOD(ClauseParam_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s such that Modifies(1,\"x\") pattern a(\"x\",_) with p1.procName = \"second\"" ;
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		vector<vector<string>> result = check.getClauseParam();
		vector<vector<string>> param = { { "1","\"x\"" }, {"\"x\"","_"},{"p1.procName","\"second\""} };
		Assert::IsTrue(result == param);
	}
	};
}