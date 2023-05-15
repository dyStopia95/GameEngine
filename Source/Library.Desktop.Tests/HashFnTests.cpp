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
	TEST_CLASS(HashFnTests)
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

		TEST_METHOD(Foohash)
		{
			FooHashFN fooFn;
			Foo a{ 10 };
			Foo b{ 20 };
			Foo c{ 10 };

			Assert::AreEqual(fooFn(a), fooFn(c));
			Assert::AreNotEqual(fooFn(a), fooFn(b));
		}

	private:
		inline static _CrtMemState _startMemState;
	};
}