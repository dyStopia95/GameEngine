#pragma once
#include "HashMap.h"

namespace FieaGameEngine
{
	template <typename T>
	class Factory
	{
		inline static HashMap<const std::string, std::shared_ptr<Factory<T>>> _factoryList;

	public:
		/// <summary>
		/// Virtual Destructor for correct inheritance
		/// </summary>
		virtual ~Factory() = default;

		/// <summary>
		/// takes in the class whose factory is required. returns pointer to Factory if found, nullptr otherwise.
		/// </summary>
		/// <param name="className">name of class whose factory is to be found. Do not add Factory in this argument</param>
		/// <returns>raw pointer to relevant Factory stored in static HashMap</returns>
		inline static Factory<T>* Find(const std::string& className)
		{
			//std::string factoryName = className + "Factory";

			auto it = _factoryList.Find(className);
			
			if (it != _factoryList.end())
				return (*it).second.get();
			else
				return nullptr;
		}
		
		/// <summary>
		/// Convenience Fn to directly create an object from given class name. returns nullptr if Factory not available
		/// !!! Ownership is transferred !!!
		/// </summary>
		/// <param name="className">name of class whose instance is to be created</param>
		/// <returns>raw Pointer to instance of this class.</returns>
		inline static T* Create(const std::string& className)
		{
			Factory<T>* factoryPtr = Find(className);
			if (factoryPtr != nullptr)
				return factoryPtr->Create();
			else
				return nullptr;
		}

		/// <summary>
		/// takes shared_ptr to a factory to be added to list of factories. Stores this pointer in a HashMap.
		/// Extracts name of the Factory using typeid().name()
		/// </summary>
		/// <param name="factoryRef">shared_ptr to the Factory to be added</param>
		inline static void Add(std::shared_ptr<Factory<T>>factoryRef)
		{
			assert(factoryRef != nullptr);
			
			std::string className = factoryRef->ClassName();

			//std::string className = typeid(* (factoryRef.get())).name();
			//className = className.substr(className.find_last_of(':') + 1);
			_factoryList.insert(std::pair<const std::string, std::shared_ptr<Factory<T>>> { className, std::move(factoryRef) });
		}
		
		/// <summary>
		/// Removes Factory associated to the given class. Do not add Factory to class name.
		/// </summary>
		/// <param name="className">name of class whose associated Factory is to be removed</param>
		inline static void Remove(const std::string& className)
		{
			_factoryList.Remove(std::string{ className });
		}
		
		/// <summary>
		/// clears all Factories stored in the HashMap
		/// </summary>
		inline static void Clear()
		{
			_factoryList.Clear();
		}

		/// <summary>
		/// Virtual function that will be implemented by the derived specialized Factories.
		/// Will return raw pointer of associated base class of this group of Factories.
		/// !!! Ownership will be transferred !!!
		/// </summary>
		/// <returns>raw pointer to associated base class of this group of Factories</returns>
		virtual T* Create() = 0;

		/// <summary>
		/// virtual function to be implemented by derived specialized Factory so as to check the name of the associated object that is being produced.
		/// </summary>
		/// <returns></returns>
		virtual std::string ClassName() = 0;
	};
}

#define CONCRETE_FACTORY(ProductName,BaseName)								\
class ProductName ## Factory : public FieaGameEngine::Factory<FieaGameEngine::BaseName>\
{																			\
	std::string ClassName() override {										\
		return std::string{#ProductName};													\
	}																		\
																			\
	ProductName* Create() override{											\
		return new ProductName{};											\
	}																		\
};																			


