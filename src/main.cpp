#include "version.h"
#include "Hooks.h"
#include "Papyrus.h"
#include "Survival.h"
#include "Warmth.h"
#include "Json.h"

enum
{
	kSerializationVersion = 1,
	kHUDIndicators = 'HUDS',
	kInventoryUI = 'IUIS',
	kSleepToLevelUp = 'STLS',
	kArrowWeight = 'ARWS',
	kLockpickWeight = 'LKPS',
	kWarmthSettings = 'WRMT',
};

void InitLogger()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= fmt::format("{}.log"sv, Version::PROJECT);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), Version::PROJECT, Version::NAME);
}

extern "C" DLLEXPORT SKSE::PluginVersionData SKSEPlugin_Version =
{
	
	.dataVersion = SKSE::PluginVersionData::kVersion,
	.pluginVersion = Version::MAJOR,
	.name = PROJECT_NAME,
	.author = "Parapets and colinswrath",
	.supportEmail = "",


	.compatibleVersions = { SKSE::RUNTIME_1_6_318.packed(), 0},
	//.xseMinimum = 0,
	.seVersionRequired=0,
};

/*
extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return false;
	}

	*path /= "SurvivalCP.log"sv;
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::warn);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%g(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("Survival Control Panel v{}"), SRCP_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "Survival Control Panel";
	a_info->version = SRCP_VERSION_MAJOR;

	if (a_skse->IsEditor()) {
		logger::critical("Loaded in editor, marking as incompatible"sv);
		return false;
	}

	const auto ver = a_skse->RuntimeVersion();
	if (ver < SKSE::RUNTIME_1_5_39) {
		logger::critical(FMT_STRING("Unsupported runtime version {}"), ver.string());
		return false;
	}

	return true;
}
*/

void SaveCallback(SKSE::SerializationInterface* a_intfc)
{
	logger::info("Save callback start");

	auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);
	auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);
	auto HUDSettings = Survival::GetSettings(Survival::Feature::HUDIndicators);
	auto lockSettings = Survival::GetSettings(Survival::Feature::LockpickWeight);
	auto SleepSettings = Survival::GetSettings(Survival::Feature::SleepToLevelUp);
	auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

	if (!arrowSettings->SerializeSave(a_intfc, kArrowWeight, kSerializationVersion)) {
		logger::error("Failed to save arrow settings!\n");
	}

	if (!uiSettings->SerializeSave(a_intfc, kInventoryUI, kSerializationVersion)) {
		logger::error("Failed to save Inventory UI Settings!\n");
	}

	if (!HUDSettings->SerializeSave(a_intfc, kHUDIndicators, kSerializationVersion)) {
		logger::error("Failed to save Hud Settings Settings!\n");
	}

	if (!lockSettings->SerializeSave(a_intfc, kLockpickWeight, kSerializationVersion)) {
		logger::error("Failed to save lockpick Settings!\n");
	}

	if (!SleepSettings->SerializeSave(a_intfc, kSleepToLevelUp, kSerializationVersion)) {
		logger::error("Failed to save Sleep to level Settings!\n");
	}

	if (!warmthSettings.SerializeSave(a_intfc, kWarmthSettings, kSerializationVersion)) {
		logger::error("Failed to save Warmth Settings!\n");
	}
	logger::info("Save callback stop");

}

void LoadCallBack(SKSE::SerializationInterface* a_intfc)
{
	logger::info("load callback start");
	auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);
	auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);
	auto HUDSettings = Survival::GetSettings(Survival::Feature::HUDIndicators);
	auto lockSettings = Survival::GetSettings(Survival::Feature::LockpickWeight);
	auto SleepSettings = Survival::GetSettings(Survival::Feature::SleepToLevelUp);
	auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

	uint32_t type;
	uint32_t version;
	uint32_t length;

	while (a_intfc->GetNextRecordInfo(type, version, length))
	{
		if (version != kSerializationVersion)
		{
			logger::error("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)", version, kSerializationVersion, type);
			continue;
		}

		switch (type)
		{
		case kArrowWeight:
			if (!arrowSettings->DeserializeLoad(a_intfc))
			{
				logger::error("Failed to load arrow settings!\n");
			}
			break;

		case kInventoryUI:

			if (!uiSettings->DeserializeLoad(a_intfc))
			{
				logger::error("Failed to load UI settings!\n");
			}
			break;

		case kHUDIndicators:

			if (!HUDSettings->DeserializeLoad(a_intfc))
			{
				logger::error("Failed to load UI settings!\n");
			}
			break;

		case kLockpickWeight:

			if (!lockSettings->DeserializeLoad(a_intfc))
			{
				logger::error("Failed to load UI settings!\n");
			}
			break;

		case kSleepToLevelUp:

			if (!SleepSettings->DeserializeLoad(a_intfc))
			{
				logger::error("Failed to load UI settings!\n");
			}
			break;
		case kWarmthSettings:

			if (!warmthSettings.DeserializeLoad(a_intfc))
			{
				logger::error("Failed to load warmth settings!");
			}
			break;
		default:
			logger::error(FMT_STRING("Unrecognized signature type! {}"), type);
			break;
		}
	}
	logger::info("load callback stop");
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	//SKSE::WinAPI::MessageBox(nullptr,"WAIT FOR DEBUGGER","WAIT", 0x00001010L);
	InitLogger();
	logger::info("Survival Control Panel loading begin");

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(84);

	Papyrus::Register();
	Hooks::Install();
	logger::info("Papyrus registered");

	auto s_interface = SKSE::GetSerializationInterface();
	s_interface->SetUniqueID('SURV');						//<-Handle ID
	s_interface->SetSaveCallback(SaveCallback);
	s_interface->SetLoadCallback(LoadCallBack);
	logger::info("Callbacks set");

	auto m_interface = SKSE::GetMessagingInterface();
	m_interface->RegisterListener("SKSE", [](SKSE::MessagingInterface::Message* a_msg) {
		if (a_msg && a_msg->type == SKSE::MessagingInterface::kNewGame)
		{
			auto userDir = Json::GetUserDirectory();
			auto userDefault = userDir / "default.json";
			auto dataDefault = std::filesystem::path{ "Data/Survival.json" };
			if (std::filesystem::directory_entry{ userDefault }.exists())
			{
				Json::Load(userDefault);
			}
			else if (std::filesystem::directory_entry{ dataDefault }.exists())
			{
				Json::Load(dataDefault);
			}
		}
	});
	
	logger::info("Survival Control Panel loaded");

	return true;
}
