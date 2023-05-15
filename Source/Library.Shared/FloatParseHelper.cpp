#include "pch.h"
#include "FloatParseHelper.h"

namespace FieaGameEngine 
{
	RTTI_DEFINITIONS(FloatParseHelper);

	bool FloatParseHelper::StartHandler(std::shared_ptr<Wrapper_Base> , const std::string& key, const Json::Value& jsonOBJ, bool , std::size_t index )
	{
		assert(key == std::string{ "Value" });
		key;
		if (jsonOBJ.isDouble())
		{
			StackFrame& currentFrame = _contextRef->back();
			assert(currentFrame._currentType == std::string{ "Float" });
			Scope& activeScope = *(currentFrame._currentScope);
			Datum& datum = activeScope.Append(currentFrame._currentKey);

			if (datum.IsExternal())
			{
				double value = jsonOBJ.asDouble();
				float valAsFloat = static_cast<float>(value);
				datum.Set(valAsFloat, index);
			}

			else
			{
				datum.SetType(Datum::DatumTypes::Float);
				double value = jsonOBJ.asDouble();
				float valAsFloat = static_cast<float>(value);
				datum.pushBack(valAsFloat);
			}

	
			return true;
		}
	
		return false;
	}
	
	void FloatParseHelper::EndHandler()
	{
	}

}

