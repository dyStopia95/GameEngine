#include "pch.h"
#include "ParseCoordinator.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ParseCoordinator);
	RTTI_DEFINITIONS(Wrapper_Base);

	void Wrapper_Base::SetParseCoordinator(std::shared_ptr<ParseCoordinator> coordinator)
	{
		_coordinatorPtr.reset();
		_coordinatorPtr = coordinator;
	}

	std::weak_ptr<ParseCoordinator> Wrapper_Base::GetParseCoordinator()
	{
		return _coordinatorPtr;
	}

	void Wrapper_Base::IncrementDepth()
	{
		++_depth;
	}

	void Wrapper_Base::DecrementDepth()
	{
		--_depth;
	}

	std::size_t Wrapper_Base::CurrentDepth()
	{
		return _depth;
	}

	ParseCoordinator::ParseCoordinator(std::shared_ptr<Wrapper_Base> wrapperOBJ) :
		_wrapperPtr{ wrapperOBJ }
	{
		_contextStack = std::make_shared<Vector<StackFrame>>();
	}

	void ParseCoordinator::AddHelper(std::shared_ptr<ParseHelper> helper)
	{
		_helperList.push_back(helper);
		_helperList.back()->SetStackFrame(_contextStack);
	}

	void ParseCoordinator::RemoveHelper(std::shared_ptr<ParseHelper> helper)
	{
		_helperList.erase(helper);
	}

	void ParseCoordinator::DeserializeObjectFromFile(const std::string& filename)
	{
		std::ifstream file{filename};
		if (!file.good())
			throw std::runtime_error("Could not open file!");

		DeserializeObject(file);

	}

	void ParseCoordinator::DeserializeObject(const std::string& data)
	{
		std::istringstream inputStream(data);
		DeserializeObject(inputStream);
	}

	void ParseCoordinator::DeserializeObject(std::istream& inputStream)
	{

		Json::Value root;
		inputStream >> root;

		_wrapperPtr->IncrementDepth();
		ParseValue(root);
		_wrapperPtr->DecrementDepth();

		assert(_wrapperPtr->CurrentDepth() == 0);
		Cleanup();
	}

	void ParseCoordinator::ParseValue(const Json::Value& jsonObj, bool isArray, std::size_t index)
	{
		for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it)
		{
			std::string key = it.name();
			ParseKeyValuePair(key, (*it), isArray, index);
		}
	}

	void ParseCoordinator::ParseKeyValuePair(const std::string& key, const Json::Value& value, bool isArray, std::size_t index)
	{
		key; value; isArray; index;
		if (value.isObject())
		{
			_wrapperPtr->IncrementDepth();
			for (auto& helper : _helperList)
			{
				if (helper->StartHandler(_wrapperPtr, key, value, isArray, index))
				{
					ParseValue(value);
					helper->EndHandler();
					break;
				}
			}
			_wrapperPtr->DecrementDepth();
		}

		else if (value.isArray())
		{
			size_t i = 0;
			for (const auto& element : value)
			{

				if (element.isObject())
				{
					_wrapperPtr->IncrementDepth();
					for (auto& helper : _helperList)
					{
						if (helper->StartHandler(_wrapperPtr, key, value, true, i))
						{
							ParseValue(element, i);
							helper->EndHandler();
							break;
						}
					}
					_wrapperPtr->DecrementDepth();
				}
				else
				{
					ParseKeyValuePair(key, element, true, i);
				}

				++i;
			}
		}

		else
		{
			bool successful = false;

			for (auto& helper : _helperList)
			{
				if (helper->StartHandler(_wrapperPtr, key, value, isArray, index))
				{
					helper->EndHandler();
					successful = true;
					break;
				}
			}

			if (successful == false)
				throw std::runtime_error("Unable to parse file. Please add relevant helper.");
		}
	}

	void ParseCoordinator::Cleanup()
	{
		for (auto& helper : _helperList)
		{
			helper->Cleanup();
		}

		_helperList.clear();
		_wrapperPtr.reset();
		_contextStack.reset();
	}

}