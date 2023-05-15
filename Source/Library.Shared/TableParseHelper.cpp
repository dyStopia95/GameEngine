#include "pch.h"
#include "TableParseHelper.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(TableParseHelper);
	RTTI_DEFINITIONS(TableParseHelper::Wrapper_Table);

	bool TableParseHelper::StartHandler(std::shared_ptr<Wrapper_Base> wrapper, const std::string& key, const Json::Value& jsonOBJ, bool isArray, std::size_t)
	{
		if ((key != std::string{ "Type" }) && (key != std::string{"Value"}) && (key != std::string{"Class"}))
		{
			if(!(_contextRef->empty()))
			{
				Scope* currentScope = _contextRef->back()._currentScope;
				_contextRef->push_back(StackFrame{ currentScope,key });
			}
			
			else 
			{
				assert(wrapper->Is(Wrapper_Table::TypeIdClass()));
				Wrapper_Table* wrapperPtr = static_cast<Wrapper_Table*>(wrapper.get());
				_contextRef->push_back(StackFrame{ wrapperPtr->_scopeOBJ.get(), std::string{"root"} });
				_contextRef->push_back(StackFrame{ wrapperPtr->_scopeOBJ.get(), key });
			}
			
			return true;
		}

		StackFrame& currentFrame = _contextRef->back();

		if (key == std::string{ "Type" })
		{
			++currentFrame._nTableHandles;
			
			{
				std::string typeValue = jsonOBJ.asString();
				currentFrame._currentType = typeValue;
			}

			return true;
		}

		if (key == std::string{ "Class" })
		{
			++currentFrame._nTableHandles;

			currentFrame._className = jsonOBJ.asString();

			return true;
		}

		if ((key == std::string{ "Value" }) && (jsonOBJ.isObject()))
		{
			assert((currentFrame._currentType == std::string{"Table"}));

			if(currentFrame._className == std::string{"NotSet"})
			{
				Scope* newNestedScope = new Scope{};

				currentFrame._currentScope->Adopt(*newNestedScope, currentFrame._currentKey);
				currentFrame._currentScope = newNestedScope;

				++currentFrame._nTableHandles;
				return true;
			}

			else
			{
				Factory<Scope>* classFactory = Factory<Scope>::Find(currentFrame._className);

				if (classFactory != nullptr)
				{
					Scope* newNestedScope = classFactory->Create();
					
					currentFrame._currentScope->Adopt(*newNestedScope, currentFrame._currentKey);
					currentFrame._currentScope = newNestedScope;
					
					++currentFrame._nTableHandles;
					return true;
				}
				else
					throw std::runtime_error("Associated Factory not found!");

			}
		}

		if ((key == std::string{ "Value" }) && (isArray) && (currentFrame._currentType == std::string{"Table"}))
		{
			if (currentFrame._className == std::string{ "NotSet" })
			{
				Scope* newScopeArray = new Scope{};
				currentFrame._currentScope->Adopt(*newScopeArray, currentFrame._currentKey);
				_contextRef->push_back(StackFrame{ newScopeArray, currentFrame._currentKey });
			}

			return true;
		}

		return false;
	}

	void TableParseHelper::EndHandler()
	{
		StackFrame& currentFrame = _contextRef->back();
		if (currentFrame._nTableHandles == 0)
			_contextRef->pop_back();
		else
			--currentFrame._nTableHandles;
	}
}