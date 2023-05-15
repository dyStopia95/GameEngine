#include "pch.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	void Attributed::Populate(size_t typeID)
	{
		Vector<Signature>& signatureSet = TypeManager::RetrieveSignature(typeID);
		{
			Datum& tempDatum = Append("This");
			tempDatum.SetType(Datum::DatumTypes::Pointer);
			tempDatum.setCapacity(1);
			RTTI* dummyPointer = reinterpret_cast<RTTI*>(this);
			if (tempDatum.Size() == 1)
				tempDatum.Set(dummyPointer, 0);
			else
				tempDatum.pushBack(dummyPointer);
		}

		for (size_t i = 0; i < signatureSet.size(); ++i)
		{
			Signature currentSig = signatureSet[i];
			Datum& tempDatum = Append(currentSig.Key);

			if (currentSig.Type != Datum::DatumTypes::Table)
			{
				void* storagePtr = reinterpret_cast<std::byte*>(this) + currentSig.Offset;
				tempDatum.setStorage(currentSig.Type, currentSig.Size, storagePtr);
			}
			else
			{
				assert(currentSig.Offset == 0);
				tempDatum.SetType(currentSig.Type);

				if (currentSig.Size != 0)
					tempDatum.reserve(currentSig.Size);
			}
		}
	}

	Attributed::Attributed(size_t typeID)
	{
		Populate(typeID);
	}

	Attributed::Attributed(const Attributed& rhs) :
		Scope(rhs)
	{
		size_t testID = rhs.TypeIdInstance();
		Populate(testID);
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		Clear();
		Scope::operator=(rhs);
		Populate(rhs.TypeIdInstance());
		return *this;
	}

	bool Attributed::IsAttribute(std::string AttName)
	{
		auto [index, prescribedSize] = GetIndexOfAttribute(AttName);

		return !(index == _dataTable.size());
	}

	bool Attributed::IsPrescribedAttribute(std::string AttName)
	{
		auto [index, prescribedSize] = GetIndexOfAttribute(AttName);

		return (index <= prescribedSize) ? true : false;
	}

	bool Attributed::IsAuxiliaryAttribute(std::string AttName)
	{
		auto [index, prescribedSize] = GetIndexOfAttribute(AttName);

		return ((index > prescribedSize) && (index < _dataTable.size())) ? true : false;
	}

	std::pair<size_t, size_t> Attributed::GetIndexOfAttribute(std::string AttName)
	{
		assert(_dataTable[0]->first == "This");
		Vector<Signature>& Signature = TypeManager::RetrieveSignature(this->TypeIdInstance());
		size_t prescribedSize = Signature.size();

		size_t i = 1;
		while ((i < _dataTable.size()) && (_dataTable[i]->first != AttName))
		{
			++i;
		}

		return std::pair<size_t, size_t>(i, prescribedSize);
	}

	Vector<std::pair<const std::string, Datum>*>& Attributed::Attributes()
	{
		return _dataTable;
	}

	std::pair<Vector<std::pair<const std::string, Datum>*>::iterator, Vector<std::pair<const std::string, Datum>*>::iterator> Attributed::PrescribedAttributes()
	{
		Vector<Signature>& signatureSet = TypeManager::RetrieveSignature(this->TypeIdInstance());

		Vector < std::pair<const std::string, Datum>*>::iterator itFront = _dataTable.begin();
		++itFront;
		Vector < std::pair<const std::string, Datum>*>::iterator itEnd = itFront;
		itEnd = itEnd + signatureSet.size();
		return std::pair(itFront, itEnd);
	}

	std::pair< Vector<std::pair<const std::string, Datum>*>::iterator, Vector<std::pair<const std::string, Datum>*>::iterator> Attributed::AuxiliaryAttributes()
	{
		Vector<Signature>& signatureSet = TypeManager::RetrieveSignature(this->TypeIdInstance());

		Vector < std::pair<const std::string, Datum>*>::iterator itFront = _dataTable.begin();
		itFront = itFront + (signatureSet.size() + 1);
		Vector < std::pair<const std::string, Datum>*>::iterator itEnd = _dataTable.end();

		return std::pair(itFront, itEnd);
	}

	std::pair< Vector<std::pair<const std::string, Datum>*>::const_iterator, Vector<std::pair<const std::string, Datum>*>::const_iterator> Attributed::AuxiliaryAttributes() const
	{
		Vector<Signature>& signatureSet = TypeManager::RetrieveSignature(this->TypeIdInstance());

		Vector < std::pair<const std::string, Datum>*>::const_iterator itFront = _dataTable.cbegin();
		itFront = itFront + (signatureSet.size() + 1);
		Vector < std::pair<const std::string, Datum>*>::const_iterator itEnd = _dataTable.cend();

		return std::pair(itFront, itEnd);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(std::string AttName)
	{
		if (IsPrescribedAttribute(AttName))
			throw std::runtime_error("Cannot append auxiliary attribute with same name as prescribed attribute!!");

		return(Append(AttName));
	}
}
