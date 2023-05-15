#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Action);
	RTTI_DEFINITIONS(ActionList);


	std::string Action::Name() const
	{
		return _name;
	}

	void Action::SetName(const std::string& name)
	{
		_name = name;
	}

	Vector<Signature> Action::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(Action, _name) },
		};
	}

	void ActionList::SetAssociations() //probably redundant
	{
		Datum* actionsDatum = Find("Actions");
		for (size_t i = 0; i < actionsDatum->Size(); ++i)
		{
			Action* currentAction = static_cast<Action*>(actionsDatum->Get<Scope*>(i));
			currentAction->SetAssociations();
		}
	}

	void ActionList::Update(GameTime currentTime)
	{
		Datum* actionsDatum = Find("Actions");
		for (size_t i = 0; i < actionsDatum->Size(); ++i)
		{
			Action* currentAction = static_cast<Action*>(actionsDatum->Get<Scope*>(i));
			currentAction->Update(currentTime);
		}
	}

	Vector<Signature> ActionList::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "Actions", Datum::DatumTypes::Table, 0, 0 }
		};
	}
}