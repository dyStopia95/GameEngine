#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "GameClock.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Class: Action
	/// Attributed derived class that serves the purpose of modifying data members / Attributes of the parent GameObject
	/// </summary>
	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed);

	private:
		std::string _name{"NoName"};

	protected:
		/// <summary>
		/// : protected constructor that accepts ID of child class to be passed on to Attributed constructor
		/// </summary>
		/// <param name="childID">: RTTI classID of derived class</param>
		/// <param name="name">: value to set for _name attribute</param>
		Action(size_t childID, std::string name = std::string{"NoName"}) :
			Attributed(childID), _name{ name }
		{
		};

	public:
		virtual ~Action(){};

		/// <summary>
		/// : Get name of the Action
		/// </summary>
		/// <returns>: copy of _name attribute of Action</returns>
		std::string Name() const;
		/// <summary>
		/// : Set the name of the Action
		/// </summary>
		/// <param name="name">: name to set</param>
		void SetName(const std::string& name);
		/// <summary>
		/// : Pure virtual Update Method. to be implemented by derived classes
		/// </summary>
		/// <param name="currentTime">: accepts the current time to be passed on to relevant functions / children</param>
		virtual void Update(GameTime currentTime) = 0;
		virtual void SetAssociations() {};
		static Vector<Signature> Signatures();
	};

	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		ActionList() : 
			Action(ActionList::TypeIdClass())
		{
		};

		ActionList(size_t childID) :
			Action(childID)
		{
		};

		virtual ~ActionList() {};

		virtual void SetAssociations() override;
		/// <summary>
		/// : calls update on all child Actions.
		/// </summary>
		/// <param name="currentTime"></param>
		virtual void Update(GameTime currentTime) override;
		static Vector<Signature> Signatures();
	};

	CONCRETE_FACTORY(ActionList, Scope);

}