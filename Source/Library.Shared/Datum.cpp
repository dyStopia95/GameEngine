#include "pch.h"
#include "Datum.h"
#include "Scope.h"

namespace FieaGameEngine
{
	inline void FieaGameEngine::Datum::Set(Scope* const value, std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Table)
			throw std::runtime_error("Wrong type!");

		_data.t[index] = value;
	}

	void Datum::reserve(std::size_t capacity)
	{
		if (capacity > _capacity)
		{
			void* data = realloc(_data.vp, capacity * _dataSize);
			assert(data != nullptr);
			_data.vp = data;
			_capacity = capacity;
		}
	}

	Datum& Datum::operator=(const Datum& rhs)
	{
		/*if (_isExternal == true)
		{
			memcpy(_data.vp, rhs._data.vp, (_dataSize * rhs._size));
		}*/
		if (_isExternal == true)
		{
			assert(_type == rhs._type);
			if (_type == DatumTypes::String)
			{
				for (std::size_t i = 0; i < rhs._size; ++i)
				{
					Set(rhs._data.s[i], i);
				}
			}

			else if(_type == DatumTypes::Integer)
			{
				for (std::size_t i = 0; i < rhs._size; ++i)
				{
					Set(rhs._data.i[i], i);
				}
			}

		}

		else if(rhs._isExternal == false)
		{
			
			if (_capacity != 0)
			{
				//if (_type != rhs._type)
				resize(rhs._size);
			}

			else if (_type == DatumTypes::Unknown)
			{
				SetType(rhs._type);
				reserve(rhs._size);
			}

			if (_type != DatumTypes::String)
			{
				memcpy(_data.vp, rhs._data.vp, (_dataSize * rhs._size));
				_size = rhs._size;
			}

			else
			{
				clear();
				for (std::size_t i = 0; i < rhs._size; ++i)
				{
					pushBack(rhs._data.s[i]);
				}

			}
			
		}



		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (_capacity != 0)
		{
			clear();
			_type = rhs._type; //is this ok??
			_dataSize = _sizeTable[static_cast<int>(_type)];
			free(_data.vp);
		}

		_isExternal = rhs._isExternal;
		_data.vp = rhs._data.vp;
		_capacity = rhs._capacity;
		_size = rhs._size;
		shrinkToFit();

		rhs._data.vp = nullptr;
		rhs._size = 0;
		rhs._capacity = 0;

		return *this;
	}

	Scope* Datum::operator[](std::size_t index)
	{
		return (Get<Scope*>(index));
	}

	const Scope* Datum::operator[](std::size_t index) const
	{
		return Get<Scope*>(index);
	}

	bool Datum::operator!=(const Datum& rhs) const
	{
		if ((rhs._type != _type) || (rhs._size != _size))
			return true;

		if ((_type != DatumTypes::String) && (_type != DatumTypes::Pointer) && (_type != DatumTypes::Table))
		{
			int result = std::memcmp(_data.vp, rhs._data.vp, (_dataSize * _size));;
			if (result != 0)
				return true;

			return false;
		}

		std::size_t i = 0;
		if (_type == DatumTypes::Pointer)
		{
			while (i < _size)
			{
				if (!(rhs._data.p[i]->Equals(_data.p[i])))
					return true;

				++i;
			}

			return false;
		}

		if (_type == DatumTypes::Table)
		{
			while (i < _size)
			{
				if (!(rhs._data.t[i]->Equals(_data.t[i])))
					return true;

				++i;
			}

			return false;
		}

		while (i < _size)
		{
			int res = rhs._data.s[i].compare(_data.s[i]);
			++i;

			if (res != 0)
				return true;
		}
		return false;
	}

	inline void Datum::resize(std::size_t size)
	{
		if (_isExternal == true)
			throw std::runtime_error("Cannot modify external memory!");

		if (size == _size)
			shrinkToFit();

		else if (size < _size)
		{
			if (_type == DatumTypes::String)
			{
				std::size_t i = size;
				while (i < _size)
				{
					_data.s[i].~basic_string();
					++i;
				}
			}

			void* data = realloc(_data.vp, size * _dataSize);
			assert(data != nullptr);
			_data.vp = data;
			_size = size;
			_capacity = _size;
		}

		else
		{
			reserve(size);

			std::byte* indexPtr = reinterpret_cast<std::byte*>(_data.vp);
			indexPtr += _size * _dataSize;
			std::size_t i = _size;

			switch (_type)
			{
			case DatumTypes::Integer:
			case DatumTypes::Float:
				memset(indexPtr, 0, (size - _size) * _dataSize); //working now was not working before
				break;
			case DatumTypes::String:
				while (i < size)
				{
					pushBack("");
					++i;
				}
				break;
			case DatumTypes::Vector:
				while (i < size)
				{
					pushBack(glm::vec4{ 0 });
					++i;
				}
				break;
			case DatumTypes::Matrix:
				while (i < size)
				{
					pushBack(glm::mat4{ 0 });
					++i;
				}
				break;
			case DatumTypes::Pointer:
				throw std::runtime_error("cannot default construct RTTI* objects :'(");
				break;
			default:
				break;
			}

			_size = size;
		}
	}


}