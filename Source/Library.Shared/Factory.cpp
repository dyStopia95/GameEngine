#include "pch.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/*template <typename T>
	Factory<T>* Factory<T>::Find(const std::string& className)
	{
		auto it = _factoryList.Find(className);
		Factory<T>* factoryRef = ((*it).second).get();
		return factoryRef;
	}*/

	/*template<typename T>
	void Factory<T>::Add(Factory<T>* factoryRef)
	{
		std::string className = typeid(factoryRef).name();
		className = className.substr(className.find_last_of(':') + 1);
		_factoryList.insert(std::make_pair(std::string{ className }, factoryRef));
	}*/
}