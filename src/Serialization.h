#pragma once

namespace Serialization
{

	template<typename T>
	bool Save(SKSE::SerializationInterface* a_intfc, std::vector<T> setting)
	{
		std::size_t size = setting.size();
		if (!a_intfc->WriteRecordData(size))
		{
			logger::error("Failed to write size of record data!");
		}
		else
		{
			for (auto& elem : setting)
			{
				if (!a_intfc->WriteRecordData(elem))
				{
					logger::error("Failed to write data for warmth elem!");
					return false;
					break;
				}
			}
		}
		return true;
	}

	template<typename T>
	bool Load(SKSE::SerializationInterface* a_intfc, std::vector<T> &returnVector)
	{
		std::size_t size;
		if (!a_intfc->ReadRecordData(size))
		{
			logger::error("Failed to load size!");
			return false;
		}

		for (std::size_t i = 0; i < size; ++i)
		{
			T elem;
			if (!a_intfc->ReadRecordData(elem))
			{
				logger::error("Failed to load setting element!");
				break;
			}
			else
			{
				returnVector.push_back(elem);
			}
		}
		return true;
	}
}