#pragma once
#include "EventSystem.h"

namespace FieaGameEngine
{

	template <typename T>
	RTTI_DEFINITIONS(Event<T>);

	template <typename T>
	inline Event<T>::Event(T messageRef) :
		EventPublisher(_subscriberList), _message{std::move(messageRef)}
	{

	}



}