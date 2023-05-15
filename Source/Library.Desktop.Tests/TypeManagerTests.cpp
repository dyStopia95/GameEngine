#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "FooSpecializations.h"
#include "Attributed.h"
#include "ManagerTestClasses.h"
#include "TypeManager.h"
//#include "AttributedFoo.h"



using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(TypeManagerTests)
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

		TEST_METHOD(testingBasic)
		{
			//Vector

			Vector<Signature> testV{ Bar1::Signatures() };
			Vector<Signature> compareV{
				{ "IntX", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(Bar1, x) },
				{ "IntY", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(Bar1, y) }
			};

			std::size_t idNo = Bar1::TypeIdClass();
			idNo;
			for (size_t i = 0; i < testV.size(); ++i)
			{
				Assert::IsTrue(testV[i] == compareV[i]);
			}

			Derived1 d1;

			//TypeManager::Add(Bar1::TypeIdClass(), Bar1::Signatures());

			RegisterType<Bar1>();
			Vector<Signature>& retrievedSignature = TypeManager::RetrieveSignature(Bar1::TypeIdClass());

			for (size_t i = 0; i < retrievedSignature.size(); ++i)
			{
				Assert::IsTrue(retrievedSignature[i] == compareV[i]);
			}

			//Assert::IsTrue(retrievedSignature == compareV);
			TypeManager::Clear();
		}
	};
}

			//std::vector
			//std::vector<Signature> testV{ Bar1::Signatures() };
			//std::vector<Signature> compareV{
			//	{ "IntX", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(Bar1, x) },
			//	{ "IntY", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(Bar1, y) }
			//};

			//std::size_t idNo = Bar1::TypeIdClass();
			//idNo;
			//for (size_t i = 0; i < testV.size(); ++i)
			//{
			//	Assert::IsTrue(testV[i] == compareV[i]);
			//}

			//Derived1 d1;

			//TypeManager::Add(Bar1::TypeIdClass(), Bar1::Signatures());
			//std::vector<Signature>& retrievedSignature = TypeManager::RetrieveSignature(Bar1::TypeIdClass());

			//for (size_t i = 0; i < retrievedSignature.size(); ++i)
			//{
			//	Assert::IsTrue(retrievedSignature[i] == compareV[i]);
			//}

			////Assert::IsTrue(retrievedSignature == compareV);
			//TypeManager::Clear();