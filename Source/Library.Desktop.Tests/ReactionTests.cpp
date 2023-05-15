#include "pch.h"
#include "CppUnitTest.h"
#include "Reaction.h"
#include "EventTestClasses.h"
#include "ConcreteActions.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(ReactionTests)
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
			RegisterType<Action>();
			RegisterType<ActionList, Action>();
			RegisterType<ActionIncrement, Action>();
			RegisterType<ActionEvent, Action>();
			RegisterType<EventMessageAttributed>();
			Factory<Scope>::Add(make_unique<GameObjectFactory>());
			Factory<Scope>::Add(make_unique<ActionIncrementFactory>());
			Factory<Scope>::Add(make_unique<ActionListFactory>());
			Factory<Scope>::Add(make_unique<ActionEventFactory>());
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
				GameObject rootObject;
				GameState game{ rootObject };

				rootObject.CreateAction("ActionEvent"s, "TestActionEvent"s);
				Datum* tDatum = rootObject.Find("Actions"s);
				Scope* tScope = tDatum->Get<Scope*>();

				Attributed* bufferScope = static_cast<Attributed*>(tScope);
				Datum& targetDatum = bufferScope->AppendAuxiliaryAttribute("Target"s);
				targetDatum.SetType(Datum::DatumTypes::String);
				targetDatum.pushBack("BLAH"s);
				Datum& stepDatum = bufferScope->AppendAuxiliaryAttribute("Step"s);
				stepDatum.SetType(Datum::DatumTypes::Integer);
				stepDatum.pushBack(20);

				tDatum = tScope->Find("SubType"s);
				Assert::IsTrue("NotSet"s == tDatum->Get<string>());

				ReactionTestClass1 subscriber1;
				Event<EventMessageAttributed>::Subscribe(&subscriber1);

				game.Update();
				game.Update();

				Assert::IsTrue(subscriber1._testVariable == 1);

				EventMessageAttributed testMessage = subscriber1._message;
				Datum* messageAtts = testMessage.Find("Target"s);
				Assert::IsTrue(messageAtts->Get<string>() == "BLAH"s);
				messageAtts = testMessage.Find("Step"s);
				Assert::IsTrue(messageAtts->Get<int>() == 20);

				GameState::_gameEventQueue.Clear();
				Event<EventMessageAttributed>::UnsubscribeAll();
			}
		}

		TEST_METHOD(MessageTesting)
		{
			{
				GameObject rootObject;
				GameState game{ rootObject };

				Datum& A = rootObject.AppendAuxiliaryAttribute("A"s);
				A.SetType(Datum::DatumTypes::Integer);
				A.pushBack(1);

				rootObject.CreateAction("ActionEvent"s, "TestActionEvent"s);
				Datum* tDatum = rootObject.Find("Actions"s);
				Scope* tScope = tDatum->Get<Scope*>();
				Attributed* bufferScope = static_cast<Attributed*>(tScope);
				Datum& targetDatum = bufferScope->AppendAuxiliaryAttribute("TargetName"s);
				targetDatum.SetType(Datum::DatumTypes::String);
				targetDatum.pushBack("A"s);
				Datum& stepDatum = bufferScope->AppendAuxiliaryAttribute("Step"s);
				stepDatum.SetType(Datum::DatumTypes::Integer);
				stepDatum.pushBack(20);

				static_cast<ActionEvent*>(tScope)->SetSubType("Test"s);
				static_cast<ActionEvent*>(tScope)->SetDelay(0);

				ReactionTestClass1 subscriber1;
				Event<EventMessageAttributed>::Subscribe(&subscriber1);
				ReactionAttributed* reaction1 = new ReactionAttributed{};
				reaction1->SetSubType("Test"s);
				ActionIncrement* incrementAction = new ActionIncrement{};
				reaction1->Adopt(*incrementAction, "Actions"s);

				rootObject.Adopt(*reaction1, "Reactions"s);


				game.Update();
				game.Update();

				EventMessageAttributed testMessage = subscriber1._message;

				Datum* incrementAtts = incrementAction->Find("TargetName");
				Assert::IsTrue(incrementAtts->Get<string>(0) == "A"s);
				incrementAtts = incrementAction->Find("Step"s);
				Assert::IsTrue(incrementAtts->Get<int>(0) == 20);

				Datum* tDatum1 = testMessage.Find("TargetName"s);


				tDatum1 = testMessage.Find("Step"s);

				tDatum = testMessage.Find("TargetName"s);
				Assert::IsTrue("A"s == tDatum->Get<string>(0));
				tDatum = testMessage.Find("Step"s);
				Assert::IsTrue(20 == tDatum->Get<int>(0));

				ActionIncrement tIncrement;

				auto [front, end] = testMessage.AuxiliaryAttributes();
				if (front != end)
				{
					string key = (*front)->first;
					Datum& value = (*front)->second;

					Assert::IsTrue((key == "TargetName"s));
					Assert::IsTrue(value.Get<string>(0) == "A"s);
				}

				++front;

				if (front != end)
				{
					string key = (*front)->first;
					Datum& value = (*front)->second;

					Assert::IsTrue((key == "Step"s));
					Assert::IsTrue(value.Get<int>(0) == 20);
				}

				++front;

				Assert::IsTrue(front == end);

				Assert::IsTrue(A.Get<int>(0) == 21);

				GameState::_gameEventQueue.Clear();
				Event<EventMessageAttributed>::UnsubscribeAll();
			}
		}
		
	};
}