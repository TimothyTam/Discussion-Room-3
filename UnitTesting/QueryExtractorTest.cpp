#include "stdafx.h"
#include "CppUnitTest.h"
#include "QueryExtractor.h"
#include "Query.h"
#include "QueryPair.h"
#include <algorithm>
#include <vector>

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
			string input2 = "Select <a, a2> such that Modifies(a, \"v\") pattern a (\"x\", _)";
			string input3 = "Select zxc asd zxc     such that Uses( _ ,_ ) pattern a1(xv, ak )";

			string ans = "Selectvsuchthatmodifies(s,v)";
			string ans2 = "Select<a,a2>suchthatModifies(a,\"v\")patterna(\"x\",_)";
			string ans3 = "SelectzxcasdzxcsuchthatUses(_,_)patterna1(xv,ak)";

			string output = extractor.removeSpaces(input);
			string output2 = extractor.removeSpaces(input2);
			string output3 = extractor.removeSpaces(input3);

			Assert::IsTrue(ans == output);
			Assert::IsTrue(ans2 == output2);
			Assert::IsTrue(ans3 == output3);
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

			Assert::IsTrue(qp1 == output1);
			Assert::IsTrue(qp2 == output2);
			Assert::IsTrue(qp3 == output3);
			Assert::IsTrue(qp4 == output4);

		}

		TEST_METHOD(QE_GetClauses) {
			QueryExtractor extractor = QueryExtractor();

			// ==== Initialising attribute of extractor instance ==== //
			unordered_map<string, SynonymType> testmap;
			testmap.insert(std::pair<string, SynonymType>("a1", SYNONYM_TYPE_ASSIGN));
			testmap.insert(std::pair<string, SynonymType>("if", SYNONYM_TYPE_IF));
			testmap.insert(std::pair<string, SynonymType>("v", SYNONYM_TYPE_VARIABLE));
			testmap.insert(std::pair<string, SynonymType>("s1", SYNONYM_TYPE_STMT));
			extractor.decHashMap = testmap;

			// ==== Test Strings ==== //
			string input = "such that Modifies(v, s1) such that Uses(\"x\", _)";
			string input2 = "pattern a1 (\"x\", _)";
			string input3 = "such that Parent (s1 , 5) pattern a1 ( v   , _ ) such that Modifies(a1, \"b\")";

			// ==== Creating QueryParam objects for each parameter detected ==== //
			QueryParam qp1 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_VARIABLE, "v");
			QueryParam qp2 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_STMT, "s1");
			QueryParam qp3 = QueryParam(PARAMTYPE_ENT_NAME, SYNONYM_TYPE_NULL, "\"x\"");
			QueryParam qp4 = QueryParam(PARAMTYPE_PLACEHOLDER, SYNONYM_TYPE_NULL, "_");
			// input3
			QueryParam input3qp1 = QueryParam(PARAMTYPE_ENT_NAME, SYNONYM_TYPE_NULL, "5");
			QueryParam input3qp2 = QueryParam(PARAMTYPE_ENT_NAME, SYNONYM_TYPE_NULL, "\"b\"");
			QueryParam input3qp3 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_ASSIGN, "a1");


			// ==== Creating ParametersList for each clause ==== //
			// input1
			vector<QueryParam> clause1List;
			clause1List.push_back(qp1);
			clause1List.push_back(qp2);
			vector<QueryParam> clause2List;
			clause2List.push_back(qp3);
			clause2List.push_back(qp4);

			// input2
			vector<QueryParam> input2clause1list;
			input2clause1list.push_back(qp3);
			input2clause1list.push_back(qp4);

			// input3
			vector<QueryParam> input3clause1list;
			input3clause1list.push_back(qp2);
			input3clause1list.push_back(input3qp1);
			vector<QueryParam> input3clause2list;
			input3clause2list.push_back(qp1);
			input3clause2list.push_back(qp4);
			vector<QueryParam> input3clause3list;
			input3clause3list.push_back(input3qp3);
			input3clause3list.push_back(input3qp2);

			// ==== Creating QueryClause objects for each clause ==== //
			QueryClause qc1 = QueryClause(CLAUSETYPE_MODIFIES, 2, clause1List);
			QueryClause qc2 = QueryClause(CLAUSETYPE_USES, 2, clause2List);
			QueryClause qc3 = QueryClause(CLAUSETYPE_PATTERN_ASSIGN, 2, input2clause1list);
			QueryClause input3qc1 = QueryClause(CLAUSETYPE_PARENT, 2, input3clause1list);
			QueryClause input3qc2 = QueryClause(CLAUSETYPE_PATTERN_ASSIGN, 2, input3clause2list);
			QueryClause input3qc3 = QueryClause(CLAUSETYPE_MODIFIES, 2, input3clause3list);

			// ==== Creating Answerlists, the correct one to be compared against ==== //
			vector<QueryClause> ansList;
			ansList.push_back(qc1);
			ansList.push_back(qc2);

			vector<QueryClause> input2anslist;
			input2anslist.push_back(qc3);

			vector<QueryClause> input3anslist;
			input3anslist.push_back(input3qc1);
			input3anslist.push_back(input3qc2);
			input3anslist.push_back(input3qc3);

			// ==== Generating Output lists using the instance and method ==== //
			vector<QueryClause> outputList = extractor.getClauses(input);
			vector<QueryClause> input2outputlist = extractor.getClauses(input2);
			vector<QueryClause> intput3outputlist = extractor.getClauses(input3);

			// ==== Assertions ==== //
			Assert::IsTrue(ansList.at(0) == outputList.at(0));
			Assert::IsTrue(ansList.at(1) == outputList.at(1));

			Assert::IsTrue(input2anslist.at(0) == input2outputlist.at(0));

			Assert::IsTrue(input3anslist.at(0) == intput3outputlist.at(0));
			Assert::IsTrue(input3anslist.at(1) == intput3outputlist.at(1));
			Assert::IsTrue(input3anslist.at(2) == intput3outputlist.at(2));


		}

	};
}