#pragma once
#include "Datum.h"

namespace FieaGameEngine
{
	inline void Datum::rawPush(const void* value)
	{
		if (_size == _capacity)
			reserve(_capacity + 1);

		std::byte* dataPointer = reinterpret_cast<std::byte*>(_data.vp);
		memcpy((dataPointer + _size * _dataSize), value, _dataSize);
		++_size;
	}

	inline void Datum::shiftRemainder(std::size_t i)
	{
		std::size_t index = i;
		if (index != _size - 1)
		{
			std::byte* dataPointer = reinterpret_cast<std::byte*>(_data.vp);
			std::memmove((dataPointer + index * _dataSize), (dataPointer + (index + 1) * _dataSize), (_size - index - 1) * _dataSize);
		}

	}

	inline Datum::Datum()
	{
	}

	inline Datum::Datum(DatumTypes type,size_t capacity)
	{
		if (type == DatumTypes::Unknown)
			throw std::runtime_error("Type cannot be unknown!");

		SetType(type);

		reserve(capacity);
	}

	inline Datum::Datum(std::initializer_list<int> list)
	{
		SetType(DatumTypes::Integer);
		reserve(list.size());
		for (const auto& value : list)
			pushBack(value);
	}

	inline Datum::Datum(std::initializer_list<float> list)
	{
		SetType(DatumTypes::Float);
		reserve(list.size());
		for (const auto& value : list)
			pushBack(value);
	}

	inline Datum::Datum(std::initializer_list<glm::vec4> list)
	{
		SetType(DatumTypes::Vector);
		reserve(list.size());
		for (const auto& value : list)
			pushBack(value);
	}

	inline Datum::Datum(std::initializer_list<glm::mat4> list)
	{
		SetType(DatumTypes::Matrix);
		reserve(list.size());
		for (const auto& value : list)
			pushBack(value);
	}

	inline Datum::Datum(std::initializer_list<std::string> list)
	{
		SetType(DatumTypes::String);
		reserve(list.size());
		for (const auto& value : list)
			pushBack(value);
	}

	inline Datum::Datum(std::initializer_list<RTTI*> list)
	{
		SetType(DatumTypes::Pointer);
		reserve(list.size());
		for (const auto& value : list)
			pushBack(value);
	}

	inline Datum::Datum(const Datum& value)
	{
		if (value._isExternal == true)
		{
			_type = value._type;
			_data = value._data;
			_size = value._size;
			_capacity = value._capacity;
			_isExternal = true;
		}

		else
		{
			SetType(value._type);
			reserve(value._size);
			if (_type != DatumTypes::String)
			{
				memcpy(_data.vp, value._data.vp, (_dataSize * value._size));
				_size = value._size;
			}

			else
			{
				for (std::size_t i = 0; i < value._size; ++i)
				{
					pushBack(value._data.s[i]);
				}
			}
		}
	}

	inline Datum::Datum(Datum&& value) noexcept
	{
		SetType(value._type);
		_data.vp = value._data.vp;
		_size = value._size;
		_capacity = value._capacity;
		_isExternal = value._isExternal;

		value._data.vp = nullptr;
		value._size = 0;
		value._capacity = 0;
	}

	inline Datum::~Datum()
	{
		if(_isExternal == false)
		{
			if (_type == DatumTypes::String)
			{
				std::size_t i = 0;
				while (i < _size)
				{
					_data.s[i].~basic_string();
					++i;
				}
			}

			free(_data.vp);
		}
	}

	inline bool Datum::operator!=(const int rhs) const //should you return true if size > 1 ?
	{
		if (_type != DatumTypes::Integer)
			return true;

		if (_size > 1)
			return true;

		if (_data.i[0] != rhs)
			return true;

		return false;
	}

	inline bool Datum::operator!=(const float rhs) const
	{
		if (_type != DatumTypes::Float)
			return true;

		if (_size > 1)
			return true;

		if (_data.f[0] != rhs)
			return true;

		return false;
	}

	inline bool Datum::operator!=(const glm::vec4& rhs) const
	{
		if (_type != DatumTypes::Vector)
			return true;

		if (_size > 1)
			return true;

		if (_data.v[0] != rhs)
			return true;

		return false;
	}

	inline bool Datum::operator!=(const glm::mat4& rhs) const
	{
		if (_type != DatumTypes::Matrix)
			return true;

		if (_size > 1)
			return true;

		if (_data.m[0] != rhs)
			return true;

		return false;
	}

	inline bool Datum::operator!=(const std::string& rhs) const
	{
		if (_type != DatumTypes::String)
			return true;

		if (_size > 1)
			return true;

		int res = _data.s[0].compare(rhs);
		if (res != 0)
			return true;

		return false;
	}

	inline bool Datum::operator!=(RTTI* const& rhs) const
	{
		if (_type != DatumTypes::Pointer)
			return true;

		if (_size > 1)
			return true;

		if (_data.p[0]->Equals(rhs))
			return false;

		return true;
	}

	inline bool Datum::operator==(const Datum& rhs) const
	{
		return !(operator != (rhs));
	}

	inline bool Datum::operator==(const int rhs) const
	{
		return !(operator != (rhs));
	}

	inline bool Datum::operator==(const float rhs) const
	{
		return !(operator != (rhs));
	}

	inline bool Datum::operator==(const glm::vec4& rhs) const
	{
		return !(operator != (rhs));
	}

	inline bool Datum::operator==(const glm::mat4& rhs) const
	{
		return !(operator != (rhs));
	}

	inline bool Datum::operator==(const std::string& rhs) const
	{
		return !(operator != (rhs));
	}

	inline bool Datum::operator==(RTTI* const& rhs) const
	{
		return !(operator != (rhs));
	}

	inline size_t Datum::Size() const
	{
		return _size;
	}

	inline Datum::DatumTypes Datum::Type() const
	{
		return _type;
	}

	inline void Datum::SetType(DatumTypes type)
	{
		if (_type == type)
			return;
		if (_type != DatumTypes::Unknown)
			throw std::runtime_error("Cannot change type after being set!");

		_type = type;
		_dataSize = _sizeTable[static_cast<int>(_type)];
	}

	inline std::size_t Datum::getDataSize() const
	{
		return _dataSize;
	}

	inline std::size_t Datum::getCapacity() const
	{
		return _capacity;
	}

	inline void Datum::setCapacity(std::size_t capacity)
	{
		if (_isExternal == true)
			throw std::runtime_error("Cannot modify external memory!");
		if (_type == DatumTypes::Unknown)
			throw std::runtime_error("Cannot allocate storage without knowing type of data you want to store!!");

		reserve(capacity);
	}

	inline void Datum::shrinkToFit()
	{
		if (_isExternal == true)
			throw std::runtime_error("Cannot modify external memory!");

		if (_capacity == _size)
			return;

		void* data = realloc(_data.vp, _size * _dataSize);
		assert(data != nullptr);
		_data.vp = data;
		_capacity = _size;
	}

	inline void Datum::clear()
	{
		if (_isExternal == true)
		{
			_data.vp = nullptr;
			_size = 0;
			_capacity = 0;
			_type = DatumTypes::Unknown;
			return;
		}
			//throw std::runtime_error("Cannot modify external memory!"); //this is messing up the external storage if clear is called against a datum inside scope. should this just set _data to nullptr?

		if (_type == DatumTypes::String)
		{
			while (_size != 0)
				popBack();

			return;
		}

		_size = 0;
	}

	inline void Datum::setStorage(int* iArray, std::size_t size)
	{
		if (_capacity != 0)
			throw std::runtime_error("Already internal Storage");

		_isExternal = true;

		SetType(DatumTypes::Integer);
		_data.i = iArray;
		_size = size;
		_capacity = _size;
	}

	inline void Datum::setStorage(float* fArray, std::size_t size)
	{
		if (_capacity != 0)
			throw std::runtime_error("Already internal Storage");

		_isExternal = true;

		SetType(DatumTypes::Float);
		_data.f = fArray;
		_size = size;
		_capacity = _size;
	}

	inline void Datum::setStorage(std::string* sArray, std::size_t size)
	{
		if (_capacity != 0)
			throw std::runtime_error("Already internal Storage");

		_isExternal = true;

		SetType(DatumTypes::String);
		_data.s = sArray;
		_size = size;
		_capacity = _size;
	}

	inline void Datum::setStorage(glm::vec4* vArray, std::size_t size)
	{
		if (_capacity != 0)
			throw std::runtime_error("Already internal Storage");

		_isExternal = true;

		SetType(DatumTypes::Vector);
		_data.v = vArray;
		_size = size;
		_capacity = _size;
	}

	inline void Datum::setStorage(glm::mat4* mArray, std::size_t size)
	{
		if (_capacity != 0)
			throw std::runtime_error("Already internal Storage");

		_isExternal = true;

		SetType(DatumTypes::Matrix);
		_data.m = mArray;
		_size = size;
		_capacity = _size;
	}

	inline void Datum::setStorage(RTTI** pArray, std::size_t size)
	{
		if (_capacity != 0)
			throw std::runtime_error("Already internal Storage");

		_isExternal = true;

		SetType(DatumTypes::Pointer);
		_data.p = pArray;
		_size = size;
		_capacity = _size;
	}

	template<typename ...Args>
	inline bool Datum::Remove(const Args&... args)
	{
		std::size_t index = Find(args...);
		if (index == _size)
			return false;

		if(_type == DatumTypes::String)
			_data.s[index].~basic_string();

		shiftRemainder(index);
		--_size;
		return true;
	}

	inline bool Datum::RemoveAt(std::size_t index)
	{
		if (index >= _size)
			return false;

		if (_type == DatumTypes::String)
			_data.s[index].~basic_string();

		shiftRemainder(index);
		--_size;
		return true;
	}

	inline std::size_t Datum::Find(const int value) const
	{
		if (_type != DatumTypes::Integer)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (_data.i[index] == value)
				return static_cast<int>(index);

			++index;
		}

		return _size;
	}

	inline std::size_t Datum::Find(const float value) const
	{
		if (_type != DatumTypes::Float)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (_data.f[index] == value)
				return index;

			++index;
		}

		return _size;
	}

	inline std::size_t Datum::Find(const glm::vec4& value) const
	{
		if (_type != DatumTypes::Vector)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (_data.v[index] == value)
				return index;

			++index;
		}

		return _size;
	}

	inline std::size_t Datum::Find(const glm::mat4& value) const
	{
		if (_type != DatumTypes::Matrix)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (_data.m[index] == value)
				return index;

			++index;
		}

		return _size;
	}

	inline std::size_t Datum::Find(const std::string& value) const
	{
		if (_type != DatumTypes::String)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (_data.s[index] == value)
				return index;

			++index;
		}

		return _size;
	}

	inline std::size_t Datum::Find(const RTTI* value) const
	{
		if (_type != DatumTypes::Pointer)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (value->Equals(_data.p[index]))
				return index;

			++index;
		}

		return _size;
	}

	inline std::size_t Datum::Find(const Scope* value) const
	{
		if (_type != DatumTypes::Table)
			throw std::runtime_error("Type mismatch >: (");

		std::size_t index = 0;
		while (index < _size)
		{
			if (_data.t[index] == value)
				return index;

			++index;
		}

		return _size;
	}

	inline void Datum::Set(const int value, std::size_t index)
	{
		if (index >= _capacity)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Integer)
			throw std::runtime_error("Wrong type!");

		_data.i[index] = value;
	}

	inline void Datum::Set(const float value, std::size_t index)
	{
		if (index >= _capacity)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Float)
			throw std::runtime_error("Wrong type!");

		_data.f[index] = value;
	}

	inline void Datum::Set(const glm::vec4& value, std::size_t index)
	{
		if (index >= _capacity)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Vector)
			throw std::runtime_error("Wrong type!");

		_data.v[index] = value;
	}

	inline void Datum::Set(const glm::mat4& value, std::size_t index)
	{
		if (index >= _capacity)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Matrix)
			throw std::runtime_error("Wrong type!");

		_data.m[index] = value;
	}

	inline void Datum::Set(const std::string& value, std::size_t index)
	{
		if (index >= _capacity)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::String)
			throw std::runtime_error("Wrong type!");

		_data.s[index] = value;
	}

	inline void Datum::Set( RTTI* const value, std::size_t index)
	{
		if (index >= _capacity)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Pointer)
			throw std::runtime_error("Wrong type!");

		_data.p[index] = value;
	}

	template<>
	inline int& Datum::Get<int>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Integer)
			throw std::runtime_error("Wrong type!");

		return _data.i[index];
	}

	template<>
	inline float& Datum::Get<float>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Float)
			throw std::runtime_error("Wrong type!");

		return _data.f[index];
	}

	template<>
	inline glm::vec4& Datum::Get<glm::vec4>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Vector)
			throw std::runtime_error("Wrong type!");

		return _data.v[index];
	}

	template<>
	inline glm::mat4& Datum::Get<glm::mat4>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Matrix)
			throw std::runtime_error("Wrong type!");

		return _data.m[index];
	}

	template<>
	inline std::string& Datum::Get<std::string>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::String)
			throw std::runtime_error("Wrong type!");

		return _data.s[index];
	}

	template<>
	inline RTTI*& Datum::Get<RTTI*>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Pointer)
			throw std::runtime_error("Wrong type!");

		return _data.p[index];
	}

	template<>
	inline Scope*& Datum::Get<Scope*>(std::size_t index)
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Table)
			throw std::runtime_error("Wrong type!");

		return _data.t[index];
	}

	template<>
	inline const int& Datum::Get<int>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Integer)
			throw std::runtime_error("Wrong type!");

		return _data.i[index];
	}

	template<>
	inline const float& Datum::Get<float>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Float)
			throw std::runtime_error("Wrong type!");

		return _data.f[index];
	}

	template<>
	inline const glm::vec4& Datum::Get<glm::vec4>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Vector)
			throw std::runtime_error("Wrong type!");

		return _data.v[index];
	}

	template<>
	inline const glm::mat4& Datum::Get<glm::mat4>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Matrix)
			throw std::runtime_error("Wrong type!");

		return _data.m[index];
	}

	template<>
	inline const std::string& Datum::Get<std::string>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::String)
			throw std::runtime_error("Wrong type!");

		return _data.s[index];
	}

	template<>
	inline  RTTI* const & Datum::Get<RTTI*>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Pointer)
			throw std::runtime_error("Wrong type!");

		return _data.p[index];
	}

	template<>
	inline Scope* const & Datum::Get<Scope*>(std::size_t index) const
	{
		if (index >= _size)
			throw std::out_of_range("index out of bounds!");

		if (_type != DatumTypes::Pointer)
			throw std::runtime_error("Wrong type!");

		return _data.t[index];
	}

	inline void Datum::pushBack(int value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::Integer)
			throw std::runtime_error("Wrong type object >:(");

		rawPush(&value);
	}

	inline void Datum::pushBack(float value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::Float)
			throw std::runtime_error("Wrong type object >:(");

		rawPush(&value);
	}

	inline void Datum::pushBack(const glm::vec4& value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::Vector)
			throw std::runtime_error("Wrong type object >:(");

		rawPush(&value);
	}

	inline void Datum::pushBack(const glm::mat4& value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::Matrix)
			throw std::runtime_error("Wrong type object >:(");

		rawPush(&value);
	}

	inline void Datum::pushBack(const std::string& value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::String)
			throw std::runtime_error("Wrong type object >:(");

		if (_size == _capacity)
			reserve(_capacity + 1);

		new(_data.s + _size) std::string{ value };
		++_size;
	}

	inline void Datum::pushBack(const RTTI* value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::Pointer)
			throw std::runtime_error("Wrong type object >:(");

		rawPush(&value);
	}

	inline void Datum::pushBack(const Scope* value)
	{
		if (_isExternal)
			throw std::runtime_error("Cannot push to external memory :(");

		if (_type != DatumTypes::Table)
			throw std::runtime_error("Wrong type object >:(");

		rawPush(&value);
	}

	inline void Datum::popBack()
	{
		if (_size == 0)
			throw std::runtime_error("There is nothing to delete bruh.");

		if (_type == DatumTypes::String)
		{
			std::size_t i = _size - 1;
				_data.s[i].~basic_string();
		}

		--_size;
	}

	inline int& Datum::frontAsInt()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return *_data.i;
	}

	inline float& Datum::frontAsFloat()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return *_data.f;
	}

	inline glm::vec4& Datum::frontAsVector()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return *_data.v;
	}

	inline glm::mat4& Datum::frontAsMatrix()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return *_data.m;
	}

	inline std::string& Datum::frontAsString()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return *_data.s;
	}

	inline RTTI* Datum::frontAsPointer()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return *_data.p;
	}

	inline int& Datum::backAsInt()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return _data.i[_size - 1];
	}

	inline float& Datum::backAsFloat()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return _data.f[_size - 1];
	}

	inline glm::vec4& Datum::backAsVector()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return _data.v[_size - 1];
	}

	inline glm::mat4& Datum::backAsMatrix()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return _data.m[_size - 1];
	}

	inline std::string& Datum::backAsString()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return _data.s[_size - 1];
	}

	inline RTTI* Datum::backAsPointer()
	{
		if (_size == 0)
			throw std::runtime_error("Datum is empty :(");

		return _data.p[_size - 1];
	}

	inline bool Datum::IsExternal()
	{
		return _isExternal;
	}

	inline void Datum::setStorage(DatumTypes type, std::size_t size, void* dataPtr)
	{
		_isExternal = true;
		_type = type;
		_size = size;
		_capacity = size;
		_data.vp = dataPtr;
	}

	inline void Datum::UpdateExtStoragePointer(std::byte* PreviousThis, void* newThis)
	{
		size_t offset = _data.b - PreviousThis;
		_data.vp = newThis;
		_data.b += offset;
	}

}



/*template <typename T>
	inline T& Datum::Get(std::size_t index)
	{

	}*/

//void* valuePointer = &value;

/*std::byte* dataPointer = reinterpret_cast<std::byte*>(_data.vp);
memcpy((dataPointer + _size * _dataSize), &value, _dataSize);
++_size;*/


/*_sizeTable.emplace(DatumTypes::Integer, sizeof(int));
_sizeTable.emplace(DatumTypes::Integer, sizeof(int));
_sizeTable.emplace(DatumTypes::Integer, sizeof(int));
_sizeTable.emplace(DatumTypes::Integer, sizeof(int));
_sizeTable.emplace(DatumTypes::Integer, sizeof(int));*/


/*inline bool Datum::Remove(const int value)
{
	int index = Find(value);
	if (index == -1)
		return false;

	shiftRemainder(index);
	--_size;
	return true;
}

inline bool Datum::Remove(const float value)
{
	int index = Find(value);
	if (index == -1)
		return false;

	shiftRemainder(index);
	--_size;
	return true;
}*/

/*template<typename T>
	inline const T& Datum::Get(std::size_t index) const
	{

	}*/

	/*template<>
	inline Datum::Get(std::size_t index)
	{
	}*/


	/*inline void Datum::initializeSizeTable()
	{
		const std::pair<DatumTypes, size_t> Integer{ DatumTypes::Integer, sizeof(int) };
		const std::pair<DatumTypes, size_t> Float{ DatumTypes::Float, sizeof(float) };
		const std::pair<DatumTypes, size_t> Pointer{ DatumTypes::Pointer, sizeof(RTTI*) };
		const std::pair<DatumTypes, size_t> Vector{ DatumTypes::Vector, sizeof(glm::vec4) };
		const std::pair<DatumTypes, size_t> Matrix{ DatumTypes::Matrix, sizeof(glm::mat4) };
		const std::pair<DatumTypes, size_t> String{ DatumTypes::String, sizeof(std::string) };

		_sizeTable.insert(Integer);
		_sizeTable.insert(Float);
		_sizeTable.insert(Pointer);
		_sizeTable.insert(Vector);
		_sizeTable.insert(Matrix);
		_sizeTable.insert(String);

	}*/