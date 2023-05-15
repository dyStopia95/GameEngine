#include "vector.h"

namespace FieaGameEngine
{
#pragma region Iterator

	template<typename T>
	inline Vector<T>::iterator::iterator(Vector& Container, size_t index) :
		_container{ &Container }, _index{ index }
	{

	}

	template<typename T>
	inline Vector<T>::iterator::iterator(const iterator& it) :
		_container{ it._container }, _index{ it._index }
	{

	}

	template<typename T>
	inline std::size_t Vector<T>::iterator::GetIndex()
	{
		return _index;
	}

	template<typename T>
	inline void Vector<T>::iterator::operator=(const iterator& rhs)
	{
		if (_container == nullptr)
			_container = rhs._container;

		else if(_container != rhs._container)
			throw std::runtime_error("Iterators do not have the same container.");

		_index = rhs._index;
	}

	template<typename T>
	inline bool Vector<T>::iterator::operator != (const iterator& rhs) const
	{
		if (_container != rhs._container)
			return true;

		if (_index != rhs._index)
			return true;

		return false;
	}

	template<typename T>
	inline bool Vector<T>::iterator::operator == (const iterator& rhs) const
	{
		if (_container != rhs._container)
			return false;

		if (_index != rhs._index)
			return false;

		return true;
	}

	template<typename T>
	inline T& Vector<T>::iterator::operator	* () const
	{
		if (_index == _container->_size || _index > _container->_size)
			throw std::runtime_error("Cannot dereference end of vector.");

		return _container->_data[_index];
	}

	template<typename T>
	inline T& Vector<T>::iterator::operator[] (std::size_t index) const
	{
		if (index >= _container->_size)
			throw std::runtime_error("Invalid index.");

		return _container->operator[](index);
	}

	template<typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator+ (size_t increment)
	{
		_index += increment;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator- (size_t increment)
	{
		_index -= increment;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator ++ ()
	{
		if (_index == _container->_size)
			throw std::runtime_error("Cannot increment. Iterator at end of vector");

		++_index;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::iterator::operator ++ (int)
	{
		if (_index == _container->_size)
			throw std::runtime_error("Cannot increment. Iterator at end of vector");

		iterator tempIT = *this;
		this->operator++();
		return tempIT;
	}

	template<typename T>
	inline typename Vector<T>::iterator& Vector<T>::iterator::operator--()
	{
		if (_index == 0)
			throw std::runtime_error("Cannot decrement below start of vector");

		--_index;
		return *this;
	}

#pragma endregion Iterator

#pragma region ConstIterator

	template<typename T>
	inline Vector<T>::const_iterator::const_iterator(const Vector& Container, size_t Index) :
		_container{ &Container }, _index{ Index }
	{

	}

	template<typename T>
	inline Vector<T>::const_iterator::const_iterator(const const_iterator& it) :
		_container{ it._container }, _index{ it._index }
	{

	}

	template<typename T>
	inline Vector<T>::const_iterator::const_iterator(const iterator& it) :
		_container{ it._container }, _index{ it._index }
	{

	}

	template<typename T>
	inline std::size_t Vector<T>::const_iterator::GetIndex()
	{
		return _index;
	}

	template<typename T>
	inline void Vector<T>::const_iterator::operator= (const const_iterator& rhs)
	{
		if (_container == nullptr)
			_container = rhs._container;

		else if (_container != rhs._container)
			throw std::runtime_error("Iterators do not have the same container.");

		_index = rhs._index;
	}

	template<typename T>
	inline bool Vector<T>::const_iterator::operator!= (const const_iterator& rhs) const
	{
		if (_container != rhs._container)
			return true;

		if (_index != rhs._index)
			return true;

		return false;
	}

	template<typename T>
	inline bool Vector<T>::const_iterator::operator == (const const_iterator& rhs) const
	{
		if (_container != rhs._container)
			return false;

		if (_index != rhs._index)
			return false;

		return true;
	}

	template<typename T>
	inline const T& Vector<T>::const_iterator::operator* () const
	{
		if (_index == _container->_size)
			throw std::runtime_error("Cannot dereference end of vector.");

		return _container->_data[_index];
	}

	template<typename T>
	inline const T& Vector<T>::const_iterator::operator[] (std::size_t index) const
	{
		if (index >= _container->_size)
			throw std::runtime_error("Invalid index.");

		return _container->operator[](index);
	}

	template<typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator+(size_t increment)
	{
		_index += increment;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator-(size_t increment)
	{
		_index -= increment;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator++()
	{
		if (_index == _container->_size)
			throw std::runtime_error("Cannot increment. Iterator at end of vector");

		++_index;
		return *this;
	}

	template<typename T>
	inline typename Vector<T>::const_iterator Vector<T>::const_iterator::operator++(int)
	{
		if (_index == _container->_size)
			throw std::runtime_error("Cannot increment. Iterator at end of vector");

		const_iterator tempIT = *this;
		this->operator++();
		return tempIT;
	}

	template<typename T>
	inline typename Vector<T>::const_iterator& Vector<T>::const_iterator::operator--()
	{
		if (_index == 0)
			throw std::runtime_error("Cannot decrement below start of vector");

		--_index;
		return *this;
	}

#pragma endregion ConstIterator

#pragma region VectorMain

	template<typename T>
	inline Vector<T>::Vector() :
		_data{ nullptr }, _capacity{ 0 }, _size{ 0 }
	{
		//reserve(100);
	}

	template<typename T>
	inline Vector<T>::Vector(std::size_t capacity) :
		_data{ nullptr }, _capacity{ 0 }, _size{ 0 }
	{
		reserve(capacity);
	}

	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<T> vector)
	{
		for (const auto& value : vector)
		{
			push_back(value);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(const Vector& vector2)
	{
		reserve(vector2._size); //this vector is not right ** for typeManager, when copying the signature vector to pair.second in hashmap _signatures(of TypeManager)

		for (size_t i = 0; i < vector2._size; ++i)
		{
			push_back(vector2._data[i]); // 
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& rhs) noexcept:
		_data{ rhs._data }, _capacity{ rhs._capacity }, _size{ rhs._size }
	{
		rhs._data = nullptr;
		rhs._capacity = 0;
		rhs._size = 0;
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		clear();
		free(_data);
		//delete _data;
		_data = nullptr;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator= (const Vector& rhs)
	{
		clear();
		shrink_to_fit();
		reserve(rhs._capacity);

		for (size_t i = 0; i < rhs._size; ++i)
		{
			push_back(rhs._data[i]);
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs) noexcept
	{
		if (operator!=( rhs ))
		{
			clear();
			shrink_to_fit();

			_data = rhs._data;
			_capacity = rhs._capacity;
			_size = rhs._size;

			rhs._data = nullptr;
			rhs._capacity = 0;
			rhs._size = 0;
		}

		return *this;
	}

	template<typename T>
	inline bool Vector<T>::operator!=(const Vector& rhs)
	{
		if (this == &rhs)
			return false;

		if ((_data != rhs._data) && (_size != rhs._size))
			return true;

		for (size_t i = 0; i < _size; ++i)
		{
			if (operator[](i) == rhs.operator[](i))
				return false;
		}

		return true;
	}

	template<typename T>
	inline bool Vector<T>::operator==(const Vector& rhs)
	{
		return !(operator!=(rhs));
	}

	template<typename T>
	inline T& Vector<T>::operator[](std::size_t index)
	{
		if (index >= _size)
			throw std::runtime_error("oh no! :(");

			return *(_data + index);
	}

	template<typename T>
	inline const T& Vector<T>::operator[](std::size_t index) const
	{
		if (index >= _size)
			throw std::runtime_error("oh no! :(");

		return *(_data + index);
	}

	template<typename T>
	inline T& Vector<T>::at(std::size_t index)
	{
		if (index >= _size)
			throw std::runtime_error("oh no! :(");

		return *(_data + index);
	}

	template<typename T>
	inline const T& Vector<T>::at(std::size_t index) const
	{
		if (index >= _size)
			throw std::runtime_error("oh no! :(");

		return *(_data + index);
	}

	template <typename T>
	inline void Vector<T>::reserve(std::size_t capacity)
	{
		if(capacity > _capacity) 
		{
			T* data = reinterpret_cast<T*>(realloc(_data, capacity * sizeof(T)));
			assert(data != nullptr);
			_data = data;
			_capacity = capacity;
		}
	}

	template<typename T>
	inline void Vector<T>::shrink_to_fit()
	{
		/*if (_capacity == _size)
			return;*/

		if (_capacity > _size)
		{
			if (_size == 0)
			{
				free(_data);
				_data = nullptr;
			}
			else
			{
				T* data = reinterpret_cast<T*>(realloc(_data, _capacity * sizeof(T)));
				assert(data != nullptr); //removed assert as this will be nullptr if _size = 0
				_data = data;
			}
			_capacity = _size;
		}
	}

	template<typename T>
	inline void Vector<T>::clear()
	{
		std::size_t i{ 0 };
		while (i < _size)
		{
			_data[i].~T();
			++i;
		}
		_size = 0;
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::end()
	{
		return iterator{ *this, _size };
	}

	template<typename T>
	inline typename Vector<T>::const_iterator Vector<T>::cend() const
	{
		return const_iterator{ *this, _size };
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::begin()
	{
		return iterator{ *this };
	}

	template<typename T>
	inline typename Vector<T>::const_iterator Vector<T>::cbegin() const
	{
		return const_iterator{ *this };
	}

	template<typename T>
	inline void Vector<T>::push_back(const T& value)
	{
		if (_size == _capacity) //capacity is not right
			reserve(_capacity + 1); 

		new(_data + _size) T{ value }; //breaks at new
		++_size;
	}

	template<typename T>
	inline void Vector<T>::push_back(T&& value)
	{
		if (_size == _capacity)
			reserve(_capacity + 1);

		new(_data + _size) T{std::forward<T>(value)};
		++_size;
	}

	template<typename T>
	template<typename... Args>
	inline void Vector<T>::emplace_back(Args&&... args)
	{
		if (_size == _capacity)
			reserve(_capacity + 1);

		new (_data + _size) T{ std::forward<Args>(args)... };
		++_size;
	}


	template<typename T>
	inline void Vector<T>::pop_back()
	{
		if (_size == 0)
			throw std::runtime_error("Vector is already empty.");

		_data[_size-1].~T();
		--_size;
	}

	template<typename T>
	inline bool Vector<T>::empty() const
	{
		return _size == 0;
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::find(const T& value)
	{
		iterator it = begin();
		while ((it != end()) && (*it != value))
		{
			++it;
		}

		return it;
	}

	template<typename T>
	inline typename Vector<T>::const_iterator Vector<T>::find(const T& value) const
	{
		const_iterator it = cbegin();
		while ((it != cend()) && (*it != value))
		{
			++it;
		}
		return it;
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::erase(const T& value)
	{
		iterator it = Vector<T>::find(value);
		if (it != end())
		{
			(*it).~T();

			if (it._index != _size - 1)
				std::memmove((_data + it._index), (_data + it._index + 1), (_size - it._index - 1) * sizeof(T));

			--_size;
		}

		return it;
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::erase(iterator pos)
	{
		if ((pos._container != this) || (pos._index >= _size))
			throw std::runtime_error("Invalid iterator.");

		(*pos).~T();

		if (pos._index != _size - 1)
			std::memmove((_data + pos._index), (_data + pos._index + 1), (_size - pos._index - 1) * sizeof(T));
		
		--_size;

		return iterator{ *this, pos._index };
	}

	template<typename T>
	inline typename Vector<T>::iterator Vector<T>::remove(iterator start, iterator end)
	{
		if ((start._container != this) || (start._index >= _size))
			throw std::runtime_error("Invalid start iterator.");

		if ((end._container != this) || (end._index > _size))
			throw std::runtime_error("Invalid end iterator.");

		if (start._index >= end._index)
			throw std::runtime_error("Invalid range.");

		iterator it = start;
		int sizeDecrement = 0;

		while (it != end)
		{
			(*it).~T();
			++sizeDecrement;
			++it;
		}

		if (end._index != _size)
			std::memmove((_data + start._index), (_data + end._index), (_size - end._index) * sizeof(T));

		_size = _size - sizeDecrement;

		return start;
	}

	template<typename T>
	inline T& Vector<T>::front()
	{
		if (_size == 0)
			throw std::runtime_error("Vector is empty.");

		return (_data[0]);
	}

	template<typename T>
	inline const T& Vector<T>::front() const
	{
		if (_size == 0)
			throw std::runtime_error("Vector is empty.");

		return (_data[0]);
	}

	template<typename T>
	inline T& Vector<T>::back()
	{
		if (_size == 0)
			throw std::runtime_error("Vector is empty.");

		return (_data[_size - 1]);
	}

	template<typename T>
	inline const T& Vector<T>::back() const
	{
		if (_size == 0)
			throw std::runtime_error("Vector is empty.");

		return (_data[_size - 1]);
	}

	template<typename T>
	inline std::size_t Vector<T>::size() const
	{
		return _size;
	}

	template<typename T>
	inline std::size_t Vector<T>::capacity() const
	{
		return _capacity;
	}

#pragma endregion VectorMain

}