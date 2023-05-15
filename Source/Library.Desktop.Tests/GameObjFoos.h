#pragma once
#include "GameObject.h"
#include "TypeManager.h"
#include "Factory.h"

namespace UnitTests
{

	class GameOBJTest final : public FieaGameEngine::GameObject
	{
		RTTI_DECLARATIONS(GameOBJTest, FieaGameEngine::GameObject);

	public:
		int _incrementINT = 30;

		GameOBJTest();

		[[nodiscard]] GameOBJTest* Clone() const override;
		void Update(FieaGameEngine::GameTime currentTime) override;

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();

	};

	CONCRETE_FACTORY(GameOBJTest, Scope);

}
