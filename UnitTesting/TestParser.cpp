#include "stdafx.h"
#include "CppUnitTest.h"
#include "Parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting {
	TEST_CLASS(Parser) {
		TEST_METHOD(TestParser_IsSpecialToken) {
			Assert::IsTrue(IsSpecialToken("{"));
			Assert::IsTrue(IsSpecialToken("}"));
			Assert::IsTrue(IsSpecialToken("("));
			Assert::IsTrue(IsSpecialToken(")"));
			Assert::IsTrue(IsSpecialToken("="));
			Assert::IsTrue(IsSpecialToken("+"));
			Assert::IsTrue(IsSpecialToken("-"));
			Assert::IsTrue(IsSpecialToken("*"));
			Assert::IsTrue(IsSpecialToken(";"));
			Assert::IsTrue(IsSpecialToken("\\"));
			Assert::IsFalse(IsSpecialToken("abc"));
			Assert::IsFalse(IsSpecialToken("999"));
			Assert::IsFalse(IsSpecialToken("d3f"));
		}

		TEST_METHOD(TestParser_IsValidName) {
			Assert::IsTrue(IsValidName("abc"));
			Assert::IsFalse(IsValidName("123"));
			Assert::IsTrue(IsValidName("abc123"));
			Assert::IsFalse(IsValidName("123abc"));
			Assert::IsFalse(IsValidName("{abc}"));
		}

		TEST_METHOD(TestParser_IsValidNumber) {
			Assert::IsTrue(IsValidNumber("123"));
			Assert::IsFalse(IsValidNumber("abc"));
		}

		TEST_METHOD(TestParser_GetConstant) {
			Assert::AreEqual(123, GetConstant("123"));
			Assert::AreEqual(-1, GetConstant("abc"));
			Assert::AreEqual(0, GetConstant("0"));
			Assert::AreEqual(-1, GetConstant("000"));
		}
	};

}