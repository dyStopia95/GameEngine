#pragma once
#include "CppUnitTest.h"
#include "Foo.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Foo>::iterator>(const FieaGameEngine::SList<UnitTests::Foo>::iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}

		return value;
	}

	template<>
	inline std::wstring ToString<FieaGameEngine::SList<UnitTests::Foo>::const_iterator>(const FieaGameEngine::SList<UnitTests::Foo>::const_iterator& t)
	{
		std::wstring value;
		try
		{
			value = ToString(*t);
		}
		catch (const std::exception&)
		{
			value = L"end()"s;
		}

		return value;
	}
}