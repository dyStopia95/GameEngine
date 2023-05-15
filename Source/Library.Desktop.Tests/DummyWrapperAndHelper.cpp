#include "pch.h"
#include "DummyWrapperAndHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(DummyHelper);
	RTTI_DEFINITIONS(DummyHelper::Wrapper_Dummy);

	bool DummyHelper::StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t index)
	{
		wrapper;
		key;
		jsonOBJ; isArray; index;
		return false;
	}

	void DummyHelper::EndHandler()
	{
	}
}