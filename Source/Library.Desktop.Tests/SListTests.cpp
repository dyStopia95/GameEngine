#include "pch.h"
#include "CppUnitTest.h"
#include "Slist.h"
#include "Foo.h"
#include "FooSpecializations.h"
#include "SListSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(SListTests)
	{
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
		
		TEST_METHOD(Constructor)
		{
			SList<Foo> list;

			Assert::AreEqual(size_t{ 0 }, list.Size());
			Assert::ExpectException<std::runtime_error>([&list]() {auto& front = list.Front(); front;});
			Assert::ExpectException<std::runtime_error>([&list]() {auto& back = list.Back(); back;});
			
		}

		TEST_METHOD(clear)
		{
			SList<Foo> list;
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			Assert::AreEqual(false, list.IsEmpty());

			list.clear();

			Assert::AreEqual(true, list.IsEmpty());
			Assert::AreEqual(std::size_t{ 0 }, list.Size());
		}

		TEST_METHOD(constFrontBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			{
				const SList<Foo> list;
				Assert::ExpectException<std::runtime_error>([&list]() {Foo x = list.Front(); });
				Assert::ExpectException<std::runtime_error>([&list]() {Foo x = list.Back(); });
			}

			const SList<Foo> list{ a,b,c,d };
			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(d, list.Back());
		}

		TEST_METHOD(PushFrontAndBack)
		{
			SList<Foo> list;

			Assert::AreEqual(true, list.IsEmpty());

			const auto data = 10;
			const Foo a(data);
			const Foo b{ 20 };
			const Foo c{ 30 };

			const Foo test{ 30 };

			Foo x{ 10 };

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);

			Assert::AreEqual(false, list.IsEmpty());

			Assert::AreEqual(data, list.Front().Data());
			Assert::AreEqual(test.Data(), list.Back().Data());
			Assert::AreNotSame(a, list.Front());
			Assert::AreNotSame(b, list.Back());

			const Foo d{ 40 };
			list.PushFront(d);

			Assert::AreEqual(d.Data(), list.Front().Data());
			Assert::AreNotSame(d, list.Front());

			list.clear();
			list.PushFront(a);
			list.PushFront(b);
			list.PushFront(c);

			Assert::AreEqual(false, list.IsEmpty());

			Assert::AreEqual(data, list.Back().Data());
			Assert::AreEqual(test.Data(), list.Front().Data());
			Assert::AreNotSame(a, list.Back());
			Assert::AreNotSame(c, list.Front());
		}

		TEST_METHOD(CopyConstructor)
		{
			SList<Foo> list;
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			Assert::AreEqual(d, list.Back());

			SList<Foo> testList{ list };

			Assert::AreEqual(a.Data(), testList.Front().Data());
			Assert::AreEqual(d, testList.Back());
			Assert::AreEqual(testList.Front().Data(), list.Front().Data());
			Assert::AreEqual(testList.Back().Data(), list.Back().Data());
			Assert::AreNotSame(testList.Back(), list.Back());
			Assert::AreNotSame(testList.Front(), list.Front());
		}

		TEST_METHOD(InitializerList)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			SList<Foo> list{ a,b,c,d };

			Assert::AreEqual(a, list.Front());
			Assert::AreEqual(d, list.Back());
		}

		TEST_METHOD(CopyOperator)
		{
			SList<Foo> list;
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			SList<Foo> testList;
			testList = list;

			Assert::AreEqual(a.Data(), testList.Front().Data());
			Assert::AreEqual(d.Data(), testList.Back().Data());
			Assert::AreEqual(testList.Front().Data(), list.Front().Data());
			Assert::AreEqual(testList.Back().Data(), list.Back().Data());
			Assert::AreNotSame(testList.Back(), list.Back());
			Assert::AreNotSame(testList.Front(), list.Front());
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;

			Assert::ExpectException<std::runtime_error>([&list]() {list.PopFront(); });

			const Foo a{ 10 };

			list.PushBack(a);
			
			const Foo testFoo = list.PopFront();
			Assert::AreEqual(size_t{ 0 }, list.Size());

			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);

			Assert::AreEqual(size_t{ 4 }, list.Size());

			const Foo testFoo2 = list.PopFront();

			Assert::AreEqual(a.Data(), testFoo2.Data());
			Assert::AreEqual(size_t{ 3 }, list.Size());
			Assert::AreEqual(b.Data(), list.Front().Data());
		}

		TEST_METHOD(PopBack)
		{
			SList<Foo> list;

			Assert::ExpectException<std::runtime_error>([&list]() {list.PopBack(); });


			const Foo a{ 10 };

			list.PushBack(a);
			const Foo testFoo = list.PopBack();
			
			Assert::AreEqual(size_t{ 0 }, list.Size());

			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };

			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);


			Assert::AreEqual(size_t{ 4 }, list.Size());

			const Foo testFoo2 = list.PopBack();

			Assert::AreEqual(d.Data(), testFoo2.Data());
			Assert::AreEqual(size_t{ 3 }, list.Size());
			Assert::AreEqual(c.Data(), list.Back().Data());
			Assert::AreNotSame(c, list.Back());
		}

		TEST_METHOD(IteratorOperators)
		{
			SList<Foo>::iterator it;
			Assert::ExpectException<std::runtime_error>([&it]() {*it; });

			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			SList<Foo> list{ a,b,c };


			// Dereference operator
			it = list.begin();
			Assert::AreEqual(a, *it);
			++it;
			Assert::AreEqual(b, *it);
			++it;
			Assert::AreEqual(c, *it);
			++it;
			Assert::AreEqual(list.end(), it);
			Assert::ExpectException<std::runtime_error>([&it]() {++it; });

			// increment operators
			it = list.begin();
			it++;
			Assert::AreEqual(b, *it);
			SList<Foo>::iterator it2;
			it = list.begin();
			it2 = list.begin();
			Assert::AreEqual(it2, it++);
			Assert::AreEqual(b, *it);

			it = list.end();
			Assert::ExpectException<std::runtime_error>([&it]() {it++; });

			//equality check operators
			Assert::IsTrue(it != it2);
			Assert::IsFalse(it == it2);
			it = list.begin();
			Assert::IsTrue(it == it2);
			Assert::IsFalse(it != it2);
		}

		TEST_METHOD(Find)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			SList<Foo> list{ a,b,c,d };

			{
				SList<Foo>::iterator testIT = list.PushBack(e);
				Assert::AreEqual(e, *testIT);
				list.PopBack();
			}
			{
				SList<Foo>::iterator testIT = list.PushFront(e);
				Assert::AreEqual(e, *testIT);
				list.PopFront();
			}

			SList<Foo>::iterator it = list.Find(b);

			Assert::AreEqual(b, *it);

			it = list.Find(d);

			Assert::AreEqual(d, *it);

			it = list.Find(e);

			Assert::AreEqual(list.end(), it);
		}

		TEST_METHOD(InsertAfter)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			SList<Foo> list{ a,b,c,d };
			SList<Foo>::iterator it;

			it = list.InsertAfter(e, b);
			Assert::AreEqual(e, *it);
			Assert::AreEqual(c, *(++it));
			it = list.begin();
			++it;
			Assert::AreEqual(e, *(++it));
		}

		TEST_METHOD(Erase)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			SList<Foo> list{ a,b,c,d };
			list.Erase(b);
			Assert::AreEqual(list.end(), list.Find(b));
			list.Erase(a);
			Assert::AreEqual(c, list.Front());
			list.PushBack(e);
			Assert::AreEqual(e, list.Back());
			list.Erase(e);
			Assert::AreEqual(d, list.Back());

			SList<Foo> list2{ a,b,c,d,e };
			SList<Foo>::iterator it = list2.Erase(b);
			Assert::AreEqual(c, *it);
		}

		TEST_METHOD(constIteratorOperators)
		{
			SList<Foo>::const_iterator it;
			Assert::ExpectException<std::runtime_error>([&it]() {*it; });



			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			const SList<Foo> list{ a,b,c };

			// testing const casting from normal iterator to const_iterator
			{
				SList<Foo> list2{ a,b,c,d,e };
				SList<Foo>::iterator testIT = list2.begin();
				SList<Foo>::const_iterator testCIT{ testIT };
				Assert::AreEqual(a, *testCIT);
			}

			// Dereference operator
			it = list.begin();
			Assert::AreEqual(a, *it);
			++it;
			Assert::AreEqual(b, *it);
			++it;
			Assert::AreEqual(c, *it);
			++it;
			Assert::AreEqual(list.end(), it);
			Assert::ExpectException<std::runtime_error>([&it]() {++it; });

			// increment operators
			it = list.begin();
			it++;
			Assert::AreEqual(b, *it);
			SList<Foo>::const_iterator it2;
			it = list.begin();
			it2 = list.begin();
			Assert::AreEqual(it2, it++);
			Assert::AreEqual(b, *it);

			it = list.end();
			Assert::ExpectException<std::runtime_error>([&it]() {it++; });

			//equality check operators
			Assert::IsTrue(it != it2);
			Assert::IsFalse(it == it2);
			it = list.begin();
			Assert::IsTrue(it == it2);
			Assert::IsFalse(it != it2);
		}

		TEST_METHOD(CBeginCEnd)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			SList<Foo> list{ a,b,c,d,e };
			SList<Foo>::const_iterator it = list.cbegin();
			Assert::AreEqual(a, *it);
			it++;
			Assert::AreEqual(b, *it);
			it++;
			it++;
			it++;
			it++;
			SList<Foo>::const_iterator it2 = list.cend();
			Assert::AreEqual(it,it2);
		}

		private:
			inline static _CrtMemState _startMemState;

	};
}
