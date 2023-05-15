#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "FooSpecializations.h"
#include "IntegerParseHelper.h"
#include "FloatParseHelper.h"
#include "StringParseHelper.h"
#include "DummyWrapperAndHelper.h"
#include "TableParseHelper.h"
#include <fstream>
#include <iostream>
#include <stdio.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(ParseHelperTests)
	{
	private:
		inline static _CrtMemState _startMemState;
	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &_startMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(jsontesting)
		{
			string inputString =
				
				R"({
  "IntAttribute": {
    "Type": "Integer",
    "Value": 100
  },
  "FloatAttribute": {
    "Type": "Float",
    "Value": 35.1
  }
})";

			Json::Reader jsonReader;
			Json::Value jsonData;
			ifstream jsonFile("ParserSample.json");

			std::shared_ptr<Wrapper_Base> argumentWrapper = make_shared<TableParseHelper::Wrapper_Table>();

			TableParseHelper::Wrapper_Table* scopeWrapper = static_cast<TableParseHelper::Wrapper_Table*>(argumentWrapper.get());
			std::shared_ptr<Scope> testScope = make_shared<Scope>();
			scopeWrapper->_scopeOBJ = testScope;

			ParseCoordinator coordinator{ argumentWrapper };

			coordinator.AddHelper(make_shared<TableParseHelper>());
			coordinator.AddHelper(make_shared<IntegerParseHelper>());
			coordinator.AddHelper(make_shared<FloatParseHelper>());
			//Assert::ExpectException<std::runtime_error>([&coordinator, &jsonFile]() {coordinator.DeserializeObject(jsonFile); });
			coordinator.AddHelper(make_shared<StringParseHelper>());
			coordinator.DeserializeObject(jsonFile);

			Datum* testDatum = testScope->Find("IntAttribute");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->Size() == 3);
			Assert::IsTrue(testDatum->backAsInt() == 98);
			Assert::IsTrue(testDatum->Get<int>(0) == 100);
			Assert::IsTrue(testDatum->Get<int>(1) == 99);

			testDatum = testScope->Find("FloatAttribute");
			Assert::IsTrue(Datum::DatumTypes::Float == testDatum->Type());
			float convertedVal = static_cast<float>(double{ 35.1 });
			Assert::IsTrue(testDatum->backAsFloat() == convertedVal);

			testDatum = testScope->Find("StringAttribute");
			Assert::IsTrue(Datum::DatumTypes::String == testDatum->Type());
			Assert::IsTrue(testDatum->backAsString() == std::string{"test string :)"});

			//checking first layer nesting
			testDatum = testScope->Find("Nested1");
			Assert::IsTrue(Datum::DatumTypes::Table == testDatum->Type());
			Scope* nestedScope = testDatum->Get<Scope*>(0);

			testDatum = nestedScope->Find("L1Int");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->backAsInt() == 25);

			testDatum = nestedScope->Find("L1String");
			Assert::IsTrue(Datum::DatumTypes::String == testDatum->Type());
			Assert::AreEqual(testDatum->backAsString(), std::string{ "Layer1 string!" });


			//checking second layer nesting
			testDatum = nestedScope->Find("Nested2");
			Scope* nested2 = testDatum->Get<Scope*>();

			testDatum = nested2->Find("L2Int");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->backAsInt() == 65);

			testDatum = nested2->Find("L2String");
			Assert::IsTrue(Datum::DatumTypes::String == testDatum->Type());
			Assert::AreEqual(testDatum->backAsString(), std::string{ "Layer2 string!" });
		}

		TEST_METHOD(ScopeArray) //testing for scope array work in progress
		{
			ifstream jsonFile("ParserSample2.json");

			std::shared_ptr<Wrapper_Base> argumentWrapper = make_shared<TableParseHelper::Wrapper_Table>();

			TableParseHelper::Wrapper_Table* scopeWrapper = static_cast<TableParseHelper::Wrapper_Table*>(argumentWrapper.get());
			std::shared_ptr<Scope> testScope = make_shared<Scope>();
			scopeWrapper->_scopeOBJ = testScope;

			ParseCoordinator coordinator{ argumentWrapper };

			coordinator.AddHelper(make_shared<TableParseHelper>());
			coordinator.AddHelper(make_shared<IntegerParseHelper>());
			coordinator.AddHelper(make_shared<StringParseHelper>());
			coordinator.AddHelper(make_shared<FloatParseHelper>());
			coordinator.DeserializeObject(jsonFile);

			Datum* testDatum;
			testDatum = testScope->Find("Scope1");

			Assert::AreEqual(std::size_t{ 2 }, testDatum->Size());
			Scope* childPtr = testDatum->Get<Scope*>(0);
			Datum* tDatum2 = childPtr->Find("Int1");
			Assert::AreEqual(10, tDatum2->Get<int>(0));

			childPtr = testDatum->Get<Scope*>(1);
			tDatum2 = childPtr->Find("Int2");
			Assert::AreEqual(20, tDatum2->Get<int>(0));
		}

	};
}
