#include "version.h"
#include "Hooks.h"
#include "Papyrus.h"
#include "Survival.h"

enum
{
	kSerializationVersion = 3,
	kHUDIndicators = 'HUDS',
	kInventoryUI = 'IUIS',
	kSleepToLevelUp = 'STLS',
	kArrowWeight = 'ARWS',
	kLockpickWeight = 'LKPS',
};

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

	logger::info(FMT_STRING("Survival Decoupler v{}"), MYFP_VERSION_VERSTRING);

	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = "Survival Decoupler";
	a_info->version = MYFP_VERSION_MAJOR;

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

void SaveCallback(SKSE::SerializationInterface* a_intfc)
{
	auto arrowSettings = Survival::GetSettings(Survival::Feature::ArrowWeight);

	if (!arrowSettings->SerializeSave(a_intfc, kArrowWeight, kSerializationVersion)) {
		logger::error("Failed to save arrow settings!\n");
		//ammo->Clear();
	}

	auto uiSettings = Survival::GetSettings(Survival::Feature::InventoryUI);

	if (!uiSettings->SerializeSave(a_intfc, kInventoryUI, kSerializationVersion)) {
		logger::error("Failed to save Inventory UI Settings!\n");
		//ammo->Clear();
	}

}

void LoadCallBack(SKSE::SerializationInterface* a_intfc)
{
	std::vector<Survival::Preference> arr;

	uint32_t type;
	uint32_t version;
	uint32_t length;
	while (a_intfc->GetNextRecordInfo(type, version, length))
	{
		std::size_t size;
		if (!a_intfc->ReadRecordData(size))
		{
			logger::error("Failed to load size!");
			break;
		}

		for (uint32_t i = 0; i < size; ++i)
		{

			Survival::Preference elem;
			if (!a_intfc->ReadRecordData(elem))
			{
				logger::error("Failed to load elem!");
				break;
			}
			else
			{
				logger::info(FMT_STRING("deserialized {}"), elem);
				arr.push_back(elem);
			}
		}
	}
}

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	logger::info("Survival Decoupler loaded");

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(128); // TODO figure out exactly how much we need

	Papyrus::Register();
	Hooks::Install();

	auto s_interface = SKSE::GetSerializationInterface();
	s_interface->SetUniqueID('SURV');						//<-Handle ID
	s_interface->SetSaveCallback(SaveCallback);
	s_interface->SetLoadCallback(LoadCallBack);

	return true;
}