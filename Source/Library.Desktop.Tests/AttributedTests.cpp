#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "FooSpecializations.h"
#include "Attributed.h"
#include "AttributedFoo.h"
//#include "ManagerTestClasses.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace LibraryDesktopTests
{
	TEST_CLASS(AttributedTests)
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
			RegisterType<AttributedFoo>();
			Vector<Signature> retrievedSig = TypeManager::RetrieveSignature(AttributedFoo::TypeIdClass());
			Vector<Signature> compareSig = 
			{
			{ "ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat", Datum::DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", Datum::DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", Datum::DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", Datum::DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", Datum::DatumTypes::Integer, 5, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray", Datum::DatumTypes::Float, 5, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray", Datum::DatumTypes::String, 5, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", Datum::DatumTypes::Vector, 5, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", Datum::DatumTypes::Matrix, 5, offsetof(AttributedFoo, ExternalMatrixArray) }
			};

			Assert::IsTrue(retrievedSig == compareSig);
			Assert::AreEqual(std::string{ "ExternalInteger" }, retrievedSig[0].Key);
			Assert::AreEqual(std::string{ "ExternalFloat" }, retrievedSig[1].Key);
			Assert::AreEqual(std::string{ "ExternalString" }, retrievedSig[2].Key);
			Assert::AreEqual(std::string{ "ExternalVector" }, retrievedSig[3].Key);
			Assert::AreEqual(std::string{ "ExternalMatrix" }, retrievedSig[4].Key);

			AttributedFoo dummyOBJ;
			dummyOBJ.ExternalInteger = 5;
			dummyOBJ.ExternalFloat = 36.f;
			dummyOBJ.ExternalString = "testString!";
			dummyOBJ.ExternalVector = glm::vec4{ 1 };
			dummyOBJ.ExternalMatrix = glm::mat4{ 33 };

			dummyOBJ.ExternalIntegerArray[0] = 1;
			dummyOBJ.ExternalIntegerArray[1] = 3;
			dummyOBJ.ExternalIntegerArray[2] = 5;
			dummyOBJ.ExternalIntegerArray[3] = 7;
			dummyOBJ.ExternalIntegerArray[4] = 9;

			dummyOBJ.ExternalFloatArray[0] = 1.f;
			dummyOBJ.ExternalFloatArray[1] = 3.f;
			dummyOBJ.ExternalFloatArray[2] = 5.f;
			dummyOBJ.ExternalFloatArray[3] = 7.f;
			dummyOBJ.ExternalFloatArray[4] = 9.f;

			dummyOBJ.ExternalStringArray[0] = "kekw1";
			dummyOBJ.ExternalStringArray[1] = "kekw2";
			dummyOBJ.ExternalStringArray[2] = "kekw3";
			dummyOBJ.ExternalStringArray[3] = "kekw4";
			dummyOBJ.ExternalStringArray[4] = "kekw5";

			dummyOBJ.ExternalVectorArray[0] = glm::vec4{ 1 };
			dummyOBJ.ExternalVectorArray[1] = glm::vec4{ 2 };
			dummyOBJ.ExternalVectorArray[2] = glm::vec4{ 3 };
			dummyOBJ.ExternalVectorArray[3] = glm::vec4{ 4 };
			dummyOBJ.ExternalVectorArray[4] = glm::vec4{ 5 };

			dummyOBJ.ExternalMatrixArray[0] = glm::mat4{ 33 };
			dummyOBJ.ExternalMatrixArray[1] = glm::mat4{ 23 };
			dummyOBJ.ExternalMatrixArray[2] = glm::mat4{ 13 };
			dummyOBJ.ExternalMatrixArray[3] = glm::mat4{ 43 };
			dummyOBJ.ExternalMatrixArray[4] = glm::mat4{ 53 };

			Datum* myPtr = dummyOBJ.Find("This");
			Assert::IsTrue(myPtr->Get<RTTI*>(0) == &dummyOBJ);

			Datum* D = dummyOBJ.Find("ExternalInteger");
			Assert::AreEqual(D->Get<int>(0), dummyOBJ.ExternalInteger);
			dummyOBJ.ExternalInteger = 95;
			Assert::AreEqual(D->Get<int>(0), 95);
			D->Set(170);
			Assert::AreEqual(dummyOBJ.ExternalInteger, 170);

			D = dummyOBJ.Find("ExternalFloat");
			Assert::AreEqual(D->Get<float>(0), dummyOBJ.ExternalFloat);
			D = dummyOBJ.Find("ExternalString");
			Assert::AreEqual(D->Get<std::string>(0), dummyOBJ.ExternalString);
			D = dummyOBJ.Find("ExternalVector");
			Assert::IsTrue(D->Get<glm::vec4>(0) == dummyOBJ.ExternalVector);
			D = dummyOBJ.Find("ExternalMatrix");
			Assert::IsTrue(D->Get<glm::mat4>(0) == dummyOBJ.ExternalMatrix);

			D = dummyOBJ.Find("ExternalIntegerArray");
			for (size_t i = 0; i < dummyOBJ.ArraySize; ++i)
			{
				Assert::AreEqual(D->Get<int>(i), dummyOBJ.ExternalIntegerArray[i]);
			}

			D = dummyOBJ.Find("ExternalFloatArray");
			for (size_t i = 0; i < dummyOBJ.ArraySize; ++i)
			{
				Assert::AreEqual(D->Get<float>(i), dummyOBJ.ExternalFloatArray[i]);
			}

			D = dummyOBJ.Find("ExternalStringArray");
			for (size_t i = 0; i < dummyOBJ.ArraySize; ++i)
			{
				Assert::AreEqual(D->Get<std::string>(i), dummyOBJ.ExternalStringArray[i]);
			}

			D = dummyOBJ.Find("ExternalVectorArray");
			for (size_t i = 0; i < dummyOBJ.ArraySize; ++i)
			{
				Assert::IsTrue(D->Get<glm::vec4>(i) == dummyOBJ.ExternalVectorArray[i]);
			}

			D = dummyOBJ.Find("ExternalMatrixArray");
			for (size_t i = 0; i < dummyOBJ.ArraySize; ++i)
			{
				Assert::IsTrue(D->Get<glm::mat4>(i) == dummyOBJ.ExternalMatrixArray[i]);
			}

			TypeManager::Clear();
		}

		TEST_METHOD(copy)
		{
			RegisterType<AttributedFoo>();

			int a = 10;
			Datum tD;
			tD.setStorage(&a,1);
			Datum tD2{tD};

			tD2.Set(25);
			Assert::AreEqual(a, 25);

			AttributedFoo dummyOBJ;

			dummyOBJ.ExternalInteger = 5;
			dummyOBJ.ExternalFloat = 36.f;
			dummyOBJ.ExternalString = "testString!";
			dummyOBJ.ExternalVector = glm::vec4{ 1 };
			dummyOBJ.ExternalMatrix = glm::mat4{ 33 };

			dummyOBJ.ExternalIntegerArray[0] = 1;
			dummyOBJ.ExternalIntegerArray[1] = 3;
			dummyOBJ.ExternalIntegerArray[2] = 5;
			dummyOBJ.ExternalIntegerArray[3] = 7;
			dummyOBJ.ExternalIntegerArray[4] = 9;

			dummyOBJ.ExternalFloatArray[0] = 1.f;
			dummyOBJ.ExternalFloatArray[1] = 3.f;
			dummyOBJ.ExternalFloatArray[2] = 5.f;
			dummyOBJ.ExternalFloatArray[3] = 7.f;
			dummyOBJ.ExternalFloatArray[4] = 9.f;

			dummyOBJ.ExternalStringArray[0] = "kekw1";
			dummyOBJ.ExternalStringArray[1] = "kekw2";
			dummyOBJ.ExternalStringArray[2] = "kekw3";
			dummyOBJ.ExternalStringArray[3] = "kekw4";
			dummyOBJ.ExternalStringArray[4] = "kekw5";

			dummyOBJ.ExternalVectorArray[0] = glm::vec4{ 1 };
			dummyOBJ.ExternalVectorArray[1] = glm::vec4{ 2 };
			dummyOBJ.ExternalVectorArray[2] = glm::vec4{ 3 };
			dummyOBJ.ExternalVectorArray[3] = glm::vec4{ 4 };
			dummyOBJ.ExternalVectorArray[4] = glm::vec4{ 5 };

			dummyOBJ.ExternalMatrixArray[0] = glm::mat4{ 33 };
			dummyOBJ.ExternalMatrixArray[1] = glm::mat4{ 23 };
			dummyOBJ.ExternalMatrixArray[2] = glm::mat4{ 13 };
			dummyOBJ.ExternalMatrixArray[3] = glm::mat4{ 43 };
			dummyOBJ.ExternalMatrixArray[4] = glm::mat4{ 53 };

			AttributedFoo copyOBJ{ dummyOBJ };


			Datum* myPtr = dummyOBJ.Find("This");
			RTTI* originalTHISptr = myPtr->Get<RTTI*>(0);
			originalTHISptr;
			myPtr = copyOBJ.Find("This");
			RTTI* copyTHISptr = myPtr->Get<RTTI*>(0);
			copyTHISptr;

			Assert::IsTrue(myPtr->Get<RTTI*>(0) == &copyOBJ);

			Datum* D = copyOBJ.Find("ExternalInteger");
			Assert::AreEqual(D->Get<int>(0), copyOBJ.ExternalInteger);
			copyOBJ.ExternalInteger = 95;
			Assert::AreEqual(D->Get<int>(0), 95);
			D->Set(170);
			Assert::AreEqual(copyOBJ.ExternalInteger, 170);

			D = copyOBJ.Find("ExternalFloat");
			Assert::AreEqual(D->Get<float>(0), copyOBJ.ExternalFloat);
			D = copyOBJ.Find("ExternalString");
			Assert::AreEqual(D->Get<std::string>(0), copyOBJ.ExternalString);
			D = copyOBJ.Find("ExternalVector");
			Assert::IsTrue(D->Get<glm::vec4>(0) == copyOBJ.ExternalVector);
			D = copyOBJ.Find("ExternalMatrix");
			Assert::IsTrue(D->Get<glm::mat4>(0) == copyOBJ.ExternalMatrix);

			D = copyOBJ.Find("ExternalIntegerArray");
			for (size_t i = 0; i < copyOBJ.ArraySize; ++i)
			{
				Assert::AreEqual(D->Get<int>(i), copyOBJ.ExternalIntegerArray[i]);
			}

			D = copyOBJ.Find("ExternalFloatArray");
			for (size_t i = 0; i < copyOBJ.ArraySize; ++i)
			{
				Assert::AreEqual(D->Get<float>(i), copyOBJ.ExternalFloatArray[i]);
			}

			D = copyOBJ.Find("ExternalStringArray");
			for (size_t i = 0; i < copyOBJ.ArraySize; ++i)
			{
				Assert::AreEqual(D->Get<std::string>(i), copyOBJ.ExternalStringArray[i]);
			}

			D = copyOBJ.Find("ExternalVectorArray");
			for (size_t i = 0; i < copyOBJ.ArraySize; ++i)
			{
				Assert::IsTrue(D->Get<glm::vec4>(i) == copyOBJ.ExternalVectorArray[i]);
			}

			D = copyOBJ.Find("ExternalMatrixArray");
			for (size_t i = 0; i < copyOBJ.ArraySize; ++i)
			{
				Assert::IsTrue(D->Get<glm::mat4>(i) == copyOBJ.ExternalMatrixArray[i]);
			}



			TypeManager::Clear();
		}

		TEST_METHOD(copysimpleTests)
		{
			RegisterType<AttFoo2>();
			AttFoo2 OBJ1;
			OBJ1.ExternalInteger = 5;
			OBJ1.ExternalIntegerArray[0] = 1;
			OBJ1.ExternalIntegerArray[1] = 3;
			OBJ1.ExternalIntegerArray[2] = 5;
			OBJ1.ExternalIntegerArray[3] = 7;
			OBJ1.ExternalIntegerArray[4] = 9;

			Datum* nestedTDatum = OBJ1.Find("NestedScope");
			Assert::IsTrue(nestedTDatum->Type() == Datum::DatumTypes::Table);

			AttFoo2 OBJ2{ OBJ1 };

			nestedTDatum = OBJ2.Find("NestedScope");
			Assert::IsTrue(nestedTDatum->Type() == Datum::DatumTypes::Table);

			Assert::AreEqual(OBJ2.ExternalInteger, 5);
			Datum* D = OBJ1.Find("ExternalInteger");
			int& tInt = D->Get<int>(0);
			D = OBJ2.Find("ExternalInteger");
			int& tInt2 = D->Get<int>(0);
			tInt2 = 56;

			Assert::AreEqual(5, tInt);
			Assert::AreEqual(56, tInt2);

			int* iadd1 = &tInt;
			int* iadd2 = &tInt2;
			Assert::IsFalse(iadd1 == iadd2);

			D = OBJ1.Find("ExternalIntegerArray");
			int arr1 = D->Get<int>(4);
			Assert::AreEqual(9, arr1);

			D = OBJ2.Find("ExternalIntegerArray");
			int arr2 = D->Get<int>(4);
			Assert::AreEqual(9, arr2);


			D = OBJ1.Find("This");
			RTTI* ptr1 = D->frontAsPointer();
			D = OBJ2.Find("This");
			RTTI* ptr2 = D->frontAsPointer();
			Assert::IsTrue(ptr1 != ptr2);

			AttFoo2 OBJ3;
			OBJ3.ExternalInteger = 77;
			OBJ3.ExternalIntegerArray[0] = 12;
			OBJ3.ExternalIntegerArray[1] = 51;
			OBJ3.ExternalIntegerArray[2] = 12;
			OBJ3.ExternalIntegerArray[3] = 78;
			OBJ3.ExternalIntegerArray[4] = 15;

			OBJ3 = OBJ1;
			D = OBJ3.Find("ExternalInteger");
			int I = D->Get<int>();
			Assert::AreEqual(I, 5);
			TypeManager::Clear();
		}

		TEST_METHOD(isAttributechecks)
		{
			RegisterType<AttributedFoo>();

			AttributedFoo OBJ1;

			Assert::ExpectException<std::runtime_error>([&OBJ1]() {OBJ1.AppendAuxiliaryAttribute("ExternalInteger"); });

			OBJ1.AppendAuxiliaryAttribute("Aux1");
			OBJ1.AppendAuxiliaryAttribute("Aux2");

			Assert::IsTrue(OBJ1.IsAttribute("ExternalInteger"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalFloat"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalString"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalVector"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalIntegerArray"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalFloatArray"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalStringArray"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalVectorArray"));
			Assert::IsTrue(OBJ1.IsAttribute("ExternalMatrixArray"));
			Assert::IsTrue(OBJ1.IsAttribute("Aux1"));
			Assert::IsTrue(OBJ1.IsAttribute("Aux2"));
			Assert::IsFalse(OBJ1.IsAttribute("Blah!"));

			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalIntegerArray"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalFloatArray"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalStringArray"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalVectorArray"));
			Assert::IsTrue(OBJ1.IsPrescribedAttribute("ExternalMatrixArray"));
			Assert::IsFalse(OBJ1.IsPrescribedAttribute("Aux1"));
			Assert::IsFalse(OBJ1.IsPrescribedAttribute("Aux2"));
			Assert::IsFalse(OBJ1.IsPrescribedAttribute("Blah!"));

			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalFloat"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalString"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalVector"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalMatrix"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalIntegerArray"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalFloatArray"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalStringArray"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalVectorArray"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("ExternalMatrixArray"));
			Assert::IsTrue(OBJ1.IsAuxiliaryAttribute("Aux1"));
			Assert::IsTrue(OBJ1.IsAuxiliaryAttribute("Aux2"));
			Assert::IsFalse(OBJ1.IsAuxiliaryAttribute("Blah!"));

			TypeManager::Clear();
		}

		TEST_METHOD(accessors)
		{
			RegisterType<AttributedFoo>();

			AttributedFoo OBJ1;
			OBJ1.AppendAuxiliaryAttribute("Aux1");
			OBJ1.AppendAuxiliaryAttribute("Aux2");

			auto& dataTable = OBJ1.Attributes();
			Assert::AreEqual(dataTable[0]->first, std::string("This"));
			Assert::AreEqual(dataTable[12]->first, std::string("Aux2"));

			auto [auxStart, auxEnd] = OBJ1.AuxiliaryAttributes();
			auto [prescStart, prescEnd] = OBJ1.PrescribedAttributes();
			std::string start = (*prescStart)->first;
			start;
			Assert::AreEqual((*prescStart)->first, std::string("ExternalInteger"));
			Assert::AreEqual((*prescEnd)->first, std::string("Aux1"));

			Assert::AreEqual((*auxStart)->first, std::string("Aux1"));
			Assert::IsTrue(auxEnd == dataTable.end());

			TypeManager::Clear();
		}



	};
}



			//Attributed attOBJ{AttributedFoo::TypeIdClass(),&dummyOBJ};

			//Scope tScope;
			//Datum& tDatum = tScope.Append(retrievedSig[0].Key);
			//tDatum.SetType(Datum::DatumTypes::Integer);
			//tDatum.pushBack(5);
			//int ExtInt = 5;
			//tDatum.setStorage(&ExtInt, 1);

			////testing non-scope datum
			//Datum freeD;
			//freeD.setStorage(&ExtInt, 1);
			//freeD.clear();
			//freeD.clear();
			//tDatum.setStorage(retrievedSig[0].Type, retrievedSig[0].Size, &dummyOBJ, retrievedSig[0].Offset);
			//int i = tDatum.Get<int>();
			//Assert::AreEqual(i, 5);


			//Assert::IsTrue(attOBJ._signatureSet == compareSig);
			//Assert::IsTrue(attOBJ._activeID == AttributedFoo::TypeIdClass());
			//Scope tScope;