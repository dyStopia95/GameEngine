#pragma once

namespace FieaGameEngine
{
	template<typename TKey>
	struct DefaultHash final
	{
		size_t operator()(const TKey& key)
		{
			const auto hashPrime = 3;
			size_t hash = 5;
			const std::byte* data = reinterpret_cast<const std::byte*> (&key);

			for (std::size_t i = 0; i < sizeof(TKey); ++i)
			{
				hash += static_cast<size_t>(data[i]) * hashPrime;
			}

			return hash;
		}
	};

	template<>
	struct DefaultHash<std::size_t> final
	{
		size_t operator()(std::size_t key)
		{
			return key;
		}
	};

	template<>
	struct DefaultHash<std::string> final
	{
		size_t operator()(const std::string& key)
		{
			const auto hashPrime = 3;
			size_t hash = 5;
			for (size_t i = 0; i < key.length(); ++i)
			{
				hash = hashPrime * hash + key.at(i);
			}

			return hash;
		}
	};
}