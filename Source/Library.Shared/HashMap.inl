#pragma once

#include "HashMap.h"

namespace FieaGameEngine
{

#pragma region HashMapIterator

	template <typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::iterator::iterator(HashMap& container, size_t chainIndex, const typename chain_type::iterator& currentPair) :
		_container{ &container }, _chainIndex{ chainIndex }, _currentPair{ currentPair }
	{
	}

	template <typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::iterator::iterator(const iterator& rhs) :
		_container{ rhs._container }, _currentPair{ rhs._currentPair }, _chainIndex{ rhs._chainIndex }
	{
	}

	template<typename TKey, typename TValue>
	inline std::pair<const TKey, TValue>* HashMap<TKey, TValue>::iterator::operator->() const
	{
		if (_container == nullptr)
			throw std::runtime_error("There is no HashMap to dereference!");

		/*if (_container->_baseVector.capacity() == 0)
			throw std::runtime_error("There is no base Vector to dereference!");*/

		if (_chainIndex == _container->_chainCapacity)
			throw std::runtime_error("Cannot dereference end of map.");
		return &(*_currentPair); // no checks as checks implemented in slist iterator
	}

	template <typename TKey, typename TValue>
	inline std::pair<const TKey, TValue>& HashMap<TKey, TValue>::iterator::operator* () const
	{
		if (_container == nullptr)
			throw std::runtime_error("There is no HashMap to dereference!");

		/*if (_container->_baseVector.capacity() == 0)
			throw std::runtime_error("There is no base Vector to dereference!");*/

		if (_chainIndex == _container->_chainCapacity)
			throw std::runtime_error("Cannot dereference end of map.");
		return (*_currentPair); // no checks as checks implemented in slist iterator
	}

	template <typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::iterator::operator= (const iterator& rhs)
	{
		_container = rhs._container;
		_chainIndex = rhs._chainIndex;
		_currentPair = rhs._currentPair;
	}

	template <typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::iterator& HashMap<TKey, TValue>::iterator::operator++ ()
	{
		if (_chainIndex == _container->_chainCapacity)
			throw std::runtime_error("Cannot Increment, already at end! :'(");

		++_currentPair;
		while (_currentPair == _container->_baseVector[_chainIndex].end())
		{
			++_chainIndex;
			if (_chainIndex == _container->_chainCapacity)
			{
				_currentPair = typename chain_type::iterator{};
				break;
			}

			_currentPair = _container->_baseVector[_chainIndex].begin();
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::iterator HashMap<TKey, TValue>::iterator::operator++(int)
	{
		iterator temp = *this;
		this->operator++();
		return temp;
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::iterator::operator==(const iterator& rhs) const
	{
		if (_container == rhs._container)
		{
			if (_chainIndex == rhs._chainIndex)
			{
				if (_currentPair == rhs._currentPair)
					return true;
			}
		}

		return false;
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::iterator::operator!=(const iterator& rhs) const
	{
		if (*this == rhs)
			return false;

		return true;
	}

#pragma endregion HashMapIterator

#pragma region HashMapConstIterator

	template<typename TKey, typename TValue>
	inline HashMap<TKey,TValue>::const_iterator::const_iterator(const HashMap& container, size_t chainIndex, const typename chain_type::const_iterator& currentPair) :
		_container{ &container }, _chainIndex{ chainIndex }, _currentPair{ currentPair }
	{
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey,TValue>::const_iterator::const_iterator(const const_iterator& rhs) :
		_container{ rhs._container }, _currentPair{ rhs._currentPair }, _chainIndex{ rhs._chainIndex }
	{
	}

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::const_iterator::const_iterator(const iterator& rhs) :
		_container{ rhs._container }, _currentPair{ rhs._currentPair }, _chainIndex{ rhs._chainIndex }
	{
	}

	template<typename TKey, typename TValue>
	inline const std::pair<const TKey, TValue>& HashMap<TKey, TValue>::const_iterator::operator* () const
	{
		if (_container == nullptr)
			throw std::runtime_error("There is no HashMap to dereference!");

		/*if (_container->_baseVector.capacity() == 0)
			throw std::runtime_error("There is no base Vector to dereference!");*/

		if (_chainIndex == _container->_chainCapacity)
			throw std::runtime_error("Cannot dereference end of map.");
		return (*_currentPair);
	}

	template<typename TKey, typename TValue>
	inline const std::pair<const TKey, TValue>* HashMap<TKey, TValue>::const_iterator::operator-> () const
	{
		if (_container == nullptr)
			throw std::runtime_error("There is no HashMap to dereference!");

		/*if (_container->_baseVector.capacity() == 0)
			throw std::runtime_error("There is no base Vector to dereference!");*/

		if (_chainIndex == _container->_chainCapacity)
			throw std::runtime_error("Cannot dereference end of map.");
		return &(*_currentPair);
	}

	template <typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::const_iterator::operator= (const const_iterator& rhs)
	{
		_container = rhs._container;
		_chainIndex = rhs._chainIndex;
		_currentPair = rhs._currentPair;
	}

	template <typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::const_iterator& HashMap<TKey, TValue>::const_iterator::operator++ ()
	{
		if (_chainIndex == _container->_chainCapacity)
			throw std::runtime_error("Cannot Increment, already at end! :'(");

		++_currentPair;
		while (_currentPair == _container->_baseVector[_chainIndex].end())
		{
			++_chainIndex;
			if (_chainIndex == _container->_chainCapacity)
			{
				_currentPair = typename chain_type::const_iterator{};
				break;
			}

			_currentPair = _container->_baseVector[_chainIndex].begin();
		}

		return *this;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::const_iterator HashMap<TKey, TValue>::const_iterator::operator++(int)
	{
		const_iterator temp = *this;
		this->operator++();
		return temp;
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::const_iterator::operator==(const const_iterator& rhs) const
	{
		if (_container == rhs._container)
		{
			if (_chainIndex == rhs._chainIndex)
			{
				if (_currentPair == rhs._currentPair)
					return true;
			}
		}

		return false;
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::const_iterator::operator!=(const const_iterator& rhs) const
	{
		if (*this == rhs)
			return false;

		return true;
	}

#pragma endregion HashMapConstIterator

#pragma region HashMapMain

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(size_t chainCapacity, std::function<size_t(const TKey&)> userHashFn) :
		_baseVector{ chainCapacity },_lowestActiveIndex{ chainCapacity }, _chainCapacity{chainCapacity}, HashFn{ userHashFn }
	{
		size_t i = 0;
		while (i < chainCapacity)
		{
			_baseVector.push_back(SList<pair_type>{}); //went into Slist Destructor?? probably destructor of local slist that was created
			++i;
		}
	}

	/*template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(HashMap&& rhs) :
		_baseVector{std::move(rhs._baseVector)}, _population{rhs._population}, _chainCapacity{rhs._chainCapacity}, _lowestActiveIndex{rhs._lowestActiveIndex}, HashFn{rhs.HashFn}
	{
		
	}*/

	template<typename TKey, typename TValue>
	inline HashMap<TKey, TValue>::HashMap(std::initializer_list<pair_type> list) :
		HashMap(list.size() * 3)
	{
		for (auto& entry : list)
			insert(entry);
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::operator[](const TKey& key)
	{
		iterator it = Find(key);
		if (it == end())
			it = (insert(std::pair<TKey, TValue>{key, TValue{}})).first;

		return it->second;
	}

	template<typename TKey, typename TValue>
	inline const TValue& HashMap<TKey, TValue>::operator[](const TKey& key) const
	{
		const_iterator it = Find(key);
		
		return it->second;
	}

	template<typename TKey, typename TValue>
	inline std::pair<typename HashMap<TKey,TValue>::iterator, bool> HashMap<TKey, TValue>::CreateAt(const TKey& key)
	{
		bool isFresh = true;
		iterator it = Find(key);
		if (it == end())
			it = (insert(std::pair<TKey, TValue>{key, TValue{}})).first;
		else
			isFresh = false;

		return std::make_pair(it, isFresh);
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey,TValue>::iterator HashMap<TKey, TValue>::begin()
	{
		if(_lowestActiveIndex == _chainCapacity)
			return iterator{ *this, _lowestActiveIndex};

		return iterator{*this, _lowestActiveIndex, _baseVector[_lowestActiveIndex].begin()};
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::const_iterator HashMap<TKey, TValue>::cbegin() const
	{
		if (_lowestActiveIndex == _chainCapacity)
			return const_iterator{ *this, _lowestActiveIndex };

		return const_iterator{ *this, _lowestActiveIndex, _baseVector[_lowestActiveIndex].cbegin() };
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::iterator HashMap<TKey, TValue>::end()
	{
		return iterator{ *this, _chainCapacity };
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::const_iterator HashMap<TKey, TValue>::cend() const
	{
		return const_iterator{ *this, _chainCapacity };
	}

	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::hashFn(const TKey& key) const
	{
		return HashFn(key) % _chainCapacity;
	}

	template<typename TKey, typename TValue>
	inline std::pair<typename HashMap<TKey, TValue>::iterator, bool> HashMap<TKey, TValue>::insert(const pair_type& pair)
	{
		HashMap<TKey,TValue>::iterator temp = Find(pair.first);
		if (temp != end())
			return std::make_pair(temp,false);

		size_t currentIndex = hashFn(pair.first);

		typename chain_type::iterator insertIT = _baseVector[currentIndex].PushBack(pair);
		++_population;

		if (currentIndex < _lowestActiveIndex)
			_lowestActiveIndex = currentIndex;

		return std::make_pair(iterator{ *this,currentIndex,insertIT }, true);
	}

	template<typename TKey, typename TValue>
	inline std::pair<typename HashMap<TKey, TValue>::iterator, bool> HashMap<TKey, TValue>::insert(pair_type&& pair)
	{
		HashMap<TKey, TValue>::iterator temp = Find(pair.first);
		if (temp != end())
			return std::make_pair(temp, false);

		size_t currentIndex = hashFn(pair.first);
		typename chain_type::iterator insertIT = _baseVector[currentIndex].PushBack(std::move(pair));

		++_population;

		if (currentIndex < _lowestActiveIndex)
			_lowestActiveIndex = currentIndex;

		return std::make_pair(iterator{ *this,currentIndex,insertIT }, true);
	}

	template<typename TKey, typename TValue>
	inline size_t HashMap<TKey, TValue>::Size()
	{
		return _population;
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey, TValue>::iterator HashMap<TKey, TValue>::Find(const TKey& key)
	{
		if (_chainCapacity == 0)
			throw std::runtime_error("Your map doesnt have a vector!");

		size_t chainIndex = hashFn(key);

		typename chain_type::iterator chainIterator = _baseVector[chainIndex].begin();
		typename chain_type::iterator chainEnd = _baseVector[chainIndex].end();


		while ((chainIterator != chainEnd) && ((*chainIterator).first != key))
			++chainIterator; // needs to be tested for key with same hash

		if (chainIterator == chainEnd)
			return end();

		return iterator{*this, chainIndex, chainIterator};
	}

	template<typename TKey, typename TValue>
	inline typename HashMap<TKey,TValue>::const_iterator HashMap<TKey, TValue>::Find(const TKey& key) const
	{
		if (_baseVector.capacity() == 0)
			throw std::runtime_error("Your map doesnt have a vector!");

		size_t chainIndex = hashFn(key);

		typename chain_type::const_iterator chainIterator = _baseVector[chainIndex].cbegin();
		typename chain_type::const_iterator chainEnd = _baseVector[chainIndex].cend();


		while ((chainIterator != chainEnd) && ((*chainIterator).first != key))
			++chainIterator; // needs to be tested for key with same hash

		if (chainIterator == chainEnd)
			return cend();

		return const_iterator{ *this, chainIndex, chainIterator };
	}

	template<typename TKey, typename TValue>
	inline bool HashMap<TKey, TValue>::containsKey(const TKey& key) const
	{
		if (Find(key) != cend())
			return true;
		
		return false;
	}

	template<typename TKey, typename TValue>
	inline TValue& HashMap<TKey, TValue>::At(const TKey& key)
	{
		iterator it = Find(key);
		if (it == end())
			throw std::runtime_error("Key not found in map. Sad.");

		return it->second;
	}

	template<typename TKey, typename TValue>
	inline const TValue& HashMap<TKey, TValue>::At(const TKey& key) const
	{
		const_iterator it = Find(key);
		if (it == cend())
			throw std::runtime_error("Key not found in map. Sad.");

		return it->second;
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Clear()
	{
		for (size_t i = 0; i < _baseVector.size(); ++i)
		{
			_baseVector[i].clear();
		}
		_population = 0;
		_lowestActiveIndex = _chainCapacity;
		//_baseVector.clear();
		//_chainCapacity = 0;
	}

	template<typename TKey, typename TValue>
	inline void HashMap<TKey, TValue>::Remove(const TKey& key)
	{
		iterator it = Find(key);
		if (it == end())
			return;

		_baseVector[it._chainIndex].Erase(it._currentPair);
		--_population;

		if (_population == 0)
		{
			_lowestActiveIndex = _chainCapacity;
			return;
		}

		size_t index = 0;
		while ((_baseVector[index].Size() == 0) && (index != _chainCapacity))
			++index;

		_lowestActiveIndex = index;
	}



#pragma endregion HashMapMain
}