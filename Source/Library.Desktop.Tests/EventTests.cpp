#include "pch.h"
#include "CppUnitTest.h"
#include "EventSystem.h"
#include "EventTestClasses.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(EventTests)
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
			{
				GenericMessage1 message1;
				EventTestClass1 subscriber1;

				Event<GenericMessage1> testEvent1{ message1 };
				Event<GenericMessage1>::Subscribe(&subscriber1);
				
				EventPublisher* publisher = static_cast<EventPublisher*>(&testEvent1);
				publisher->Deliver();

				Assert::IsTrue(subscriber1._testVariable = 1);

				Event<GenericMessage1>::UnsubscribeAll();
			}

			{
				GenericMessage1 message1;
				GenericMessage2	message2;
				GenericMessage3	message3;
				GenericMessage4	message4;

				EventTestClass1 subscriber1;
				EventTestClass1T2 Type1Subscriber2;
				EventTestClass2 subscriber2;
				EventTestClass3 subscriber3;
				EventTestClass4 subscriber4;

				std::shared_ptr<Event<GenericMessage1>> testEvent1 = make_shared<Event<GenericMessage1>>(message1);
				Event<GenericMessage1>::Subscribe(&subscriber1);
				Event<GenericMessage1>::Subscribe(&Type1Subscriber2);
				std::shared_ptr<Event<GenericMessage2>> testEvent2 = make_shared<Event<GenericMessage2>>(message2);
				Event<GenericMessage2>::Subscribe(&subscriber2);
				std::shared_ptr<Event<GenericMessage3>> testEvent3 = make_shared<Event<GenericMessage3>>(message3);
				Event<GenericMessage3>::Subscribe(&subscriber3);
				std::shared_ptr<Event<GenericMessage4>> testEvent4 = make_shared<Event<GenericMessage4>>(message4);
				Event<GenericMessage4>::Subscribe(&subscriber4);

				GenericMessage1 testMessage = testEvent1->Message(); //nothing to assert, message is empty class

				GameTime Time;
				std::chrono::steady_clock::time_point currentTime { std::chrono::seconds {0} };
				Time.SetCurrentTime(currentTime);

				EventQueue queue;
				queue.Enqueue(testEvent1, Time, std::chrono::seconds{ 3 });
				queue.Enqueue(testEvent2, Time, std::chrono::seconds{ 1 });
				queue.Enqueue(testEvent3, Time, std::chrono::seconds{ 2 });

				queue.Update(Time);

				Assert::IsTrue(testEvent1->GetExpirationTime() == std::chrono::steady_clock::time_point{ std::chrono::seconds {3} });
				Assert::IsTrue(testEvent2->GetExpirationTime() == std::chrono::steady_clock::time_point{ std::chrono::seconds {1} });
				Assert::IsTrue(testEvent3->GetExpirationTime() == std::chrono::steady_clock::time_point{ std::chrono::seconds {2} });
				
				Assert::IsTrue(subscriber1._testVariable == 0);

				currentTime = std::chrono::steady_clock::time_point{ std::chrono::seconds {1} };
				Time.SetCurrentTime(currentTime);
				queue.Enqueue(testEvent4, Time, std::chrono::seconds{ 4 });
				Assert::IsTrue(testEvent4->GetExpirationTime() == std::chrono::steady_clock::time_point{ std::chrono::seconds {5} });
				queue.Update(Time);

				Assert::IsTrue(subscriber1._testVariable == 0);
				Assert::IsTrue(Type1Subscriber2._testVariable == 5);


				currentTime = std::chrono::steady_clock::time_point{ std::chrono::seconds {3} };
				Time.SetCurrentTime(currentTime);
				queue.Update(Time);

				Assert::IsTrue(subscriber1._testVariable == 1);
				Assert::IsTrue(Type1Subscriber2._testVariable == 6);
				Assert::IsTrue(subscriber2._testVariable == 1);
				Assert::IsTrue(subscriber3._testVariable == 1);
				Assert::IsTrue(subscriber4._testVariable == 0);

				currentTime = std::chrono::steady_clock::time_point{ std::chrono::seconds {6} };
				Time.SetCurrentTime(currentTime);
				queue.Update(Time);

				Assert::IsTrue(subscriber1._testVariable == 1);
				Assert::IsTrue(Type1Subscriber2._testVariable == 6);
				Assert::IsTrue(subscriber2._testVariable == 1);
				Assert::IsTrue(subscriber3._testVariable == 1);
				Assert::IsTrue(subscriber4._testVariable == 1);

				queue.Clear();
				Event<GenericMessage1>::UnsubscribeAll();
				Event<GenericMessage2>::UnsubscribeAll();
				Event<GenericMessage3>::UnsubscribeAll();
				Event<GenericMessage4>::UnsubscribeAll();
			}
		}
	};
}