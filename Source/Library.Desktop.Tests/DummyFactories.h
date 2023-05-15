#pragma once
#include "Factory.h"
#include "Scope.h"
#include <string>

using namespace FieaGameEngine;
namespace UnitTests
{
	class dummyClass : public FieaGameEngine::Scope
	{
	public:
		int _dummyInt = 9;
	};


	//class dummyClassFactory : public Factory<Scope>
	//{
	//	/*Scope* Create() override
	//	{
	//		return new dummyClass{};
	//	}*/

	//	std::string ClassName() override
	//	{
	//		return std::string{ "dummyClass" };
	//	}

	//	dummyClass* Create() override
	//	{
	//		return new dummyClass{};
	//	}
	//};

	CONCRETE_FACTORY(dummyClass, Scope);
}