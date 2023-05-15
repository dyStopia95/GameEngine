#pragma once
#include "Attributed.h"

namespace UnitTests
{
	class Base1
	{
	public:
		int a = 10;
		int b = 5;
		int c = 0;

		Base1(void* thas) :
			c{ a + b }
		{
			void* tempP = this;
			tempP;
			thas;
		}
	};

	class Derived1 : public Base1
	{
	public:
		float x = 20;
		float y = 15;
		float z = 0;

		Derived1() :
			Base1{ this }, z { x + y }
		{
			void* tempP = this;
			tempP;
		}
	};

	class Bar1 : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar1, FieaGameEngine::RTTI);
	public:
		int x;
		int y;

		inline static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures()
		{
			return FieaGameEngine::Vector<FieaGameEngine::Signature>
			{
				{ "IntX", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(Bar1, x) },
				{ "IntY", FieaGameEngine::Datum::DatumTypes::Integer, 1, offsetof(Bar1, y) }
			};
		}
	};

	class Bar2 : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Bar2, FieaGameEngine::RTTI);
	public:
		float x;
		float y;

		inline static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures()
		{
			return FieaGameEngine::Vector<FieaGameEngine::Signature>
			{
				{ "FloatX", FieaGameEngine::Datum::DatumTypes::Float, 1, offsetof(Bar2, x) },
				{ "FloatY", FieaGameEngine::Datum::DatumTypes::Float, 1, offsetof(Bar2, y) }
			};
		}
	};

	RTTI_DEFINITIONS(Bar1);
	RTTI_DEFINITIONS(Bar2);

}