#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "vector.h"
#include "FooSpecializations.h"
#include "VectorSpecializations.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(VectorTests)
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

		TEST_METHOD(ConstructorsDestructors)
		{
			Vector<int> v;
			//Assert::AreEqual(size_t{ 100 }, v.capacity());
			Assert::AreEqual(size_t{ 0 }, v.size());

			v.reserve(200);
			Assert::AreEqual(size_t{ 200 }, v.capacity());
			Assert::ExpectException<std::runtime_error>([&v]() {v[0]; });

			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			//initializer list + pushback resize section
			Vector<Foo> testVector{ a,b,c,d,e };
			Assert::AreEqual(size_t{ 5 }, testVector.size());
			Assert::AreEqual(a, testVector[0]);
			Assert::AreEqual(b, testVector[1]);
			Assert::AreEqual(c, testVector[2]);
			Assert::AreEqual(d, testVector[3]);
			Assert::AreEqual(e, testVector[4]);

			//Destructor getting checked by not having memory leak.
		}

		TEST_METHOD(testBack)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo>	v{ a,b,c };

			struct incFn final
			{
				size_t operator()(size_t capacity)
				{
					return capacity * 3;
				}
			};
			incFn fn{};

			v.test_back(d);
			Assert::AreEqual(size_t{ 6 }, v.capacity());

			v.shrink_to_fit();

			v.test_back<incFn>(e);
			Assert::AreEqual(size_t{ 12 }, v.capacity());

		}
		TEST_METHOD(pushpop_back)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			Foo x{ 60 };

			{
				Vector<Foo> v{ a,b,c };
				v.push_back(move(x));
				Assert::AreEqual(size_t{ 4 }, v.size());
				Assert::AreEqual(Foo{ 60 }, v.back());
			}

			{
				const Vector<Foo> v{ a,b,c,d };
				Assert::ExpectException<std::runtime_error>([&v]() {v[6]; });
				Assert::AreEqual(size_t{ 4 }, v.size());
				Assert::AreEqual(a, v[0]);
			}

			{
				const Vector<Foo> v{ a,b,c,d };
				Assert::ExpectException<std::runtime_error>([&v]() {v.at(6); });
				Assert::AreEqual(a, v.at(0));
				Assert::AreEqual(d, v.at(3));
			}

			{
				Vector<Foo> v{ a,b,c,d };
				Assert::ExpectException<std::runtime_error>([&v]() {v.at(6); });
				Assert::AreEqual(a, v.at(0));
				Assert::AreEqual(d, v.at(3));
			}

			Vector<Foo> v;
			v.push_back(a);
			v.push_back(b);
			v.push_back(c);
			v.push_back(d);

			Assert::ExpectException<std::runtime_error>([&v]() {v[6]; });
			Assert::AreEqual(size_t{ 4 }, v.size());
			Assert::AreNotEqual(true, v.empty());
			Assert::AreEqual(a, v[0]);
			Assert::AreEqual(d, v[3]);

			v.pop_back();
			Assert::AreEqual(size_t{ 3 }, v.size());
			Assert::AreEqual(c, v[2]);

			v.pop_back();
			Assert::AreEqual(size_t{ 2 }, v.size());
			Assert::AreEqual(b, v[1]);

			v.pop_back();
			Assert::AreEqual(size_t{ 1 }, v.size());
			Assert::AreEqual(a, v[0]);

			v.pop_back();
			Assert::AreEqual(size_t{ 0 }, v.size());

			v.push_back(d);
			v.clear();
			Assert::AreEqual(size_t{ 0 }, v.size());
			Assert::AreEqual(true, v.empty());

			Assert::ExpectException<std::runtime_error>([&v]() {v.pop_back(); });
		}

		TEST_METHOD(emplace_back)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			Foo x{ 60 };
			Foo y{ 70 };

			{
				Vector<Foo> testV{ a,b,c };
				testV.emplace_back(60);
				Assert::AreEqual(x, testV.back());
				Assert::AreEqual(size_t{ 4 }, testV.size());
				testV.emplace_back(Foo{ 70 });
				Assert::AreEqual(y, testV.back());
				Assert::AreEqual(size_t{ 5 }, testV.size());
			}
		}

		TEST_METHOD(iterator)
		{
			
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> v;
			v.push_back(a);
			v.push_back(b);
			v.push_back(c);
			v.push_back(d);

			Vector<Foo>::iterator it{ v };
			Assert::AreEqual(a, *it);
			++it;
			Assert::AreEqual(b, *it);

			// Test post increment
			Assert::AreEqual(b, *(it++));

			Assert::AreEqual(c, *it);
			++it;
			Assert::AreEqual(d, *it);

			++it;
			Assert::ExpectException<std::runtime_error>([&it]() { ++it; });
			Assert::ExpectException<std::runtime_error>([&it]() { it++; });

			--it;
			Assert::AreEqual(d, *it);

			// copy constructor
			Vector<Foo>::iterator it2{ it };
			Assert::AreEqual(d, *it2);

			//assignment operator
			Vector<Foo>::iterator it3{ v };
			Assert::AreEqual(a, *it3);
			it3 = it2;
			Assert::AreEqual(d, *it3);

			Vector<Foo> v2{ a,b,c,d };
			Vector<Foo>::iterator falseIt{ v2 };
			Assert::ExpectException<std::runtime_error>([&falseIt, &it2]() {falseIt = it2; });

			Vector<Foo>::iterator it4{ v,2 };
			Assert::AreEqual(c, *it4);
			
		}

		TEST_METHOD(iteratorOperators)
		{
			
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> testVector{ a,b,c,d,e };
			Vector<Foo> testVector2{ a,b,c,d,e };

			{
				Vector<Foo>::iterator testIT1{ testVector };
				Assert::AreEqual(a, testIT1[0]);
				Assert::AreEqual(c, testIT1[2]);
				Assert::AreEqual(e, testIT1[4]);

				Assert::ExpectException<std::runtime_error>([&testIT1, &testVector]() {testIT1[8]; });
				testIT1 = testIT1 + 2;
				Assert::AreEqual(c, *testIT1);
				testIT1 = testIT1 + 1;
				Assert::AreEqual(d, *testIT1);

				testIT1 = testIT1 - 1;
				Assert::AreEqual(c, *testIT1);
			}
			
			{
				Vector<Foo>::iterator testIT1{ testVector };
				Vector<Foo>::iterator testIT2{ testVector2 };

				Assert::IsTrue(testIT1 != testIT2);
				Assert::IsFalse(testIT1 == testIT2);
			}

			{
				Vector<Foo>::iterator testIT1{ testVector.begin()};
				Vector<Foo>::iterator testIT2{ testVector.begin()};

				Assert::IsTrue(testIT1 == testIT2);
				Assert::IsFalse(testIT1 != testIT2);
				Assert::AreNotSame(testIT1, testIT2);
			}

			{
				Vector<Foo>::iterator testIT1{ testVector.begin() };
				Vector<Foo>::iterator testIT2{ testVector.end() };

				Assert::IsTrue(testIT1 != testIT2);
				Assert::IsFalse(testIT1 == testIT2);
				Assert::AreNotSame(testIT1, testIT2);
				Assert::AreNotEqual(testIT1, testIT2);
			}

			{
				Vector<Foo>::iterator testIT1;
				Vector<Foo>::iterator testIT2{ testVector.end() };

				testIT1 = testIT2;
				Assert::IsTrue(testIT1 == testIT2);
				Assert::IsFalse(testIT1 != testIT2);
				Assert::AreNotSame(testIT1, testIT2);
			}
		}

		TEST_METHOD(ConstIterator)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> v{ a,b,c,d };

			Vector<Foo>::const_iterator it{ v };
			Assert::AreEqual(a, *it);
			++it;
			Assert::AreEqual(b, *it);

			// Test post increment
			Assert::AreEqual(b, *(it++));

			Assert::AreEqual(c, *it);
			++it;
			Assert::AreEqual(d, *it);

			++it;
			Assert::ExpectException<std::runtime_error>([&it]() { ++it; });
			Assert::ExpectException<std::runtime_error>([&it]() { it++; });

			--it;
			Assert::AreEqual(d, *it);

			// copy constructor
			Vector<Foo>::const_iterator it2{ it };
			Assert::AreEqual(d, *it2);

			//assignment operator
			Vector<Foo>::const_iterator it3{ v };
			Assert::AreEqual(a, *it3);
			it3 = it2;
			Assert::AreEqual(d, *it3);

			Vector<Foo> v2{ a,b,c,d };
			Vector<Foo>::const_iterator falseIt{ v2 };
			Assert::ExpectException<std::runtime_error>([&falseIt, &it2]() {falseIt = it2; });

			Vector<Foo>::const_iterator it4{ v,2 };
			Assert::AreEqual(c, *it4);

			Vector<Foo>::iterator testIT1{ v };
			Vector<Foo>::const_iterator testCIT{ testIT1 };

			Assert::AreEqual(a, *testCIT);
		}

		TEST_METHOD(ConstIteratorOperators)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> testVector{ a,b,c,d,e };
			Vector<Foo> testVector2{ a,b,c,d,e };

			{
				Vector<Foo>::const_iterator testIT1{ testVector };
				Assert::AreEqual(a, testIT1[0]);
				Assert::AreEqual(c, testIT1[2]);
				Assert::AreEqual(e, testIT1[4]);

				Assert::ExpectException<std::runtime_error>([&testIT1, &testVector]() {testIT1[8]; });
				testIT1 = testIT1 + 2;
				Assert::AreEqual(c, *testIT1);
				testIT1 = testIT1 + 1;
				Assert::AreEqual(d, *testIT1);

				testIT1 = testIT1 - 1;
				Assert::AreEqual(c, *testIT1);
			}

			{
				Vector<Foo>::const_iterator testIT1{ testVector };
				Vector<Foo>::const_iterator testIT2{ testVector2 };

				Assert::IsTrue(testIT1 != testIT2);
				Assert::IsFalse(testIT1 == testIT2);
			}

			{
				Vector<Foo>::const_iterator testIT1{ testVector.begin() };
				Vector<Foo>::const_iterator testIT2{ testVector.begin() };

				Assert::IsTrue(testIT1 == testIT2);
				Assert::IsFalse(testIT1 != testIT2);
				Assert::AreNotSame(testIT1, testIT2);
			}

			{
				Vector<Foo>::const_iterator testIT1{ testVector.begin() };
				Vector<Foo>::const_iterator testIT2{ testVector.end() };

				Assert::IsTrue(testIT1 != testIT2);
				Assert::IsFalse(testIT1 == testIT2);
				Assert::AreNotSame(testIT1, testIT2);
				Assert::AreNotEqual(testIT1, testIT2);
			}

			{
				Vector<Foo>::const_iterator testIT1;
				Vector<Foo>::const_iterator testIT2{ testVector.end() };

				testIT1 = testIT2;
				Assert::IsTrue(testIT1 == testIT2);
				Assert::IsFalse(testIT1 != testIT2);
				Assert::AreNotSame(testIT1, testIT2);
			}
		}

		TEST_METHOD(BeginEnd)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			{
				const Vector<Foo> testVector{ a,b,c,d,e };

				Vector<Foo>::const_iterator it = testVector.cbegin();
				Assert::AreEqual(a, *it);
				it = testVector.cend();
				--it;
				Assert::AreEqual(e, *it);
			}

			Vector<Foo> testVector{ a,b,c,d,e };

			Vector<Foo>::iterator it = testVector.begin();
			Assert::AreEqual(a, *it);

			Assert::ExpectException<std::runtime_error>([&it]() {--it; });

			Vector<Foo>::const_iterator cit = testVector.begin();
			Assert::AreEqual(a, *cit);
			Assert::ExpectException<std::runtime_error>([&cit]() {--cit; });


			it = testVector.end();
			Assert::ExpectException<std::runtime_error>([&it]() {*it; });

			--it;
			Assert::AreEqual(e, *it);
		}

		TEST_METHOD(shrinkToFit)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };

			Vector<Foo> testVector{ a,b,c,d,e };
			testVector.shrink_to_fit();
			Assert::AreEqual(size_t{ 5 }, testVector.capacity());
			testVector.reserve(50);
			Assert::AreEqual(size_t{ 50 }, testVector.capacity());
			Assert::AreEqual(a, testVector[0]);
			Assert::AreEqual(e, testVector[4]);
			testVector.shrink_to_fit();
			Assert::AreEqual(size_t{ 5 }, testVector.capacity());
			Assert::AreEqual(a, testVector[0]);
			Assert::AreEqual(e, testVector[4]);
		}

		TEST_METHOD(CopyConstructor)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			Vector<Foo> testVector{ a,b,c,d,e };

			{
				Vector<Foo> v{ testVector };
				Assert::AreEqual(size_t{ 5 }, testVector.capacity());
				Assert::AreEqual(a, v[0]);
				Assert::AreNotSame(v[0], testVector[0]);
			}

			testVector.push_back(f);
			testVector.push_back(g);

			{
				Vector<Foo> v{ testVector };
				Assert::AreEqual(size_t{ 7 }, testVector.capacity());
				Assert::AreEqual(g, v[6]);
				Assert::AreNotSame(v[6], testVector[6]);
			}

			testVector.reserve(50);

			{
				Vector<Foo> v{ testVector };
				Assert::AreEqual(size_t{ 50 }, testVector.capacity());
				Assert::AreEqual(a, v[0]);
				Assert::AreEqual(g, v[6]);
			}
		}

		TEST_METHOD(CopyAssignment)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			Vector<Foo> testVector{ a,b,c };
			Vector<Foo> testVector2{ f,g,h,d,e };

			testVector2 = testVector;

			Assert::AreEqual(size_t{ 3 }, testVector2.size());
			Assert::AreEqual(testVector.size(), testVector2.size());
			Assert::AreEqual(a, testVector2[0]);
			Assert::AreEqual(c, testVector2[2]);

			Assert::IsFalse(testVector != testVector2);
			Assert::IsTrue(testVector == testVector2);
		}

		TEST_METHOD(FrontAndBack)
		{
			{
				Vector<Foo> emptyV;
				Assert::ExpectException<std::runtime_error>([&emptyV]() {emptyV[0]; });
				Assert::ExpectException<std::runtime_error>([&emptyV]() {Foo x = emptyV.front(); });
				Assert::ExpectException<std::runtime_error>([&emptyV]() {Foo x = emptyV.back(); });
			}

			{
				const Vector<Foo> emptyV;
				Assert::ExpectException<std::runtime_error>([&emptyV]() {emptyV[0]; });
				Assert::ExpectException<std::runtime_error>([&emptyV]() {Foo x = emptyV.front(); });
				Assert::ExpectException<std::runtime_error>([&emptyV]() {Foo x = emptyV.back(); });
			}

			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			Vector<Foo> testVector{ a,b,c,d,e };
			const Vector<Foo> testVector2{ a,b,c,d,e };

			Assert::AreEqual(a, testVector.front());
			Assert::AreEqual(e, testVector.back());

			Assert::AreEqual(a, testVector2.front());
			Assert::AreEqual(e, testVector2.back());
		}

		TEST_METHOD(MoveConstructor)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			Vector<Foo> testVector{ a,b,c,d,e };
			Vector<Foo> cannibalVector{ move(testVector) };

			Assert::AreEqual(a, cannibalVector.front());
			Assert::AreEqual(e, cannibalVector.back());

			Assert::AreEqual(size_t{ 0 }, testVector.capacity());
			Assert::ExpectException<std::runtime_error>([&testVector]() {testVector[0]; });
		}

		TEST_METHOD(MoveAssignment)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			Vector<Foo> testVector{ a,b,c,d,e };
			Vector<Foo> cannibalVector{ f,g,h };
			cannibalVector = move(testVector);

			Assert::AreEqual(size_t{ 5 }, cannibalVector.size());
			Assert::AreEqual(a, cannibalVector[0]);
			Assert::AreEqual(e, cannibalVector.back());

			cannibalVector = move(cannibalVector);
			Assert::AreEqual(size_t{ 5 }, cannibalVector.size());
			Assert::AreEqual(a, cannibalVector[0]);
			Assert::AreEqual(e, cannibalVector.back());
		}

		TEST_METHOD(Find)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			{
				Vector<Foo> testVector{ a,b,c,d,e };
				Vector<Foo>::iterator it = testVector.find(c);

				Assert::AreEqual(c, *it);
				++it;
				Assert::AreEqual(d, *it);
				it = testVector.find(h);
				Assert::AreEqual(it, testVector.end());
			}

			{
				const Vector<Foo> testVector{ a,b,c,d,e };
				Vector<Foo>::const_iterator it = testVector.find(c);

				Assert::AreEqual(c, *it);
				++it;
				Assert::AreEqual(d, *it);
				it = testVector.find(h);
				Assert::AreEqual(it, testVector.cend());
			}
		}

		TEST_METHOD(Erase)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			{
				Vector<Foo> testVector{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator it = testVector.find(c);
				Vector<Foo>::iterator testIT = testVector.erase(it);
				Assert::AreEqual(size_t{ 7 }, testVector.size());
				Assert::AreEqual(d, *testIT);
				it = testVector.find(c);
				Assert::AreEqual(it, testVector.end());
				it = testVector.find(h);
				testIT = testVector.erase(it);
				Assert::AreEqual(size_t{ 6 }, testVector.size());
				Assert::AreEqual(testIT, testVector.end());
				it = testVector.find(h);
				Assert::AreEqual(it, testVector.end());
			}

			{
				Vector<Foo> testVector{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator testIT = testVector.erase(c);
				Assert::AreEqual(size_t{ 7 }, testVector.size());
				Assert::AreEqual(d, *testIT);
				Vector<Foo>::iterator it = testVector.find(c);
				Assert::AreEqual(it, testVector.end());

				testIT = testVector.erase(h);
				Assert::AreEqual(size_t{ 6 }, testVector.size());
				Assert::AreEqual(testIT, testVector.end());
				it = testVector.find(h);
				Assert::AreEqual(it, testVector.end());
			}

			{
				Vector<Foo> testVector{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator it1{ testVector,9 };
				Assert::ExpectException<std::runtime_error>([&it1, &testVector]() {testVector.erase(it1); });
				Vector<Foo>::iterator it2;
				Assert::ExpectException<std::runtime_error>([&it2, &testVector]() {testVector.erase(it2); });
			}

		}

		TEST_METHOD(Remove)
		{
			const Foo a{ 10 };
			const Foo b{ 20 };
			const Foo c{ 30 };
			const Foo d{ 40 };
			const Foo e{ 50 };
			const Foo f{ 60 };
			const Foo g{ 70 };
			const Foo h{ 80 };

			{
				Vector<Foo> testV{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator start = testV.find(c);
				Vector<Foo>::iterator end = testV.find(f);
				testV.remove(start, end);
				Assert::AreEqual(size_t{ 5 }, testV.size());
				Assert::AreEqual(f, testV[2]);
				Assert::AreEqual(h, testV[4]);
			}

			{
				Vector<Foo> testV{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator start = testV.find(c);
				Vector<Foo>::iterator end = testV.find(h);
				testV.remove(start, end);
				Assert::AreEqual(size_t{ 3 }, testV.size());
				Assert::AreEqual(h, testV[2]);
			}

			{
				Vector<Foo> testV{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator start = testV.find(c);
				Vector<Foo>::iterator end = testV.end();
				testV.remove(start, end);
				Assert::AreEqual(size_t{ 2 }, testV.size());
				Assert::AreEqual(b, testV[1]);
			}

			{
				Vector<Foo> testV{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator start{ testV,10 };
				Vector<Foo>::iterator end{ testV,7 };
				Assert::ExpectException<std::runtime_error>([&testV, &start, &end]() {testV.remove(start, end); });
			}

			{
				Vector<Foo> testV{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator start{ testV,4 };
				Vector<Foo>::iterator end{ testV,11 };
				Assert::ExpectException<std::runtime_error>([&testV, &start, &end]() {testV.remove(start, end); });
			}

			{
				Vector<Foo> testV{ a,b,c,d,e,f,g,h };
				Vector<Foo>::iterator start{ testV,6 };
				Vector<Foo>::iterator end{ testV,4 };
				Assert::ExpectException<std::runtime_error>([&testV, &start, &end]() {testV.remove(start, end); });
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}