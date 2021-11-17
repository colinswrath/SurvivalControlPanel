#include "picojson/picojson.h"
#include "Json.h"
#include "Survival.h"
#include "Warmth.h"
#include <windows.h>
#include "ShlObj_core.h"

using namespace Survival;
namespace Json
{
	// Change this when we change the format
	const int32_t version = 1;

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
				uint16_t lightModIndex = static_cast<uint16_t>(relativeID >> 12);
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

	}

	std::filesystem::path GetUserDirectory()
	{
		//char mydocuments[MAX_PATH];
		wchar_t mydocuments[MAX_PATH];
		SHGetFolderPath(nullptr, CSIDL_MYDOCUMENTS, nullptr, SHGFP_TYPE_CURRENT, mydocuments);

		std::filesystem::path path{ mydocuments };
		path = path / "My Games" / "Skyrim Special Edition" / "SurvivalCP";
		return path;
	}

	bool Save(std::filesystem::path filePath)
	{
		auto settings = RE::GameSettingCollection::GetSingleton();
		auto hudIndicators = Settings::GetSingleton(Feature::HUDIndicators);
		auto inventoryUI = Settings::GetSingleton(Feature::InventoryUI);
		auto sleepToLevelUp = Settings::GetSingleton(Feature::SleepToLevelUp);
		auto arrowWeight = Settings::GetSingleton(Feature::ArrowWeight);
		auto lockpickWeight = Settings::GetSingleton(Feature::LockpickWeight);
		auto& warmth = WarmthSettings::GetSingleton();

		JObject jSettings;
		jSettings["__version"] = JValue{ static_cast<double>(version) };

		jSettings["HUDIndicators"] = JValue{ static_cast<double>(hudIndicators->UserPreference) };
		jSettings["InventoryUI"] = JValue{ static_cast<double>(inventoryUI->UserPreference) };
		jSettings["SleepToLevelUp"] = JValue{ static_cast<double>(sleepToLevelUp->UserPreference) };
		jSettings["ArrowWeight"] = JValue{ static_cast<double>(arrowWeight->UserPreference) };
		jSettings["LockpickWeight"] = JValue{ static_cast<double>(lockpickWeight->UserPreference) };

		JObject jWarmth;
		jWarmth["EnableFrostfallKeywords"] = JValue{ static_cast<double>(warmth.EnableFrostfallKeywords) };
		jWarmth["EnableWarmthForCloaks"] = JValue{ static_cast<double>(warmth.EnableWarmthForCloaks) };

		jWarmth["NormalBodyBonus"] = JValue{ settings->GetSetting("fSurvNormalBodyBonus")->GetFloat() };
		jWarmth["WarmBodyBonus"] = JValue{ settings->GetSetting("fSurvWarmBodyBonus")->GetFloat() };
		jWarmth["ColdBodyBonus"] = JValue{ settings->GetSetting("fSurvColdBodyBonus")->GetFloat() };
		jWarmth["NormalHandsBonus"] = JValue{ settings->GetSetting("fSurvNormalHandsBonus")->GetFloat() };
		jWarmth["WarmHandsBonus"] = JValue{ settings->GetSetting("fSurvWarmHandsBonus")->GetFloat() };
		jWarmth["ColdHandsBonus"] = JValue{ settings->GetSetting("fSurvColdHandsBonus")->GetFloat() };
		jWarmth["NormalHeadBonus"] = JValue{ settings->GetSetting("fSurvNormalHeadBonus")->GetFloat() };
		jWarmth["WarmHeadBonus"] = JValue{ settings->GetSetting("fSurvWarmHeadBonus")->GetFloat() };
		jWarmth["ColdHeadBonus"] = JValue{ settings->GetSetting("fSurvColdHeadBonus")->GetFloat() };
		jWarmth["NormalFeetBonus"] = JValue{ settings->GetSetting("fSurvNormalFeetBonus")->GetFloat() };
		jWarmth["WarmFeetBonus"] = JValue{ settings->GetSetting("fSurvWarmFeetBonus")->GetFloat() };
		jWarmth["ColdFeetBonus"] = JValue{ settings->GetSetting("fSurvColdFeetBonus")->GetFloat() };
		jWarmth["NormalCloakBonus"] = JValue{ warmth.GetCloakNormalBonus() };
		jWarmth["WarmCloakBonus"] = JValue{ warmth.GetCloakWarmBonus() };
		jWarmth["ColdCloakBonus"] = JValue{ warmth.GetCloakColdBonus() };
		jWarmth["TorchBonus"] = JValue{ settings->GetSetting("fSurvTorchBonus")->GetFloat() };
		jWarmth["ArmorScalar"] = JValue{ settings->GetSetting("fSurvArmorScalar")->GetFloat() };

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

		std::ofstream stream{ filePath };
		stream << JValue{ jSettings }.serialize(true);

		return true;
	}

	bool Load(std::filesystem::path filePath)
	{
		std::filesystem::directory_entry dir_entry{ filePath };
		if (!dir_entry.exists())
		{
			return false;
		}

		auto settings = RE::GameSettingCollection::GetSingleton();
		auto hudIndicators = Settings::GetSingleton(Feature::HUDIndicators);
		auto inventoryUI = Settings::GetSingleton(Feature::InventoryUI);
		auto sleepToLevelUp = Settings::GetSingleton(Feature::SleepToLevelUp);
		auto arrowWeight = Settings::GetSingleton(Feature::ArrowWeight);
		auto lockpickWeight = Settings::GetSingleton(Feature::LockpickWeight);
		auto& warmth = WarmthSettings::GetSingleton();

		std::ifstream stream{ filePath };

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

		// In the future, we may need to check version and defer to different loading functions/classes
		//int32_t version = static_cast<int32_t>(jSettings["__version"].get<double>());

		hudIndicators->UserPreference = static_cast<Preference>(jSettings["HUDIndicators"].get<double>());
		inventoryUI->UserPreference = static_cast<Preference>(jSettings["InventoryUI"].get<double>());
		sleepToLevelUp->UserPreference = static_cast<Preference>(jSettings["SleepToLevelUp"].get<double>());
		arrowWeight->UserPreference = static_cast<Preference>(jSettings["ArrowWeight"].get<double>());
		lockpickWeight->UserPreference = static_cast<Preference>(jSettings["LockpickWeight"].get<double>());

		auto jWarmth = jSettings["WarmthSettings"].get<JObject>();
		warmth.EnableFrostfallKeywords = jWarmth["EnableFrostfallKeywords"].get<double>();
		warmth.EnableWarmthForCloaks = jWarmth["EnableWarmthForCloaks"].get<double>();

#pragma warning( push )
#pragma warning( disable : 4244 )
		settings->GetSetting("fSurvNormalBodyBonus")->data.f = jWarmth["NormalBodyBonus"].get<double>();
		settings->GetSetting("fSurvWarmBodyBonus")->data.f = jWarmth["WarmBodyBonus"].get<double>();
		settings->GetSetting("fSurvColdBodyBonus")->data.f = jWarmth["ColdBodyBonus"].get<double>();
		settings->GetSetting("fSurvNormalHandsBonus")->data.f = jWarmth["NormalHandsBonus"].get<double>();
		settings->GetSetting("fSurvWarmHandsBonus")->data.f = jWarmth["WarmHandsBonus"].get<double>();
		settings->GetSetting("fSurvColdHandsBonus")->data.f = jWarmth["ColdHandsBonus"].get<double>();
		settings->GetSetting("fSurvNormalHeadBonus")->data.f = jWarmth["NormalHeadBonus"].get<double>();
		settings->GetSetting("fSurvWarmHeadBonus")->data.f = jWarmth["WarmHeadBonus"].get<double>();
		settings->GetSetting("fSurvColdHeadBonus")->data.f = jWarmth["ColdHeadBonus"].get<double>();
		settings->GetSetting("fSurvNormalFeetBonus")->data.f = jWarmth["NormalFeetBonus"].get<double>();
		settings->GetSetting("fSurvWarmFeetBonus")->data.f = jWarmth["WarmFeetBonus"].get<double>();
		settings->GetSetting("fSurvColdFeetBonus")->data.f = jWarmth["ColdFeetBonus"].get<double>();
		warmth.SetCloakNormalBonus(jWarmth["NormalCloakBonus"].get<double>());
		warmth.SetCloakWarmBonus(jWarmth["WarmCloakBonus"].get<double>());
		warmth.SetCloakColdBonus(jWarmth["ColdCloakBonus"].get<double>());
		settings->GetSetting("fSurvTorchBonus")->data.f = jWarmth["TorchBonus"].get<double>();
		settings->GetSetting("fSurvArmorScalar")->data.f = jWarmth["ArmorScalar"].get<double>();
#pragma warning( pop )

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
}
