#pragma once
#include "Datum.h"
#include "RTTI.h"
#include "HashMap.h"
#include "vector.h"
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
	class Scope : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Scope, FieaGameEngine::RTTI);

		using pair_type = std::pair<std::string, Datum>;

	private:
		std::size_t _capacity;
		std::size_t _population = 0;
		HashMap<std::string, Datum> _data;
		Scope* _parent{ nullptr };

	protected:
		Vector<std::pair<const std::string,Datum>*> _dataTable{0};

	public:
		/// <summary>
		/// constructor for Scope class. Takes in size_t to set capacity of the hashmap. The size for the pointer lookup table is also set from this.
		/// </summary>
		/// <param name="capacity"></param>
		Scope(std::size_t capacity = 3);
		/// <summary>
		/// virtual destructor for Scope. empties and deletes all nested scopes. clears hashmap and pointer lookup table.
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Clone virtual constructor substitute. called when copy of scope is made. polymorphically returns a scope* to copy of object from the
		/// object it is called against.
		/// </summary>
		/// <returns>Heap allocated pointer to a cloned object !!! you take ownership when calling this Fn !!!</returns>
		[[nodiscard]] virtual Scope* Clone() const;
		/// <summary>
		/// Copy constructor for Scope. Deep copies all datums and nested scopes maintaining order of insertion.
		/// </summary>
		/// <param name="value"></param>
		Scope(const Scope& value);
		/// <summary>
		/// copy assignment op. clears old values, sets population to 0. does not copy parentage.
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Scope& operator= (const Scope& rhs);
		Scope(Scope&& value) noexcept;
		Scope& operator= (Scope&& rhs) noexcept;

		/// <summary>
		/// Removes the two-way relation of nested scope that calls this. searches in parent scope for this object and removes it from their datum.
		/// then sets the _parent pointer to nullptr.
		/// </summary>
		/// <returns>Returns a pointer to the scope that was emancipated. !!! You take ownership of this object when calling this Fn !!!</returns>
		[[nodiscard]] Scope* Orphan();

		/// <summary>
		/// polymorphic overload to the RTTI Equals fn. compares all datums and keys of both scopes. order of insertion matters. first casts rhs as scope pointers
		/// </summary>
		/// <param name="rhs">object to compare against</param>
		/// <returns></returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// just calls Equals
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		bool operator== (const Scope& rhs) const;
		/// <summary>
		/// just does !(operator==())
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		bool operator!= (const Scope& rhs) const;
		/// <summary>
		/// returns the datum of the string-Datum pair stored in _dataTable as pointers. takes in index of insertion of the string-Datum pair.
		/// </summary>
		/// <param name="index">index at which the pair was inserted</param>
		/// <returns>Datum (Value) of the string-Datum pair</returns>
		Datum& operator[](std::size_t index);

		Datum& operator[](const std::string& key);

		/// <summary>
		/// clears the hashmap, pointerTable and deletes all nested scopes.
		/// </summary>
		void Clear();
		/// <summary>
		/// returns the number of string-Datum pairs present in the Scope.
		/// </summary>
		/// <returns></returns>
		std::size_t Size();
		/// <summary>
		/// Returns a pointer to the Datum of the string-Datum pair based on key passed as argument.
		/// </summary>
		/// <param name="key">string key of the string-Datum pair to search</param>
		/// <returns>Pointer to the Datum of string-Datum pair. nullptr if not found</returns>
		Datum* Find(const std::string& key);
		const Datum* Find(const std::string& key) const; //Do
		/// <summary>
		/// Searches for given key in the calling Scope object. if not found, recursively finds in all ancestor Scopes till the root Scope.
		/// returns pointer to the found Datum. optionally, user can pass in a Scope** pointer to retrieve information of the Scope object in which the key was found.
		/// </summary>
		/// <param name="key">string key of the string-Datum pair to find</param>
		/// <param name="containingScope">optional argument to retrieve pointer to the Scope in which the Key was found.</param>
		/// <returns>Pointer to the found datum. nullptr if not found.</returns>
		Datum* Search(const std::string& key, Scope** containingScope = nullptr);
		/// <summary>
		/// Adds a default constructed Datum in the hashmap against the provided key. if the key already exists, returns a reference to the associated Datum.
		/// </summary>
		/// <param name="key">string key of the string-Datum pair to append or retrieve</param>
		/// <returns>reference to the newly created / existing Datum.</returns>
		Datum& Append(const std::string& key);
		/// <summary>
		/// Looks for the key provided. if the key entry already exists, it creates a new heap allocated scope and appends this scope to the existing Datum.
		/// if the entry does not exist, it creates a new Datum and appends a new heap allocated scope to this Datum. returns reference to the newly created Scope.
		/// </summary>
		/// <param name="key">string key of the string-Datum pair.</param>
		/// <returns>reference to the newly created Scope.</returns>
		Scope& AppendScope(const std::string& key);// *** refactor adopt to have helper fn for raw pushback and one for checks regarding datum.
		/// <summary>
		/// takes a reference to a scope object and a key. if the key already has an associated Datum in the hashmap, pushesback pointer to the scope object into the Datum.
		/// if Datum does not exist, creates one and pushes back the scope to this Datum. Also sets the _parent pointer of the child scope to the calling scopes this pointer.
		/// this transfers ownership of the child to the calling Scope object.
		/// </summary>
		/// <param name="child">: reference to the child Scope to be adopted.</param>
		/// <param name="key">: string key of string-Datum pair in which child is to be inserted.</param>
		void Adopt(Scope& child, const std::string& key); //change return type && refactor to use hashmap createAt
		/// <summary>
		/// returns the _parent pointer of this Scope.
		/// </summary>
		/// <returns></returns>
		Scope* GetParent();
		/// <summary>
		/// takes pointer to a nested scope object and returns pointer to the container Datum, as well as index at which the scope was found.
		/// returns nullptr for Datum* and 0 for the index if the scope was not found.
		/// </summary>
		/// <param name="child">pointer to the scope object to be found</param>
		/// <returns>pair of pointer to container Datum and index at which the child scope was found.</returns>
		std::pair<Datum*, std::size_t> FindContainedScope(Scope* const child);

		/// <summary>
		/// takes a pair and deep copies and adds it to the hashmap. if the key already exists, returns a false bool.
		/// </summary>
		/// <param name="value">pair to be inserted</param>
		/// <returns></returns>
		bool Insert(const pair_type& value);
		/// <summary>
		/// takes separate key and value and calls the pair insert.
		/// </summary>
		/// <param name="key">key of string-Datum pair</param>
		/// <param name="value">Datum of string-Datum pair</param>
		/// <returns></returns>
		bool Insert(const std::string& key, Datum& value);

	};
}