#include "Hooks/Hooks.h"
#include "Papyrus/Papyrus.h"
#include "SCP/Json.h"
#include "SCP/Serialization.h"

void InitLogger()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= fmt::format("{}.log"sv, Plugin::NAME);
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
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version =
	[]() {
		SKSE::PluginVersionData v{};
		v.PluginVersion(Plugin::VERSION);
		v.PluginName(Plugin::NAME);
		v.AuthorName("Parapets and colinswrath"sv);
		v.UsesAddressLibrary(true);
		return v;
	}();

extern "C" DLLEXPORT bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	InitLogger();
	logger::info("{} v{}"sv, Plugin::NAME, Plugin::VERSION.string());

	SKSE::Init(a_skse);
	SKSE::AllocTrampoline(98);

	Papyrus::Register();
	Hooks::Install();
	logger::info("Registered Papyrus functions"sv);

	auto s_interface = SKSE::GetSerializationInterface();
	s_interface->SetUniqueID(Serialization::kHandleId);
	s_interface->SetSaveCallback(Serialization::SaveCallBack);
	s_interface->SetLoadCallback(Serialization::LoadCallBack);
	logger::trace("Callbacks set"sv);

	auto m_interface = SKSE::GetMessagingInterface();
	m_interface->RegisterListener(
		[](SKSE::MessagingInterface::Message* a_msg) {
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

	logger::info("{} loaded"sv, Plugin::NAME);

	return true;
}
