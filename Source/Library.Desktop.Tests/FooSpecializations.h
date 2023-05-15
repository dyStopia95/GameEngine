#pragma once
#include "CppUnitTest.h"
#include "Foo.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	using namespace std::string_literals;

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo& t)
	{
		using namespace std::string_literals;
		RETURN_WIDE_STRING(t.Data());
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(const UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<UnitTests::Foo>(UnitTests::Foo* t)
	{
		RETURN_WIDE_STRING(t);
	}

}