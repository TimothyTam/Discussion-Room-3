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

			//vector<QueryPair> ansList;
			//ansList.push_back(qp1);
			//ansList.push_back(qp2);

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

		TEST_METHOD(QEX_GetClauses1) {

		}

	};
}