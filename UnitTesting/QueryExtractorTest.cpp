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

		TEST_METHOD(Remove_Declarations_True) {
			QueryExtractor extractor = QueryExtractor();
			string query = "assign a; stmt s1; Select a zzz stupid adhakhjdjhhda";
			string ans = "Select a zzz stupid adhakhjdjhhda";
			//string ans = "hi";
			string result = extractor.removeDeclarations(query);
			Assert::IsTrue(result == ans);
		}

	};
}