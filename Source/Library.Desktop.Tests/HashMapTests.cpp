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
	TEST_CLASS(HashMapTests)
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

		TEST_METHOD(Constructor)
		{
			{
				pair<int, int> p1{ 71,35 };
				pair<int, int> p2{ 23,15 };
				pair<int, int> p3{ 22,67 };
				pair<int, int> p4{ 52,79 };

				{
					pair<string, int> s1{ "A",35 };
					pair<string, int> s2{ "B",15 };
					pair<string, int> s3{ "C",67 };
					pair<string, int> s4{ "D",79 };

					HashMap<string, int> tHashMap{31};
					size_t k1 = tHashMap.hashFn("A");
					size_t k2 = tHashMap.hashFn("B");
					size_t k3 = tHashMap.hashFn("C");
					size_t k4 = tHashMap.hashFn("D");
					k1; k2; k3; k4;
					tHashMap.insert(s1);
					tHashMap.insert(s2);
					tHashMap.insert(s3);
					tHashMap.insert(s4);

					HashMap<string, int>::iterator it = tHashMap.Find("A");
					Assert::AreEqual(35, it->second);

					it = tHashMap.Find("B");
					Assert::AreEqual(15, it->second);
					
					it = tHashMap.Find("C");
					Assert::AreEqual(67, it->second);

					it = tHashMap.Find("D");
					Assert::AreEqual(79, it->second);

				}

				{
					HashMap<int, int> h{ p1,p2,p3,p4 };
					Assert::AreEqual(size_t{ 4 }, h.Size());
					HashMap<int, int>::iterator it = h.Find(71);
					Assert::AreEqual(35, (*it).second);
				}

				HashMap<int, int> h{ 31 };

				{
					HashMap<int, int>::iterator it = h.begin();
					Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });
				}

				size_t k1, k2, k3, k4;
				k1 = h.hashFn(p1.first);
				k2 = h.hashFn(p2.first);
				k3 = h.hashFn(p3.first);
				k4 = h.hashFn(p4.first);

				h.insert(p1);
				Assert::AreEqual(size_t{ 1 }, h.Size());

				h.insert(p2);
				h.insert(p3);
				h.insert(p4);
				Assert::AreEqual(size_t{ 4 }, h.Size());

				{
					HashMap<int, int>::iterator it = h.end();
					Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });
					Assert::ExpectException<std::runtime_error>([&it, &h]() {it->second; });
				}

				HashMap<int, int>::iterator it = h.Find(71);
				Assert::AreEqual(35, (*it).second);
			}
		}

		TEST_METHOD(insert)
		{
			{
				pair<int, int> p1{ 71,35 };
				pair<int, int> p2{ 23,15 };
				pair<int, int> p3{ 22,67 };
				pair<int, int> p4{ 52,79 };
				pair<int, int> p5{ 22,86 };


				HashMap<int, int> h{ 31 };

				{
					HashMap<int, int>::iterator it = (h.insert(p1)).first;
					Assert::AreEqual(size_t{ 1 }, h.Size());
					Assert::AreEqual(35, (*it).second);
				}
				{
					h.insert(p2);
					h.insert(p3);
					h.insert(p4);
					Assert::AreEqual(size_t{ 4 }, h.Size());
				}
				{
					HashMap<int, int>::iterator it = (h.insert(p5)).first;
					Assert::AreEqual(67, (*it).second);
				}
			}
		}

		TEST_METHOD(find)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };


			HashMap<int, int> h{ 31 };
			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			{
				HashMap<int, int>::iterator it = h.Find(71);
				Assert::AreEqual(35, (*it).second);
			}

			{
				HashMap<int, int>::iterator it = h.begin();
				Assert::AreEqual(35, (*it).second);
			}
		}

		TEST_METHOD(iterator)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };


			HashMap<int, int> h{ 31 };
			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			HashMap<int, int>::iterator it1 = h.begin();
			Assert::AreEqual(35, (*it1).second);
			HashMap<int, int>::iterator it2 = (h.insert(p2)).first;
			Assert::AreEqual(15, (*it2).second);
			it1 = it2;
			Assert::AreEqual(15, (*it1).second);

		}

		TEST_METHOD(itIncrement)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };


			HashMap<int, int> h{ 31 };
			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			{
				HashMap<int, int>::iterator it = h.Find(71);
				Assert::AreEqual(35, (*it).second);

				++it;
				Assert::AreEqual(79, (*it).second);
				++it;
				Assert::AreEqual(67, (*it).second);
				++it;
				Assert::AreEqual(15, (*it).second);
				++it;
				Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });
				Assert::IsTrue(it == h.end());
				Assert::IsFalse(it != h.end());
				Assert::IsFalse(it == h.begin());
				Assert::IsTrue(it != h.begin());
			}
			{
				HashMap<int, int>::iterator it = h.Find(71);
				Assert::AreEqual(35, (*it).second);

				it++;
				Assert::AreEqual(79, (*it).second);
				it++;
				Assert::AreEqual(67, (*it).second);
				it++;
				Assert::AreEqual(15, (*it).second);
				Assert::AreEqual(15, it->second);
				it++;
				Assert::ExpectException<std::runtime_error>([&it, &h]() {*it; });
			}
			{
				HashMap<int, int>::iterator it = h.end();
				Assert::ExpectException<std::runtime_error>([&it, &h]() {++it; });
			}
		}

		TEST_METHOD(constIterator)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };


			HashMap<int, int> hBase{ 31 };
			hBase.insert(p1);
			hBase.insert(p2);
			hBase.insert(p3);
			hBase.insert(p4);

			const HashMap<int, int> h = hBase;


			//HashMap<int, int>::const_iterator it1{ h.cbegin() };
			//Assert::AreEqual(35, (*it1).second);
		}

		TEST_METHOD(containKey)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };


			HashMap<int, int> h{ 31 };
			h.insert(p1);
			h.insert(p2);
			h.insert(p3);
			h.insert(p4);

			Assert::IsTrue(h.containsKey(71));
			Assert::IsFalse(h.containsKey(60));
		}

		TEST_METHOD(At)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };


			{
				HashMap<int, int> h{ 31 };

				//Assert::AreEqual(size_t{ 6 }, h.TestHash(52));

				h.insert(p1);
				h.insert(p2);
				h.insert(p3);
				h.insert(p4);

				Assert::AreEqual(35, h.At(71));
				Assert::AreEqual(15, h.At(23));
				Assert::AreEqual(67, h.At(22));
			}
			{
				HashMap<int, int> h{ 31 };

				h.insert(p1);
				h.insert(p2);
				h.insert(p3);
				h.insert(p4);

				const HashMap<int, int> h2 = h;

				Assert::AreEqual(35, h2.At(71));
				Assert::AreEqual(15, h2.At(23));
				Assert::AreEqual(67, h2.At(22));
				h.insert(p4);
			}

		}

		TEST_METHOD(indexOp)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };

			{
				HashMap<int, int> h{ 31 };

				h.insert(p1);
				h.insert(p2);
				h.insert(p3);
				h.insert(p4);

				int test = h[71];
				Assert::AreEqual(35, test);

				test = h[93];
				Assert::AreEqual(size_t{ 5 }, h.Size());
				Assert::AreEqual(0, test);
			}
		}

		TEST_METHOD(Clear)
		{
			
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };

			{
				HashMap<int, int> h{ 31 };

				h.insert(p1);
				h.insert(p2);
				h.insert(p3);
				h.insert(p4);

				h.Clear();
				//Assert::ExpectException<std::runtime_error>([&h]() { HashMap<int, int>::iterator it = h.Find(71); });
			}
		}

		TEST_METHOD(Remove)
		{
			pair<int, int> p1{ 71,35 };
			pair<int, int> p2{ 23,15 };
			pair<int, int> p3{ 22,67 };
			pair<int, int> p4{ 52,79 };

			{
				HashMap<int, int> h{ 31 };
				h.insert(p1);
				h.insert(p2);
				h.insert(p3);
				h.insert(p4);

				h.Remove(23);

				Assert::IsTrue(h.Find(23) == h.end());
				HashMap<int, int>::iterator it = h.begin();
				Assert::AreEqual(35, it->second);
				Assert::AreEqual(size_t{ 3 }, h.Size());
			}
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}