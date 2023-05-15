#include "pch.h"
#include "GameObjFoos.h"

namespace UnitTests
{
	using namespace FieaGameEngine;

	RTTI_DEFINITIONS(GameOBJTest);

	GameOBJTest::GameOBJTest() :
		GameObject(GameOBJTest::TypeIdClass())
	{
	}

	GameOBJTest* GameOBJTest::Clone() const
	{
		return (new GameOBJTest{*this});
	}

	void GameOBJTest::Update(FieaGameEngine::GameTime currentTime)
	{
		++_incrementINT;
		GameObject::Update(currentTime);
	}

	FieaGameEngine::Vector<FieaGameEngine::Signature> GameOBJTest::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{"IncrementINT", Datum::DatumTypes::Integer, 1, offsetof(GameOBJTest, _incrementINT)}
		};
	}

}