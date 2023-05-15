#include "pch.h"
#include "GameObject.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(GameObject);

	GameObject::GameObject() :
		Attributed(GameObject::TypeIdClass())
	{
	}

	GameObject::GameObject(size_t childID) :
		Attributed(childID)
	{
	}

	GameObject::GameObject(const GameObject& value) :
		Attributed(value),
		_name{ value._name }, _transform{ value._transform }
	{
	}

	GameObject* GameObject::Clone() const
	{
		return (new GameObject{ *this });
	}

	void GameObject::Update(GameTime currentTime)
	{
		Datum* children = Find("Children");
		assert(children != nullptr);
		for (size_t i = 0; i < children->Size(); ++i)
		{
			Scope* child = children->Get<Scope*>(i);
			assert(child->Is(GameObject::TypeIdClass()));
			GameObject* childObject = static_cast<GameObject*>(child);
			childObject->Update(currentTime);
		}

		Datum* actions = Find("Actions");
		assert(actions != nullptr);
		for (size_t i = 0; i < actions->Size(); ++i)
		{
			Scope* action = actions->Get<Scope*>(i);
			assert(action->Is(Action::TypeIdClass()));
			static_cast<Action*>(action)->Update(currentTime);
		}
	}

	void GameObject::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Factory<Scope>* classFactory = Factory<Scope>::Find(className);

		if (classFactory != nullptr)
		{
			Scope* newNestedScope = classFactory->Create();
			static_cast<Action*>(newNestedScope)->SetName(instanceName);
			Adopt(*newNestedScope, std::string{"Actions"});
		}
		else
			throw std::runtime_error("Associated Factory not found!");
	}

	Datum* GameObject::Actions() 
	{
		return Find("Actions");
	}

	Vector<Signature> GameObject::Signatures()
	{
		return Vector<Signature>
		{
			{ "Name", Datum::DatumTypes::String, 1, offsetof(GameObject, _name) },
			{ "Transform", Datum::DatumTypes::String, 1, offsetof(GameObject, _transform) },
			{ "Children", Datum::DatumTypes::Table, 0, 0 },
			{ "Actions", Datum::DatumTypes::Table, 0, 0 }
		};
	}


}