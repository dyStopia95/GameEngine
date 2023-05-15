#pragma once
#include "EventSystem.h"
#include "Action.h"
#include "Factory.h"
#include "GameObject.h"

namespace FieaGameEngine
{

	/// <summary>
	/// Game State / World class holding the root Game Object, event Queue and Game Times.
	/// On construction it Appends an AuxAttribute to the rootGameObject so that any gameObject in the
	/// heirarchy can acess game time.
	/// </summary>
	class GameState : public RTTI
	{
		RTTI_DECLARATIONS(GameState, RTTI);

	private:
		GameObject& _rootGameObject;
		GameTime _gameTime;

	public:
		/// <summary>
		/// constructor, adds itself to Auxilliary attribute in rootObject.
		/// </summary>
		/// <param name="rootObject">: reference to the root Game Object</param>
		GameState(GameObject& rootObject);
		/// <summary>
		/// Set the GameTime externally -> to be replaced by a GameClock that will supply a Time on query.
		/// </summary>
		/// <param name="gameTime"></param>
		void SetGameTime(const GameTime& gameTime);
		/// <summary>
		/// returns current GameTime
		/// </summary>
		/// <returns></returns>
		GameTime GetGameTime()
		{
			return _gameTime;
		};
		/// <summary>
		/// calls Update on the EventQueue, and on the RootGameObject.
		/// </summary>
		void Update()
		{
			_gameEventQueue.Update(_gameTime);
			_rootGameObject.Update(_gameTime);
		};

		inline static EventQueue _gameEventQueue;
	};

	class Reaction : public	ActionList, public EventSubscriber
	{
		RTTI_DECLARATIONS(Reaction, ActionList);

	protected:
		Reaction() = default;
	};

	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

		std::string _subType{ "NotSet" };

	public:
		EventMessageAttributed();
		/// <summary>
		/// returns the SubType.
		/// </summary>
		/// <returns></returns>
		const std::string& GetSubType() const;
		/// <summary>
		/// Sets the SubType.
		/// </summary>
		/// <param name="subType"></param>
		void SetSubType(const std::string& subType);

		static Vector<Signature> Signatures();
	};

	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

		std::string _subType{ "NotSet" };

	public:
		/// <summary>
		/// On construction subscribe to the generic event : Event of EventMessageAttributed.
		/// </summary>
		ReactionAttributed();
		/// <summary>
		/// Overrides the Subscriber Notify.
		/// First checks if its assigned subType matches that of the received message.
		/// If subType matches, copies over the auxilliaryAttributes provided in the message (data that the stored Actions require)
		/// into the respective attributes of child Actions.
		/// Finally calls update on these Actions.
		/// </summary>
		/// <param name="publisher"></param>
		void Notify(EventPublisher* publisher) override;
		void SetSubType(const std::string& subType);

		static Vector<Signature> Signatures();
	};

	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

		std::string _subType{ "NotSet" };
		int _delay = 0;

	public:
		ActionEvent();
		/// <summary>
		/// User must first AppendAuxilliary all the data that the Target Actions will require to this ActionEvent.
		/// When Update is called against this action, it assembles an EventMessageAttributed by setting the subType,
		/// and appending auxilliary all the user supplied auxilliary attributes.
		/// it then enques an event of type Event of EventMessageAttributed with the given delay.
		/// </summary>
		/// <param name="currentTime"></param>
		void Update(GameTime currentTime) override;
		void SetSubType(const std::string& subType);
		void SetDelay(int delay);

		static Vector<Signature> Signatures();
	};

	CONCRETE_FACTORY(ActionEvent, Scope);

}