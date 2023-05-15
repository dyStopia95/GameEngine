#pragma once
#include "CppUnitTest.h"
#include "Foo.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Foo>::iterator>(const FieaGameEngine::Vector<UnitTests::Foo>::iterator& t)
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
	inline std::wstring ToString<FieaGameEngine::Vector<UnitTests::Foo>::const_iterator>(const FieaGameEngine::Vector<UnitTests::Foo>::const_iterator& t)
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