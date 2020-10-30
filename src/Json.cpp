#include "picojson/picojson.h"
#include "Json.h"
#include "Survival.h"
#include "Warmth.h"

using namespace Survival;
namespace Json
{
	using JObject = picojson::object;
	using JValue = picojson::value;

	namespace
	{
		std::string FormToString(RE::FormID formID)
		{
			RE::FormID relativeID = formID % (1 << 24);

			auto dataHandler = RE::TESDataHandler::GetSingleton();

			std::string plugin = "";
			uint8_t modIndex = formID >> 24;
			if (modIndex < 0xFE)
			{
				auto file = dataHandler->LookupLoadedModByIndex(modIndex);
				plugin = file->fileName;
			}
			if (modIndex == 0xFE)
			{
				uint16_t lightModIndex = relativeID >> 12;
				relativeID %= 1 << 12;
				auto file = dataHandler->LookupLoadedLightModByIndex(lightModIndex);
				plugin = file->fileName;
			}

			std::stringstream stream;
			stream << "__formData|" << plugin << "|" << std::hex << std::showbase << relativeID;
			return stream.str();
		}

		RE::TESForm* FormFromString(std::string formData)
		{
			std::stringstream stream{ formData };
			std::string prefix, plugin, id;

			std::getline(stream, prefix, '|');
			if (prefix != "__formData")
				return nullptr;

			std::getline(stream, plugin, '|');

			std::getline(stream, id);
			uint32_t relativeID;
			std::istringstream{ id } >> std::hex >> std::showbase >> relativeID;

			auto dataHandler = RE::TESDataHandler::GetSingleton();
			return dataHandler->LookupForm(relativeID, plugin);
		}

		std::filesystem::path GetUserDirectory()
		{
			char mydocuments[MAX_PATH];
			SHGetFolderPath(nullptr, CSIDL_MYDOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, mydocuments);

			std::filesystem::path path{ mydocuments };
			path = path / "My Games" / "Skyrim Special Edition" / "SurvivalCP";
			return path;
		}
	}

	std::vector<RE::BSFixedString> ListFiles(RE::StaticFunctionTag*)
	{
		std::vector<RE::BSFixedString> files;
		auto userDir = GetUserDirectory();
		std::filesystem::directory_entry dir_entry{ userDir };
		if (!dir_entry.exists())
		{
			return files;
		}

		std::filesystem::directory_iterator dirContents{ userDir };

		for (auto file : dirContents)
		{
			if (!file.is_regular_file())
				continue;

			if (file.path().extension() == ".json")
			{
				files.push_back(RE::BSFixedString{ file.path().stem().string() });
			}
		}

		return files;
	}

	bool Exists(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto fullPath = GetUserDirectory() / std::string_view{ a_filePath };
		fullPath += ".json";

		std::filesystem::directory_entry dir_entry{ fullPath };

		return dir_entry.exists();
	}

	bool Save(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto hudIndicators = Settings::GetSingleton(Feature::HUDIndicators);
		auto inventoryUI = Settings::GetSingleton(Feature::InventoryUI);
		auto sleepToLevelUp = Settings::GetSingleton(Feature::SleepToLevelUp);
		auto arrowWeight = Settings::GetSingleton(Feature::ArrowWeight);
		auto lockpickWeight = Settings::GetSingleton(Feature::LockpickWeight);
		auto warmth = WarmthSettings::GetSingleton();

		JObject jSettings;
		jSettings["HUDIndicators"] = JValue{ static_cast<double>(hudIndicators->UserPreference) };
		jSettings["InventoryUI"] = JValue{ static_cast<double>(inventoryUI->UserPreference) };
		jSettings["SleepToLevelUp"] = JValue{ static_cast<double>(sleepToLevelUp->UserPreference) };
		jSettings["ArrowWeight"] = JValue{ static_cast<double>(arrowWeight->UserPreference) };
		jSettings["LockpickWeight"] = JValue{ static_cast<double>(lockpickWeight->UserPreference) };

		JObject jWarmth;
		jWarmth["EnableFrostfallKeywords"] = JValue{ static_cast<double>(warmth.EnableFrostfallKeywords) };
		jWarmth["EnableWarmthForCloaks"] = JValue{ static_cast<double>(warmth.EnableWarmthForCloaks) };

		// TODO save standard game settings and cloak settings

		JObject jOverrides;
		jOverrides["__metaInfo"] = JValue{ JObject{ { "typeName", JValue{ "JFormMap"} } } };

		for (auto [formID, value] : warmth.WarmthOverrides)
		{
			auto idString = FormToString(formID);
			jOverrides[idString] = JValue{ static_cast<double>(value) };
		}

		jWarmth["WarmthOverrides"] = JValue{ jOverrides };
		jSettings["WarmthSettings"] = JValue{ jWarmth };

		auto userDir = GetUserDirectory();
		std::filesystem::directory_entry dir_entry{ userDir };
		if (!dir_entry.exists())
		{
			std::filesystem::create_directory(userDir);
		}

		auto fullPath = userDir / std::string_view{ a_filePath };
		fullPath += ".json";
		std::ofstream stream{ fullPath };
		stream << JValue{ jSettings }.serialize(true);

		return true;
	}

	bool Load(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto hudIndicators = Settings::GetSingleton(Feature::HUDIndicators);
		auto inventoryUI = Settings::GetSingleton(Feature::InventoryUI);
		auto sleepToLevelUp = Settings::GetSingleton(Feature::SleepToLevelUp);
		auto arrowWeight = Settings::GetSingleton(Feature::ArrowWeight);
		auto lockpickWeight = Settings::GetSingleton(Feature::LockpickWeight);
		auto warmth = WarmthSettings::GetSingleton();

		auto userDir = GetUserDirectory();
		std::filesystem::directory_entry dir_entry{ userDir };
		if (!dir_entry.exists())
		{
			return false;
		}

		auto fullPath = userDir / std::string_view{ a_filePath };
		fullPath += ".json";
		std::ifstream stream{ fullPath };

		JValue v;
		std::string err = picojson::parse(v, stream);
		if (!err.empty())
		{
			return false;
		}

		if (!v.is<JObject>())
		{
			return false;
		}

		auto jSettings = v.get<JObject>();

		hudIndicators->UserPreference = static_cast<Preference>(jSettings["HUDIndicators"].get<double>());
		inventoryUI->UserPreference = static_cast<Preference>(jSettings["InventoryUI"].get<double>());
		sleepToLevelUp->UserPreference = static_cast<Preference>(jSettings["SleepToLevelUp"].get<double>());
		arrowWeight->UserPreference = static_cast<Preference>(jSettings["ArrowWeight"].get<double>());
		lockpickWeight->UserPreference = static_cast<Preference>(jSettings["LockpickWeight"].get<double>());

		auto jWarmth = jSettings["WarmthSettings"].get<JObject>();
		warmth.EnableFrostfallKeywords = jWarmth["EnableFrostfallKeywords"].get<double>();
		warmth.EnableWarmthForCloaks = jWarmth["EnableWarmthForCloaks"].get<double>();

		// TODO load standard game settings and cloak settings

		auto jOverrides = jWarmth["WarmthOverrides"].get<JObject>();
		for (auto [formData, value] : jOverrides)
		{
			auto form = FormFromString(formData);
			if (!form)
				continue;

			warmth.WarmthOverrides[form->formID] = static_cast<WarmthClass>(value.get<double>());
		}

		return true;
	}

	bool Delete(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto userDir = GetUserDirectory();
		std::filesystem::directory_entry dir_entry{ userDir };
		if (!dir_entry.exists())
		{
			return false;
		}

		auto fullPath = userDir / std::string_view{ a_filePath };
		fullPath += ".json";
		std::filesystem::remove(fullPath);
		return true;
	}

	bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("ListFiles"sv, "Survival_Json"sv, ListFiles);
		a_vm->RegisterFunction("Exists"sv, "Survival_Json"sv, Exists);
		a_vm->RegisterFunction("Save"sv, "Survival_Json"sv, Save);
		a_vm->RegisterFunction("Load"sv, "Survival_Json"sv, Load);
		a_vm->RegisterFunction("Delete"sv, "Survival_Json"sv, Delete);

		return true;
	}
}