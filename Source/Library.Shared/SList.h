#pragma once

#include <cstdint>
#include <cstddef>
#include <initializer_list>


namespace FieaGameEngine
{
	template <typename T>
	class SList final
	{


	private:

		/// <summary>
		/// Node: Struct to store data. One instance of Node will be one element of the SList.
		/// </summary>
		struct Node
		{
			T Data;
			Node* Next{ nullptr };	// Pointer to the next node in the list. This will be nullptr if this is the last node in the list.

			Node(const T&, Node* nextNode = nullptr);
			Node(T&&, Node* nextNode = nullptr);
		};

	public:
		
		/// <summary>
		/// iterator: Class to iterate through the elements of the List.
		/// </summary>
		class iterator
		{
			friend SList;
			friend class const_iterator;

		private:
			Node* _currentNode { nullptr };	// Pointer to the current node being looked at by the iterator.
			const SList* _container { nullptr };

			iterator(const SList&, Node* node = nullptr);

		public:
			iterator() = default;
			iterator(const iterator&);
			bool operator!= (const iterator&) const;
			bool operator== (const iterator&) const;
			iterator& operator++ ();
			iterator operator++ (int);
			T& operator* () const;
			//const T& operator* () const;
		};

		class const_iterator
		{
			friend SList;
			friend class const_iterator;

		private:
			Node* _currentNode{ nullptr };
			const SList* _container{ nullptr };

			const_iterator(const SList&, Node* node = nullptr);

		public:
			const_iterator() = default;
			const_iterator(const iterator&);
			bool operator!= (const const_iterator&) const;
			bool operator == (const const_iterator&) const;
			const_iterator& operator++();
			const_iterator operator++(int);
			const T& operator* () const;
		};

	public:
		SList() = default;
		SList(std::initializer_list<T>);
		SList(const SList&);
		SList& operator= (const SList&);
		~SList();

		void	clear();

		iterator PushFront(const T& x);
		iterator PushBack(const T& x);
		iterator PushBack(T&& value);
		T		PopFront(); //change when shifting to typeT
		T		PopBack();

		[[nodiscard]] bool IsEmpty() const;

		[[nodiscard]] T& Front();
		[[nodiscard]] const T& Front() const;

		[[nodiscard]] T& Back();
		[[nodiscard]] const T& Back() const;
		
		[[nodiscard]] std::size_t Size() const;

		/// <summary>
		/// end(): returns iterator pointing past end of list
		/// </summary>
		/// <returns></returns>
		iterator end();

		const_iterator end() const;

		const_iterator cend() const;

		/// <summary>
		/// begin(): returns iterator to the start of the list
		/// </summary>
		/// <returns></returns>
		iterator begin();

		const_iterator begin() const;

		const_iterator cbegin() const;

		/// <summary>
		/// Find(): attempts to find the given value in the list. If not found returns end().
		/// </summary>
		/// <param name=""> value to find </param>
		/// <returns></returns>
		iterator Find(const T&);

		/// <summary>
		/// InsertAfter: inserts given value after node to be inserted after.
		/// </summary>
		/// <param name=""> value to insert </param>
		/// <param name=""> value to insert after </param>
		/// <returns></returns>
		iterator InsertAfter(const T&, const T&);

		/// <summary>
		/// Erase(): finds and removes first node having provided value;
		/// </summary>
		/// <param name=""> value to be removed</param>
		/// <returns> iterator to the node after erased node </returns>
		iterator Erase(const T&);
		iterator Erase(iterator);

	private:
		Node* _front{ nullptr };
		Node* _back{ nullptr };
		std::size_t _size{ 0 };
	};
}

#include "SList.inl"