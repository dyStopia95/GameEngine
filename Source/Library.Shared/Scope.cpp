#include "pch.h"
#include "Scope.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope);

	Scope::Scope(std::size_t capacity) :
		_data{ capacity * 3 }, _capacity{ capacity }, _dataTable{ capacity }
	{
	}

	Scope::~Scope()
	{
		Clear();

		if(_parent != nullptr) //this is definitely getting hit but not showing up in code coverage
		{
			auto [parentDatum, Index] = _parent->FindContainedScope(this);
			parentDatum->RemoveAt(Index);
			_parent = nullptr;
		}
	}

	Scope* Scope::Clone() const
	{
		Scope* newScope = new Scope{ *this };
		return newScope;
	}

	//why cant i use _capacity of this to set _data's capacity (_data gets initialized before _capacity despite being set later)
	//might be because data member _capacity is declared after _data. (this is true)
	Scope::Scope(const Scope& value) :
		_capacity{ value._capacity }, _dataTable{ value._population }, _data{ _capacity * 3 }
	{
		auto it = value._dataTable.cbegin();
		while (it != value._dataTable.cend())
		{
			std::pair<const std::string, Datum>& tempPair = **it;
			Datum& sourceDatum = tempPair.second;															//this is faster than dereferencing pointer multiple times?

			if (sourceDatum.Type() != Datum::DatumTypes::Table)
				Insert(tempPair);

			else
			{
				Datum& tempDatum = Append(tempPair.first);
				tempDatum.SetType(Datum::DatumTypes::Table);
				tempDatum.setCapacity(sourceDatum.Size());
				
				for (size_t i = 0; i < sourceDatum.Size(); ++i)
				{
					Scope* clonedScope = sourceDatum[i]->Clone();
					clonedScope->_parent = this;
					tempDatum.pushBack(clonedScope);
				}
			}

			++it;
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		Clear();
		//cannot match hashmap chain capacities without resize for hashmap
		_dataTable.reserve(rhs._population);

		auto it = rhs._dataTable.cbegin();
		while (it != rhs._dataTable.cend())
		{
			std::pair<const std::string, Datum>& tempPair = **it;
			Datum& sourceDatum = tempPair.second;															//this is faster than dereferencing pointer multiple times?

			if (sourceDatum.Type() != Datum::DatumTypes::Table)
				Insert(tempPair);

			else
			{
				Datum& tempDatum = Append(tempPair.first);
				tempDatum.SetType(Datum::DatumTypes::Table);
				tempDatum.setCapacity(sourceDatum.Size());

				for (size_t i = 0; i < sourceDatum.Size(); ++i)
				{
					Scope* clonedScope = sourceDatum[i]->Clone();
					clonedScope->_parent = this;
					tempDatum.pushBack(clonedScope);
				}
			}

			++it;
		}

		return *this;
	}

	Scope::Scope(Scope&& value) noexcept :
		_data{std::move(value._data)}, _dataTable{std::move(value._dataTable)}
	{
		/*Scope* discardP = value.Orphan();
		discardP;*/
		for (auto it = _dataTable.begin(); it != _dataTable.end(); ++it)
		{
			Datum& tempDatum = (*it)->second;
			if (tempDatum.Type() == Datum::DatumTypes::Table)
			{
				for (size_t i = 0; i < tempDatum.Size(); ++i)
				{
					tempDatum[i]->_parent = this;
				}
			}
		}

		_capacity = value._capacity;
		_population = value._population;
	}

	Scope* Scope::Orphan()
	{
		if (_parent != nullptr)
		{
			auto [ContainerDatum, index] = _parent->FindContainedScope(this);
			ContainerDatum->RemoveAt(index);
			_parent = nullptr;
		}
		
		return this;
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
			return false;
		const Scope* other = rhs->As<Scope>();
		if (other == nullptr)
			return false;

		if (_population != other->_population)
			return false;

		auto itL = _dataTable.cbegin();
		auto itR = other->_dataTable.cbegin();
		while (itL != _dataTable.cend())
		{
			auto [keyL, datumL] = **itL;
			auto [keyR, datumR] = **itR;
			if (keyL != keyR)
				return false;
			if (datumL.Type() != datumR.Type())
				return false;

			if (datumL != datumR)
				return false;

			++itL;
			++itR;
		}
		return true;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		return Equals(&rhs);
	}

	bool Scope::operator!=(const Scope& rhs) const
	{
		return !(operator==(rhs));
	}

	Datum& Scope::operator[](std::size_t index)
	{
		if (index >= _population)
			throw std::out_of_range("Provided index out of range!");

		return _dataTable[index]->second;
	}

	Datum& Scope::operator[](const std::string& key)
	{
		return Append(key);
	}

	void Scope::Clear()
	{
		Scope* tempScope;
		Datum* tempDatum;
		HashMap<std::string, Datum>::iterator it = _data.begin();
		while (it != _data.end())
		{
			tempDatum = &(it->second);
			if (tempDatum->Type() == Datum::DatumTypes::Table)
			{
				for (std::size_t i = 0; i < tempDatum->Size(); ++i)
				{
					tempScope = tempDatum->Get<Scope*>(i);
					if (tempScope != nullptr)
					{
						tempScope->_parent = nullptr;
						delete tempScope; //Delete nested scopes. using free breaks this why?
					}
				}
				//tempDatum->clear(); //clear pointers to nested scopes so as not to access them while reusing datum.
			}
			//else
			//	tempDatum->clear(); //call clear on non Table Datums.

			++it;
		}

		_population = 0;
		_data.Clear();
		_dataTable.clear();
	}

	std::size_t Scope::Size()
	{
		return _population;
	}

	Datum* Scope::Find(const std::string& key)
	{
		auto it = _data.Find(key);
		return (it == _data.end()) ? nullptr : &(it->second);
	}

	const Datum* Scope::Find(const std::string& key) const
	{
		auto it = _data.Find(key);
		return (it == _data.cend()) ? nullptr : &(it->second);
	}

	bool Scope::Insert(const pair_type& value)
	{
		if (value.second.Type() == Datum::DatumTypes::Table)
			throw std::runtime_error("Use AppendScope or Adopt to add scopes");

		auto [PairP,wasInserted] = (_data.insert(value));
		if (wasInserted == false)
			return false;

		++_population;
		_dataTable.push_back(&(*PairP));

		return true;
	}

	bool Scope::Insert(const std::string& key, Datum& value)
	{
		return Insert(std::make_pair(key,value));
	}

	Datum* Scope::Search(const std::string& key, Scope** containingScope)
	{
		Datum* rPointer = Find(key);
		if (rPointer == nullptr)
		{
			if (_parent == nullptr)
			{
				if (containingScope != nullptr)
					*containingScope = nullptr;
				return rPointer;
			}
			else
				rPointer = _parent->Search(key, containingScope);
		}

		else if(containingScope != nullptr)
		{
			*containingScope = this;
		}

		return rPointer;
	}

	Datum& Scope::Append(const std::string& key)
	{
		auto [PairP, isFresh] = _data.CreateAt(key);
		if (isFresh)
		{
			++_population;
			_dataTable.push_back(&( * PairP));
		}
		return (PairP->second);
	}

	Scope& Scope::AppendScope(const std::string& key)
	{
		Scope* tempScope = new Scope{ 5 };
		Adopt(*tempScope, key);
		return *tempScope;
	}

	void Scope::Adopt(Scope& child, const std::string& key)
	{
		auto [PairP, isFresh] = _data.CreateAt(key);

		Datum& tempDatum = PairP->second;
		if(tempDatum.Type() == Datum::DatumTypes::Unknown)
			tempDatum.SetType(Datum::DatumTypes::Table);
		else if(tempDatum.Type() != Datum::DatumTypes::Table)
			throw std::runtime_error("Existing key not of type Table >: (");

		tempDatum.pushBack(&child);
		child._parent = this;

		if (isFresh)
		{
			++_population;
			_dataTable.push_back(&(*PairP));
		}
	}

	Scope* Scope::GetParent()
	{
		return _parent;
	}

	std::pair<Datum*, std::size_t> Scope::FindContainedScope(Scope* const child)
	{
		if (child == nullptr)
			throw std::runtime_error("Invalid pointer");

		auto it = _data.begin();
		while (it != _data.end())
		{
			Datum& tempDatum = (it->second);
			if (tempDatum.Type() == Datum::DatumTypes::Table)
			{
				std::size_t index = tempDatum.Find(child);
				if (index != tempDatum.Size())
				{
					return std::make_pair(&tempDatum, index);
				}
			}
			++it;
		}
		return std::make_pair(nullptr, 0);
	}
}



//bool Scope::Equals(const RTTI* rhs) const
	//{
	//	if (rhs == nullptr)
	//		return false;

	//	//const Scope* other = rhs->As<Scope>();
	//	/*if (other != nullptr)
	//	{

	//	}*/
	//}





/*bool datumExists = true;
if (_data.Find(key) == _data.end())
	datumExists = false;

Datum& d = _data[key];
if (d.Type() == Datum::DatumTypes::Unknown)
{
	d.SetType(Datum::DatumTypes::Table);
}

if (d.Type() != Datum::DatumTypes::Table)
	throw std::runtime_error("Existing key not of type Table >: (");

d.pushBack(&child);
child._parent = this;

if (datumExists == false)
{
	++_population;
}*/