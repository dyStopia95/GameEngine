#pragma once
#include "HashMap.h"
#include "vector.h"
//#include "Attributed.h"

namespace FieaGameEngine
{
	struct Signature
	{
		std::string Key;
		Datum::DatumTypes Type;
		std::size_t Size;
		std::size_t Offset;

		Signature(const std::string& key, Datum::DatumTypes type, std::size_t size, std::size_t offset)
		{
			Key = key;
			Type = type;
			Size = size;
			Offset = offset;
		};

		Signature(const Signature& rhs)
		{
			Key = rhs.Key;
			Type = rhs.Type;
			Size = rhs.Size;
			Offset = rhs.Offset;
		}

		bool operator==(const Signature& rhs)
		{
			if (Key != rhs.Key)
				return false;
			if (Type != rhs.Type)
				return false;
			if (Size != rhs.Size)
				return false;
			if (Offset != rhs.Offset)
				return false;

			return true;
		}
	};

	class TypeManager final
	{
	private:
		//inline static TypeManager* _instance{ nullptr };
		inline static HashMap<std::size_t,Vector<Signature>> _signatures;
	
	public:
		inline static void Add(size_t ID, const Vector<Signature>& signatureSet, size_t parentID = 0)
		{
			if(parentID == 0)
				_signatures.insert({ ID,signatureSet });
			else
			{
				assert(_signatures.Find(parentID) != _signatures.end());
				auto childSig = RetrieveSignature(parentID);
				for (size_t i = 0; i < signatureSet.size(); ++i)
				{
					childSig.push_back(signatureSet.at(i));
				}
				_signatures.insert({ ID, childSig });
			}
		}

		inline static Vector<Signature>& RetrieveSignature(size_t ID)
		{
			return _signatures.At(ID);
		}

		inline static void Clear()
		{
			_signatures.Clear();
		}
	};

	template <typename Type>
	void RegisterType()
	{
		TypeManager::Add(Type::TypeIdClass(), Type::Signatures());
	}

	template <typename Type, typename ParentType>
	void RegisterType()
	{
		TypeManager::Add(Type::TypeIdClass(), Type::Signatures(), ParentType::TypeIdClass());
	}
}



//TypeManager() :
		//	_signatures{ 31 }
		//{
		//}

		//static TypeManager* Instance()
		//{
		//	return _instance;
		//}

		//void CreateInstance()
		//{
		//	if (_instance == nullptr)
		//		_instance = new TypeManager{};
		//}

		//void DestroyInstance()
		//{
		//	//_instance->_signatures.~HashMap();
		//	delete _instance;
		//	_instance = nullptr;
		//}

		//Vector<Signature>& GetSignature(std::size_t typeID)
		//{
		//	auto it = _signatures.Find(typeID);
		//	return it->second;
		//}


		//	//std::pair<std::size_t, Vector<Signature>> p1{ tempID,tempSignatures };

		//static bool Add(RTTI::IdType typeID, const Vector<Signature>& signatures)
		//{
		//	assert(_instance != nullptr);
		//	Vector<Signature> tempSignatures = signatures;
		//	size_t tempID = typeID;
		//	auto [it, status] = _instance->_signatures.insert(std::pair<size_t,Vector<Signature>>{tempID,tempSignatures});
		//	it->second = tempSignatures;
		//	return status;
		//}

		//static void Remove(RTTI::IdType typeID)
		//{
		//	assert(_instance != nullptr);
		//	_instance->_signatures.Remove(typeID);
		//}

		//static void Clear()
		//{
		//	_instance->_signatures.Clear();
		//}