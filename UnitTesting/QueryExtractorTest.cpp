#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryExtractor.h"
#include "Query.h"
#include "QueryPair.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(TestQueryExtractor) {
	public:
		
		TEST_METHOD(QE_GetDeclarationsList) {
			QueryExtractor extractor = QueryExtractor();
			unordered_map<string, string> testMap = {{"assign", "a"}, {"stmt", "s1"}};
			QueryPair qp1 = QueryPair(SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(SYNONYM_TYPE_STMT, "s1");

			vector<QueryPair> resultList = extractor.getDeclarations(testMap);
			QueryPair o1 = resultList.at(0);
			QueryPair o2 = resultList.at(1);
			Assert::IsTrue(qp1.getSynonymType() == o1.getSynonymType());
			Assert::IsTrue(qp1.getValue() == o1.getValue());
			Assert::IsTrue(qp2.getSynonymType() == o2.getSynonymType());
			Assert::IsTrue(qp2.getValue() == o2.getValue());


		}

		TEST_METHOD(QE_RemoveDeclarations_True) {
			QueryExtractor extractor = QueryExtractor();
			string query = "assign a; stmt s1; Select a zzz stupid adhakhjdjhhda";
			string ans = "Select a zzz stupid adhakhjdjhhda";
			//string ans = "hi";
			string result = extractor.removeDeclarations(query);
			Assert::IsTrue(result == ans);
		}

		TEST_METHOD(QE_DetermineSynonymType) {
			QueryExtractor extractor = QueryExtractor();
			string input = "assign";
			SynonymType result = extractor.determineSynonymType(input);
			SynonymType ans = SYNONYM_TYPE_ASSIGN;
			Assert::IsTrue(result == ans);
		}

		TEST_METHOD(QE_SanitiseSelects) {
			QueryExtractor extractor = QueryExtractor();
			string input = "Select a1 w; <ha haw> such that Parent(a, x)";
			string ans = "a1";
			string ans2 = "w";
			string ans3 = "ha";
			string ans4 = "haw";
			string result = extractor.sanitiseForSelects(input).at(0);
			string result2 = extractor.sanitiseForSelects(input).at(1);
			string result3 = extractor.sanitiseForSelects(input).at(2);
			string result4 = extractor.sanitiseForSelects(input).at(3);
			Assert::IsTrue(result == ans);
			Assert::IsTrue(result2 == ans2);
		}

		TEST_METHOD(QE_GetSelects) {
			QueryExtractor extractor = QueryExtractor();
			unordered_map<string, string> testMap = { { "a", "assign" },{ "s1", "stmt" } };
			string input = "Select a s1 such that Parent(a, x)";

			QueryPair qp1 = QueryPair(SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(SYNONYM_TYPE_STMT, "s1");
			vector<QueryPair> ansList;
			ansList.push_back(qp1); ansList.push_back(qp2);

			vector<QueryPair> outputList = extractor.getSelects(testMap, input);
			QueryPair o1 = outputList.at(0);
			QueryPair o2 = outputList.at(1);

			Assert::IsTrue(qp1.getSynonymType() == o1.getSynonymType());
			Assert::IsTrue(qp1.getValue() == o1.getValue());
			Assert::IsTrue(qp2.getSynonymType() == o2.getSynonymType());
			Assert::IsTrue(qp2.getValue() == o2.getValue());
		}

	};
}