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
																		 { "BOOLEAN", QueryUtility::SYNONYM_TYPE_BOOLEAN },
																		 { "vblabla", QueryUtility::SYNONYM_TYPE_VARIABLE} };
			string select1 = "a";
			string select2 = "s1";
			string select3 = "<a,s1,vblabla>";

			QueryPair qp1 = QueryPair(QueryUtility::SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(QueryUtility::SYNONYM_TYPE_STMT, "s1");
			QueryPair qp3 = QueryPair(QueryUtility::SYNONYM_TYPE_VARIABLE, "vblabla");

			vector<QueryPair> outputList1 = extractor.getSelects(select1, testMap);
			vector<QueryPair> outputList2 = extractor.getSelects(select2, testMap);
			vector<QueryPair> outputList3 = extractor.getSelects(select3, testMap);

			Assert::IsTrue(qp1 == outputList1.at(0));
			Assert::IsTrue(qp2 == outputList2.at(0));

			Assert::IsTrue(qp1 == outputList3.at(0));
			Assert::IsTrue(qp2 == outputList3.at(1));
			Assert::IsTrue(qp3 == outputList3.at(2));

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

		TEST_METHOD(QEX_GetClauses_With) {
			unordered_map<string, QueryUtility::SynonymType> decList = { { "a", QueryUtility::SYNONYM_TYPE_ASSIGN },
																		 { "s1", QueryUtility::SYNONYM_TYPE_STMT },
																		 { "s2", QueryUtility::SYNONYM_TYPE_STMT }, 
																		 { "pocky1", QueryUtility::SYNONYM_TYPE_PROCEDURE },
																		 { "corn", QueryUtility::SYNONYM_TYPE_CONSTANT },
																		 { "v", QueryUtility::SYNONYM_TYPE_VARIABLE },
																		 { "call1", QueryUtility::SYNONYM_TYPE_CALL } };

			vector<QueryUtility::ClauseType> testClauseList = { QueryUtility::CLAUSETYPE_WITH,
																QueryUtility::CLAUSETYPE_WITH,
																QueryUtility::CLAUSETYPE_WITH,
																QueryUtility::CLAUSETYPE_WITH,
																QueryUtility::CLAUSETYPE_WITH,
																QueryUtility::CLAUSETYPE_WITH
																};

			vector<vector<string>> testParamList = {{"a.stmt#", "5"}, {"v.varName","vara"}, {"s1.stmt#", "1"}, 
													{"pocky1.procName", "First"}, {"call1.procName", "somename"},
													{"corn.value", "10"} };

			QueryParam qc1p1 = QueryParam(QueryUtility::PARAMTYPE_WITH, QueryUtility::SYNONYM_TYPE_ASSIGN, "5");
			QueryParam qc2p1 = QueryParam(QueryUtility::PARAMTYPE_WITH, QueryUtility::SYNONYM_TYPE_VARIABLE, "vara");
			QueryParam qc3p1 = QueryParam(QueryUtility::PARAMTYPE_WITH, QueryUtility::SYNONYM_TYPE_STMT, "1");
			QueryParam qc4p1 = QueryParam(QueryUtility::PARAMTYPE_WITH, QueryUtility::SYNONYM_TYPE_PROCEDURE, "First");
			QueryParam qc5p1 = QueryParam(QueryUtility::PARAMTYPE_WITH, QueryUtility::SYNONYM_TYPE_CALL, "somename");
			QueryParam qc6p1 = QueryParam(QueryUtility::PARAMTYPE_WITH, QueryUtility::SYNONYM_TYPE_CONSTANT, "10");

			vector<QueryParam> qc1List; qc1List.push_back(qc1p1);
			vector<QueryParam> qc2List; qc2List.push_back(qc2p1);
			vector<QueryParam> qc3List; qc3List.push_back(qc3p1);
			vector<QueryParam> qc4List; qc4List.push_back(qc4p1);
			vector<QueryParam> qc5List; qc5List.push_back(qc5p1);
			vector<QueryParam> qc6List; qc6List.push_back(qc6p1);

			QueryClause qc1 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STMTNO, "a", 1, qc1List);
			QueryClause qc2 = QueryClause(QueryUtility::CLAUSETYPE_WITH_VARNAME, "v", 1, qc2List);
			QueryClause qc3 = QueryClause(QueryUtility::CLAUSETYPE_WITH_STMTNO, "s1", 1, qc3List);
			QueryClause qc4 = QueryClause(QueryUtility::CLAUSETYPE_WITH_PROCNAME, "pocky1", 1, qc4List);
			QueryClause qc5 = QueryClause(QueryUtility::CLAUSETYPE_WITH_PROCNAME, "call1", 1, qc5List);
			QueryClause qc6 = QueryClause(QueryUtility::CLAUSETYPE_WITH_VALUE, "corn", 1, qc6List);

			vector<QueryClause> outputList = extractor.getClauses(testClauseList, testParamList, decList);

			Assert::IsTrue(qc1 == outputList.at(0));
			Assert::IsTrue(qc2 == outputList.at(1));
			Assert::IsTrue(qc3 == outputList.at(2));
			Assert::IsTrue(qc4 == outputList.at(3));
			Assert::IsTrue(qc5 == outputList.at(4));
			Assert::IsTrue(qc6 == outputList.at(5));

		}

	};
}