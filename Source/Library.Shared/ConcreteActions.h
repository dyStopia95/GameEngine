#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionIncrement : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

	public:
		std::string _targetAttributeName{ "Not Set" };
		int _step = 1;
		std::size_t _targetIndex = 0;
		Datum* _targetDatum{ nullptr };

		ActionIncrement() :
			Action(ActionIncrement::TypeIdClass())
		{
		};

		virtual void SetAssociations() override;
		/// <summary>
		/// : Increments the target Attribute at the target Index by the provided step value
		/// </summary>
		/// <param name="currentTime"></param>
		virtual void Update(GameTime currentTime) override;
		static Vector<Signature> Signatures();
	};

	class ActionListIf : public ActionList
	{
		RTTI_DECLARATIONS(ActionListIf, ActionList);

	public:
		int _condition = 0;

		ActionListIf() :
			ActionList(ActionListIf::TypeIdClass())
		{
		};

		/// <summary>
		/// : checks if the Condition is true or false and then executes the Actions associated
		/// to the Then or Else attributes respectively
		/// </summary>
		/// <param name="currentTime"></param>
		virtual void Update(GameTime currentTime) override;
		static Vector<Signature> Signatures();
	};

	CONCRETE_FACTORY(ActionIncrement, Scope);
	CONCRETE_FACTORY(ActionListIf, Scope);

}