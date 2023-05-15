#pragma once
#include "Scope.h"
#include "TypeManager.h"
#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <string>
#include <functional>

namespace FieaGameEngine
{
	

	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, FieaGameEngine::Scope);
	private:
		/// <summary>
		/// used to populate the scope with string datum pairs. uses the signature of a class taken from the TypeManager.
		/// sets the datum to external storage and pointing to the data members of the class.
		/// </summary>
		/// <param name="typeID">RTTI typeID of the child class</param>
		void Populate(size_t typeID);

	protected:
		/// <summary>
		/// Default constructor. Set to protected so as to make Attributed pure virtual. Takes typeID of the child class
		/// as argument, to be passed in during construction of the class.
		/// </summary>
		/// <param name="typeID">RTTI typeID of child class</param>
		Attributed(size_t typeID);
		virtual ~Attributed() = default;
		/// <summary>
		/// copy constructor of Attributed. uses scopes copy constructor to input data to the scope and then updates the pointers
		/// with a populate call.
		/// </summary>
		/// <param name="rhs">class to be copied</param>
		Attributed(const Attributed& rhs);
		Attributed& operator= (const Attributed& rhs);

	public:
		/// <summary>
		/// returns true if the string passed as argument exists as an entry in the scope.
		/// </summary>
		/// <param name="AttName">Attribute name to be searched</param>
		/// <returns></returns>
		bool IsAttribute(std::string AttName);
		/// <summary>
		/// returns true if the Attribute name is one of the prescribed attributes
		/// </summary>
		/// <param name="AttName">Attribute name to be searched</param>
		/// <returns></returns>
		bool IsPrescribedAttribute(std::string AttName);
		/// <summary>
		/// returns true if the Attribute name is one of the auxiliary attributes
		/// </summary>
		/// <param name="AttName">Attribute name to be searched</param>
		/// <returns></returns>
		bool IsAuxiliaryAttribute(std::string AttName);
		/// <summary>
		/// adds a auxiliary attribute if the provided name is not one of the prescribed attributes
		/// </summary>
		/// <param name="AttName">Attribute name to be inserted</param>
		/// <returns></returns>
		Datum& AppendAuxiliaryAttribute(std::string AttName);

		/// <summary>
		/// returns the index at which the attribute was found as well as the size of the prescribed attributes list.
		/// returns number of total attributes as index to indicate that attribute was not found. Intended to be used as helper Fn
		/// </summary>
		/// <param name="AttName">Attribute name to be searched</param>
		/// <returns></returns>
		std::pair<size_t, size_t> GetIndexOfAttribute(std::string AttName);

		/// <summary>
		/// returns reference to the _dataTable / orderVector of the scope
		/// </summary>
		/// <returns></returns>
		Vector<std::pair<const std::string, Datum>*>& Attributes();
		/// <summary>
		/// returns iterator to start of the prescribed attributes and start of auxiliary attributes (behaves as generic end() but is still valid iterator
		/// </summary>
		/// <returns></returns>
		std::pair< Vector<std::pair<const std::string, Datum>*>::iterator, Vector<std::pair<const std::string, Datum>*>::iterator> PrescribedAttributes();
		/// <summary>
		/// returns iterator to start of the auxiliary attributes and end of the _dataTable / orderVector.
		/// </summary>
		/// <returns></returns>
		std::pair< Vector<std::pair<const std::string, Datum>*>::iterator, Vector<std::pair<const std::string, Datum>*>::iterator> AuxiliaryAttributes();
		std::pair< Vector<std::pair<const std::string, Datum>*>::const_iterator, Vector<std::pair<const std::string, Datum>*>::const_iterator> AuxiliaryAttributes() const;

	};
}