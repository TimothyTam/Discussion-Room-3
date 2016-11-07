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
		string query = "assign a; procedure p1;while w; select a";
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
		string query = "assign a1,a2; Select <   a1,   a2> ";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectTuple_Valid1) {
		string query = "assign a;call call;stmt s1; Select <a, call.procName, s1.stmt#>";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		string decl = check.getSelect();
		string select = "<a,call.procName,s1>";
		Assert::IsTrue(decl == select);
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
		string query = "assign a; procedure p1; Select a such that modifies(   p1  ,  \"a\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternAssign_Valid) {
		string query = "assign a; procedure p1; Select a pattern a(   \"x\"    ,   _)";
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
	TEST_METHOD(PatternAssign_Valid_3) {
		string query = "assign a; procedure p1; Select a pattern a(\"x\", _\"x+2\"_)";
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
		string query = "assign a;while w; Select a pattern w(\"x\",_) pattern a(\"x\", _\"x+2\"_)";
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
		string query = "assign a; procedure p ; Select a WiTh a.stmt# =12";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(With_Valid_3) {
		string query = "assign a; prog_line n ; Select a WiTh n =12";
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
	TEST_METHOD(SelectBOOLEAN_Valid_2) {
		string query = "Select booLEAN such that Follows(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(FollowStar_Valid) {
		string query = "stmt s;Select s SUch That Follows*(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(ParentStar_Valid) {
		string query = "stmt s;Select s such that Parent*(4,5	)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SuchThatSpellingError) {
		string query = "stmt s;Select s suc that Parent*(4,5)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(PatternSpellingError) {
		string query = "stmt s;Select s patten a(_,\"x\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(extra) {
		string query = "if if1;select boolean pattern if1(\"c\",_,_)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(extra_1) {
		string query = "assign a;Select a pattern a(_,_\"e*(f - g)\"_)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		vector<vector<string>> result = check.getClauseParam();
		vector<vector<string>> param = { { "a","_","_\"e*(f-g)\"_" } };
		Assert::IsTrue(result == param);
	}
	TEST_METHOD(extra_2) {
		string query = "assign a,a1;Select a pattern a(\"c\",_) and a1(_,\"b - a\")";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(WithSpellingError) {
		string query = "stmt s;Select s wth s.stmt#=1";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(ANDLeftBlank_Invalid) {
		string query = "stmt s;Select s with s.stmt#=1 and such that Modifies(\"x\",s)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectAttrName_Valid) {
		string query = "stmt s;Select s.stmt#";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectAttrName_Valid2) {
		string query = "procedure p;Select p.procName";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(SelectAttrName_InValid) {
		string query = "procedure p;Select p.stmt#";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(Keyword_Invalid) {
		string query = "stmt s;Select s such that s.stmt#=1 with Modifies(\"x\",s)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(DeclarationList_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		unordered_map<string, QueryUtility::SynonymType> decl = check.getDeclaration();
		unordered_map<string, QueryUtility::SynonymType> table = { { "p1", QueryUtility::SYNONYM_TYPE_PROCEDURE },{ "s", QueryUtility::SYNONYM_TYPE_STMT },{ "a", QueryUtility::SYNONYM_TYPE_ASSIGN } };
		Assert::IsTrue(decl == table);
	}
	TEST_METHOD(SelectList_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		string decl = check.getSelect();
		string select = "s";
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
		vector<QueryUtility::ClauseType> enums = { QueryUtility::CLAUSETYPE_MODIFIES,QueryUtility::CLAUSETYPE_PATTERN_ASSIGN };
		Assert::IsTrue(result == enums);
	}
	TEST_METHOD(ClauseParam_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s PatTERN a(\"x\",_) such that Modifies(1,\"x\") with p1.procName = \"second\"";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		vector<vector<string>> result = check.getClauseParam();
		vector<vector<string>> param = { { "a","\"x\"","_" },{ "1","\"x\"" },{ "p1.procName","\"second\"" } };
		Assert::IsTrue(result == param);
	}
	TEST_METHOD(PatternClauseParam_Valid) {
		string query = "procedure p1; stmt s; assign a;Select s pattern a(\"x\",\"x   +		1\") such that Modifies(1,\"x\")  with p1.procName = \"second\"";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		vector<vector<string>> result = check.getClauseParam();
		vector<vector<string>> param = { { "a", "\"x\"","\"x+1\"" },{ "1","\"x\"" },{ "p1.procName","\"second\"" } };
		Assert::IsTrue(result == param);
	}
	TEST_METHOD(SameDeclaration_InValid) {
		string query = "procedure p1; stmt p1; assign a;Select a";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = false;
		Assert::IsTrue(valid == ans);
	}
	TEST_METHOD(MultipleClause_valid) {
		string query = "assign a1,a2,a3;stmt s1,s2,s3;variable v1,v2,v3;Select<s1,s2,v2> such that Uses(s3,v1) and Uses(5,\"y\") such that Follows(3,4) pattern a1(v2, _\"x + y\"_) such that Next(a1,a2) with a2.stmt#=20 such that Modifies(a3,v3) pattern a3(\"z\", _)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		unordered_map<string, QueryUtility::SynonymType> decl = check.getDeclaration();
		unordered_map<string, QueryUtility::SynonymType> table = { { "a1", QueryUtility::SYNONYM_TYPE_ASSIGN },{ "a2", QueryUtility::SYNONYM_TYPE_ASSIGN } ,{ "a3", QueryUtility::SYNONYM_TYPE_ASSIGN } ,
		{ "s1", QueryUtility::SYNONYM_TYPE_STMT } ,{ "s2", QueryUtility::SYNONYM_TYPE_STMT },{ "s3", QueryUtility::SYNONYM_TYPE_STMT },
		{ "v1", QueryUtility::SYNONYM_TYPE_VARIABLE },{ "v2", QueryUtility::SYNONYM_TYPE_VARIABLE },{ "v3", QueryUtility::SYNONYM_TYPE_VARIABLE } };
		Assert::IsTrue(decl == table);
		string sel = check.getSelect();
		string select = "<s1,s2,v2>";
		Assert::IsTrue(sel == select);
		vector<QueryUtility::ClauseType> resultenum = check.getClauseEnum();
		vector<QueryUtility::ClauseType> enums = { QueryUtility::CLAUSETYPE_USES,QueryUtility::CLAUSETYPE_USES,QueryUtility::CLAUSETYPE_FOLLOWS,QueryUtility::CLAUSETYPE_PATTERN_ASSIGN,QueryUtility::CLAUSETYPE_NEXT,QueryUtility::CLAUSETYPE_WITH,QueryUtility::CLAUSETYPE_MODIFIES,QueryUtility::CLAUSETYPE_PATTERN_ASSIGN };
		Assert::IsTrue(resultenum == enums);
		vector<vector<string>> result = check.getClauseParam();
		vector<vector<string>> param = { { "s3","v1" },{ "5","\"y\"" },{ "3","4" },{ "a1","v2","_\"x+y\"_" },
		{ "a1","a2" },{ "a2.stmt#","20" },{ "a3","v3" },{ "a3","\"z\"","_" } };
		Assert::IsTrue(result == param);
	}
	TEST_METHOD(MultipleClause_Valid_1) {
		string query = "assign a1, a2, a3; stmt s1, s2, s3; variable v1, v2, v3; Select <s1, s2, v2> such that Uses(s3, v1) and Uses(5, \"y\") such that Follows(3, 4) pattern a1(v2, _\"x + y\"_) and a3(\"z\", _) such that Next(a1, a2) with a2.stmt#  = 20 and a1.stmt# = 21 such that Modifies(a3, v3) ";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
		unordered_map<string, QueryUtility::SynonymType> decl = check.getDeclaration();
		unordered_map<string, QueryUtility::SynonymType> table = { { "a1", QueryUtility::SYNONYM_TYPE_ASSIGN },{ "a2", QueryUtility::SYNONYM_TYPE_ASSIGN } ,{ "a3", QueryUtility::SYNONYM_TYPE_ASSIGN } ,
		{ "s1", QueryUtility::SYNONYM_TYPE_STMT } ,{ "s2", QueryUtility::SYNONYM_TYPE_STMT },{ "s3", QueryUtility::SYNONYM_TYPE_STMT },
		{ "v1", QueryUtility::SYNONYM_TYPE_VARIABLE },{ "v2", QueryUtility::SYNONYM_TYPE_VARIABLE },{ "v3", QueryUtility::SYNONYM_TYPE_VARIABLE } };
		Assert::IsTrue(decl == table);
		string sel = check.getSelect();
		string select = "<s1,s2,v2>";
		Assert::IsTrue(sel == select);
		vector<QueryUtility::ClauseType> resultenum = check.getClauseEnum();
		vector<QueryUtility::ClauseType> enums = { QueryUtility::CLAUSETYPE_USES,QueryUtility::CLAUSETYPE_USES,QueryUtility::CLAUSETYPE_FOLLOWS,QueryUtility::CLAUSETYPE_PATTERN_ASSIGN,QueryUtility::CLAUSETYPE_PATTERN_ASSIGN,QueryUtility::CLAUSETYPE_NEXT,QueryUtility::CLAUSETYPE_WITH,QueryUtility::CLAUSETYPE_WITH,QueryUtility::CLAUSETYPE_MODIFIES };
		Assert::IsTrue(resultenum == enums);
		vector<vector<string>> result = check.getClauseParam();
		vector<vector<string>> param = { { "s3","v1" },{ "5","\"y\"" },{ "3","4" },{ "a1", "v2","_\"x+y\"_" } ,{ "a3", "\"z\"","_" },
		{ "a1","a2" },{ "a2.stmt#","20" },{ "a1.stmt#","21" },{ "a3","v3" } };
		Assert::IsTrue(result == param);
	}
	TEST_METHOD(longQueries_valid) {
	//	string query = "stmt s1,s2,s3; Select <s1, s2, s3> with 1 =s1.stmt#  and s2.stmt# = 15 and s2.stmt# = 15";
	// string query = "stmt s6, s5, s4, s3, s2, s1; while w1; if if1; Select <s1, s5> such that Next*(s3, s5) and Next*(s4, s6) and Next*(s3, s4) and Next*(s3, s4) and Next*(s1, s3) and Next*(s1, s2) and Follows*(s1, s2)  with 1 =s1.stmt#  and s2.stmt# = 15 and s2.stmt# = 15 pattern if1(\"b\", _, _) and if1(\"b\", _, _) and if1(\"b\", _, _) and if1(\"b\", _, _) and if1(\"b\", _, _)";
	//	string query = "if if1; Select if1 pattern if1(\"b\", _, _) and if1(\"b\", _, _) and if1(\"b\", _, _) and if1(\"b\", _, _) and if1(\"b\", _, _)";
		string query = "stmt s1,s2; Select s1 such that Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2) and Follows*(s1, s2)";
		QueryValidation check = QueryValidation();
		bool valid = check.isValidQuery(query);
		bool ans = true;
		Assert::IsTrue(valid == ans);
	}
	};
}