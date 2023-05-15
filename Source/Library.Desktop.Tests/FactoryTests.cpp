#include "pch.h"
#include "CppUnitTest.h"
#include "DummyFactories.h"
#include "Factory.h"
#include "Scope.h"
#include "Foo.h"
#include "ParseCoordinator.h"
#include "TableParseHelper.h"
#include "IntegerParseHelper.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(FactoryTests)
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
		TEST_METHOD(hashMapTesting)
		{
			using namespace UnitTests;
			dummyClassFactory testFactory;
			std::string testString = typeid(testFactory).name();

			std::string factoryName = testString.substr(testString.find_last_of(':')+1);
			Assert::AreEqual(std::string{ "dummyClassFactory" }, factoryName);
			Factory<Scope>::Add(make_unique<dummyClassFactory>());
			Factory<Scope>* factoryPtr = Factory<Scope>::Find("dummyClass");

			Assert::AreEqual(factoryPtr->ClassName(), std::string{ "dummyClass" });
			dummyClass* newDummy = static_cast<dummyClass*>(Factory<Scope>::Create("dummyClass"));
			Assert::AreEqual(9, newDummy->_dummyInt);

			factoryPtr = Factory<Scope>::Find("asvne");
			Assert::IsTrue(factoryPtr == nullptr);

			Scope* failPtr = Factory<Scope>::Create("asvne");
			Assert::IsTrue(failPtr == nullptr);

			Factory<Scope>::Remove("dummyClass");
			delete newDummy;
		}

		TEST_METHOD(ParseAttributed)
		{
			RegisterType<AttFoo2>();
			Factory<Scope>::Add(make_unique<AttFoo2Factory>());

			Json::Reader jsonReader;
			Json::Value jsonData;
			ifstream jsonFile("FactorySample1.json");

			std::shared_ptr<Wrapper_Base> argumentWrapper = make_shared<TableParseHelper::Wrapper_Table>();

			TableParseHelper::Wrapper_Table* scopeWrapper = static_cast<TableParseHelper::Wrapper_Table*>(argumentWrapper.get());
			std::shared_ptr<Scope> testScope = make_shared<Scope>();
			scopeWrapper->_scopeOBJ = testScope;
			
			ParseCoordinator coordinator{ argumentWrapper };
			coordinator.AddHelper(make_shared<TableParseHelper>());
			coordinator.AddHelper(make_shared<IntegerParseHelper>());

			coordinator.DeserializeObject(jsonFile);

			Datum* testDatum = testScope->Find("Scope1");
			Assert::IsTrue(Datum::DatumTypes::Table == testDatum->Type());
			Scope* nestedScope = testDatum->Get<Scope*>(0);

			AttFoo2* classRef = static_cast<AttFoo2*> (nestedScope);
			testDatum = nestedScope->Find("ExternalInteger");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->IsExternal());
			Assert::IsTrue(testDatum->Get<int>(0) == 10);

			Assert::AreEqual(10, classRef->ExternalInteger);
			classRef->ExternalInteger = 67;
			Assert::IsTrue(testDatum->Get<int>(0) == 67);

			testDatum = nestedScope->Find("ExternalIntegerArray");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->Size() == 5);
			Assert::IsTrue(testDatum->Get<int>(0) == 10);
			Assert::IsTrue(testDatum->Get<int>(1) == 20);
			Assert::IsTrue(testDatum->Get<int>(2) == 30);
			Assert::IsTrue(testDatum->Get<int>(3) == 40);
			Assert::IsTrue(testDatum->Get<int>(4) == 50);

			testDatum = nestedScope->Find("NestedScope");
			Assert::IsTrue(testDatum->Type() == Datum::DatumTypes::Table);
			nestedScope = testDatum->Get<Scope*>(0);
			testDatum = nestedScope->Find("ExternalInteger");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->IsExternal());
			Assert::IsTrue(testDatum->Get<int>(0) == 56);

			testDatum = nestedScope->Find("ExternalIntegerArray");
			Assert::IsTrue(Datum::DatumTypes::Integer == testDatum->Type());
			Assert::IsTrue(testDatum->Size() == 5);
			Assert::IsTrue(testDatum->Get<int>(0) == 23);
			Assert::IsTrue(testDatum->Get<int>(1) == 34);
			Assert::IsTrue(testDatum->Get<int>(2) == 97);

			Factory<Scope>::Clear();
			TypeManager::Clear();
		}
	};
}