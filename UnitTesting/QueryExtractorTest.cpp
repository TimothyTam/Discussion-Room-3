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
		
		TEST_METHOD(QEX_GetDeclarationsList) {
			QueryExtractor extractor = QueryExtractor();
			unordered_map<string, string> testMap = {{"assign", "a"}, {"stmt", "s1"}};
			QueryPair qp1 = QueryPair(SYNONYM_TYPE_ASSIGN, "a");
			QueryPair qp2 = QueryPair(SYNONYM_TYPE_STMT, "s1");

			vector<QueryPair> outputList = extractor.getDeclarations(testMap);
			QueryPair o1 = outputList.at(0);
			QueryPair o2 = outputList.at(1);

			Assert::IsTrue(qp1 == o1);
			Assert::IsTrue(qp2 == o2);

		}

		TEST_METHOD(QEX_RemoveDeclarations_True) {
			QueryExtractor extractor = QueryExtractor();

			string input = "assign a; stmt s1; Select a something else here";
			string input2 = "while w, w2; if i1; Select <w, w2> such that Parent (w, w2)";

			string ans = "Select a something else here";
			string ans2 = "Select <w, w2> such that Parent (w, w2)";

			string output = extractor.removeDeclarations(input);
			string output2 = extractor.removeDeclarations(input2);

			Assert::IsTrue(output == ans);
			Assert::IsTrue(output2 == ans2);
		}

		TEST_METHOD(QEX_DetermineSynonymType) {
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

		TEST_METHOD(QEX_SanitiseSelects) {
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

		TEST_METHOD(QEX_GetSelects) {
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

		TEST_METHOD(QEX_RemoveSpaces) {
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

		TEST_METHOD(QEX_CreateQueryParam) {
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

		TEST_METHOD(QEX_CreateQueryParamForPatternAssign) {
			QueryExtractor extractor = QueryExtractor();
			unordered_map<string, SynonymType> testmap;
			testmap.insert(std::pair<string, SynonymType>("v", SYNONYM_TYPE_VARIABLE));
			testmap.insert(std::pair<string, SynonymType>("x", SYNONYM_TYPE_VARIABLE));
			extractor.decHashMap = testmap;

			QueryParam qp1 = QueryParam(PARAMTYPE_PATTERN_STRING_LEFT_OPEN, SYNONYM_TYPE_NULL, "_\"x+y\"");
			QueryParam qp2 = QueryParam(PARAMTYPE_PATTERN_STRING_BOTH_OPEN, SYNONYM_TYPE_NULL, "_\"x+y*z\"_");
			QueryParam qp3 = QueryParam(PARAMTYPE_PATTERN_STRING_RIGHT_OPEN, SYNONYM_TYPE_NULL, "\"x+y+z*a\"_");
			QueryParam qp4 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_VARIABLE, "v");

			string input1 = "_\"x+y\"";
			string input2 = "_\"x+y*z\"_";
			string input3 = "\"x+y+z*a\"_";
			string input4 = "v";

			QueryParam output1 = extractor.createQueryParamForPatternAssign(input1);
			QueryParam output2 = extractor.createQueryParamForPatternAssign(input2);
			QueryParam output3 = extractor.createQueryParamForPatternAssign(input3);
			QueryParam output4 = extractor.createQueryParamForPatternAssign(input4);

			Assert::IsTrue(qp1 == output1);
			Assert::IsTrue(qp2 == output2);
			Assert::IsTrue(qp3 == output3);
			Assert::IsTrue(qp4 == output4);

		}

		TEST_METHOD(QEX_GetClauses) {
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

		TEST_METHOD(QEX_GetClauses_2) {
			QueryExtractor extractor = QueryExtractor();

			// ==== Initialising attribute of extractor instance ==== //
			unordered_map<string, SynonymType> testmap;
			testmap.insert(std::pair<string, SynonymType>("a1", SYNONYM_TYPE_ASSIGN));
			testmap.insert(std::pair<string, SynonymType>("if", SYNONYM_TYPE_IF));
			testmap.insert(std::pair<string, SynonymType>("v", SYNONYM_TYPE_VARIABLE));
			testmap.insert(std::pair<string, SynonymType>("x", SYNONYM_TYPE_VARIABLE));
			testmap.insert(std::pair<string, SynonymType>("s1", SYNONYM_TYPE_STMT));
			testmap.insert(std::pair<string, SynonymType>("w", SYNONYM_TYPE_WHILE));
			extractor.decHashMap = testmap;

			// ==== Test strings ==== //
			string input1 = "such that Modifies (s1, v) and Follows* (w, if ) pattern a1 (\"y\", _) and pattern a1 ( v , \"x + y\") and pattern a1(x, _\"y +z\"_)";

			// ==== Creating QueryParam objects for each parameter detected ==== //
			QueryParam input1qp1 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_STMT, "s1");
			QueryParam input1qp2 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_VARIABLE, "v");
			QueryParam input1qp3 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_WHILE, "w");
			QueryParam input1qp4 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_IF, "if");
			QueryParam input1qp5 = QueryParam(PARAMTYPE_ENT_NAME, SYNONYM_TYPE_NULL, "\"y\"");
			QueryParam input1qp6 = QueryParam(PARAMTYPE_PLACEHOLDER, SYNONYM_TYPE_NULL, "_");
			QueryParam input1qp7 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_VARIABLE, "v");
			QueryParam input1qp8 = QueryParam(PARAMTYPE_PATTERN_STRING_EXACT, SYNONYM_TYPE_NULL, "\"x+y\"");
			QueryParam input1qp9 = QueryParam(PARAMTYPE_SYNONYM, SYNONYM_TYPE_VARIABLE, "x");
			QueryParam input1qp10 = QueryParam(PARAMTYPE_PATTERN_STRING_BOTH_OPEN, SYNONYM_TYPE_NULL, "_\"y+z\"_");

			vector<QueryParam> input1clause1list;
			input1clause1list.push_back(input1qp1);
			input1clause1list.push_back(input1qp2);
			vector<QueryParam> input1clause2list;
			input1clause2list.push_back(input1qp3);
			input1clause2list.push_back(input1qp4);
			vector<QueryParam> input1clause3list;
			input1clause3list.push_back(input1qp5);
			input1clause3list.push_back(input1qp6);
			vector<QueryParam> input1clause4list;
			input1clause4list.push_back(input1qp7);
			input1clause4list.push_back(input1qp8);
			vector<QueryParam> input1clause5list;
			input1clause5list.push_back(input1qp9);
			input1clause5list.push_back(input1qp10);

			QueryClause input1qc1 = QueryClause(CLAUSETYPE_MODIFIES, 2, input1clause1list);
			QueryClause input1qc2 = QueryClause(CLAUSETYPE_FOLLOWS_STAR, 2, input1clause2list);
			QueryClause input1qc3 = QueryClause(CLAUSETYPE_PATTERN_ASSIGN, 2, input1clause3list);
			QueryClause input1qc4 = QueryClause(CLAUSETYPE_PATTERN_ASSIGN, 2, input1clause4list);
			QueryClause input1qc5 = QueryClause(CLAUSETYPE_PATTERN_ASSIGN, 2, input1clause5list);

			vector<QueryClause> ansList1;
			ansList1.push_back(input1qc1);
			ansList1.push_back(input1qc2);
			ansList1.push_back(input1qc3);
			ansList1.push_back(input1qc4);
			ansList1.push_back(input1qc5);

			vector<QueryClause> outputList1 = extractor.getClauses(input1);

			Assert::IsTrue(ansList1.at(0) == outputList1.at(0));
			Assert::IsTrue(ansList1.at(1) == outputList1.at(1));
			Assert::IsTrue(ansList1.at(2) == outputList1.at(2));
			Assert::IsTrue(ansList1.at(3) == outputList1.at(3));
			Assert::IsTrue(ansList1.at(4) == outputList1.at(4));

		}

	};
}