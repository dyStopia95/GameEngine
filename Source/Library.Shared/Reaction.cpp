#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventMessageAttributed);
	RTTI_DEFINITIONS(Reaction);
	RTTI_DEFINITIONS(ReactionAttributed);
	RTTI_DEFINITIONS(ActionEvent);
	RTTI_DEFINITIONS(GameState);

	GameState::GameState(GameObject& rootObject) :
		_rootGameObject { rootObject }
	{
		_rootGameObject._name = std::string{ "RootObject" };
		Datum& gState = _rootGameObject.AppendAuxiliaryAttribute("GameStateRef");
		gState.SetType(Datum::DatumTypes::Pointer);
		gState.pushBack(this);
	}

	void GameState::SetGameTime(const GameTime& gameTime)
	{
		_gameTime = gameTime;
	}

	ActionEvent::ActionEvent() :
		Action(ActionEvent::TypeIdClass())
	{
	}

	void ActionEvent::Update(GameTime currentTime)
	{
		EventMessageAttributed payload;
		payload.SetSubType(_subType);
		auto [front,end] = AuxiliaryAttributes();

		for (; front != end; ++front)
		{
			auto& auxDatum = payload.AppendAuxiliaryAttribute((*front)->first);
			auxDatum = (*front)->second;
		}

		GameState::_gameEventQueue.Enqueue(
			std::make_shared<Event<EventMessageAttributed>>(payload),
			currentTime,
			std::chrono::seconds(_delay)
		);
	}

	Vector<Signature> ActionEvent::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "SubType", Datum::DatumTypes::String, 1, offsetof(ActionEvent, _subType) },
			{ "Delay", Datum::DatumTypes::Integer, 1, offsetof(ActionEvent, _delay) }
		};
	}

	void ActionEvent::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	void ActionEvent::SetDelay(int delay)
	{
		_delay = delay;
	}

	EventMessageAttributed::EventMessageAttributed() :
		Attributed(EventMessageAttributed::TypeIdClass())
	{
	}

	const std::string& EventMessageAttributed::GetSubType() const
	{
		return _subType;
	}

	void EventMessageAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	Vector<Signature> EventMessageAttributed::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "SubType", Datum::DatumTypes::String, 1, offsetof(EventMessageAttributed, _subType) },
		};
	}

	ReactionAttributed::ReactionAttributed()
	{
		Event<EventMessageAttributed>::Subscribe(this);
	}

	void ReactionAttributed::Notify(EventPublisher* publisher)
	{
		assert(publisher->Is(Event<EventMessageAttributed>::TypeIdClass()));

		const EventMessageAttributed& message = static_cast<Event<EventMessageAttributed>*>(publisher)->_message;

		if (message.GetSubType() == _subType)
		{
			Datum* actionDatum = Find(std::string{"Actions"});
			Datum* gStateDatum = Search(std::string{ "GameStateRef" });
			GameState* gameState = static_cast<GameState*>(gStateDatum->Get<RTTI*>());
			for (size_t i = 0; i < actionDatum->Size(); ++i)
			{
				Action* currentAction = static_cast<Action*>(actionDatum->Get<Scope*>(i));
				auto [front, end] = message.AuxiliaryAttributes();
				for (; front != end; ++front)
				{
					std::string key = (*front)->first;
					Datum value = (*front)->second;
					Datum* datum = currentAction->Find(key);
					*datum = value;
				}
				currentAction->Update(gameState->GetGameTime());
			}
		}
		
	}

	void ReactionAttributed::SetSubType(const std::string& subType)
	{
		_subType = subType;
	}

	Vector<Signature> ReactionAttributed::Signatures()
	{
		return FieaGameEngine::Vector<FieaGameEngine::Signature>
		{
			{ "SubType", Datum::DatumTypes::String, 1, offsetof(ReactionAttributed, _subType) },
		};
	}

}