#pragma once

#include "RTTI.h"
#include "vector.h"
#include <json/json.h>
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
	class Wrapper_Base;
	struct StackFrame;

	class ParseHelper : public RTTI
	{
		RTTI_DECLARATIONS(ParseHelper, FieaGameEngine::RTTI);

	protected:
		std::shared_ptr<Vector<StackFrame>> _contextRef = nullptr;

	public:
		void SetStackFrame(std::shared_ptr<Vector<StackFrame>> contextStack);
		virtual bool StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t index) = 0;
		virtual void EndHandler() = 0;

		void Cleanup();
	};
}