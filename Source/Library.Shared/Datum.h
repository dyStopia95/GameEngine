#pragma once
#include "glm/glm.hpp"
#include "RTTI.h"
#include "HashMap.h"
#include <map>
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
	class Scope;

	class Datum final
	{
	public:
		enum class DatumTypes
		{
			Unknown,
			Integer,
			Float,
			String,
			Vector,
			Matrix,
			Table,
			Pointer,

			End = Pointer
		};

	private:
		union DatumValues final
		{
			int* i;
			float* f;
			std::string* s;
			glm::vec4* v;
			glm::mat4* m;
			Scope** t;
			RTTI** p;
			void* vp;
			std::byte* b;
		};

		DatumValues _data{nullptr};
		std::size_t _size{ 0 };
		std::size_t _capacity{ 0 };
		DatumTypes _type{ DatumTypes::Unknown };
		bool _isExternal{ false };
		std::size_t _dataSize = 0;

		inline static size_t _sizeTable[static_cast<int>(DatumTypes::End) + 1] =
		{
			0,							//DatumTypes::Unknown,
			sizeof(int),				//DatumTypes::Integer,
			sizeof(float),				//DatumTypes::Float,
			sizeof(std::string),		//DatumTypes::String,
			sizeof(glm::vec4),			//DatumTypes::Vector,
			sizeof(glm::mat4),			//DatumTypes::Matrix,
			sizeof(Scope*),				//DatumTypes::Table,
			sizeof(RTTI*),				//DatumTypes::Pointer
		};

		/// <summary>
		/// reserve memory based on capacity desired using realloc
		/// </summary>
		/// <param name="capacity"></param>
		void reserve(std::size_t capacity);
		/// <summary>
		/// used for pushback of trivially copyable data using memcpy
		/// </summary>
		/// <param name="value">value to be pushed</param>
		void rawPush(const void* value);
		/// <summary>
		/// used by the remove functions to shift subsequent entries to fill up gap in array
		/// </summary>
		/// <param name="index">index that will be deleted</param>
		void shiftRemainder(std::size_t index);

	public:
		/// <summary>
		/// default constructor. initializes the size table
		/// </summary>
		Datum();
		/// <summary>
		/// constructor that takes a type and capacity 
		/// </summary>
		/// <param name="type"></param>
		/// <param name="capacity"></param>
		Datum(DatumTypes type, size_t capacity);
		
		/// <summary>
		/// constructs a datum of type int with an array of entries supplied
		/// </summary>
		/// <param name="list"></param>
		Datum(std::initializer_list<int> list);
		/// <summary>
		/// constructs a datum of type float with an array of entries supplied
		/// </summary>
		/// <param name="list"></param>
		Datum(std::initializer_list<float> list);
		/// <summary>
		/// constructs a datum of type vector with an array of entries supplied
		/// </summary>
		/// <param name="list"></param>
		Datum(std::initializer_list<glm::vec4> list);
		/// <summary>
		/// constructs a datum of type matrix with an array of entries supplied
		/// </summary>
		/// <param name="list"></param>
		Datum(std::initializer_list<glm::mat4> list);
		/// <summary>
		/// constructs a datum of type string with an array of entries supplied
		/// </summary>
		/// <param name="list"></param>
		Datum(std::initializer_list<std::string> list);
		/// <summary>
		/// constructs a datum of type RTTI pointer with an array of entries supplied
		/// </summary>
		/// <param name="list"></param>
		Datum(std::initializer_list<RTTI*> list);

		/// <summary>
		/// copy constructor of Datum. rejects if type is different. will clear existing values if any. will not keep excess capacity
		/// </summary>
		/// <param name="value"></param>
		Datum(const Datum& value);
		/// <summary>
		/// copy Assignment operator. Will reject if type is different. Will clear any existing entries. Will not keep excess capacity.
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Datum& operator= (const Datum& rhs);

		/// <summary>
		/// Move constructor. Will overwrite existing Datum irrespective of type. Will not keep excess capacity.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		Datum(Datum&& value) noexcept;
		/// <summary>
		/// Move assignment. Will overwrite Datum irrespective of type. Will not keep excess capacity.
		/// </summary>
		/// <param name="rhs"></param>
		/// <returns></returns>
		Datum& operator= (Datum&& rhs) noexcept;

		Scope* operator[](std::size_t index);
		const Scope* operator[](std::size_t index) const;

		~Datum();

		/// <summary>
		/// datum comparison operator. iterates through all elements of datum to check equality.
		/// </summary>
		/// <param name="rhs">datum to be compared against</param>
		/// <returns></returns>
		bool operator!= (const Datum& rhs) const;
		/// <summary>
		/// scalar comparison with int. returns true if size > 1.
		/// </summary>
		/// <param name="rhs">scalar int to be compared</param>
		/// <returns></returns>
		bool operator!= (const int rhs) const;
		/// <summary>
		/// scalar comparison with float. returns true if size > 1.
		/// </summary>
		/// <param name="rhs">scalar float to be compared</param>
		/// <returns></returns>
		bool operator!= (const float rhs) const;
		/// <summary>
		/// scalar comparison with vector. returns true if size > 1.
		/// </summary>
		/// <param name="rhs">vector to be compared</param>
		/// <returns></returns>
		bool operator!= (const glm::vec4& rhs) const;
		/// <summary>
		/// scalar comparison with matrix. returns true if size > 1.
		/// </summary>
		/// <param name="rhs">matrix to be compared</param>
		/// <returns></returns>
		bool operator!= (const glm::mat4& rhs) const;
		/// <summary>
		/// scalar comparison with string. returns true if size > 1.
		/// </summary>
		/// <param name="rhs">string to be compared</param>
		/// <returns></returns>
		bool operator!= (const std::string& rhs) const;
		/// <summary>
		/// scalar comparison with RTTI object. returns true if size > 1. uses the Equals operator of RTTI object
		/// </summary>
		/// <param name="rhs">pointer to RTTI object to be compared</param>
		/// <returns></returns>
		bool operator!= (RTTI* const & rhs) const;

		bool operator == (const Datum& rhs) const;
		bool operator == (const int rhs) const;
		bool operator == (const float rhs) const;
		bool operator == (const glm::vec4& rhs) const;
		bool operator == (const glm::mat4& rhs) const;
		bool operator == (const std::string& rhs) const;
		bool operator == (RTTI* const& rhs) const;

		/// <summary>
		/// returns currently set type of the Datum.
		/// </summary>
		/// <returns></returns>
		DatumTypes Type() const;
		/// <summary>
		/// returns current population of the Datum array.
		/// </summary>
		/// <returns></returns>
		size_t Size() const;
		/// <summary>
		/// Set type of the Datum. refuses to change type if type was already set before. ie type is not Unknown. Only overwritten by move operations
		/// </summary>
		/// <param name="type"></param>
		void SetType(DatumTypes type);
		/// <summary>
		/// get size of currently set type
		/// </summary>
		/// <returns></returns>
		std::size_t getDataSize() const;
		/// <summary>
		/// get current capacity
		/// </summary>
		/// <returns></returns>
		std::size_t getCapacity() const;
		/// <summary>
		/// set desired capacity using a reserve call
		/// </summary>
		/// <param name="capacity"></param>
		void setCapacity(std::size_t capacity);
		/// <summary>
		/// gets rid of excess capacity and sets it to size. uses realloc.
		/// </summary>
		void shrinkToFit();
		/// <summary>
		/// sets capacity to size. can grow the capacity, will lead to default construction of objects from previous size to new size.
		/// rejects if type is set to RTTI pointer as we cannot default construct RTTI objects. Can reduce current size if new size is less than current size.
		/// will lead to destroyed entries from previous size to new size.
		/// </summary>
		/// <param name="size"></param>
		void resize(std::size_t size);
		/// <summary>
		/// clears all entries. does not change capacity.
		/// </summary>
		void clear();

		/// <summary>
		/// sets storage to an external array of ints. size of the array should be specified correctly, and array should not be modified after passing to Datum
		/// </summary>
		/// <param name="iArray"></param>
		/// <param name="size"></param>
		void setStorage(int* iArray, std::size_t size);
		/// <summary>
		/// sets storage to an external array of floats. size of the array should be specified correctly, and array should not be modified after passing to Datum
		/// </summary>
		/// <param name="fArray"></param>
		/// <param name="size"></param>
		void setStorage(float* fArray, std::size_t size);
		/// <summary>
		/// sets storage to an external array of strings. size of the array should be specified correctly, and array should not be modified after passing to Datum
		/// </summary>
		/// <param name="sArray"></param>
		/// <param name="size"></param>
		void setStorage(std::string* sArray, std::size_t size);
		/// <summary>
		/// sets storage to an external array of vectors. size of the array should be specified correctly, and array should not be modified after passing to Datum
		/// </summary>
		/// <param name="vArray"></param>
		/// <param name="size"></param>
		void setStorage(glm::vec4* vArray, std::size_t size);
		/// <summary>
		/// sets storage to an external array of matrices. size of the array should be specified correctly, and array should not be modified after passing to Datum
		/// </summary>
		/// <param name="mArray"></param>
		/// <param name="size"></param>
		void setStorage(glm::mat4* mArray, std::size_t size);
		/// <summary>
		/// sets storage to an external array of RTTI pointers. size of the array should be specified correctly, and array should not be modified after passing to Datum
		/// </summary>
		/// <param name="pArray"></param>
		/// <param name="size"></param>
		void setStorage(RTTI** pArray, std::size_t size);

		/// <summary>
		/// Removes the item passed as argument if found. uses the Find function.
		/// </summary>
		/// <typeparam name="...Args"></typeparam>
		/// <param name="...args"></param>
		/// <returns>true if item is removed</returns>
		template<typename... Args>
		bool Remove(const Args&... args);
		/// <summary>
		/// Removes item at index provided. refuses if the index provided is greater than size.
		/// </summary>
		/// <param name="index"></param>
		/// <returns></returns>
		bool RemoveAt(std::size_t index);

		/// <summary>
		/// find the int provided. if found return the index of the entry. else return size of array.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		std::size_t Find(const int value) const;
		/// <summary>
		/// find the float provided. if found return the index of the entry. else return size of array.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		std::size_t Find(const float value) const;
		/// <summary>
		/// find the vector provided. if found return the index of the entry. else return size of array.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		std::size_t Find(const glm::vec4& value) const;
		/// <summary>
		/// find the matrix provided. if found return the index of the entry. else return size of array.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		std::size_t Find(const glm::mat4& value) const;
		/// <summary>
		/// find the string provided. if found return the index of the entry. else return size of array.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		std::size_t Find(const std::string& value) const;
		/// <summary>
		/// find the RTTI object provided. if found return the index of the entry. else return size of array.
		/// </summary>
		/// <param name="value"></param>
		/// <returns></returns>
		std::size_t Find(const RTTI* value) const;
		std::size_t Find(const Scope* value) const;

		/// <summary>
		/// modify the entry at provided index (default 0). sets it to the int passed as argument
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void Set(const int value, std::size_t index = 0);
		/// <summary>
		/// modify the entry at provided index (default 0). sets it to the float passed as argument
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void Set(const float value, std::size_t index = 0);
		/// <summary>
		/// modify the entry at provided index (default 0). sets it to the vector passed as argument
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void Set(const glm::vec4& value, std::size_t index = 0);
		/// <summary>
		/// modify the entry at provided index (default 0). sets it to the matrix passed as argument
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void Set(const glm::mat4& value, std::size_t index = 0);
		/// <summary>
		/// modify the entry at provided index (default 0). sets it to the string passed as argument
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void Set(const std::string& value, std::size_t index = 0);
		/// <summary>
		/// modify the entry at provided index (default 0). sets it to the RTTI object reference passed as argument
		/// </summary>
		/// <param name="value"></param>
		/// <param name="index"></param>
		void Set( RTTI* const value, std::size_t index = 0);
		void Set(Scope* const value, std::size_t index = 0);


		/// <summary>
		/// returns reference to the object at provided index. rejects if there is a type mismatch or index provided is out of bounds.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="index"></param>
		/// <returns></returns>
		template<typename T>
		T& Get(std::size_t index = 0); //correct syntax??

		/// <summary>
		/// returns reference to the object at provided index. rejects if there is a type mismatch or index provided is out of bounds.
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="index"></param>
		/// <returns></returns>
		template<typename T>
		const T& Get(std::size_t index = 0) const; //check pointer syntax

		/// <summary>
		/// inserts integer provided to the back of the array. rejects if there is a type mismatch.
		/// </summary>
		/// <param name="value"></param>
		void pushBack(int value);
		/// <summary>
		/// inserts float provided to the back of the array. rejects if there is a type mismatch.
		/// </summary>
		/// <param name="value"></param>
		void pushBack(float value);
		/// <summary>
		/// inserts vector provided to the back of the array. rejects if there is a type mismatch.
		/// </summary>
		/// <param name="value"></param>
		void pushBack(const glm::vec4& value);
		/// <summary>
		/// inserts matrix provided to the back of the array. rejects if there is a type mismatch.
		/// </summary>
		/// <param name="value"></param>
		void pushBack(const glm::mat4& value);
		/// <summary>
		/// inserts string provided to the back of the array. rejects if there is a type mismatch.
		/// </summary>
		/// <param name="value"></param>
		void pushBack(const std::string& value);
		/// <summary>
		/// inserts RTTI object provided to the back of the array. rejects if there is a type mismatch.
		/// </summary>
		/// <param name="value"></param>
		void pushBack(const RTTI* value);
		void pushBack(const Scope* value);

		/// <summary>
		/// popBack: removes last element of the datum.
		/// </summary>
		void popBack();

		/// <summary>
		/// returns reference to the first element of array.
		/// </summary>
		/// <returns></returns>
		int& frontAsInt();
		/// <summary>
		/// returns reference to the first element of array.
		/// </summary>
		/// <returns></returns>
		float& frontAsFloat();
		/// <summary>
		/// returns reference to the first element of array.
		/// </summary>
		/// <returns></returns>
		glm::vec4& frontAsVector();
		/// <summary>
		/// returns reference to the first element of array.
		/// </summary>
		/// <returns></returns>
		glm::mat4& frontAsMatrix();
		/// <summary>
		/// returns reference to the first element of array.
		/// </summary>
		/// <returns></returns>
		std::string& frontAsString();
		/// <summary>
		/// returns reference to the first element of array.
		/// </summary>
		/// <returns></returns>
		RTTI* frontAsPointer();

		/// <summary>
		/// returns reference to last element of array.
		/// </summary>
		/// <returns></returns>
		int& backAsInt();
		/// <summary>
		/// returns reference to last element of array.
		/// </summary>
		/// <returns></returns>
		float& backAsFloat();
		/// <summary>
		/// returns reference to last element of array.
		/// </summary>
		/// <returns></returns>
		glm::vec4& backAsVector();
		/// <summary>
		/// returns reference to last element of array.
		/// </summary>
		/// <returns></returns>
		glm::mat4& backAsMatrix();
		/// <summary>
		/// returns reference to last element of array.
		/// </summary>
		/// <returns></returns>
		std::string& backAsString();
		/// <summary>
		/// returns reference to last element of array.
		/// </summary>
		/// <returns></returns>
		RTTI* backAsPointer();
		bool IsExternal();

	private:
		friend class Attributed;
		void setStorage(DatumTypes type, std::size_t size, void* dataPtr);
		friend class Attributed;
		void UpdateExtStoragePointer(std::byte* PreviousThis, void* newThis);
	};
}

#include "Datum.inl"


//bool Remove(const int value); //is there any point marking local copies const??
//bool Remove(const float value);
//bool Remove(const glm::vec4 value);
//bool Remove(const glm::mat4 value);
//bool Remove(const std::string& value);

/// <summary>
		/// setup table to lookup sizes of different types
		/// </summary>
		//void initializeSizeTable();
//HashMap<DatumTypes, size_t> _sizeTable{ 7 };
		//std::map<DatumTypes, size_t> _sizeTable;