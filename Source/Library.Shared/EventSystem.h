#pragma once
#include "RTTI.h"
#include "vector.h"
#include "GameTime.h"
#include "GameClock.h"
#include <memory>

namespace FieaGameEngine
{
	class EventSubscriber;

	/// <summary>
	/// : Abstract base class for Event T. Stores reference to the subscriber list of a unique Event.
	/// EventPublisher pointers are stored in the EventQueue and Deliver function calls notify on subscribers present in the
	/// subscriberList reference. Having Deliver method in the base class prevents code bloat if it was implemented as a polymorphic
	/// function to be called from Event T.
	/// </summary>
	class EventPublisher : public RTTI
	{
		RTTI_DECLARATIONS(EventPublisher, RTTI);

		Vector<EventSubscriber*>& _subscriberList;
		std::chrono::time_point<std::chrono::steady_clock> _expirationTime;

	protected:
		/// <summary>
		/// : protected constructor to make EventPublisher abstract. Derived Event<T> must pass in reference to its static
		/// subscriberList vector.
		/// </summary>
		/// <param name="subList">: reference to the static subscriberList vector.</param>
		EventPublisher(Vector<EventSubscriber*>& subList);

	public:
		/// <summary>
		/// : function called when this Publisher is enqueued by the EventQueue. the Queue will take currentTime and add any delay
		/// specified during Enqueue call.
		/// </summary>
		/// <param name="expirationTime">: Time at which this Publisher expires.</param>
		void SetExpirationTime(std::chrono::time_point<std::chrono::steady_clock> expirationTime);
		/// <summary>
		/// : return currently set Expiration time.
		/// </summary>
		/// <returns></returns>
		std::chrono::time_point<std::chrono::steady_clock> GetExpirationTime();
		/// <summary>
		/// : calls Notify against all the subscribers in the subscriberList.
		/// </summary>
		void Deliver();
	};

	class EventSubscriber
	{
	public:
		/// <summary>
		/// : pure Virtual function to be implemented by derived subscriber class. Intended for multiple inheritance.
		/// override with any desired behavior to be triggered by the Event. Subscriber must also do type checking for the 
		/// type of message it has subscribed to.
		/// </summary>
		/// <param name="publisherRef"></param>
		void virtual Notify(EventPublisher* publisherRef) = 0;
	};

	template < typename T >
	class Event : public EventPublisher
	{
		RTTI_DECLARATIONS(Event, EventPublisher);

		inline static Vector<EventSubscriber*> _subscriberList;

	public:
		const T _message;
		/// <summary>
		/// : Constructor that takes a copy of a message and moves the copy. _message is marked const so that subscribers may not
		/// mutate the message during traversal of subscriberList.
		/// </summary>
		/// <param name="messageRef"></param>
		Event(T messageRef);
		
		/// <summary>
		/// : Takes in pointer to a Subscriber and adds it to the subscriberList.
		/// </summary>
		/// <param name="subscriberRef">: raw ptr to subscriber.</param>
		inline static void Subscribe(EventSubscriber* subscriberRef)
		{
			_subscriberList.push_back(subscriberRef);
		}

		/// <summary>
		/// : clears the subscriberList.
		/// </summary>
		inline static void UnsubscribeAll()
		{
			_subscriberList.clear();
			_subscriberList.shrink_to_fit();
		}

		/// <summary>
		/// : returns the message.
		/// </summary>
		/// <returns></returns>
		T Message()
		{
			return _message;
		}
	};

	class EventQueue final
	{
		Vector<std::shared_ptr< EventPublisher>> _publishedQueue;
		Vector<std::shared_ptr< EventPublisher>> _dequeueList;
		Vector<std::shared_ptr< EventPublisher>> _enqueueList;

	public:
		/// <summary>
		/// : adds a given publisher to the event queue. takes a GameTime, and delay. Adds the current time to delay, and
		/// sets it as expiration time in the Publisher.
		/// </summary>
		/// <param name="publishedEvent">: ptr to Publisher</param>
		/// <param name="T">: GameTime object. extracts current time from this.</param>
		/// <param name="delay">: delay to be added to current time to get expiration time of this publisher.</param>
		void Enqueue(std::shared_ptr<EventPublisher> publishedEvent, GameTime T, std::chrono::seconds delay = std::chrono::seconds{0});
		/// <summary>
		/// : Clears the current Queue
		/// </summary>
		void Clear();
		/// <summary>
		/// : called every tick. Takes GameTime to check current time against expirationTime of the Publishers in the publishedQueue.
		/// before publishing, iterates through enqueueList, and adds publishers to publishedQueue.
		/// during publishing, adds expired publishers to dequeueList.
		/// after publishing, removes publishers in dequeueList from publishedQueue.
		/// </summary>
		/// <param name="T"></param>
		void Update(GameTime T);
	};
}

#include "EventSystem.inl"