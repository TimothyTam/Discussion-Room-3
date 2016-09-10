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

			vector<QueryPair> outputList = extractor.getDeclarations(testMap);
			QueryPair o1 = outputList.at(0);
			QueryPair o2 = outputList.at(1);

			Assert::IsTrue(qp1.getSynonymType() == o1.getSynonymType());
			Assert::IsTrue(qp1.getValue() == o1.getValue());
			Assert::IsTrue(qp2.getSynonymType() == o2.getSynonymType());
			Assert::IsTrue(qp2.getValue() == o2.getValue());


		}

		TEST_METHOD(QE_RemoveDeclarations_True) {
			QueryExtractor extractor = QueryExtractor();
			string query = "assign a; stmt s1; Select a something else here";
			string ans = "Select a something else here";
			string output = extractor.removeDeclarations(query);
			Assert::IsTrue(output == ans);
		}

		TEST_METHOD(QE_DetermineSynonymType) {
			QueryExtractor extractor = QueryExtractor();

			string input = "assign";
			string input2 = "procedure";
			string input3 = "invalid";

			SynonymType ans = SYNONYM_TYPE_ASSIGN;
			SynonymType ans2 = SYNONYM_TYPE_PROCEDURE;
			SynonymType ans3 = SYNONYM_TYPE_NULL;

			SynonymType output = extractor.determineSynonymType(input);
			SynonymType output2 = extractor.determineSynonymType(input2);
			SynonymType output3 = extractor.determineSynonymType(input3);

			Assert::IsTrue(output == ans);
			Assert::IsTrue(output2 == ans2);
			Assert::IsTrue(output3 == ans3);
		}

		TEST_METHOD(QE_SanitiseSelects) {
			QueryExtractor extractor = QueryExtractor();
			string input = "Select a1 w; <ha haw> such that Parent(a, x)";
			string ans = "a1";
			string ans2 = "w";
			string ans3 = "ha";
			string ans4 = "haw";

			string output = extractor.sanitiseForSelects(input).at(0);
			string output2 = extractor.sanitiseForSelects(input).at(1);
			string output3 = extractor.sanitiseForSelects(input).at(2);
			string output4 = extractor.sanitiseForSelects(input).at(3);

			Assert::IsTrue(output == ans);
			Assert::IsTrue(output2 == ans2);
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

		TEST_METHOD(QE_RemoveSpaces) {
			QueryExtractor extractor = QueryExtractor();
			string input = "Select v such that modifies (s, v)";
			string ans = "Selectvsuchthatmodifies(s,v)";
			string output = extractor.removeSpaces(input);
			Assert::IsTrue(ans == output);
		}

		TEST_METHOD(QE_CreateQueryParam) {
			QueryExtractor extractor = QueryExtractor();
			unordered_map<string, SynonymType> testmap;
			testmap.insert(std::pair<string, SynonymType>("a1", SYNONYM_TYPE_ASSIGN));
			testmap.insert(std::pair<string, SynonymType>("if", SYNONYM_TYPE_IF));
			extractor.decHashMap = testmap;

			QueryParam qp1 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_ASSIGN, "a1");
			QueryParam qp2 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_IF, "if");
			QueryParam qp3 = QueryParam(PARAMTYPE_PLACEHOLDER, SYNONYM_TYPE_NULL, "_");
			QueryParam qp4 = QueryParam(PARAMTYPE_ENT_NAME, SYNONYM_TYPE_NULL, "\"x\"");

			// add future tests here

			QueryParam output1 = extractor.createQueryParam("a1");
			QueryParam output2 = extractor.createQueryParam("if");
			QueryParam output3 = extractor.createQueryParam("_");
			QueryParam output4 = extractor.createQueryParam("\"x\"");

			Assert::IsTrue(qp1.getParamType() == output1.getParamType());
			Assert::IsTrue(qp1.getParamValue() == output1.getParamValue());
			Assert::IsTrue(qp2.getParamType() == output2.getParamType());
			Assert::IsTrue(qp2.getParamValue() == output2.getParamValue());
			Assert::IsTrue(qp3.getParamType() == output3.getParamType());
			Assert::IsTrue(qp3.getParamValue() == output3.getParamValue());
			Assert::IsTrue(qp4.getParamType() == output4.getParamType());
			Assert::IsTrue(qp4.getParamValue() == output4.getParamValue());


		}

	};
}