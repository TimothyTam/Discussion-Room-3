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
		/*
		TEST_METHOD(QE_GetDeclarationsList) {
			QueryExtractor extractor = QueryExtractor();
			unordered_map<string, string> testMap = {{"assign", "a"}, {"stmt", "s1"}};
			QueryPair qp1 = QueryPair(SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(SYNONYM_TYPE_STMT, "s1");
			vector<QueryPair> ansList;
			ansList.push_back(qp1);
			ansList.push_back(qp2);

			vector<QueryPair> resultList = extractor.getDeclarations(testMap);


		}*/

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
			string input = "Select a1 w, such that Parent(a, x)";
			string ans = "a1";
			string ans2 = "w";
			string result = extractor.sanitiseForSelects(input).at(0);
			string result2 = extractor.sanitiseForSelects(input).at(1);
			Assert::IsTrue(result == ans);
			Assert::IsTrue(result2 == ans2);
		}

	};
}