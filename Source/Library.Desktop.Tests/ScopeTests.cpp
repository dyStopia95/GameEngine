#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "FooSpecializations.h"
#include "Scope.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(ScopeTests)
	{
	private:
		inline static _CrtMemState _startMemState;
		//Scope tScope;
		/*Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d, e;
		std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
		std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4, p5;*/

	public:

		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&_startMemState);
#endif

			//p1 = std::pair<std::string, Datum>{ aString,a };
			//p2 = std::pair<std::string, Datum>{ bString,b };
			//p3 = std::pair<std::string, Datum>{ cString,c };
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
			//tScope.Clear();

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

			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d, e;
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4, p5;
			Scope tScope1{5};

			tScope1.Insert(p1);
			tScope1.Insert(p2);
			tScope1.Insert(p3);

			{
				Datum dummyDatum = tScope1[0];
				Assert::IsTrue(dummyDatum == a);
				Assert::IsFalse(&dummyDatum == &a);
			}
			{
				Datum dummyDatum = tScope1[1];
				Assert::IsTrue(dummyDatum == b);
				Assert::IsFalse(&dummyDatum == &b);
			}
			Assert::AreEqual(size_t{ 3 }, tScope1.Size());
			{
				Assert::ExpectException<std::out_of_range>([&tScope1]() { Datum dummyDatum = tScope1[4]; });
			}

			Datum* tPointer = tScope1.Find("A");
			Assert::IsTrue(*tPointer == a);
			Assert::IsFalse(tPointer == &a);

			tPointer = tScope1.Find("B");
			Assert::IsTrue(*tPointer == b);
			Assert::AreEqual(size_t{ 1 }, tPointer->Find("S2"));
			Assert::AreEqual(tPointer->Size(), tPointer->Find("XYZ"));
			Assert::ExpectException<std::runtime_error>([&tPointer]() {tPointer->Find(1); });

			tPointer = tScope1.Find("C");
			Assert::IsTrue(*tPointer == c);

			tPointer = tScope1.Find("X");
			Assert::IsTrue(tPointer == nullptr);

			Datum& tDatum = tScope1.Append("XYZ");
			Assert::AreEqual(tDatum.Size(), size_t{ 0 });
			Assert::IsTrue(tDatum.Type() == Datum::DatumTypes::Unknown);

			tDatum.SetType(Datum::DatumTypes::Integer);
			tDatum.pushBack(70);
			tDatum.pushBack(80);
			tDatum.pushBack(90);

			Datum* tDatum2 = tScope1.Find("XYZ");
			Assert::AreEqual(tDatum2->Size(), size_t{ 3 });
			Assert::AreEqual(tDatum2->Find(80), size_t{ 1 });

			Datum& tDatum3 = tScope1.Append("XYZ");
			Assert::AreEqual(tDatum3.Size(), size_t{ 3 });
			Assert::AreEqual(tDatum3.Find(80), size_t{ 1 });

			tDatum = tScope1.Append("A");
			Assert::IsTrue(tDatum == a);

			Datum& dummyDatum = tScope1[0];
			dummyDatum.pushBack(40);
			tDatum2 = tScope1.Find("A");
			Assert::AreEqual(size_t{ 4 }, tDatum2->Size());
			Assert::AreEqual(40, tDatum2->Get<int>(3));
		}

		TEST_METHOD(Adopt)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			Datum scopeDatum{ Datum::DatumTypes::Table,5 };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" }, fString{"Fail"};
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e }, p6{ fString,scopeDatum };


			Scope* tScope1 = new Scope{5};
			Scope tScope2{ 5 };
			tScope1->Insert(p1);
			tScope1->Insert(p2);
			tScope1->Insert(p3);
			tScope2.Insert(p1);
			Assert::ExpectException<std::runtime_error>([&tScope1, &p6]() {tScope1->Insert(p6); });
			//tScope2.Adopt(*tScope1, aString);

			Assert::ExpectException<std::runtime_error>([&tScope1, &tScope2, &aString]() {tScope2.Adopt(*tScope1, aString); });
			tScope2.Adopt(*tScope1, "Child");

			tScope2.Insert(bString, b);

			Assert::IsTrue(tScope1->GetParent() == &tScope2);
			Assert::IsTrue(tScope2.GetParent() == nullptr);
		}

		TEST_METHOD(Search)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e };

			Scope* tScope = new Scope{ 5 };
			Scope* tScope1 = new Scope{ 5 };
			Scope* tScope2 = new Scope{ 5 };
			Scope* tScope3 = new Scope{ 5 };

			tScope->Adopt(*tScope1, "Layer1");
			tScope->Insert(p1);

			tScope1->Adopt(*tScope2, "Layer2");
			tScope1->Insert(p2);

			tScope2->Adopt(*tScope3, "Layer3");
			tScope2->Insert(p3);

			tScope3->Insert(p4);

			Scope** ContainingScope = new Scope*{};

			Datum* DatumtoFind;
			DatumtoFind = tScope->Search("X", ContainingScope);
			Assert::IsTrue(*ContainingScope == nullptr);
			Assert::IsTrue(DatumtoFind == nullptr);

			DatumtoFind = tScope->Search("A", ContainingScope);
			Assert::IsTrue(*DatumtoFind == a);
			Assert::IsTrue(*ContainingScope == tScope);

			DatumtoFind = tScope1->Search("A", ContainingScope);
			Assert::IsTrue(*DatumtoFind == a);
			Assert::IsFalse(DatumtoFind == &a);
			Assert::IsTrue(*ContainingScope == tScope);

			DatumtoFind = tScope1->Search("B", ContainingScope);
			Assert::IsTrue(*DatumtoFind == b);
			Assert::IsTrue(*ContainingScope == tScope1);

			DatumtoFind = tScope1->Search("C", ContainingScope);
			Assert::IsTrue(DatumtoFind == nullptr);
			Assert::IsTrue(*ContainingScope == nullptr);

			DatumtoFind = tScope2->Search("C", ContainingScope);
			Assert::IsTrue(*DatumtoFind == c);
			Assert::IsTrue(*ContainingScope == tScope2);

			DatumtoFind = tScope2->Search("B", ContainingScope);
			Assert::IsTrue(*DatumtoFind == b);
			Assert::IsTrue(*ContainingScope == tScope1);

			DatumtoFind = tScope1->Search("A", ContainingScope);
			Assert::IsTrue(*DatumtoFind == a);
			Assert::IsTrue(*ContainingScope == tScope);

			//Testing deleting layer2 child before parent shouldnt leak memory
			tScope2->~Scope();
			delete tScope2;
			//Success

			delete ContainingScope;
			delete tScope;

		}

		TEST_METHOD(FindContainedScope)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e };

			Scope* tScope = new Scope{ 5 };
			Scope* tScope1 = new Scope{ 5 };
			Scope* tScope2 = new Scope{ 5 };
			Scope* tScope3 = new Scope{ 5 };
			Scope* tScope4 = new Scope{ 5 };

			tScope1->Insert(p1);
			tScope->Adopt(*tScope1, "Layer1");
			Assert::AreEqual(tScope->Size(), size_t{ 1 });

			tScope2->Insert(p2);
			tScope->Adopt(*tScope2, "Layer1");
			Assert::AreEqual(tScope->Size(), size_t{ 1 });

			tScope3->Insert(p3);
			tScope->Adopt(*tScope3, "Layer1");
			Assert::AreEqual(tScope->Size(), size_t{ 1 });

			tScope4->Insert(p4);
			tScope->Adopt(*tScope4, "dummy");
			Assert::AreEqual(tScope->Size(), size_t{ 2 });

			Datum* tempDtm = tScope->Find("Layer1");
			/*Datum* tempDtm2;
			size_t index;*/
			{
				auto [x, y] = tScope->FindContainedScope(tScope1);
				Assert::IsTrue(x == tempDtm);
				Assert::IsTrue(y == size_t{ 0 });
			}
			{
				auto [x, y] = tScope->FindContainedScope(tScope2);
				Assert::IsTrue(x == tempDtm);
				Assert::IsTrue(y == size_t{ 1 });
			}
			{
				auto [x, y] = tScope->FindContainedScope(tScope3);
				Assert::IsTrue(x == tempDtm);
				Assert::IsTrue(y == size_t{ 2 });
			}

			tempDtm = tScope->Find("dummy");
			{
				auto [x, y] = tScope->FindContainedScope(tScope4);
				Assert::IsTrue(x == tempDtm);
				Assert::IsTrue(y == size_t{ 0 });
			}

			//checking Orphan()
			Scope* orphanPtr = tScope3->Orphan();
			Scope* invalidScope = nullptr;
			Assert::ExpectException<std::runtime_error>([&orphanPtr, &invalidScope]() {orphanPtr->FindContainedScope(invalidScope); });

			delete orphanPtr;
			delete tScope;

		}

		TEST_METHOD(CopyScope)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e };

			{
				/*Scope* tScope = new Scope{ 5 };

				tScope->Insert(p1);
				tScope->Insert(p2);
				tScope->Insert(p3);

				Scope* tScope1 = new Scope{ std::move(*tScope) };

				tScope->~Scope();
				delete tScope;
				delete tScope1;*/
			}

			{
				Scope* tScope = new Scope{ 5 };

				tScope->Insert(p1);
				tScope->Insert(p2);
				tScope->Insert(p3);

				Scope* tScope1 = new Scope{ *tScope };
				Datum* bufferDatum = tScope1->Find("A");
				Assert::IsTrue(*bufferDatum == a);
				bufferDatum = tScope1->Find("B");
				Assert::IsTrue(*bufferDatum == b);
				bufferDatum = tScope1->Find("C");
				Assert::IsTrue(*bufferDatum == c);
				bufferDatum = tScope1->Find("XYZ");
				Assert::IsTrue(bufferDatum == nullptr);

				Datum* bufferDatum2 = tScope->Find("A");
				Assert::IsTrue(bufferDatum != bufferDatum2);

				Datum& DatumRef = (*tScope1)[0]; //Weird syntax works??
				Assert::IsTrue(DatumRef == a);
				Datum& DatumRef2 = tScope1->operator[](1);
				Assert::IsTrue(DatumRef2 == b);

				delete tScope;
				delete tScope1;
			}

			{
				Scope* tScope = new Scope{ 5 };
				Scope* tScope2 = new Scope{ 5 };
				tScope->Insert(p1);
				tScope->Adopt(*tScope2, "Layer1");
				tScope2->Insert(p2);
				Datum* tDatum = tScope->Find("Layer1");
				Assert::IsTrue((*tDatum)[0] == tScope2);

				Scope* tScope3 = new Scope{ *tScope };
				Assert::IsTrue(tScope3->Size() == size_t{ 2 });
				Datum* ScopeDatumOf3 = tScope3->Find("Layer1");
				Scope* ChildOf3 = (*ScopeDatumOf3)[0];
				Datum* stringDatumofChild = ChildOf3->Find("B");
				stringDatumofChild;
				Assert::IsTrue(stringDatumofChild[0] == b);

				delete tScope3;

				Scope S1{ 5 };
				S1.Insert(p3);
				S1.Insert(p4);

				S1 = *tScope;
				Assert::AreEqual(S1.Size(), size_t{ 2 });
				ScopeDatumOf3 = S1.Find("Layer1");
				ChildOf3 = (*ScopeDatumOf3)[0];
				stringDatumofChild = ChildOf3->Find("B");
				Assert::IsTrue(stringDatumofChild[0] == b);

				delete tScope;
			}
		}

		TEST_METHOD(AppendScope)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e };

			Scope* SBase = new Scope{ 5 };
			SBase->Insert(p1);
			Scope& child = SBase->AppendScope("L1");
			child;
			delete SBase;
		}

		TEST_METHOD(Clone)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e };

			Scope* tScope = new Scope{ 5 };

			tScope->Insert(p1);
			tScope->Insert(p2);
			tScope->Insert(p3);

			Scope* cloneScope = tScope->Clone();
			Assert::AreEqual(cloneScope->Size(), size_t{ 3 });

			Assert::IsTrue(*(cloneScope->Find("A")) == a);
			Assert::IsTrue(cloneScope->Find("A") != tScope->Find("A"));

			delete tScope;
			delete cloneScope;
		}

		TEST_METHOD(Equality)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d{ 40,50,60 }, e{ 4.f,5.f,6.f };
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4{ dString,d }, p5{ eString,e }, trojanPair{ cString,e };

			Scope* S1 = new Scope{ 5 };
			Scope* S2 = new Scope{ 5 };
			Scope* S3 = new Scope{ 5 };

			S1->Insert(p1);
			S1->Insert(p2);
			S1->Insert(p3);
			
			S2->Insert(p1);
			S2->Insert(p2);
			S2->Insert(p3);

			S3->Insert(p1);
			S3->Insert(p1);
			S3->Insert(trojanPair);

			Assert::IsTrue(*S1 == *S2);
			Assert::IsFalse(*S1 != *S2);

			Assert::IsFalse(*S1 == *S3);
			Assert::IsTrue(*S1 != *S3);

			delete S1;
			delete S2;
			delete S3;
		}
	};
}
