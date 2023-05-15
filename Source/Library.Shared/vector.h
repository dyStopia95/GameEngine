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

namespace FieaGameEngine
{
	template<typename T>
	class Vector final
	{
	public:

		//Aliases
		using size_type = std::size_t;
		using value_type = T;
		using reference = value_type&;
		using const_reference = const value_type&;
		using rvalue_reference = T&&;

		/// <summary>
		/// iterator class for vector
		/// </summary>
		class iterator
		{
			friend Vector;
			friend class const_iterator;

		private:
			std::size_t _index{ 0 };
			Vector* _container{ nullptr };

		public:
			iterator() = default;
			iterator(Vector&, size_t index = 0);
			iterator(const iterator&); //* unable to mark this explicit
			std::size_t GetIndex();
			void operator =  (const iterator&);
			bool operator != (const iterator&) const;
			bool operator == (const iterator&) const;
			T& operator* () const;
			T& operator[](std::size_t) const;
			Vector<T>::iterator& operator+ (size_t);
			Vector<T>::iterator& operator- (size_t);
			Vector<T>::iterator& operator ++ ();
			Vector<T>::iterator  operator ++ (int);
			Vector<T>::iterator& operator -- ();
		};

		class const_iterator
		{
			friend Vector;

		private:
			std::size_t _index{ 0 };
			const Vector* _container{ nullptr };

		public:
			const_iterator() = default;
			const_iterator(const Vector&, size_t index = 0);
			const_iterator(const const_iterator&);
			const_iterator(const iterator&);
			std::size_t GetIndex();
			void operator = (const const_iterator&);
			bool operator != (const const_iterator&) const;
			bool operator == (const const_iterator&) const;
			const T& operator* () const;
			const T& operator[](std::size_t) const;
			Vector<T>::const_iterator& operator+ (size_t);
			Vector<T>::const_iterator& operator- (size_t);
			Vector<T>::const_iterator& operator ++ ();
			Vector<T>::const_iterator  operator ++ (int);
			Vector<T>::const_iterator& operator -- ();
		};

	public:
		// Special members

		/// <summary>
		/// Default constructor: sets capacity to 100
		/// </summary>
		Vector();
		Vector(std::size_t);
		Vector(std::initializer_list<T>);
		Vector(const Vector&);
		Vector(Vector&& rhs) noexcept;
		~Vector();
		Vector& operator= (const Vector&);
		Vector& operator= (Vector&&) noexcept;
		bool operator != (const Vector&);
		bool operator == (const Vector& rhs);

		T& operator[](std::size_t);
		const T& operator[](std::size_t) const;

		//Member functions

		T& at(std::size_t);
		const T& at(std::size_t) const;

		/// <summary>
		/// front: returns object at start of vector.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] T& front();
		[[nodiscard]] const T& front() const;

		/// <summary>
		/// back: returns value at end of vector.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] T& back(); // doxygen not working with nodiscard attribute
		[[nodiscard]] const T& back() const;

		/// <summary>
		/// size: return current size.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] std::size_t size() const;

		/// <summary>
		/// capacity: return current capacity.
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] std::size_t capacity() const;

		/// <summary>
		/// reserve: takes in new capacity. If it is greater than current capacity
		/// reallocate vector with new capacity.
		/// </summary>
		/// <param name=""> target capacity </param>
		void reserve(std::size_t);

		/// <summary>
		/// shrink_to_fit: if capacity is more than the size, reallocate vector
		/// with new capacity equal to current size.
		/// </summary>
		void shrink_to_fit();

		/// <summary>
		/// clear: clear all elements of the vector. Does not change capacity.
		/// </summary>
		void clear();

		/// <summary>
		/// end: return iterator to last element of the vector
		/// </summary>
		/// <returns></returns>
		[[nodiscard]] iterator end();
		[[nodiscard]] const_iterator cend() const;

		/// <summary>
		/// begin: return iterator to the start of the vector.
		/// </summary>
		/// <returns>iterator</returns>
		[[nodiscard]] iterator begin();
		[[nodiscard]] const_iterator cbegin() const;

		struct DefIncFn final
		{
			size_t operator()(size_t capacity)
			{
				return capacity * 2;
			}
		};

		template<typename Fn = DefIncFn>
		void test_back(const T& value, Fn incFn = Fn{})
		{
			if (_size == _capacity)
				reserve(incFn(_capacity));

			new (_data + _size) T{ value };
			++_size;
		}

		/// <summary>
		/// push_back: insert data at the end of the vector
		/// </summary>
		/// <param name="">type T reference</param>		
		void push_back(const T&);
		void push_back(T&&);

		template <typename... Args>
		void emplace_back(Args&&... args);

		/// <summary>
		/// pop_back: removes object at end of vector
		/// </summary>
		void pop_back();

		/// <summary>
		/// empty: returns true if the vector has size = 0
		/// </summary>
		/// <returns></returns>
		bool empty() const;

		/// <summary>
		/// find: returns iterator to element in vector having same value passed to the function
		/// .Returns end() if element was not found.
		/// </summary>
		/// <param name="">object T</param>
		/// <returns>iterator to element in vector having value T</returns>
		iterator find(const T&);
		const_iterator find(const T&) const;

		/// <summary>
		/// erase: erases 1 element from vector.
		/// </summary>
		/// <param name="">object T or iterator to element</param>
		/// <returns>iterator to next element in vector</returns>
		iterator erase(const T&);
		iterator erase(iterator);

		/// <summary>
		/// remove: removes all elements in a given range of iterators
		/// </summary>
		/// <param name="">start of range</param>
		/// <param name="">end of range (not inclusive)</param>
		/// <returns>iterator to end of removed range</returns>
		iterator remove(iterator, iterator);

	private:
		T* _data{nullptr};
		std::size_t _capacity{0};
		std::size_t _size{0};

	};
}

#include "vector.inl"