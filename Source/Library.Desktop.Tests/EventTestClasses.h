#pragma once
#include "EventSystem.h"
#include "Reaction.h"

namespace UnitTests
{
	class GenericMessage1
	{

	};

	class GenericMessage2
	{

	};

	class GenericMessage3
	{

	};

	class GenericMessage4
	{

	};
	
	using namespace FieaGameEngine;
	class EventTestClass1 : public EventSubscriber
	{
	public:
		int _testVariable = 0;

		void virtual Notify(EventPublisher* publisherRef) override
		{
			publisherRef;
			assert(publisherRef->Is(Event<GenericMessage1>::TypeIdClass()));
			++_testVariable;
		}

	};

	class EventTestClass1T2 : public EventSubscriber
	{
	public:
		int _testVariable = 5;

		void virtual Notify(EventPublisher* publisherRef) override
		{
			publisherRef;
			assert(publisherRef->Is(Event<GenericMessage1>::TypeIdClass()));
			++_testVariable;
		}

	};

	class EventTestClass2 : public EventSubscriber
	{
	public:
		int _testVariable = 0;

		void virtual Notify(EventPublisher* publisherRef) override
		{
			publisherRef;
			assert(publisherRef->Is(Event<GenericMessage2>::TypeIdClass()));
			++_testVariable;
		}

	};

	class EventTestClass3 : public EventSubscriber
	{
	public:
		int _testVariable = 0;

		void virtual Notify(EventPublisher* publisherRef) override
		{
			publisherRef;
			assert(publisherRef->Is(Event<GenericMessage3>::TypeIdClass()));
			++_testVariable;
		}

	};

	class EventTestClass4 : public EventSubscriber
	{
	public:
		int _testVariable = 0;

		void virtual Notify(EventPublisher* publisherRef) override
		{
			publisherRef;
			assert(publisherRef->Is(Event<GenericMessage4>::TypeIdClass()));
			++_testVariable;
		}

	};

	class ReactionTestClass1 : public EventSubscriber
	{
	public:
		int _testVariable = 0;
		EventMessageAttributed _message;

		void virtual Notify(EventPublisher* publisherRef) override
		{
			_message = static_cast<Event<EventMessageAttributed>*>(publisherRef)->_message;
			//assert(publisherRef->Is(Event<GenericMessage4>::TypeIdClass()));
			++_testVariable;
		}

	};
}