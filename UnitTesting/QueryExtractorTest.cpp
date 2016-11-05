#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryExtractor.h"
#include "Query.h"
#include "QueryPair.h"
#include "QueryUtility.h"
#include <algorithm>
#include <vector>

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryExtractor) {
public:
	QueryExtractor extractor = QueryExtractor();
		TEST_METHOD(QEX_GetDeclarationsList) {
			unordered_map<string, QueryUtility::SynonymType> testMap = { { "a",QueryUtility::SYNONYM_TYPE_ASSIGN },
																		 { "s1",QueryUtility::SYNONYM_TYPE_STMT },
																		 { "BOOLEAN", QueryUtility::SYNONYM_TYPE_BOOLEAN } };

			QueryPair qp1 = QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(QueryUtility::SYNONYM_TYPE_STMT, "s1");
			QueryPair qp3 = QueryPair(QueryUtility::SYNONYM_TYPE_BOOLEAN, "BOOLEAN");

			vector<QueryPair> outputList = extractor.getDeclarations(testMap);
			QueryPair o1 = outputList.at(0);
			QueryPair o2 = outputList.at(1);
			QueryPair o3 = outputList.at(2);

			Assert::IsTrue(qp1 == o1);
			Assert::IsTrue(qp2 == o2);
			Assert::IsTrue(qp3 == o3);
		}

		TEST_METHOD(QEX_GetSelects) {
			unordered_map<string, QueryUtility::SynonymType> testMap = { { "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
																		 { "s1", QueryUtility::SYNONYM_TYPE_STMT },
																		 //{ "BOOLEAN", QueryUtility::SYNONYM_TYPE_BOOLEAN },
																		 { "vblabla", QueryUtility::SYNONYM_TYPE_VARIABLE},
																		 { "proc", QueryUtility::SYNONYM_TYPE_PROCEDURE},
																		 { "sel", QueryUtility::SYNONYM_TYPE_STMTLST}};
			string select1 = "a";
			string select2 = "s1";
			string select3 = "<a,s1,vblabla>";
			string select4 = "BOOLEAN";
			string select5 = "coca.procName";
			string select6 = "<proc,chocolate.procName,black.procName>";
			string select7 = "<hi.procName,s1>";
			string select8 = "sel";

			QueryPair qp1 = QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(QueryUtility::SYNONYM_TYPE_STMT, "s1");
			QueryPair qp3 = QueryPair(QueryUtility::SYNONYM_TYPE_VARIABLE, "vblabla");
			QueryPair qp4 = QueryPair(QueryUtility::SYNONYM_TYPE_BOOLEAN, "BOOLEAN");
			QueryPair qp5 = QueryPair(QueryUtility::SYNONYM_TYPE_CALL_PROCNAME, "coca");
			QueryPair qp6 = QueryPair(QueryUtility::SYNONYM_TYPE_PROCEDURE, "proc");
			QueryPair qp7 = QueryPair(QueryUtility::SYNONYM_TYPE_CALL_PROCNAME, "chocolate");
			QueryPair qp8 = QueryPair(QueryUtility::SYNONYM_TYPE_CALL_PROCNAME, "hi");
			QueryPair qp9 = QueryPair(QueryUtility::SYNONYM_TYPE_STMT, "s1");
			QueryPair qp10 = QueryPair(QueryUtility::SYNONYM_TYPE_STMTLST, "sel");

			vector<QueryPair> outputList1 = extractor.getSelects(select1, testMap);
			vector<QueryPair> outputList2 = extractor.getSelects(select2, testMap);
			vector<QueryPair> outputList3 = extractor.getSelects(select3, testMap);
			vector<QueryPair> outputList4 = extractor.getSelects(select4, testMap);
			vector<QueryPair> outputList5 = extractor.getSelects(select5, testMap);
			vector<QueryPair> outputList6 = extractor.getSelects(select6, testMap);
			vector<QueryPair> outputList7 = extractor.getSelects(select7, testMap);
			vector<QueryPair> outputList8 = extractor.getSelects(select8, testMap);


			Assert::IsTrue(qp1 == outputList1.at(0));
			Assert::IsTrue(qp2 == outputList2.at(0));

			Assert::IsTrue(qp1 == outputList3.at(0));
			Assert::IsTrue(qp2 == outputList3.at(1));
			Assert::IsTrue(qp3 == outputList3.at(2));

			Assert::IsTrue(qp4 == outputList4.at(0));
			Assert::IsTrue(qp5 == outputList5.at(0));

			Assert::IsTrue(qp6 == outputList6.at(0));
			Assert::IsTrue(qp7 == outputList6.at(1));

			Assert::IsTrue(qp8 == outputList7.at(0));
			Assert::IsTrue(qp9 == outputList7.at(1));

			Assert::IsTrue(qp10 == outputList8.at(0));

		}

		TEST_METHOD(QEX_CreateQueryParam) {
			unordered_map<string, QueryUtility::SynonymType> testMap = { { "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
																		 { "s1", QueryUtility::SYNONYM_TYPE_STMT },
																		 { "BOOLEAN", QueryUtility::SYNONYM_TYPE_BOOLEAN } };

			string input1 = "_";			// wild card
			string input2 = "\"y\"";		// entity variable name
			string input3 = "a";			// synonym assign
			string input4 = "5";			// entity statement no.

			QueryParam qp1 = QueryParam(QueryUtility::PARAMTYPE_PLACEHOLDER, QueryUtility::SYNONYM_TYPE_NULL, input1);
			QueryParam qp2 = QueryParam(QueryUtility::PARAMTYPE_ENT_NAME, QueryUtility::SYNONYM_TYPE_NULL, input2);
			QueryParam qp3 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_ASSIGN, input3);
			QueryParam qp4 = QueryParam(QueryUtility::PARAMTYPE_ENT_NAME, QueryUtility::SYNONYM_TYPE_NULL, input4);

			QueryParam out1 = extractor.createQueryParam(input1, testMap);
			QueryParam out2 = extractor.createQueryParam(input2, testMap);
			QueryParam out3 = extractor.createQueryParam(input3, testMap);
			QueryParam out4 = extractor.createQueryParam(input4, testMap);

			Assert::IsTrue(qp1 == out1);
			Assert::IsTrue(qp2 == out2);
			Assert::IsTrue(qp3 == out3);
			Assert::IsTrue(qp4 == out4);


		}

		TEST_METHOD(QEX_CreateQueryParamForPatternAssign) {
			unordered_map<string, QueryUtility::SynonymType> testMap = { { "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
																		 { "s1", QueryUtility::SYNONYM_TYPE_STMT },
																		 { "BOOLEAN", QueryUtility::SYNONYM_TYPE_BOOLEAN } };

			string input1 = "_\"x+y\"";
			string input2 = "_\"x+y*z\"_";
			string input3 = "\"x+y+z*a\"_";
			string input4 = "a";

			QueryParam qp1 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_LEFT_OPEN, QueryUtility::SYNONYM_TYPE_NULL, "_\"x+y\"");
			QueryParam qp2 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_BOTH_OPEN, QueryUtility::SYNONYM_TYPE_NULL, "_\"x+y*z\"_");
			QueryParam qp3 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_RIGHT_OPEN, QueryUtility::SYNONYM_TYPE_NULL, "\"x+y+z*a\"_");
			QueryParam qp4 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_ASSIGN, "a");

			QueryParam out1 = extractor.createQueryParamForPatternAssign(input1, testMap);
			QueryParam out2 = extractor.createQueryParamForPatternAssign(input2, testMap);
			QueryParam out3 = extractor.createQueryParamForPatternAssign(input3, testMap);
			QueryParam out4 = extractor.createQueryParamForPatternAssign(input4, testMap);

			Assert::IsTrue(qp1 == out1);
			Assert::IsTrue(qp2 == out2);
			Assert::IsTrue(qp3 == out3);
			Assert::IsTrue(qp4 == out4);

		}

		TEST_METHOD(QEX_CreateQueryParamForWith) {
			unordered_map<string, QueryUtility::SynonymType> decList = { 
			{ "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
			{ "s1", QueryUtility::SYNONYM_TYPE_STMT },
			{ "s2", QueryUtility::SYNONYM_TYPE_STMT },
			{ "pocky1", QueryUtility::SYNONYM_TYPE_PROCEDURE },
			{ "corn", QueryUtility::SYNONYM_TYPE_CONSTANT },
			{ "v", QueryUtility::SYNONYM_TYPE_VARIABLE },
			{ "progline", QueryUtility::SYNONYM_TYPE_PROG_LINE } };

			string input1 = "pocky1.procName";
			string input2 = "a.stmt#";
			string input3 = "v.varName";
			string input4 = "progline";
			string input5 = "5";
			string input6 = "\"RandomProcName\"";

			QueryParam qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_PROCEDURE, "pocky1");
			QueryParam qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_STMTNO, QueryUtility::SYNONYM_TYPE_ASSIGN, "a");
			QueryParam qp3 = QueryParam(QueryUtility::PARAMTYPE_WITH_VARNAME, QueryUtility::SYNONYM_TYPE_VARIABLE, "v");
			QueryParam qp4 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_PROG_LINE, "progline");
			QueryParam qp5 = QueryParam(QueryUtility::PARAMTYPE_NULL, QueryUtility::SYNONYM_TYPE_NULL, "5");
			QueryParam qp6 = QueryParam(QueryUtility::PARAMTYPE_NULL, QueryUtility::SYNONYM_TYPE_NULL, "\"RandomProcName\"");

			QueryParam out1 = extractor.createQueryParamForWith(input1, decList);
			QueryParam out2 = extractor.createQueryParamForWith(input2, decList);
			QueryParam out3 = extractor.createQueryParamForWith(input3, decList);
			QueryParam out4 = extractor.createQueryParamForWith(input4, decList);
			QueryParam out5 = extractor.createQueryParamForWith(input5, decList);
			QueryParam out6 = extractor.createQueryParamForWith(input6, decList);

			Assert::IsTrue(qp1 == out1);
			Assert::IsTrue(qp2 == out2);
			Assert::IsTrue(qp3 == out3);
			Assert::IsTrue(qp4 == out4);
			Assert::IsTrue(qp5 == out5);
			Assert::IsTrue(qp6 == out6);

		}

		TEST_METHOD(QEX_GetClauses_Follows) {
			unordered_map<string, QueryUtility::SynonymType> testMap = { { "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
																		 { "s1", QueryUtility::SYNONYM_TYPE_STMT },
																		 { "s2", QueryUtility::SYNONYM_TYPE_STMT } };

			vector<QueryUtility::ClauseType> testClauseList = {QueryUtility::CLAUSETYPE_FOLLOWS,
															   QueryUtility::CLAUSETYPE_FOLLOWS_STAR
															   };
			vector<vector<string>> testParamList = { {"a", "5"}, {"s1", "s2"}};

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_ASSIGN, "a");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_ENT_NAME, QueryUtility::SYNONYM_TYPE_NULL, "5");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_STMT, "s1");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_STMT, "s2");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_FOLLOWS, "none", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_FOLLOWS_STAR, "none", 2, qc2List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, testMap);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));

		}
		
		TEST_METHOD(QEX_GetClauses_With_StmtNo2Ways) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "a", QueryUtility::SYNONYM_TYPE_ASSIGN }};
			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH, QueryUtility::CLAUSETYPE_WITH };
			vector<vector<string>> testParamList = { {"a.stmt#", "5"}, {"123", "a.stmt#"} };

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_STMTNO, QueryUtility::SYNONYM_TYPE_ASSIGN, "a");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_STMTNO, QueryUtility::SYNONYM_TYPE_NULL, "5");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_STMTNO, QueryUtility::SYNONYM_TYPE_NULL, "123");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_STMTNO, QueryUtility::SYNONYM_TYPE_ASSIGN, "a");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_INT, "none", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_WITH_INT, "none", 2, qc2List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));
		}

		TEST_METHOD(QEX_GetClauses_With_SynonymsOnBothSides) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "variab", QueryUtility::SYNONYM_TYPE_VARIABLE },
				{ "proc123", QueryUtility::SYNONYM_TYPE_PROCEDURE }};

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH, QueryUtility::CLAUSETYPE_WITH };
			vector<vector<string>> testParamList = { { "variab.varName", "proc123.procName" },{ "proc123.procName", "variab.varName" } };

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_VARNAME, QueryUtility::SYNONYM_TYPE_VARIABLE, "variab");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_PROCEDURE, "proc123");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_PROCEDURE, "proc123");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_VARNAME, QueryUtility::SYNONYM_TYPE_VARIABLE, "variab");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STRING, "none", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STRING, "none", 2, qc2List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));

		}

		TEST_METHOD(QEX_GetClauses_With_Ident) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "variab", QueryUtility::SYNONYM_TYPE_VARIABLE },
				{ "proc123", QueryUtility::SYNONYM_TYPE_PROCEDURE } };

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH, QueryUtility::CLAUSETYPE_WITH };
			vector<vector<string>> testParamList = { { "variab.varName", "\"bananacake\"" },{ "proc123.procName", "\"blackbutler\"" } };

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_VARNAME, QueryUtility::SYNONYM_TYPE_VARIABLE, "variab");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_VARNAME, QueryUtility::SYNONYM_TYPE_NULL, "\"bananacake\"");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_PROCEDURE, "proc123");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_NULL, "\"blackbutler\"");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STRING, "none", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STRING, "none", 2, qc2List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));

		}

		TEST_METHOD(QEX_GetClauses_With_Meaningless) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "variab", QueryUtility::SYNONYM_TYPE_VARIABLE },
				{ "proc123", QueryUtility::SYNONYM_TYPE_PROCEDURE } };

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH, QueryUtility::CLAUSETYPE_WITH };
			vector<vector<string>> testParamList = { { "\"sebastian\"", "sebastian" },{ "123", "123" } };

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_NULL, QueryUtility::SYNONYM_TYPE_NULL, "\"sebastian\"");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_NULL, QueryUtility::SYNONYM_TYPE_NULL, "sebastian");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_NULL, QueryUtility::SYNONYM_TYPE_NULL, "123");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_NULL, QueryUtility::SYNONYM_TYPE_NULL, "123");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH, "none", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_WITH, "none", 2, qc2List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));
		}

		TEST_METHOD(QEX_GetClauses_With_Progline) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "ciel", QueryUtility::SYNONYM_TYPE_CONSTANT },
				{ "proc123", QueryUtility::SYNONYM_TYPE_PROCEDURE },
				{ "progliney", QueryUtility::SYNONYM_TYPE_PROG_LINE} };

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH, QueryUtility::CLAUSETYPE_WITH,  QueryUtility::CLAUSETYPE_WITH };
			vector<vector<string>> testParamList = { { "progliney", "123" },{ "ciel.value", "progliney" }, {"1000", "progliney"}};

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_PROG_LINE, "progliney");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_NULL, "123");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_VALUE, QueryUtility::SYNONYM_TYPE_CONSTANT, "ciel");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_PROG_LINE, "progliney");
			QueryParam qc3qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_NULL, "1000");
			QueryParam qc3qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_PROG_LINE, "progliney");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);
			vector<QueryParam> qc3List; qc3List.push_back(qc3qp1); qc3List.push_back(qc3qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_INT, "none", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_WITH_INT, "none", 2, qc2List);
			QueryClause qc3 = QueryClause(QueryUtility::CLAUSETYPE_WITH_INT, "none", 2, qc3List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));
			Assert::IsTrue(qc3 == outputList.at(2));

		}

		TEST_METHOD(QEX_GetClauses_With_AssignEqualProgLine) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "abra", QueryUtility::SYNONYM_TYPE_ASSIGN },
				{ "proc123", QueryUtility::SYNONYM_TYPE_PROCEDURE },
				{ "progliney", QueryUtility::SYNONYM_TYPE_PROG_LINE } };

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH};
			vector<vector<string>> testParamList = { { "abra.stmt#", "progliney" }};

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_STMTNO, QueryUtility::SYNONYM_TYPE_ASSIGN, "abra");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROG_LINE, QueryUtility::SYNONYM_TYPE_PROG_LINE, "progliney");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_INT, "none", 2, qc1List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
		}

		TEST_METHOD(QEX_GetClauses_With_CallProcname) {
			unordered_map<string, QueryUtility::SynonymType> decList = {
				{ "coconut", QueryUtility::SYNONYM_TYPE_CALL }};

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH };
			vector<vector<string>> testParamList = { { "coconut.procName", "\"fishcake\"" } };

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_CALL, "coconut");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_WITH_PROCNAME, QueryUtility::SYNONYM_TYPE_NULL, "\"fishcake\"");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STRING, "none", 2, qc1List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
		}
		

		TEST_METHOD(QEX_GetClauses_PatternAssign) {
			unordered_map<string, QueryUtility::SynonymType> decList = { { "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
			{ "applepear123", QueryUtility::SYNONYM_TYPE_ASSIGN },
			{ "var", QueryUtility::SYNONYM_TYPE_VARIABLE } };
			
			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_PATTERN_ASSIGN,
				QueryUtility::CLAUSETYPE_PATTERN_ASSIGN, QueryUtility::CLAUSETYPE_PATTERN_ASSIGN, QueryUtility::CLAUSETYPE_PATTERN_ASSIGN };

			vector<vector<string>> testParamList = { {"a", "\"x\"", "_\"x+y\""},
													  {"applepear123", "var", "_\"x+y\"_"},
													  {"a", "\"x\"", "\"z+y+x\"_"},
													  {"applepear123", "var", "a+b+c"}};

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_ENT_NAME, QueryUtility::SYNONYM_TYPE_NULL, "\"x\"");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_LEFT_OPEN, QueryUtility::SYNONYM_TYPE_NULL, "_\"x+y\"");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_VARIABLE, "var");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_BOTH_OPEN, QueryUtility::SYNONYM_TYPE_NULL, "_\"x+y\"_");
			QueryParam qc3qp1 = QueryParam(QueryUtility::PARAMTYPE_ENT_NAME, QueryUtility::SYNONYM_TYPE_NULL, "\"x\"");
			QueryParam qc3qp2 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_RIGHT_OPEN, QueryUtility::SYNONYM_TYPE_NULL, "\"z+y+x\"_");
			QueryParam qc4qp1 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_VARIABLE, "var");
			QueryParam qc4qp2 = QueryParam(QueryUtility::PARAMTYPE_PATTERN_STRING_EXACT, QueryUtility::SYNONYM_TYPE_NULL, "a+b+c");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2);
			vector<QueryParam> qc3List; qc3List.push_back(qc3qp1); qc3List.push_back(qc3qp2);
			vector<QueryParam> qc4List; qc4List.push_back(qc4qp1); qc4List.push_back(qc4qp2);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_PATTERN_ASSIGN, "a", 2, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_PATTERN_ASSIGN, "applepear123", 2, qc2List);
			QueryClause qc3 = QueryClause(QueryUtility::CLAUSETYPE_PATTERN_ASSIGN, "a", 2, qc3List);
			QueryClause qc4 = QueryClause(QueryUtility::CLAUSETYPE_PATTERN_ASSIGN, "applepear123", 2, qc4List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));
			Assert::IsTrue(qc3 == outputList.at(2));
			Assert::IsTrue(qc4 == outputList.at(3));

		}

		TEST_METHOD(QEX_GetClauses_PatternIf) {
			unordered_map<string, QueryUtility::SynonymType> decList = { { "iffa", QueryUtility::SYNONYM_TYPE_IF },
																		 { "iffoo", QueryUtility::SYNONYM_TYPE_IF },
																		 { "varb", QueryUtility::SYNONYM_TYPE_VARIABLE}};

			vector<vector<string>> testParamList = {{"iffa", "\"x\"", "_", "_"},
													{"iffoo", "varb", "_", "_"}};
			
			vector<QueryUtility::ClauseType> testClauseList = {QueryUtility::CLAUSETYPE_PATTERN_IF, QueryUtility::CLAUSETYPE_PATTERN_IF};

			QueryParam qc1qp1 = QueryParam(QueryUtility::PARAMTYPE_ENT_NAME, QueryUtility::SYNONYM_TYPE_NULL, "\"x\"");
			QueryParam qc1qp2 = QueryParam(QueryUtility::PARAMTYPE_PLACEHOLDER, QueryUtility::SYNONYM_TYPE_NULL, "_");
			QueryParam qc1qp3 = QueryParam(QueryUtility::PARAMTYPE_PLACEHOLDER, QueryUtility::SYNONYM_TYPE_NULL, "_");
			QueryParam qc2qp1 = QueryParam(QueryUtility::PARAMTYPE_SYNONYM, QueryUtility::SYNONYM_TYPE_VARIABLE, "varb");
			QueryParam qc2qp2 = QueryParam(QueryUtility::PARAMTYPE_PLACEHOLDER, QueryUtility::SYNONYM_TYPE_NULL, "_");
			QueryParam qc2qp3 = QueryParam(QueryUtility::PARAMTYPE_PLACEHOLDER, QueryUtility::SYNONYM_TYPE_NULL, "_");

			vector<QueryParam> qc1List; qc1List.push_back(qc1qp1); qc1List.push_back(qc1qp2); qc1List.push_back(qc1qp3);
			vector<QueryParam> qc2List; qc2List.push_back(qc2qp1); qc2List.push_back(qc2qp2); qc2List.push_back(qc1qp3);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_PATTERN_IF, "iffa", 3, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_PATTERN_IF, "iffoo", 3, qc2List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));
		}

		TEST_METHOD(QEX_IsNumber) {
			bool b1 = extractor.is_number("555");
			bool b2 = extractor.is_number("no");
			Assert::IsTrue(b1);
			Assert::IsFalse(b2);
		}

	};
}