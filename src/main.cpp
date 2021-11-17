#include "Hooks.h"
#include "Json.h"
#include "Papyrus.h"
#include "Survival.h"
#include "Version.h"
#include "Warmth.h"

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

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version =
	[]() {
		SKSE::PluginVersionData v{};
		v.pluginVersion = Version::MAJOR;
		v.PluginName(Version::PROJECT);
		v.AuthorName("Parapets and colinswrath"sv);
		v.CompatibleVersions({ SKSE::RUNTIME_1_6_318 });
		return v;
	}();

void SaveCallback(SKSE::SerializationInterface* a_intfc)
{
	logger::trace("Save callback start"sv);

	auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);
	auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);
	auto HUDSettings = Survival::GetSettings(Survival::Feature::HUDIndicators);
	auto lockSettings = Survival::GetSettings(Survival::Feature::LockpickWeight);
	auto SleepSettings = Survival::GetSettings(Survival::Feature::SleepToLevelUp);
	auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

	if (!arrowSettings->SerializeSave(a_intfc, kArrowWeight, kSerializationVersion)) {
		logger::error("Failed to save arrow settings!"sv);
	}

	if (!uiSettings->SerializeSave(a_intfc, kInventoryUI, kSerializationVersion)) {
		logger::error("Failed to save Inventory UI Settings!"sv);
	}

	if (!HUDSettings->SerializeSave(a_intfc, kHUDIndicators, kSerializationVersion)) {
		logger::error("Failed to save Hud Settings Settings!"sv);
	}

	if (!lockSettings->SerializeSave(a_intfc, kLockpickWeight, kSerializationVersion)) {
		logger::error("Failed to save lockpick Settings!"sv);
	}

	if (!SleepSettings->SerializeSave(a_intfc, kSleepToLevelUp, kSerializationVersion)) {
		logger::error("Failed to save Sleep to level Settings!"sv);
	}

	if (!warmthSettings.SerializeSave(a_intfc, kWarmthSettings, kSerializationVersion)) {
		logger::error("Failed to save Warmth Settings!"sv);
	}
	logger::trace("Save callback stop");
}

void LoadCallBack(SKSE::SerializationInterface* a_intfc)
{
	logger::trace("load callback start"sv);
	auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);
	auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);
	auto HUDSettings = Survival::GetSettings(Survival::Feature::HUDIndicators);
	auto lockSettings = Survival::GetSettings(Survival::Feature::LockpickWeight);
	auto SleepSettings = Survival::GetSettings(Survival::Feature::SleepToLevelUp);
	auto& warmthSettings = Survival::WarmthSettings::GetSingleton();

	uint32_t type;
	uint32_t version;
	uint32_t length;

	while (a_intfc->GetNextRecordInfo(type, version, length)) {
		if (version != kSerializationVersion) {
			logger::error("Loaded data is out of date! Read (%u), expected (%u) for type code (%s)"sv, version, kSerializationVersion, type);
			continue;
		}

		switch (type) {
		case kArrowWeight:
			if (!arrowSettings->DeserializeLoad(a_intfc)) {
				logger::error("Failed to load arrow settings!"sv);
			}
			break;

		case kInventoryUI:

			if (!uiSettings->DeserializeLoad(a_intfc)) {
				logger::error("Failed to load UI settings!"sv);
			}
			break;

		case kHUDIndicators:

			if (!HUDSettings->DeserializeLoad(a_intfc)) {
				logger::error("Failed to load UI settings!"sv);
			}
			break;

		case kLockpickWeight:

			if (!lockSettings->DeserializeLoad(a_intfc)) {
				logger::error("Failed to load UI settings!"sv);
			}
			break;

		case kSleepToLevelUp:

			if (!SleepSettings->DeserializeLoad(a_intfc)) {
				logger::error("Failed to load UI settings!"sv);
			}
			break;
		case kWarmthSettings:

			if (!warmthSettings.DeserializeLoad(a_intfc)) {
				logger::error("Failed to load warmth settings!"sv);
			}
			break;
		default:
			logger::error(FMT_STRING("Unrecognized signature type! {}"), type);
			break;
		}
	}
	logger::trace("load callback stop"sv);
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitLogger();
	logger::trace("Survival Control Panel loading begin"sv);

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(84);

	Papyrus::Register();
	Hooks::Install();
	logger::info("Registered Papyrus functions"sv);

	auto s_interface = SKSE::GetSerializationInterface();
	s_interface->SetUniqueID('SURV');  //<-Handle ID
	s_interface->SetSaveCallback(SaveCallback);
	s_interface->SetLoadCallback(LoadCallBack);
	logger::trace("Callbacks set"sv);

	auto m_interface = SKSE::GetMessagingInterface();
	m_interface->RegisterListener("SKSE", [](SKSE::MessagingInterface::Message* a_msg) {
		if (a_msg && a_msg->type == SKSE::MessagingInterface::kNewGame) {
			auto userDir = Json::GetUserDirectory();
			auto userDefault = userDir / "default.json";
			auto dataDefault = std::filesystem::path{ "Data/Survival.json" };
			if (std::filesystem::directory_entry{ userDefault }.exists()) {
				Json::Load(userDefault);
			} else if (std::filesystem::directory_entry{ dataDefault }.exists()) {
				Json::Load(dataDefault);
			}
		}
	});

	logger::info("{} loaded"sv, Version::PROJECT);

	return true;
}
