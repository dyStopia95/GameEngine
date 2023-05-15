#include "pch.h"
#include "StringParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(StringParseHelper);

	bool StringParseHelper::StartHandler(std::shared_ptr<Wrapper_Base>, const std::string& key, const Json::Value& jsonOBJ, bool, std::size_t index)
	{
		assert(key == std::string{ "Value" });
		key;
		if (jsonOBJ.isString())
		{
			StackFrame& currentFrame = _contextRef->back();
			assert(currentFrame._currentType == std::string{ "String" });
			Scope& activeScope = *(currentFrame._currentScope);
			Datum& datum = activeScope.Append(currentFrame._currentKey);

			if (datum.IsExternal())
			{
				datum.Set(jsonOBJ.asString(), index);
			}

			else
			{
				datum.SetType(Datum::DatumTypes::String);
				datum.pushBack(jsonOBJ.asString());
			}

			return true;
		}

		return false;
	}

	void StringParseHelper::EndHandler()
	{
	}

}

