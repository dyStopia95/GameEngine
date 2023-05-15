#pragma once
#include "Attributed.h"
#include "Factory.h"
#include "Action.h"
#include "GameTime.h"

namespace FieaGameEngine
{
	class GameObject : public Attributed
	{
		RTTI_DECLARATIONS(GameObject, Attributed);

	protected:
		/// <summary>
		/// : takes the ID of derived child class to be passed on to Attributed's constructor.
		/// marked protected so that user cannot instantiate a GameObject with a size_t as argument.
		/// </summary>
		/// <param name="childID"></param>
		GameObject(size_t childID);

	public:
		/// <summary>
		/// : Default constructor that passes on the typeID for GameObject to Attributed's constructor
		/// </summary>
		GameObject();
		GameObject(const GameObject& value);
		GameObject(GameObject&&) = default;
		GameObject& operator= (const GameObject&) = default;
		GameObject& operator= (GameObject&&) = default;
		virtual ~GameObject() = default;

		
		[[nodiscard]] 
		/// <summary>
		/// : returns a pointer to a heap allocated GameObject using the clonable pattern.
		/// !!! TRANSFERS OWNERSHIP !!!
		/// </summary>
		/// <returns></returns>
		GameObject* Clone() const override;

		std::string _name{"NoName"};
		std::string _transform{ "Placeholder" };

		/// <summary>
		/// : Executes any functionality provided and calls update on all GameObjects in the Children attribute.
		/// </summary>
		/// <param name="currentTime"></param>
		void virtual Update(GameTime currentTime);

		/// <summary>
		/// : uses Factory to create the Action associated to the provided class name and assigns instancecName to
		/// its _name attribute. Adopts it into the Actions attribute.
		/// </summary>
		/// <param name="className">: Class name of the required Action</param>
		/// <param name="instanceName">: value to set for _name attribute of the Action</param>
		void CreateAction(const std::string& className, const std::string& instanceName);
		/// <summary>
		/// : returns Datum associated with the Actions attribute.
		/// </summary>
		/// <returns></returns>
		Datum* Actions();

		static FieaGameEngine::Vector<FieaGameEngine::Signature> Signatures();
	};

	CONCRETE_FACTORY(GameObject, Scope);

}