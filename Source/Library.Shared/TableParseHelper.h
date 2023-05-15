#pragma once

#include "Scope.h"
#include "ParseHelper.h"
#include "ParseCoordinator.h"
#include "Factory.h"
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
	class TableParseHelper : public ParseHelper
	{
	public:
		class Wrapper_Table : public Wrapper_Base
		{
			RTTI_DECLARATIONS(Wrapper_Table, FieaGameEngine::Wrapper_Base);

		public:
			std::shared_ptr<Scope> _scopeOBJ;
		};

		RTTI_DECLARATIONS(TableParseHelper, FieaGameEngine::ParseHelper);

	private:

	public:
		bool StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t index) override;
		void EndHandler() override;
	};
}