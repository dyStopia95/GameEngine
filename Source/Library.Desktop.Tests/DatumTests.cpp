#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "FooSpecializations.h"
#include "Datum.h"
#include "Scope.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	/*constexpr std::size_t operator "" _z(unsigned long long n)
	{
		return n;
	}*/

	TEST_CLASS(DatumTests)
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

		TEST_METHOD(Scopeadd)
		{
			Datum a{ 10,20,30 }, b{ "S1","S2","S3" }, c{ 1.f,2.f,3.f }, d, e;
			std::string aString{ "A" }, bString{ "B" }, cString{ "C" }, dString{ "D" }, eString{ "E" };
			std::pair<std::string, Datum> p1{ aString,a }, p2{ bString,b }, p3{ cString,c }, p4, p5;

			Datum testA{ Datum::DatumTypes::Table, size_t{4} };
			Scope tScope{ 5 };
			tScope.Insert(p1);
			tScope.Insert(p2);
			tScope.Insert(p3);

			testA.pushBack(&tScope);
			Scope* dummyScope = testA.Get<Scope*>(0);
			Datum* testDatum = dummyScope->Find("A");

			Scope tScope2{ 5 };
			tScope2.Insert(p1);
			tScope2.Insert(p2);
			tScope2.Insert(p3);

			testA.pushBack(&tScope2);

			Scope tScope3{ 5 };
			tScope3.Insert(p1);
			tScope3.Insert(p1);
			tScope3.Insert(p1);

			testA.pushBack(&tScope3);

			Assert::IsTrue(dummyScope = &tScope);
			Assert::IsTrue(*testDatum == a);

			size_t sIndex = testA.Find(&tScope2);
			Assert::AreEqual(size_t{ 1 }, sIndex);
		}

		TEST_METHOD(Constructor)
		{
			{
				Datum testA{ 1,2,3,4 };
				Assert::AreEqual(size_t{ 4 }, testA.Size());
				Assert::AreEqual(1, testA.frontAsInt());
				Assert::AreEqual(4, testA.backAsInt());
			}
			{
				Datum testA{ 1,2,3,4 };
				Datum testCopy{ testA };
				Assert::AreNotSame(testA.frontAsInt(), testCopy.frontAsInt());
				Assert::AreEqual(testA.frontAsInt(), testCopy.frontAsInt());
			}
			{
				Datum testA{ "blah!","asdf","nvjwaliv" };
				Datum testCopy{ testA };
				Assert::AreNotSame(testA.frontAsString(), testCopy.frontAsString());
				Assert::AreEqual(testA.frontAsString(), testCopy.frontAsString());
			}
			{
				Datum testD;
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.popBack(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.frontAsFloat(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.frontAsInt(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.frontAsMatrix(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.frontAsVector(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.frontAsString(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.frontAsPointer(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.backAsInt(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.backAsFloat(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.backAsString(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.backAsVector(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.backAsMatrix(); });
				Assert::ExpectException<std::runtime_error>([&testD]() {testD.backAsPointer(); });
				Datum testA{ Datum::DatumTypes::Integer, 2 };
				Assert::IsTrue(testA.Type() == Datum::DatumTypes::Integer);
				Assert::AreEqual(size_t{ 0 }, testA.Size());
			}
			{
				Assert::ExpectException<std::runtime_error>([]() {Datum testA{ Datum::DatumTypes::Unknown, 2 }; });
			}
			{
				Datum testA{ glm::vec4{0} };
				Assert::AreEqual(size_t{ 1 }, testA.Size());
				Assert::IsTrue(glm::vec4{ 0 } == testA.frontAsVector());
			}
			{
				Datum testA{ glm::mat4{0} };
				Assert::AreEqual(size_t{ 1 }, testA.Size());
				Assert::IsTrue(glm::mat4{ 0 } == testA.frontAsMatrix());
			}
		}

		TEST_METHOD(CopyAssignment)
		{
			{
				Datum testA{ 1,2,3,4,5,6 };
				Datum testDatum{ 1.f,2.f,3.f };
				//Assert::ExpectException<std::runtime_error>([&testA, &testDatum]() {testDatum = testA; });
			}
			{
				Datum testA{ 1,2,3,4,5,6 };
				Datum testDatum{ 5,2,3,6,7,8,3,2 };
				testDatum = testA;
				Assert::AreEqual(size_t{ 6 }, testDatum.Size());
				Assert::AreEqual(size_t{ 6 }, testDatum.getCapacity());
				Assert::AreEqual(1, testDatum.frontAsInt());
				Assert::AreEqual(6, testDatum.backAsInt());
			}
			{
				Datum testA{ 1,2,3,4,5,6 };
				Datum testDatum;
				testDatum = testA;
				Assert::AreEqual(size_t{ 6 }, testDatum.Size());
				Assert::AreEqual(size_t{ 6 }, testDatum.getCapacity());
				Assert::AreEqual(1, testDatum.frontAsInt());
				Assert::AreEqual(6, testDatum.backAsInt());
			}
			{
				Datum testA{ "blah!","asdf","nvjwaliv" };
				Datum testCopy{ "awevve","awivblibu",";onwvuh","libibuwve2" };
				testCopy = testA;
				Assert::AreNotSame(testA.frontAsString(), testCopy.frontAsString());
				Assert::AreEqual(testA.frontAsString(), testCopy.frontAsString());
			}
		}

		TEST_METHOD(MoveOperations)
		{
			{
				Datum testA{ 1,2,3,4,5,6 };
				Datum testDatum{ move(testA) };
				Assert::AreEqual(size_t{ 6 }, testDatum.Size());
				Assert::AreEqual(size_t{ 6 }, testDatum.getCapacity());
				Assert::AreEqual(1, testDatum.frontAsInt());
				Assert::AreEqual(6, testDatum.backAsInt());
			}
			{
				Datum testA{ 1,2,3,4,5,6 };
				Datum testDatum{ 5,2,3,6,7,8,3,2 };
				testDatum = move(testA);
				Assert::AreEqual(size_t{ 6 }, testDatum.Size());
				Assert::AreEqual(size_t{ 6 }, testDatum.getCapacity());
				Assert::AreEqual(1, testDatum.frontAsInt());
				Assert::AreEqual(6, testDatum.backAsInt());
			}
			{
				Datum testA{ "blah!","asdf","nvjwaliv" };
				Datum testCopy{ "awevve","awivblibu",";onwvuh","libibuwve2" };
				testCopy = move(testA);
				Assert::AreNotSame(string("blah!"), testCopy.frontAsString());
				Assert::AreEqual(string("blah!"), testCopy.frontAsString());
				Assert::AreEqual(string("nvjwaliv"), testCopy.backAsString());
			}
		}

		TEST_METHOD(SetType)
		{
			Datum testD;
			Assert::ExpectException<std::runtime_error>([&testD]() {testD.setCapacity(5); });
			Datum testA{ Datum::DatumTypes::Integer, 2 };
			testA.SetType(Datum::DatumTypes::Integer);
			Datum testB{ Datum::DatumTypes::Vector, 2 };

			testD.SetType(Datum::DatumTypes::Float);

			Assert::AreEqual(sizeof(int), testA.getDataSize());
			Assert::AreEqual(sizeof(float), testD.getDataSize());
			Assert::AreNotEqual(sizeof(glm::vec4), testD.getDataSize());

			Assert::AreEqual(sizeof(glm::vec4), testB.getDataSize());

			Assert::IsTrue(testD.Type() == Datum::DatumTypes::Float);
			Assert::ExpectException<std::runtime_error>([&testA]() {testA.SetType(Datum::DatumTypes::Float); });
		}

		TEST_METHOD(pushback)
		{
			{
				Datum testA{ Datum::DatumTypes::Integer, 0 };
				testA.pushBack(4);
				Assert::AreEqual(4, testA.frontAsInt());
				Assert::AreEqual(size_t{ 1 }, testA.Size());
				Assert::AreEqual(size_t{ 1 }, testA.getCapacity());
			}
			{
				Datum testC{ Datum::DatumTypes::String, 0 };
				testC.pushBack("awsen;!");
				Assert::AreEqual(std::string{ "awsen;!" }, testC.frontAsString());
				Assert::AreEqual(size_t{ 1 }, testC.Size());
				Assert::AreEqual(size_t{ 1 }, testC.getCapacity());
			}
			{
				Datum testA{ Datum::DatumTypes::Integer, 9 };
				Datum testB{ Datum::DatumTypes::Float, 9 };
				Datum testC{ Datum::DatumTypes::String, 9 };
				Datum testD{ Datum::DatumTypes::Vector, 9 };
				Datum testE{ Datum::DatumTypes::Vector, 9 };
				Datum testF{ Datum::DatumTypes::Pointer, 9 };

				Foo a{ 10 };
				RTTI* aRef = &a;

				testA.pushBack(4);
				Assert::IsFalse(testA != 4);
				Assert::IsTrue(testA != 5);
				Assert::IsTrue(testA != 5.f);
				Assert::IsTrue(testA != glm::vec4{0});
				Assert::IsTrue(testA != glm::mat4{0});
				Assert::IsTrue(testA != std::string(""));
				Assert::IsTrue(testA != aRef);

				testA.pushBack(5);

				Assert::IsTrue(testA != 4);
				Assert::IsFalse(testA == 4);

				{
					Datum testA2{ Datum::DatumTypes::Integer, 9 };
					testA2.pushBack(4);
					Assert::IsTrue(testA != testA2);
					Assert::IsFalse(testA == testA2);
					testA2.pushBack(5);
					Assert::IsFalse(testA != testA2);
					Assert::IsTrue(testA == testA2);
					testA2.popBack();
					testA2.pushBack(6);
					Assert::IsFalse(testA == testA2);
				}

				std::string testSt1 = "blah!";
				std::string testSt2 = "awefawger!";
				std::string testSt3 = "awefaasdfwger!";

				testC.pushBack(testSt1);
				Assert::IsFalse(testC != "blah!");

				testC.pushBack(testSt2);

				{
					Datum testC2{ Datum::DatumTypes::String, 9 };
					testC2.pushBack(testSt1);
					Assert::IsTrue(testC != testC2);
					testC2.pushBack(testSt2);
					Assert::IsFalse(testC != testC2);
					testC2.popBack();
					Assert::AreEqual(size_t{ 1 }, testC2.Size());
					testC2.pushBack(testSt3);
					Assert::IsTrue(testC != testC2);
				}

				{
					Assert::AreEqual(size_t{ 2 }, testC.Size());
					Assert::ExpectException<std::runtime_error>([&testB]() {testB.pushBack(2); });

					testC.popBack();
					Assert::AreEqual(size_t{ 1 }, testC.Size());
				}
			}
			{
				//test throw for external storage
			}
		}

		TEST_METHOD(Comparisons)
		{
			{
				Foo a{ 10 }, b{ 20 }, c{ 30 }, d{ 40 }, e{ 50 };
				RTTI* aRef = &a;
				RTTI* bRef = &b;
				RTTI* cRef = &c;
				RTTI* dRef = &d;
				RTTI* eRef = &e;
				eRef;

				Datum testA{ aRef,bRef,cRef };
				Datum dummy{ testA };
				Datum testB{ aRef,bRef,dRef };

				Assert::IsTrue(aRef->Equals(testA.frontAsPointer()));
				Assert::IsTrue(cRef->Equals(testA.backAsPointer()));
				Assert::IsTrue(dummy == testA);
				Assert::IsFalse(testB == testA);

				Assert::IsFalse(testA == 1);
			}
			{
				Datum testA{ 1.f,2.f,3.f,4.f };
				Datum testB{ 2.f,3.f,7.f,5.f };
				Assert::IsFalse(testA == testB);

				Datum testC{ 2.f };
				Assert::IsTrue(testC == 2.f);
				Assert::IsFalse(testC == 3.f);

				testC.pushBack(3.f);
				Assert::IsFalse(testC == 2.f);

			}
			{
				Datum testA{ glm::vec4{1} };
				Assert::IsFalse(testA == glm::vec4{ 2 });
				Assert::IsTrue(testA == glm::vec4{ 1 });

				testA.pushBack(glm::vec4{ 3 });
				Assert::IsFalse(testA == glm::vec4{ 1 });
			}
			{
				Datum testA{ glm::mat4{1} };
				Assert::IsFalse(testA == glm::mat4{ 0 });
				Assert::IsTrue(testA == glm::mat4{ 1 });

				testA.pushBack(glm::mat4{ 3 });
				Assert::IsFalse(testA == glm::mat4{ 1 });
			}
			{
				Datum testA{ "testing" };
				Assert::IsFalse(testA == std::string("blah!"));
				Assert::IsTrue(testA == std::string("testing"));

				testA.pushBack("smee!");
				Assert::IsFalse(testA == std::string("testing"));
			}
			{
				Foo a{ 10 };
				Foo b{ 10 };
				Foo c{ 20 };
				RTTI* aRef = &a;
				RTTI* bRef = &b;

				Datum testA{ aRef };
				Assert::IsTrue(testA == &a);
				Assert::IsTrue(testA == &b);
				Assert::IsFalse(testA == &c);

				testA.pushBack(bRef);
				Assert::IsFalse(testA == &a);
			}
		}

		TEST_METHOD(frontBack)
		{
			Datum testA{ Datum::DatumTypes::Integer, 9 };
			Datum testB{ Datum::DatumTypes::Float, 9 };
			Datum testC{ Datum::DatumTypes::String, 9 };
			Datum testD{ Datum::DatumTypes::Vector, 9 };
			Datum testE{ Datum::DatumTypes::Vector, 9 };
			Datum testF{ Datum::DatumTypes::Pointer, 9 };

			testA.pushBack(4);
			testA.pushBack(5);

			Assert::AreEqual(4, testA.frontAsInt());
			Assert::AreEqual(5, testA.backAsInt());
		}

		TEST_METHOD(shrinkToFit)
		{
			Datum testA{ Datum::DatumTypes::Integer, 9 };
			testA.pushBack(4);
			testA.pushBack(5);

			Assert::AreEqual(size_t{ 9 }, testA.getCapacity());
			Assert::AreEqual(size_t{ 2 }, testA.Size());
			testA.shrinkToFit();
			Assert::AreEqual(size_t{ 2 }, testA.getCapacity());
			Assert::AreEqual(size_t{ 2 }, testA.Size());
			testA.shrinkToFit();
			Assert::AreEqual(size_t{ 2 }, testA.getCapacity());

		}

		TEST_METHOD(clear)
		{
			{
				Datum testA{ 1,2,3,4,5 };
				Assert::AreEqual(size_t{ 5 }, testA.Size());
				testA.clear();
				Assert::AreEqual(size_t{ 0 }, testA.Size());
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.frontAsInt(); });
			}
			{
				Datum testA{ "asve","awvlrin","wvon;g" };
				Assert::AreEqual(size_t{ 3 }, testA.Size());
				testA.clear();
				Assert::AreEqual(size_t{ 0 }, testA.Size());
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.frontAsString(); });
			}

		}

		TEST_METHOD(Get)
		{
			{
				Datum testA{ 1,2,3,4,5 };
				int x = testA.Get<int>(2);
				Assert::AreEqual(x, 3);
				x = testA.Get<int>();
				Assert::AreEqual(x, 1);
				Assert::AreEqual(x, testA.frontAsInt()); // why is aresame fail?
			}
			{
				const Datum testA{ 1,2,3 };
				int x = testA.Get<int>(2);
				Assert::AreEqual(x, 3);
				x = testA.Get<int>();
				Assert::AreEqual(x, 1);
				Assert::ExpectException<runtime_error>([&testA]() {testA.Get<float>(2); });

				Assert::ExpectException<out_of_range>([&testA]() {testA.Get<int>(10); });
				Assert::ExpectException<out_of_range>([&testA]() {testA.Get<float>(10); });
				Assert::ExpectException<out_of_range>([&testA]() {testA.Get<string>(10); });
				Assert::ExpectException<out_of_range>([&testA]() {testA.Get<glm::vec4>(10); });
				Assert::ExpectException<out_of_range>([&testA]() {testA.Get<glm::mat4>(10); });
				Assert::ExpectException<out_of_range>([&testA]() {testA.Get<RTTI*>(10); });
			}
			{
				const Datum testA{ 1.f,2.f,3.f };
				float x = testA.Get<float>(2);
				Assert::AreEqual(x, 3.f);

				Assert::ExpectException<runtime_error>([&testA]() {testA.Get<int>(2); });
				Assert::ExpectException<runtime_error>([&testA]() {testA.Get<string>(2); });
				Assert::ExpectException<runtime_error>([&testA]() {testA.Get<glm::vec4>(2); });
				Assert::ExpectException<runtime_error>([&testA]() {testA.Get<glm::mat4>(2); });
				Assert::ExpectException<runtime_error>([&testA]() {testA.Get<RTTI*>(2); });
			}
			{
				const Datum testA{ "A","B","C"};
				string x = testA.Get<string>(2);
				Assert::IsTrue(x == "C");
			}
			{
				const Datum testA{ glm::vec4{1},glm::vec4{2},glm::vec4{3} };
				glm::vec4 x = testA.Get<glm::vec4>(2);
				Assert::IsTrue(x == glm::vec4{3});
			}
			{
				const Datum testA{ glm::mat4{1},glm::mat4{2},glm::mat4{3} };
				glm::mat4 x = testA.Get<glm::mat4>(2);
				Assert::IsTrue(x == glm::mat4{ 3 });
			}
			{
				Foo a{ 10 }, b{ 20 }, c{ 30 }, dummy{30};

				const Datum testA{ &a,&b,&c };
				RTTI* x = testA.Get<RTTI*>(2);
				Assert::IsTrue(dummy.Equals(x));
			}
		}

		TEST_METHOD(Set)
		{
			{
				Datum testA{ 1,2,3,4,5 };
				testA.Set(5, 2);
				Assert::AreEqual(5, testA.Get<int>(2));
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Set(12, 50); });
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Get<int>(12); });

				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Set(12.f, 2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Set(string{"A"}, 2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Set(glm::vec4{0}, 2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Set(glm::mat4{0}, 2); });
				
				Foo a{ 10 };
				Assert::ExpectException<std::runtime_error>([&testA,&a]() { testA.Set(&a, 2); });

				//testA.Get<float>(2);
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Get<float>(2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Get<string>(2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Get<glm::vec4>(2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Get<glm::mat4>(2); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Get<RTTI*>(2); });

			}
			{
				Datum testA{ 1.f,2.f,3.f,4.f };
				testA.Set(88.f, 2);
				Assert::AreEqual(88.f, testA.Get<float>(2));
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Set(12, 2); });

				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Set(12.f, 50); });
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Get<float>(12); });
				Assert::ExpectException<std::runtime_error>([&testA]() { testA.Get<int>(2); });
			}
			{
				Datum testA{ "A","B","C","D" };
				testA.Set(string("XYZ"), 2);
				Assert::AreEqual(string("XYZ"), testA.Get<string>(2));
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Set(string("XYZ"), 50); });
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Get<std::string>(12); });
			}
			{
				Datum testA{ glm::vec4{1},glm::vec4{2},glm::vec4{3} };
				testA.Set(glm::vec4{ 77 }, 2);
				Assert::IsTrue(glm::vec4{ 77 } == testA.Get<glm::vec4>(2));
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Set(glm::vec4{ 1 }, 50); });
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Get<glm::vec4>(12); });
			}
			{
				Datum testA{ glm::mat4{1},glm::mat4{2},glm::mat4{3} };
				testA.Set(glm::mat4{ 77 }, 2);
				Assert::IsTrue(glm::mat4{ 77 } == testA.Get<glm::mat4>(2));
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Set(glm::mat4{ 1 }, 50); });
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Get<glm::mat4>(12); });
			}
			{
				Foo a{ 10 }, b{ 20 }, c{ 30 }, d{ 40 };
				RTTI* aRef = &a;
				RTTI* bRef = &b;
				RTTI* cRef = &c;
				RTTI* dRef = &d;

				Datum testA{ aRef,bRef,cRef };
				testA.Set(dRef, 2);
				Assert::IsTrue(d.Equals( testA.Get<RTTI*>(2)));
				Assert::ExpectException<std::out_of_range>([&testA,&dRef]() { testA.Set(dRef, 50); });
				Assert::ExpectException<std::out_of_range>([&testA]() { testA.Get<RTTI*>(12); });
			}
		}

		TEST_METHOD(Find)
		{
			{
				Foo a{ 10 };

				Datum testA{ 1,2,3,4,5 };
				size_t index = testA.Find(4);
				Assert::AreEqual(size_t{3}, index);
				index = testA.Find(56);
				Assert::AreEqual(testA.Size(), index);
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.Find(36.f); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.Find(glm::mat4{2}); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.Find(std::string{"A"}); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.Find(glm::vec4{1}); });
				Assert::ExpectException<std::runtime_error>([&testA,&a]() {testA.Find(&a); });
			}
			{
				Foo a{ 10 };
				Foo b{ 20 };
				Foo c{ 30 };
				Foo d{ 20 };
				Foo x{ 99 };

				RTTI* aRef = &a;
				RTTI* bRef = &b;
				RTTI* cRef = &c;


				Datum testA{ aRef,bRef,cRef };

				size_t index = testA.Find(&d); //Accessing the Foo::Equals method correctly!!
				Assert::AreEqual(size_t{1}, index);
				index = testA.Find(&x);
				Assert::AreEqual(testA.Size(), index);
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.Find(1); });
			}
			{
				Datum testA{ "A","B","C" };
				size_t index = testA.Find("B");
				Assert::AreEqual(size_t{ 1 }, index);
				index = testA.Find("Blah");
				Assert::AreEqual(testA.Size(), index);
			}
			{
				Datum testA{ 1.f,2.f,3.f,4.f };
				size_t index = testA.Find(3.f);
				Assert::AreEqual(size_t{ 2 }, index);
				index = testA.Find(123.f);
				Assert::AreEqual(testA.Size(), index);
			}
			{
				Datum testA{ glm::vec4{1},glm::vec4{2},glm::vec4{3} };
				size_t index = testA.Find(glm::vec4{ 2 });
				Assert::AreEqual(size_t{ 1 }, index);
				index = testA.Find(glm::vec4{ 123 });
				Assert::AreEqual(testA.Size(), index);
			}
			{
				Datum testA{ glm::mat4{1},glm::mat4{2},glm::mat4{3} };
				size_t index = testA.Find(glm::mat4{ 2 });
				Assert::AreEqual(size_t{ 1 }, index);
				index = testA.Find(glm::mat4{ 123 });
				Assert::AreEqual(testA.Size(), index);
			}
		}

		TEST_METHOD(Remove)
		{
			{
				Datum testA{ 1,2,3,4,5 };
				Assert::IsTrue(testA.Remove(3));
				Assert::AreEqual(size_t{ 4 }, testA.Size());
				Assert::AreEqual(4, testA.Get<int>(2));
				Assert::IsFalse(testA.Remove(1234));
			}
			{
				Datum testA{ 1,2,3,4,5 };
				Assert::IsTrue(testA.RemoveAt(3));
				Assert::IsFalse(testA.RemoveAt(19));
				Assert::AreEqual(size_t{ 4 }, testA.Size());
				Assert::AreEqual(5, testA.Get<int>(3));
				Assert::IsFalse(testA.Remove(1234));
			}
			{
				Datum testA{ "A","B","C" };
				Assert::IsTrue(testA.Remove("A"));
				Assert::AreEqual(size_t{ 2 }, testA.Size());
				Assert::AreEqual(std::string{ "B" }, testA.frontAsString());
				Assert::AreEqual(std::string{ "C" }, testA.backAsString());
				Assert::IsTrue(testA.RemoveAt(0));
				Assert::AreEqual(size_t{ 1 }, testA.Size());
				Assert::AreEqual(std::string{ "C" }, testA.frontAsString());
				Assert::AreEqual(std::string{ "C" }, testA.backAsString());
			}
		}

		TEST_METHOD(Resize)
		{
			{
				Datum testA{ 1,2,3,4,5,6,7,8 };
				Assert::AreEqual(size_t{ 8 }, testA.getCapacity());
				testA.setCapacity(15);
				Assert::AreEqual(size_t{ 15 }, testA.getCapacity());
				testA.resize(8);
				Assert::AreEqual(size_t{ 8 }, testA.getCapacity());
			}
			{
				Datum testA{ 1,2,3,4,5,6,7,8 };
				testA.resize(15);
				Assert::AreEqual(size_t{ 15 }, testA.getCapacity());
				Assert::AreEqual(0, testA.Get<int>(8));
				Assert::AreEqual(0, testA.Get<int>(14));
			}
			{
				Datum testA{ 1,2,3,4,5,6,7,8 };
				testA.resize(5);

				Assert::AreEqual(size_t{ 5 }, testA.getCapacity());
				Assert::AreEqual(size_t{ 5 }, testA.Size());
				Assert::ExpectException<std::out_of_range>([&testA]() {testA.Get<int>(5); });
			}
			{
				Datum testA{ "A","B","C" };
				testA.resize(7);

				Assert::AreEqual(size_t{ 7 }, testA.getCapacity());
				Assert::AreEqual(size_t{ 7 }, testA.Size());
				Assert::AreEqual(string("A"), testA.frontAsString());
				Assert::AreEqual(string(""), testA.backAsString());
			}
			{
				Datum testA{ glm::vec4{1},glm::vec4{2} };
				testA.resize(4);
				Assert::AreEqual(size_t{ 4 }, testA.getCapacity());
				Assert::AreEqual(size_t{ 4 }, testA.Size());
				Assert::IsTrue(glm::vec4{ 1 } == testA.frontAsVector());
				Assert::IsTrue(glm::vec4{ 0 } == testA.backAsVector());
			}
			{
				Datum testA{ glm::mat4{1},glm::mat4{2} };
				testA.resize(4);
				Assert::AreEqual(size_t{ 4 }, testA.getCapacity());
				Assert::AreEqual(size_t{ 4 }, testA.Size());
				Assert::IsTrue(glm::mat4{ 1 } == testA.frontAsMatrix());
				Assert::IsTrue(glm::mat4{ 0 } == testA.backAsMatrix());
			}
			{
				Foo a{ 10 };
				RTTI* aRef = &a;
				Datum testA{ aRef };
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.resize(8); });
			}
		}

		TEST_METHOD(SetStorage)
		{
			{
				Foo x{ 30 };

				int a[] = { 1,2,3,4,5,6 };
				Datum testA;
				testA.setStorage(a, 6);
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.setCapacity(10); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.shrinkToFit(); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.resize(10); });
				//Assert::ExpectException<std::runtime_error>([&testA]() {testA.clear(); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.pushBack(1); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.pushBack(1.f); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.pushBack(std::string{"X"}); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.pushBack(glm::vec4{0}); });
				Assert::ExpectException<std::runtime_error>([&testA]() {testA.pushBack(glm::mat4{ 0 }); });
				Assert::ExpectException<std::runtime_error>([&testA,&x]() {testA.pushBack(&x); });
				Assert::AreEqual(size_t{ 6 }, testA.Size());
				Assert::AreEqual(size_t{ 6 }, testA.getCapacity());
				Assert::AreEqual(1, testA.frontAsInt());
				Assert::AreEqual(6, testA.backAsInt());

				Datum testB{ 1,2,3 };
				Assert::ExpectException<std::runtime_error>([&testB, &x]() {testB.pushBack(&x); });
				Assert::ExpectException<std::runtime_error>([&testB]() {testB.pushBack(1.f); });
				Assert::ExpectException<std::runtime_error>([&testB]() {testB.pushBack(string{"X"}); });
				Assert::ExpectException<std::runtime_error>([&testB]() {testB.pushBack(glm::vec4{1}); });
				Assert::ExpectException<std::runtime_error>([&testB]() {testB.pushBack(glm::mat4{1}); });
				Assert::ExpectException<std::runtime_error>([&testB, &a]() {testB.setStorage(a, 6); });
			}
			{
				float a[] = { 1.f,2.f,3.f,4.f,5.f,6.f };
				{
					Datum testA{ 1.f,2.f };
					Assert::ExpectException<std::runtime_error>([&testA, &a]() {testA.setStorage(a, 6); });
				}
				{
					Datum testA;
					testA.setStorage(a, 6);
					Assert::AreEqual(size_t{ 6 }, testA.Size());
					Assert::AreEqual(size_t{ 6 }, testA.getCapacity());
					Assert::AreEqual(1.f, testA.frontAsFloat());
					Assert::AreEqual(6.f, testA.backAsFloat());
				}
			}
			{
				glm::vec4 a[] = { glm::vec4{1},glm::vec4{2} };
				{
					Datum testA{ glm::vec4{5} };
					Assert::ExpectException<std::runtime_error>([&testA, &a]() {testA.setStorage(a, 6); });
				}
				{
					Datum testA;
					testA.setStorage(a, 2);
					Assert::AreEqual(size_t{ 2 }, testA.Size());
					Assert::AreEqual(size_t{ 2 }, testA.getCapacity());
					Assert::IsTrue(glm::vec4{ 1 } == testA.frontAsVector());
					Assert::IsTrue(glm::vec4{ 2 } == testA.backAsVector());
				}
			}
			{
				glm::mat4 a[] = { glm::mat4{1},glm::mat4{2} };
				{
					Datum testA{ glm::mat4{5} };
					Assert::ExpectException<std::runtime_error>([&testA, &a]() {testA.setStorage(a, 6); });
				}
				{
					Datum testA;
					testA.setStorage(a, 2);
					Assert::AreEqual(size_t{ 2 }, testA.Size());
					Assert::AreEqual(size_t{ 2 }, testA.getCapacity());
					Assert::IsTrue(glm::mat4{ 1 } == testA.frontAsMatrix());
					Assert::IsTrue(glm::mat4{ 2 } == testA.backAsMatrix());
				}
			}
			{
				std::string a[] = { "A","B" };
				{
					Datum testA{ std::string{"quack"} };
					Assert::ExpectException<std::runtime_error>([&testA, &a]() {testA.setStorage(a, 6); });
				}
				{
					Datum testA;
					testA.setStorage(a, 2);
					Assert::AreEqual(size_t{ 2 }, testA.Size());
					Assert::AreEqual(size_t{ 2 }, testA.getCapacity());
					Assert::IsTrue(std::string{"A"} == testA.frontAsString());
					Assert::IsTrue(std::string{"B"} == testA.backAsString());
				}
			}
			{
				Foo d{ 10 }, b{ 20 }, c{ 30 };
				RTTI* x = &d;
				RTTI* a[] = { &d,&b,&c };
				Foo d1{ 10 }, b1{ 20 }, c1{ 30 };
				
				{
					Datum testA{ x };
					Assert::ExpectException<std::runtime_error>([&testA, &a]() {testA.setStorage(a, 6); });
				}
				{
					Datum testA;
					testA.setStorage(a, 3);
					Assert::AreEqual(size_t{ 3 }, testA.Size());
					Assert::AreEqual(size_t{ 3 }, testA.getCapacity());
					Assert::IsTrue(testA.Get<RTTI*>()->Equals(a[0]));
					Assert::IsTrue(testA.Get<RTTI*>(2)->Equals(&c1));
					Assert::IsTrue(testA.Get<RTTI*>()->Equals(&d1));
				}
			}
		}
	};
}