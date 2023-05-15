#pragma once

#include <cstdint>
#include <cstddef>
#include <initializer_list>
#include <cstdlib>
#include <cassert>
#include <memory>
#include <stdexcept>
#include <utility>
#include <cstring>
#include <functional>
#include "vector.h"
#include "SList.h"
#include "DefaultHash.h"

namespace FieaGameEngine
{
	/// <summary>
	/// HashMap: Unordered map. Stores data as Key value pairs. Keys are used to generate a hash value that is
	/// used as the index of the base container, a vector. Secondary data structure used is an SList. If key
	/// collisions (different keys generating the same hash) occur, the previously constant time retrieval becomes linear time retrieval.
	/// </summary>
	/// <typeparam name="TKey"></typeparam>
	/// <typeparam name="TValue"></typeparam>
	template<typename TKey, typename TValue>
	class HashMap final
	{
		friend class iterator;
		using pair_type = std::pair<const TKey, TValue>;
		using chain_type = SList<pair_type>;

	public:

		/// <summary>
		/// iterator class for HashMap
		/// </summary>
		class iterator final
		{
			friend HashMap;
			friend class const_iterator;

		private:
			HashMap* _container = nullptr;
			size_t _chainIndex = 0;
			typename SList<pair_type>::iterator _currentPair;
			/// <summary>
			/// internal use iterator constructor
			/// </summary>
			/// <param name="container">reference to parent HashMap</param>
			/// <param name="chainIndex">index of hashed key</param>
			/// <param name="currentPair">Node of Slist being refered to</param>
			iterator(HashMap&, size_t, const typename chain_type::iterator& currentPair = typename chain_type::iterator{});

		public:
			iterator() = default;
			/// <summary>
			/// copy constructor for iterator
			/// </summary>
			/// <param name=""></param>
			iterator(const iterator&);
			pair_type& operator* () const;
			pair_type* operator->() const;
			void operator = (const iterator&);
			iterator& operator++ ();
			iterator operator++ (int);
			bool operator == (const iterator&) const;
			bool operator != (const iterator&) const;

		};

		/// <summary>
		/// const_iterator class for HashMap
		/// </summary>
		class const_iterator final
		{
			friend HashMap;

		private:
			const HashMap* _container = nullptr;
			size_t _chainIndex = 0;
			typename SList<pair_type>::const_iterator _currentPair;
			/// <summary>
			/// internal use const_iterator constructor
			/// </summary>
			/// <param name="container">reference to parent HashMap</param>
			/// <param name="chainIndex">index of hashed key</param>
			/// <param name="currentPair">Node of Slist being refered to</param>
			const_iterator(const HashMap&, size_t, const typename chain_type::const_iterator& currentPair = typename chain_type::const_iterator{});

		public:
			const_iterator() = default;
			/// <summary>
			/// copy constructor for const_iterator
			/// </summary>
			/// <param name=""></param>
			const_iterator(const const_iterator&);
			/// <summary>
			/// type cast constructor for iterator to const_iterator
			/// </summary>
			/// <param name=""></param>
			const_iterator(const iterator&);
			const pair_type& operator* () const;
			const pair_type* operator->() const;
			void operator = (const const_iterator&);
			const_iterator& operator++ ();
			const_iterator operator++ (int);
			bool operator == (const const_iterator&) const;
			bool operator != (const const_iterator&) const;
		};

	private:
		Vector<chain_type> _baseVector;
		size_t _chainCapacity;
		size_t _population = 0;
		size_t _lowestActiveIndex;
		std::function <size_t(const TKey&)> HashFn;

	public:

		/// <summary>
		/// Constructor for HashMap. Necessarily needs user specified chain capacity
		/// </summary>
		/// <param name="chainCapacity">Number of chains the HashMap should have</param>
		HashMap(std::size_t chainCapacity = 3, std::function<size_t(const TKey&)> = DefaultHash<TKey>{});
		//HashMap(HashMap&& rhs);
		HashMap(std::initializer_list<pair_type> list);

		/// <summary>
		/// Index operator [] : returns Value ref of key-value pair. If entry is not found, a default constructed Value is placed against the key provided.
		/// </summary>
		/// <param name="key"></param>
		/// <returns>Value of the pair</returns>
		TValue& operator[](const TKey&);
		const TValue& operator[](const TKey&) const;

		std::pair<iterator, bool> CreateAt(const TKey& key);
		/// <summary>
		/// hashFn: generate index from HashFn std::function member
		/// </summary>
		/// <param name="key"></param>
		/// <returns>size_t index</returns>
		size_t hashFn(const TKey&) const; // make functor
		/// <summary>
		/// begin: return iterator to first element in the map
		/// </summary>
		/// <returns>iterator</returns>
		iterator begin();
		/// <summary>
		/// cbegin: return const_iterator to first element in the map
		/// </summary>
		/// <returns>const_iterator</returns>
		const_iterator cbegin() const;
		/// <summary>
		/// end: returns iterator pointing to the end of baseVector
		/// </summary>
		/// <returns>iterator</returns>
		iterator end();
		/// <summary>
		/// cend: returns const_iterator pointing to the end of baseVector
		/// </summary>
		/// <returns>const_iterator</returns>
		const_iterator cend() const;
		/// <summary>
		/// insert: insert a pair after hashing the key. if the key already exists, return an iterator to that element
		/// </summary>
		/// <param name="pair"></param>
		/// <returns>iterator</returns>
		std::pair<iterator,bool> insert(const pair_type&);
		std::pair<iterator, bool> insert(pair_type&&);
		/// <summary>
		/// Find: returns iterator to the element having key passed as argument
		/// </summary>
		/// <param name="key"></param>
		/// <returns>iterator</returns>
		iterator Find(const TKey&); // needs to be tested for key with same hash
		const_iterator Find(const TKey&) const;
		/// <summary>
		/// Size: returns number of elements in the map
		/// </summary>
		/// <returns>size_t</returns>
		size_t Size();
		/// <summary>
		/// containsKey: returns true if the key passed as argument exists in the map
		/// </summary>
		/// <param name="key"></param>
		/// <returns>bool</returns>
		bool containsKey(const TKey&) const;
		/// <summary>
		/// At: returns the Value associated with the key passed as argument.
		/// </summary>
		/// <param name="key"></param>
		/// <returns>Value ref</returns>
		TValue& At(const TKey&);
		const TValue& At(const TKey&) const;
		/// <summary>
		/// Clear: clears all data and frees all allocated memory.
		/// </summary>
		void Clear();
		/// <summary>
		/// Remove: deletes the entry having key passed as argument. If not found, does nothing.
		/// </summary>
		/// <param name="key"></param>
		void Remove(const TKey&); // refactor SList remove

	};
}

#include "HashMap.inl"