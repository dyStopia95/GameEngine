#include "pch.h"
#include "ConcreteActions.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(ActionIncrement);
	RTTI_DEFINITIONS(ActionListIf);

	void ActionIncrement::SetAssociations()
	{
		_targetDatum = Search(_targetAttributeName);
	}

	void ActionIncrement::Update(GameTime )
	{
		_targetDatum = Search(_targetAttributeName);
		if (_targetDatum != nullptr)
		{
			_targetDatum->Set((_targetDatum->Get<int>(_targetIndex) + _step), _targetIndex);
		}
	}

	Vector<Signature> ActionIncrement::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "TargetName", Datum::DatumTypes::String, 1, offsetof(ActionIncrement, _targetAttributeName) },
			{ "Step", Datum::DatumTypes::Integer, 1, offsetof(ActionIncrement, _step) },
			{ "TargetIndex", Datum::DatumTypes::Integer, 1, offsetof(ActionIncrement, _targetIndex) },
		};
	}

	void ActionListIf::Update(GameTime currentTime)
	{
		if (_condition != 0)
		{
			Action* trueBlock = static_cast<Action*>(Find("Then")->Get<Scope*>(0));
			trueBlock->Update(currentTime);
		}
		else
		{
			Action* falseBlock = static_cast<Action*>(Find("Else")->Get<Scope*>(0));
			falseBlock->Update(currentTime);
		}
	}

	Vector<Signature> ActionListIf::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "Then", Datum::DatumTypes::Table, 0, 0},
			{ "Else", Datum::DatumTypes::Table, 0, 0},
			{ "Condition", Datum::DatumTypes::Integer, 1, offsetof(ActionListIf, _condition) },
		};;
	}
}