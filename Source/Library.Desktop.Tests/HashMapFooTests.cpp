#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "vector.h"
#include "SList.h"
#include "HashMap.h"
#include "FooSpecializations.h"
#include "SList.h"
#include "vector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(HashMapFooTests)
	{
	public:

		struct FooHashFN final
		{
			size_t operator()(const Foo& key)
			{
				int32_t FooVal = key.Data();
				const auto hashPrime = 3;
				size_t hash = 5;
				const std::byte* data = reinterpret_cast<const std::byte*> (&FooVal);

				for (std::size_t i = 0; i < sizeof(std::int32_t); ++i)
				{
					hash += static_cast<size_t>(data[i]) * hashPrime;
				}

				return hash;
			}
		};

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

		TEST_METHOD(constructor)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			Assert::AreEqual(size_t{ 1 }, h.Size());
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);
			Assert::AreEqual(size_t{ 4 }, h.Size());
		}

		TEST_METHOD(insert)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };
			pair<Foo, int> p5{ 22,86 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			

			HashMap<Foo, int>::iterator it = (h.insert(p5)).first;
			Assert::AreEqual(67, (*it).second);
		}

		TEST_METHOD(find)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };
			pair<Foo, int> p5{ 22,86 };

			FooHashFN Foofn;

			{
				const HashMap<Foo, int> h{ 0, Foofn };
				Assert::ExpectException<std::runtime_error>([&h]() {h.Find(Foo{71}); });
			}

			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			{
				HashMap<Foo, int> h2{ 2,Foofn };
				h2.insert(p1);
				h2.insert(p2);
				h2.insert(p3);
				h2.insert(p4);

				{
					HashMap<Foo, int>::iterator it = h2.Find(Foo{ 23 });
					Assert::AreEqual(15, (*it).second);
				}

				const HashMap<Foo, int> h3{ h2 };
				
				{
					HashMap<Foo, int>::const_iterator it = h3.Find(Foo{ 23 });
					Assert::AreEqual(15, (*it).second);
				}
			}
			{
				const HashMap<Foo, int> h2{ h };
				HashMap<Foo, int>::const_iterator it = h2.Find(Foo{ 10 });
				
				Assert::IsTrue(it == h2.cend());
			}

			{
				HashMap<Foo, int>::iterator it = h.Find(Foo{ 71 });
				Assert::AreEqual(35, (*it).second);
			}
			{
				HashMap<Foo, int>::iterator it = h.begin();
				Assert::AreEqual(35, (*it).second);
			}
		}

		TEST_METHOD(iterator)
		{
			{
				HashMap<Foo, int>::iterator it;
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}
			{
				HashMap<Foo, int>::const_iterator it;
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}

			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };
			pair<Foo, int> p5{ 22,86 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			{
				HashMap<Foo, int>::const_iterator it = h.cend();
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}
			{
				HashMap<Foo, int>::iterator it = h.end();
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			{
				HashMap<Foo, int>::iterator it = h.begin();
				Assert::AreEqual(35, it->second);
			}
			{
				HashMap<Foo, int>::const_iterator it = h.begin();
				Assert::AreEqual(35, it->second);
				Assert::AreEqual(35, (*it).second);

				HashMap<Foo, int>::const_iterator it2 = h.Find(Foo{22});
				it = it2;
				Assert::AreEqual(67, it->second);

				HashMap<Foo, int>::const_iterator it3 {it2};
				Assert::AreEqual(67, it3->second);
			}


			h.Clear();
			//Assert::ExpectException<std::runtime_error>([&h]() { HashMap<Foo, int>::iterator it = h.Find(Foo{71}); });

			{
				HashMap<Foo, int>::const_iterator it = h.cend();
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}
			{
				HashMap<Foo, int>::iterator it = h.end();
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}

				HashMap<Foo, int> h2{ 0, Foofn };
			{
				HashMap<Foo, int>::const_iterator it = h.cend();
				Assert::ExpectException<std::runtime_error>([&it]() {it->second; });
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}
		}

		TEST_METHOD(IteratorIncrement)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			{
				HashMap<Foo, int>::iterator it = h.begin();
				Assert::AreEqual(35, it->second);

				++it;
				Assert::AreEqual(79, (*it).second);
				++it;
				Assert::AreEqual(67, (*it).second);
				++it;
				Assert::AreEqual(15, (*it).second);
				++it;
				Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });
				Assert::ExpectException<std::runtime_error>([&it, &h]() {++it; });
			}
			{
				HashMap<Foo, int>::iterator it = h.begin();
				Assert::AreEqual(35, it->second);

				it++;
				Assert::AreEqual(79, (*it).second);
				it++;
				Assert::AreEqual(67, (*it).second);
				it++;
				Assert::AreEqual(15, (*it).second);
				it++;
				Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });

				Assert::IsTrue(it == h.end());
				Assert::IsFalse(it != h.end());
				Assert::IsFalse(it == h.begin());
				Assert::IsTrue(it != h.begin());

				Assert::ExpectException<std::runtime_error>([&it, &h]() {it++; });
			}
			{
				HashMap<Foo, int>::const_iterator it = h.cbegin();
				Assert::AreEqual(35, it->second);

				++it;
				Assert::AreEqual(79, (*it).second);
				++it;
				Assert::AreEqual(67, (*it).second);
				++it;
				Assert::AreEqual(15, (*it).second);
				++it;
				Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });
				Assert::ExpectException<std::runtime_error>([&it, &h]() {++it; });
			}
			{
				HashMap<Foo, int>::const_iterator it = h.begin();
				Assert::AreEqual(35, it->second);

				it++;
				Assert::AreEqual(79, (*it).second);
				it++;
				Assert::AreEqual(67, (*it).second);
				it++;
				Assert::AreEqual(15, (*it).second);
				it++;
				Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });

				Assert::IsTrue(it == h.end());
				Assert::IsFalse(it != h.end());
				Assert::IsFalse(it == h.begin());
				Assert::IsTrue(it != h.begin());

				Assert::ExpectException<std::runtime_error>([&it, &h]() {it++; });
			}
		}

		TEST_METHOD(IndexOperator)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			{
				h[Foo{30}] = 55;
				HashMap<Foo, int>::iterator it = h.Find(Foo{ 30 });
				Assert::AreEqual(55, it->second);
			}

			const HashMap<Foo, int> h2{ h };

			h2[Foo{ 30 }];

			Foo x{ 21 };
			Foo y{ 62 };
			{
				auto [intRef, fresh] = h.CreateAt(x);
				Assert::IsTrue(fresh);
				(intRef->second) = 16;
				HashMap<Foo, int>::iterator itx = h.Find(Foo{ 21 });
				Assert::AreEqual(16, itx->second);
			}
			{
				auto [intRef, fresh] = h.CreateAt(x);
				Assert::IsTrue(!fresh);
			}
			{
				auto [intRef, fresh] = h.CreateAt(y);
				Assert::IsTrue(fresh);
			}
		}

		TEST_METHOD(beginend)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			{
				HashMap<Foo, int>::iterator it = h.begin();
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}
			{
				HashMap<Foo, int>::const_iterator it = h.cbegin();
				Assert::ExpectException<std::runtime_error>([&it]() {*it; });
			}
		}

		TEST_METHOD(containsKey)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			Assert::IsTrue(h.containsKey(Foo{ 23 }));
			Assert::IsFalse(h.containsKey(Foo{ 67 }));
		}

		TEST_METHOD(at)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);
			{
				Assert::AreEqual(35, h.At(Foo{ 71 }));
				Assert::ExpectException<std::runtime_error>([&h]() {h.At(Foo{ 67 }); });
			}

			const HashMap<Foo, int> h2{ h };
			{
				Assert::AreEqual(35, h2.At(Foo{ 71 }));
				Assert::ExpectException<std::runtime_error>([&h2]() {h2.At(Foo{ 67 }); });
			}

		}

		TEST_METHOD(Remove)
		{
			pair<Foo, int> p1{ 71,35 };
			pair<Foo, int> p2{ 23,15 };
			pair<Foo, int> p3{ 22,67 };
			pair<Foo, int> p4{ 52,79 };

			FooHashFN Foofn;
			HashMap<Foo, int> h{ 31, Foofn };

			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			h.Remove(Foo{ 67 });
			h.Remove(Foo{ 71 });
			h.Remove(Foo{ 23 });
			h.Remove(Foo{ 22 });
			h.Remove(Foo{ 52 });
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}