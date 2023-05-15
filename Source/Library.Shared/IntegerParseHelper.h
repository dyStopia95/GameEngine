#pragma once

#include "ParseHelper.h"
#include "ParseCoordinator.h"
#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <string>
#include <functional>

namespace FieaGameEngine
{
	class IntegerParseHelper : public ParseHelper
	{
		RTTI_DECLARATIONS(IntegerParseHelper, FieaGameEngine::ParseHelper);
	public:
		/// <summary>
		/// Fn to be called by the Coordinator. takes in ref to wrapper, key of the associated Json::Value, and the Value.
		/// If wrapper type, value type, and the value itself match, returns true after setting its state to store these variables.
		/// </summary>
		bool StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t index) override;
		/// <summary>
		/// Deserializes the Json::Value into the given wrapper.
		/// </summary>
		void EndHandler() override;
	};
}