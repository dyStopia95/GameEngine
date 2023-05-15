#include "pch.h"
#include "AttributedFoo.h"

namespace UnitTests
{
	using namespace FieaGameEngine;
	//using FieaGameEngine::Datum;

	RTTI_DEFINITIONS(AttributedFoo);

	AttributedFoo::AttributedFoo() :
		Attributed(AttributedFoo::TypeIdClass())
	{
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs) :
		Attributed(rhs),
		ExternalInteger{ rhs.ExternalInteger },
		ExternalFloat{ rhs.ExternalFloat },
		ExternalString{ rhs.ExternalString },
		ExternalVector{ rhs.ExternalVector },
		ExternalMatrix{ rhs.ExternalMatrix }
	{
		for (size_t i = 0; i < ArraySize; ++i)
		{
			ExternalIntegerArray[i] = rhs.ExternalIntegerArray[i];
			ExternalFloatArray[i]  = rhs.ExternalFloatArray[i];
			ExternalStringArray[i] = rhs.ExternalStringArray[i];
			ExternalVectorArray[i] = rhs.ExternalVectorArray[i];
			ExternalMatrixArray[i] = rhs.ExternalMatrixArray[i];
		}
	}

	AttributedFoo* AttributedFoo::Clone() const
	{
		AttributedFoo* newAttFoo = new AttributedFoo { *this };
		return newAttFoo;
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> AttributedFoo::Signatures() //the vector that goes into Attributed(Signatures()) is wrong if I use my Vector implementation
	{
		return Vector<Signature>
		{
			{ "ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(AttributedFoo, ExternalInteger) },
			{ "ExternalFloat", Datum::DatumTypes::Float, 1, offsetof(AttributedFoo, ExternalFloat) },
			{ "ExternalString", Datum::DatumTypes::String, 1, offsetof(AttributedFoo, ExternalString) },
			{ "ExternalVector", Datum::DatumTypes::Vector, 1, offsetof(AttributedFoo, ExternalVector) },
			{ "ExternalMatrix", Datum::DatumTypes::Matrix, 1, offsetof(AttributedFoo, ExternalMatrix) },
			{ "ExternalIntegerArray", Datum::DatumTypes::Integer, ArraySize, offsetof(AttributedFoo, ExternalIntegerArray) },
			{ "ExternalFloatArray", Datum::DatumTypes::Float, ArraySize, offsetof(AttributedFoo, ExternalFloatArray) },
			{ "ExternalStringArray", Datum::DatumTypes::String, ArraySize, offsetof(AttributedFoo, ExternalStringArray) },
			{ "ExternalVectorArray", Datum::DatumTypes::Vector, ArraySize, offsetof(AttributedFoo, ExternalVectorArray) },
			{ "ExternalMatrixArray", Datum::DatumTypes::Matrix, ArraySize, offsetof(AttributedFoo, ExternalMatrixArray) }
		};
	}

	RTTI_DEFINITIONS(AttFoo2);

	AttFoo2::AttFoo2() :
		Attributed(AttFoo2::TypeIdClass())
	{
		
	}

	AttFoo2::AttFoo2(const AttFoo2& rhs) :
		Attributed(rhs),
		ExternalInteger{ rhs.ExternalInteger }
	{
		for (size_t i = 0; i < ArraySize; ++i)
		{
			ExternalIntegerArray[i] = rhs.ExternalIntegerArray[i];
		}
	}

	AttFoo2* AttFoo2::Clone() const
	{
		AttFoo2* newAttFoo = new AttFoo2{ *this };
		return newAttFoo;
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> UnitTests::AttFoo2::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "ExternalInteger", Datum::DatumTypes::Integer, 1, offsetof(AttFoo2, ExternalInteger) },
			{ "ExternalIntegerArray", Datum::DatumTypes::Integer, ArraySize, offsetof(AttFoo2, ExternalIntegerArray) },
			{ "NestedScope", Datum::DatumTypes::Table, 0, 0 }
		};
	}
}
