#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "GameObjFoos.h"
#include "ParseCoordinator.h"
#include "TableParseHelper.h"
#include "IntegerParseHelper.h"
#include "StringParseHelper.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(GameOBJTesting)
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

		TEST_METHOD(BaseTests)
		{
			RegisterType<GameObject>();
			RegisterType<GameOBJTest, GameObject>();

			GameObject parentOBJ;
			GameOBJTest* testOBJ = new GameOBJTest;

			testOBJ->_name = std::string{ "DerivedOBJ" };
			testOBJ->_transform = std::string{ "placeholder transform" };

			Datum* testD = testOBJ->Find("Name");
			Assert::AreEqual(std::string{ "DerivedOBJ" }, testD->Get<string>());

			testD = testOBJ->Find("Transform");
			Assert::AreEqual(std::string{ "placeholder transform" }, testD->Get<string>());

			testD = testOBJ->Find("IncrementINT");
			Assert::AreEqual(30, testD->Get<int>());

			parentOBJ.Adopt(*testOBJ, "Children");

			GameTime T;
			parentOBJ.Update(T);

			Assert::IsTrue(testOBJ->_incrementINT == 31);

			TypeManager::Clear();
		}

		TEST_METHOD(ParseTest)
		{
			RegisterType<GameObject>();
			RegisterType<GameOBJTest, GameObject>();
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<GameOBJTestFactory>());

			Json::Reader jsonReader;
			Json::Value jsonData;
			ifstream jsonFile("GameOBJSample1.json");

			std::shared_ptr<Wrapper_Base> argumentWrapper = make_shared<TableParseHelper::Wrapper_Table>();
			TableParseHelper::Wrapper_Table* scopeWrapper = static_cast<TableParseHelper::Wrapper_Table*>(argumentWrapper.get());
			std::shared_ptr<GameObject> testOBJ = make_shared<GameObject>();

			scopeWrapper->_scopeOBJ = testOBJ;

			ParseCoordinator coordinator{ argumentWrapper };
			coordinator.AddHelper(make_shared<TableParseHelper>());
			coordinator.AddHelper(make_shared<IntegerParseHelper>());
			coordinator.AddHelper(make_shared<StringParseHelper>());

			coordinator.DeserializeObject(jsonFile);

			Datum* testD = testOBJ->Find("Name");
			Assert::AreEqual(string{ "ParentObject" }, testD->Get<string>());
			testD = testOBJ->Find("Transform");
			Assert::AreEqual(string{ "Transform Placeholder" }, testD->Get<string>());

			testD = testOBJ->Find("Children");
			GameOBJTest* childPtr1 = static_cast<GameOBJTest*>(testD->Get<Scope*>());
			testD = childPtr1->Find("Name");
			Assert::AreEqual(string{ "Child1" }, testD->Get<string>());
			Assert::AreEqual(34, childPtr1->_incrementINT);

			testD = childPtr1->Find("Children");
			GameOBJTest* childPtr2 = static_cast<GameOBJTest*>(testD->Get<Scope*>());
			testD = childPtr2->Find("Name");
			Assert::AreEqual(string{ "Child2" }, testD->Get<string>());
			Assert::AreEqual(99, childPtr2->_incrementINT);

			GameTime T;
			testOBJ->Update(T);

			Assert::AreEqual(35, childPtr1->_incrementINT);
			Assert::AreEqual(100, childPtr2->_incrementINT);

			Factory<Scope>::Clear();
			TypeManager::Clear();
		}

	};
}