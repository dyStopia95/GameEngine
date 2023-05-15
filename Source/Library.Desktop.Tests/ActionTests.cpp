#include "pch.h"
#include "CppUnitTest.h"
#include "GameObject.h"
#include "GameObjFoos.h"
#include "ConcreteActions.h"
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
	TEST_CLASS(ActionTests)
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

			RegisterType<GameObject>();
			RegisterType<GameOBJTest, GameObject>();
			RegisterType<Action>();
			RegisterType<ActionList, Action>();
			RegisterType<ActionIncrement, Action>();
			RegisterType<ActionListIf, ActionList>();
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionListFactory>());
			Factory<Scope>::Add(make_unique<GameOBJTestFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(make_unique<ActionListIfFactory>());
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			Factory<Scope>::Clear();
			TypeManager::Clear();

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
			{
				ActionIncrement* testAction = static_cast<ActionIncrement*>(Factory<Scope>::Create("ActionIncrement"));
				Assert::AreEqual(testAction->Name(), std::string{ "NoName" });
				Assert::AreEqual(testAction->_targetAttributeName, std::string{ "Not Set" });
				Assert::AreEqual(testAction->_step, 1);
				delete testAction;
			}

			{
				GameOBJTest testGameOBJ;
				testGameOBJ.CreateAction("ActionIncrement", "incrementer");
				Datum* actions = testGameOBJ.Find("Actions");
				ActionIncrement* testAction = static_cast<ActionIncrement*>(actions->Get<Scope*>(0));
				testAction->_targetAttributeName = string{ "IncrementINT" };
				testAction->SetAssociations();
				Datum* tempDatum = testGameOBJ.Find("IncrementINT");

				Assert::IsTrue(tempDatum == testAction->_targetDatum);

				GameTime T;
				testGameOBJ.Update(T);

				Assert::IsTrue(testGameOBJ._incrementINT == 32);
			}
		}

		TEST_METHOD(ActionParseTest)
		{
			Json::Reader jsonReader;
			Json::Value jsonData;
			ifstream jsonFile("ActionSample1.json");

			std::shared_ptr<Wrapper_Base> argumentWrapper = make_shared<TableParseHelper::Wrapper_Table>();
			TableParseHelper::Wrapper_Table* scopeWrapper = static_cast<TableParseHelper::Wrapper_Table*>(argumentWrapper.get());
			std::shared_ptr<GameOBJTest> testOBJ = make_shared<GameOBJTest>();

			scopeWrapper->_scopeOBJ = testOBJ;

			ParseCoordinator coordinator{ argumentWrapper };
			coordinator.AddHelper(make_shared<TableParseHelper>());
			coordinator.AddHelper(make_shared<IntegerParseHelper>());
			coordinator.AddHelper(make_shared<StringParseHelper>());

			coordinator.DeserializeObject(jsonFile);

			Datum* testD = testOBJ->Find("Name");
			Assert::AreEqual(string{ "ParentObject" }, testD->Get<string>());
			testD = testOBJ->Find("Actions");
			ActionList* aList1 = static_cast<ActionList*>(testD->Get<Scope*>(0));
			Assert::IsTrue(std::string{ "ActionList1" } == aList1->Name());

			testD = aList1->Find("Actions");
			ActionIncrement* testAction = static_cast<ActionIncrement*>(testD->Get<Scope*>(0));

			Assert::IsTrue(std::string{ "ActionIncrement1" } == testAction->Name());

			//ActionIncrement* testAction = static_cast<ActionIncrement*>(testD->Get<Scope*>(0));
			//testD = testOBJ->Find("IncrementINT");
			//testAction->SetAssociations();
			//Assert::IsTrue(testD == testAction->_targetDatum);
			Assert::AreEqual(0, testOBJ->_incrementINT);

			GameTime T;
			testOBJ->Update(T);

			Assert::AreEqual(2, testOBJ->_incrementINT);
		}

		TEST_METHOD(IFActionTests)
		{
			Json::Reader jsonReader;
			Json::Value jsonData;
			ifstream jsonFile("ActionSample2.json");

			std::shared_ptr<Wrapper_Base> argumentWrapper = make_shared<TableParseHelper::Wrapper_Table>();
			TableParseHelper::Wrapper_Table* scopeWrapper = static_cast<TableParseHelper::Wrapper_Table*>(argumentWrapper.get());
			std::shared_ptr<GameOBJTest> testOBJ = make_shared<GameOBJTest>();

			scopeWrapper->_scopeOBJ = testOBJ;

			ParseCoordinator coordinator{ argumentWrapper };
			coordinator.AddHelper(make_shared<TableParseHelper>());
			coordinator.AddHelper(make_shared<IntegerParseHelper>());
			coordinator.AddHelper(make_shared<StringParseHelper>());

			coordinator.DeserializeObject(jsonFile);

			Assert::AreEqual(0, testOBJ->_incrementINT);

			GameTime T;
			testOBJ->Update(T);// one increment from testOBJ update and one increment from trueBlock of ActionIF
			Assert::AreEqual(2, testOBJ->_incrementINT);

			testOBJ->_incrementINT = 0;

			Datum* testD = testOBJ->Find("Actions");
			ActionListIf* ifCondition = static_cast<ActionListIf*>(testD->Get<Scope*>(0));

			Assert::AreEqual(1, ifCondition->_condition);
			ifCondition->_condition = 0;
			testOBJ->Update(T); // one increment from testOBJ update and one decrement from falseBlock of ActionIF
			Assert::AreEqual(0, testOBJ->_incrementINT);
		}

	};
}
