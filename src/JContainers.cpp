#include "JContainers.h"
#include "Survival.h"
#include "Warmth.h"

using namespace Survival;

namespace JContainers
{
	void Save(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		if (!default_domain)
		{
			logger::error("Failed to load JContainers!"sv);
			return;
		}

		auto hudIndicators = Settings::GetSingleton(Feature::HUDIndicators);
		auto inventoryUI = Settings::GetSingleton(Feature::InventoryUI);
		auto sleepToLevelUp = Settings::GetSingleton(Feature::SleepToLevelUp);
		auto arrowWeight = Settings::GetSingleton(Feature::ArrowWeight);
		auto lockpickWeight = Settings::GetSingleton(Feature::LockpickWeight);
		auto warmth = WarmthSettings::GetSingleton();

		JMap jSettings;

		JMap jHUD;
		jHUD.setInt("ModPreference", static_cast<int32_t>(hudIndicators->ModPreference));
		jHUD.setInt("UserPreference", static_cast<int32_t>(hudIndicators->UserPreference));
		jSettings.setObj("HUDIndicators", jHUD);

		JMap jInventory;
		jInventory.setInt("ModPreference", static_cast<int32_t>(inventoryUI->ModPreference));
		jInventory.setInt("UserPreference", static_cast<int32_t>(inventoryUI->UserPreference));
		jSettings.setObj("InventoryUI", jInventory);

		JMap jLvlUp;
		jLvlUp.setInt("ModPreference", static_cast<int32_t>(sleepToLevelUp->ModPreference));
		jLvlUp.setInt("UserPreference", static_cast<int32_t>(sleepToLevelUp->UserPreference));
		jSettings.setObj("SleepToLevelUp", jLvlUp);

		JMap jArrow;
		jArrow.setInt("ModPreference", static_cast<int32_t>(arrowWeight->ModPreference));
		jArrow.setInt("UserPreference", static_cast<int32_t>(arrowWeight->UserPreference));
		jSettings.setObj("ArrowWeight", jLvlUp);

		JMap jLockpick;
		jLockpick.setInt("ModPreference", static_cast<int32_t>(lockpickWeight->ModPreference));
		jLockpick.setInt("UserPreference", static_cast<int32_t>(lockpickWeight->UserPreference));
		jSettings.setObj("LockpickWeight", jLvlUp);

		JMap jWarmth;
		jWarmth.setInt("EnableFrostfallKeywords", warmth.EnableFrostfallKeywords);
		jWarmth.setInt("EnableWarmthForCloaks", warmth.EnableWarmthForCloaks);

		JFormMap jOverrides;
		for (auto [formID, value] : warmth.WarmthOverrides)
		{
			auto form = RE::TESForm::LookupByID(formID);
			jOverrides.setInt(form, static_cast<int32_t>(value));
		}

		jWarmth.setObj("WarmthOverrides", jOverrides);
		jSettings.setObj("WarmthSettings", jWarmth);

		jSettings.writeToFile(a_filePath.c_str());
	}

	void Load(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		if (!default_domain)
		{
			logger::error("Failed to load JContainers!"sv);
			return;
		}

		auto hudIndicators = Settings::GetSingleton(Feature::HUDIndicators);
		auto inventoryUI = Settings::GetSingleton(Feature::InventoryUI);
		auto sleepToLevelUp = Settings::GetSingleton(Feature::SleepToLevelUp);
		auto arrowWeight = Settings::GetSingleton(Feature::ArrowWeight);
		auto lockpickWeight = Settings::GetSingleton(Feature::LockpickWeight);
		auto warmth = WarmthSettings::GetSingleton();

		JMap jSettings{ a_filePath.c_str() };

		auto jHUD = jSettings.getObj<JMap>("HUDIndicators");
		hudIndicators->ModPreference = static_cast<Preference>(jHUD.getInt("ModPreference"));
		hudIndicators->UserPreference = static_cast<Preference>(jHUD.getInt("UserPreference"));

		auto jInventory = jSettings.getObj<JMap>("InventoryUI");
		inventoryUI->ModPreference = static_cast<Preference>(jInventory.getInt("ModPreference"));
		inventoryUI->UserPreference = static_cast<Preference>(jInventory.getInt("UserPreference"));

		auto jLvlUp = jSettings.getObj<JMap>("SleepToLevelUp");
		sleepToLevelUp->ModPreference = static_cast<Preference>(jLvlUp.getInt("ModPreference"));
		sleepToLevelUp->UserPreference = static_cast<Preference>(jLvlUp.getInt("UserPreference"));

		auto jArrow = jSettings.getObj<JMap>("ArrowWeight");
		arrowWeight->ModPreference = static_cast<Preference>(jArrow.getInt("ModPreference"));
		arrowWeight->UserPreference = static_cast<Preference>(jArrow.getInt("UserPreference"));

		auto jLockpick = jSettings.getObj<JMap>("LockpickWeight");
		lockpickWeight->ModPreference = static_cast<Preference>(jLockpick.getInt("ModPreference"));
		lockpickWeight->UserPreference = static_cast<Preference>(jLockpick.getInt("UserPreference"));

		auto jWarmth = jSettings.getObj<JMap>("WarmthSettings");
		warmth.EnableFrostfallKeywords = jWarmth.getInt("EnableFrostfallKeywords");
		warmth.EnableWarmthForCloaks = jWarmth.getInt("EnableWarmthForCloaks");

		auto jOverrides = jWarmth.getObj<JFormMap>("WarmthOverrides");
		int32_t count = jOverrides.count();
		auto keyArray = jOverrides.allKeys();

		for (int32_t i = 0; i < count; i++)
		{
			auto form = keyArray[i];
			auto value = jOverrides.getInt(form);
			warmth.WarmthOverrides[form->formID] = static_cast<WarmthClass>(value);
		}
	}

	bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm)
	{
		a_vm->RegisterFunction("Save"sv, "Survival_JContainers"sv, Save);
		a_vm->RegisterFunction("Load"sv, "Survival_JContainers"sv, Load);

		return true;
	}
}