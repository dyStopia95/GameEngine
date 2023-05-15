#include "SList.h"

namespace FieaGameEngine
{
#pragma region Node

	template<typename T>
	inline SList<T>::Node::Node(const T& data, Node* nextNode) :
		Data{ data }, Next{ nextNode }
	{
	}

	template<typename T>
	inline SList<T>::Node::Node(T&& data, Node* nextNode) :
		Data{ std::move(data) }, Next{ nextNode }
	{
	}

#pragma endregion Node

#pragma region iterator

	template<typename T>
	inline SList<T>::iterator::iterator(const SList& Container, Node* node) :
		_container{&Container}, _currentNode{node}
	{
	}

	template<typename T>
	inline SList<T>::iterator::iterator(const iterator& it) :
		_container{ it._container }, _currentNode{ it._currentNode }
	{
	}

	template<typename T>
	inline bool SList<T>::iterator::operator!=(const iterator& rhs) const
	{
		if (_container != rhs._container)
			return true;

		else
		{
			if (_currentNode == rhs._currentNode)
				return false;
			else
				return true;
		}
	}

	template<typename T>
	inline bool SList<T>::iterator::operator==(const iterator& rhs) const
	{
		if (*this != rhs)
			return false;

		else
			return true;
	}

	template<typename T>
	inline typename SList<T>::iterator& SList<T>::iterator::operator++()
	{
		if (_currentNode != nullptr)
		{
			_currentNode = _currentNode->Next;
			return *this;
		}

		throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::iterator::operator++(int)
	{
		if (_currentNode != nullptr)
		{
			iterator tempIT = *this; // ?is it ok to return this as this is local variable? or is it ok because it is going to return a copy of the local variable?
			operator++();
			return tempIT;
		}

		throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline T& SList<T>::iterator::operator*() const
	{
		if (_container != nullptr && _currentNode != nullptr)
			return _currentNode->Data;

		else
			throw std::runtime_error("");
	}

#pragma endregion iterator
	
#pragma region const_iterator

	template<typename T>
	inline SList<T>::const_iterator::const_iterator(const SList& Container, Node* node) :
		_container{&Container}, _currentNode{node}
	{
	}

	template<typename T>
	inline SList<T>::const_iterator::const_iterator(const iterator& it) :
		_container{ it._container }, _currentNode{ it._currentNode }
	{
	}

	template<typename T>
	inline bool SList<T>::const_iterator::operator!=(const const_iterator& rhs) const
	{
		if (_container != rhs._container)
			return true;

		else
		{
			if (_currentNode == rhs._currentNode)
				return false;
			else
				return true;
		}
	}

	template<typename T>
	inline bool SList<T>::const_iterator::operator==(const const_iterator& rhs) const
	{
		if (*this != rhs)
			return false;

		else
			return true;
	}

	template<typename T>
	inline typename SList<T>::const_iterator& SList<T>::const_iterator::operator++()
	{
		if (_currentNode != nullptr)
		{
			_currentNode = _currentNode->Next;
			return *this;
		}

		throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::const_iterator::operator++(int)
	{
		if (_currentNode != nullptr)
		{
			const_iterator tempIT = *this;
			operator++();
			return tempIT;
		}

		throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline const T& SList<T>::const_iterator::operator*() const
	{
		if (_container != nullptr && _currentNode != nullptr)
			return _currentNode->Data;

		else
			throw std::runtime_error("");
	}

#pragma endregion const_iterator

#pragma region SList

#pragma region SListBase

	template<typename T>
	inline SList<T>::SList(std::initializer_list<T> list)
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	inline SList<T>::SList(const SList& rhs)
	{
		Node* tempNode = rhs._front;
		while (tempNode != nullptr)
		{
			PushBack(tempNode->Data);
			tempNode = tempNode->Next;
		}
	}

	template<typename T>
	inline SList<T>& SList<T>::operator= (const SList& rhsList)
	{
		if (this != &rhsList)
		{
			clear();

			Node* tempNode = rhsList._front;
			while (tempNode != nullptr)
			{
				PushBack(tempNode->Data);
				tempNode = tempNode->Next;
			}
		}

		return *this;
	}

	template<typename T>
	inline SList<T>::~SList()
	{
		clear();
	}

	template<typename T>
	inline void SList<T>::clear()
	{	
		while (_front != nullptr)
		{
			Node* tempNode = _front->Next;
			delete _front;
			_front = tempNode;
		}

		_back = nullptr;
		_size = 0;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::PushFront( const T& value)
	{
		_front = new Node{ value, _front };

		if (_front->Next == nullptr)
		{
			_back = _front;
		}

		++_size;

		return iterator{ *this,_front };

	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::PushBack(const T& value)
	{
		if (_front == nullptr)
		{
			_back = new Node{ value };
			_front = _back;
		}

		else
		{
			Node* tempNode = new Node{ value };
			_back->Next = tempNode;
			_back = tempNode;
		}

		++_size;

		return iterator{ *this,_back };
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::PushBack(T&& value)
	{
		if (_front == nullptr)
		{
			_back = new Node{ std::move(value) };
			_front = _back;
		}

		else
		{
			Node* tempNode = new Node{ std::move(value) };
			_back->Next = tempNode;
			_back = tempNode;
		}

		++_size;

		return iterator{ *this,_back };
	}

	template<typename T>
	inline T SList<T>::PopFront()
	{
		if(_front != nullptr)
		{
			//T data;
			if (_front->Next != nullptr)
			{
				T data = _front->Data;
				Node* tempNode = _front->Next;
				delete _front;
				_front = tempNode;
				--_size;
				return data;
			}

			else
			{
				T data = _front->Data;
				delete _front;
				_front = nullptr;
				_back = nullptr;
				--_size;
				return data;
			}
		}
		
		else
		{
			throw std::runtime_error("oh no! :(");
		}
	}

	template<typename T>
	inline T SList<T>::PopBack()
	{
		if (_front != nullptr)
		{

			if (_front == _back)
			{
				T data = _front->Data;
				delete _front;
				_front = nullptr;
				_back = nullptr;
				--_size;
				return data;
			}

			else
			{
				Node* tempNode = _front;
				while (tempNode->Next != _back)
				{
					tempNode = tempNode->Next;
				}

				T data = _back->Data;
				delete _back;
				_back = tempNode;
				_back->Next = nullptr;
				--_size;
				return data;
			}
		}

		else
			throw std::runtime_error("oh no! :(");

	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return (_front == nullptr);
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (_front != nullptr)
			return _front->Data;
		else
			throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (_front != nullptr)
			return _front->Data;
		else
			throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (_back != nullptr)
			return _back->Data;
		else
			throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (_back != nullptr)
			return _back->Data;
		else
			throw std::runtime_error("oh no! :(");
	}

	template<typename T>
	inline std::size_t SList<T>::Size() const
	{
		return _size;
	}

#pragma endregion SListBase

#pragma region SListIterator

	template<typename T>
	inline typename SList<T>::iterator SList<T>::end()
	{
		return iterator(*this);
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::end() const
	{
		return const_iterator(*this);
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::cend() const
	{
		return const_iterator(*this);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::begin()
	{
		return iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::begin() const
	{
		return const_iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::const_iterator SList<T>::cbegin() const
	{
		return const_iterator(*this, _front);
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::Find(const T& value)
	{
		value;
		if (_front != nullptr)
		{
			iterator it{ begin() };
			
			while (it != end())
			{
				if (it._currentNode->Data != value)
				{
					++it;
				}
				else
				{
					return it;
				}
			}
		}

		return iterator{end()};
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::InsertAfter(const T& value, const T& targetValue)
	{
		iterator targetNode = Find(targetValue);

		if (targetNode == end())
		{
			PushBack(value);
			++_size;
			return iterator{ *this, _back };
		}

		targetNode._currentNode->Next = new Node{ value, targetNode._currentNode->Next };
		++_size;

		return ++targetNode;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::Erase(const T& value)
	{
		iterator targetNode = Find(value);

		if (targetNode._currentNode == nullptr)
			return iterator{};

		if (targetNode._currentNode == _front)
		{
			PopFront();
			return iterator{ *this,_front };
		}
		if (targetNode._currentNode == _back)
		{
			PopBack();
			return iterator{ *this,_back };
		}

		iterator previousNode = begin();
		while (previousNode._currentNode->Next != targetNode._currentNode)
		{
			++previousNode;
		}
		previousNode._currentNode->Next = targetNode._currentNode->Next;
		delete targetNode._currentNode;
		--_size;
		return ++previousNode;
	}

	template<typename T>
	inline typename SList<T>::iterator SList<T>::Erase(iterator targetNode)
	{
		if (targetNode._currentNode == nullptr)
			return iterator{};

		if (targetNode._currentNode == _front)
		{
			PopFront();
			return iterator{ *this,_front };
		}
		if (targetNode._currentNode == _back)
		{
			PopBack();
			return iterator{ *this,_back };
		}

		iterator previousNode = begin();
		while (previousNode._currentNode->Next != targetNode._currentNode)
		{
			++previousNode;
		}
		previousNode._currentNode->Next = targetNode._currentNode->Next;
		delete targetNode._currentNode;
		--_size;
		return ++previousNode;
	}

#pragma endregion SListIterator

#pragma endregion SList

}