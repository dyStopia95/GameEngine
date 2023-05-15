#pragma once
#include "ParseHelper.h"
#include "ParseCoordinator.h"

namespace FieaGameEngine
{
	class FloatParseHelper : public ParseHelper
	{
		RTTI_DECLARATIONS(FloatParseHelper, FieaGameEngine::ParseHelper);

	public:
		bool StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t index) override;
		void EndHandler() override;
	};
}