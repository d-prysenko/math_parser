#include "pch.h"
#include "CppUnitTest.h"
#include "BET.h"
#include "BET.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		TEST_METHOD(TestSimpleInitPlus)
		{
			std::string res = "a + b + c";
			BET tree("a + b + c");
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestSimpleInitMinus)
		{
			std::string res = "a - b - c";
			BET tree("a - b - c");
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestSimpleUnnecessaryParanthesis)
		{
			std::string res = "a + b + c";
			BET tree("((a) + b) + c");
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestUnnecessaryParanthesis1)
		{
			std::string res = "(A/(6*2) + 8 - 5)/E";
			BET tree("(A/(6*2) + (8 - 5))/(E)");
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestUnnecessaryParanthesis2)
		{
			std::string res = "(6/2*A + 8 - 5)/E";
			BET tree("((6/2)*A + (8 - 5))/(E)");
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestSimpleOpenParanthesisPlus)
		{
			std::string res = "a*b + a*c";
			BET tree("a*(b + c)");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestSimpleOpenParanthesisMinus1)
		{
			std::string res = "a - d*e - d*f - d*n";
			BET tree("a - (d*e + d*f + d*n)");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestSimpleOpenParanthesisMinus2)
		{
			std::string res = "a*b - a*c";
			BET tree("a*(b - c)");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestOpenParanthesis1)
		{
			std::string res = "a*b + a*c + d*e + d*f";
			BET tree("a*(b + c) + d*(e + f)");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestOpenParanthesis2)
		{
			std::string res = "k - c*a - c*b";
			BET tree("k - c*(a + b)");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestOpenParanthesis3)
		{
			std::string res = "k - c*a - c*b - p";
			BET tree("k - c*(a + b) - p");
			tree.transform();
			Assert::AreEqual(res, tree.toString());

			std::string res2 = "k - c*a - c*b + p";
			BET tree2("k - c*(a + b) + p");
			tree2.transform();
			Assert::AreEqual(res2, tree2.toString());

			std::string res3 = "k - c*a + c*b + p";
			BET tree3("k - c*(a - b) + p");
			tree3.transform();
			Assert::AreEqual(res3, tree3.toString());
		}

		TEST_METHOD(TestOpenParanthesis4)
		{
			// 
			//				+
			//			+	   q*k
			//		+	   q*c
			//	b*w	  b*p

			//				  +
			//			+	  	    +
			//		b*w	  b*p   q*c   q*k
			std::string res = "d + a*b*w + a*b*p + a*q*c + a*q*k";
			BET tree("d + a*(b*(w + p) + q*(c + k))");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}

		TEST_METHOD(TestOpenParanthesis5)
		{
			std::string res = "d + a*b*w - a*b*p - a*q*c - a*q*k";
			BET tree("d + a*(b*(w - p) - q*(c + k))");
			tree.transform();
			Assert::AreEqual(res, tree.toString());
		}
	};
}
