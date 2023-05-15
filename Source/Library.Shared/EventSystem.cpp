#include "pch.h"
#include "EventSystem.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(EventPublisher);

	void EventQueue::Enqueue(std::shared_ptr<EventPublisher> publishedEvent, GameTime T, std::chrono::seconds delay)
	{
		std::chrono::time_point expirationTime = T.CurrentTime() + delay;
		publishedEvent->SetExpirationTime(T.CurrentTime() + delay);
		_enqueueList.push_back(publishedEvent);
	}

	void EventQueue::Clear()
	{
		_enqueueList.clear();
		_enqueueList.shrink_to_fit();
		_dequeueList.clear();
		_dequeueList.shrink_to_fit();
		_publishedQueue.clear();
		_publishedQueue.shrink_to_fit();
	}

	void EventQueue::Update(GameTime T)
	{
		for (auto& enqueuedEvent : _enqueueList)
		{
			_publishedQueue.push_back(enqueuedEvent);
		}
		_enqueueList.clear();

		for (auto& publishedEvent : _publishedQueue)
		{
			if (T.CurrentTime() >= publishedEvent->GetExpirationTime())
			{
				assert(publishedEvent != nullptr);
				publishedEvent->Deliver();
				_dequeueList.push_back(publishedEvent);
			}
		}

		for (auto& deequeudEvent : _dequeueList)
		{
			_publishedQueue.erase(deequeudEvent);
		}
		_dequeueList.clear();
	}

	EventPublisher::EventPublisher(Vector<EventSubscriber*>& subList) :
		_subscriberList{ subList }
	{

	}

	void EventPublisher::Deliver()
	{
		for (auto& subscriber : _subscriberList)
		{
			assert(subscriber != nullptr);
			subscriber->Notify(this);
		}
	}

	void EventPublisher::SetExpirationTime(std::chrono::time_point<std::chrono::steady_clock> expirationTime)
	{
		_expirationTime = expirationTime; //called in Enqueue but not showing in code coverage
	}

	std::chrono::time_point<std::chrono::steady_clock> EventPublisher::GetExpirationTime()
	{
		return _expirationTime;
	}
}