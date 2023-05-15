#pragma once
#include "ParseCoordinator.h"
#include "ParseHelper.h"

namespace FieaGameEngine
{
	class DummyHelper : public ParseHelper
	{
	public:
		class Wrapper_Dummy : public Wrapper_Base
		{
			RTTI_DECLARATIONS(Wrapper_Dummy, FieaGameEngine::Wrapper_Base);
		};

		RTTI_DECLARATIONS(DummyHelper, FieaGameEngine::ParseHelper);

	public:
		bool StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t index) override;
		void EndHandler() override;
	};
}