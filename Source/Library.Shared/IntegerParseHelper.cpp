#include "pch.h"
#include "IntegerParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(IntegerParseHelper);

	bool IntegerParseHelper::StartHandler(std::shared_ptr<Wrapper_Base>, const std::string& key, const Json::Value& jsonOBJ, bool, std::size_t index)
	{
		assert(key == std::string{ "Value" });
		key;
		if (jsonOBJ.isInt())
		{
			StackFrame& currentFrame = _contextRef->back();
			assert(currentFrame._currentType == std::string{ "Integer" });
			Scope& activeScope = *(currentFrame._currentScope);
			Datum& datum = activeScope.Append(currentFrame._currentKey);
			
			if (datum.IsExternal())
			{
				datum.Set(jsonOBJ.asInt(), index);
			}

			else
			{
				datum.SetType(Datum::DatumTypes::Integer);
				datum.pushBack(jsonOBJ.asInt());
			}

			return true;
		}

		return false;
	}

	void IntegerParseHelper::EndHandler()
	{
	}
}