#include "PapyrusJson.h"
#include "Json.h"

namespace PapyrusJson
{
	std::vector<std::string> ListFiles(RE::StaticFunctionTag*)
	{
		std::vector<std::string> files;

		auto userDir = Json::GetUserDirectory();
		std::filesystem::directory_entry dir_entry{ userDir };
		if (!dir_entry.exists()) {
			return files;
		}

		std::filesystem::directory_iterator dirContents{ userDir };

		for (auto& file : dirContents) {
			if (!file.is_regular_file())
				continue;

			if (file.path().extension() == ".json") {
				files.push_back(file.path().stem().string());
			}
		}

		return files;
	}

	bool Exists(RE::StaticFunctionTag*, std::string a_filePath)
	{
		auto fullPath = ExpandPath(a_filePath);
		std::filesystem::directory_entry dir_entry{ fullPath };

		return dir_entry.exists();
	}

	bool Save(RE::StaticFunctionTag*, std::string a_filePath)
	{
		auto fullPath = ExpandPath(a_filePath);
		return Json::Save(fullPath);
	}

	bool Load(RE::StaticFunctionTag*, std::string a_filePath)
	{
		auto fullPath = ExpandPath(a_filePath);
		return Json::Load(fullPath);
	}

	bool Delete(RE::StaticFunctionTag*, std::string a_filePath)
	{
		auto fullPath = ExpandPath(a_filePath);
		std::filesystem::remove(fullPath);
		return true;
	}

	std::filesystem::path ExpandPath(const std::string& a_filename)
	{
		std::string lower = a_filename;
		std::transform(
			std::execution::unseq,
			lower.begin(),
			lower.end(),
			lower.begin(),
			[](char c) { return static_cast<char>(std::tolower(c)); });

		if (lower.starts_with("data"sv)) {
			return a_filename;
		}

		auto userDir = Json::GetUserDirectory();
		std::filesystem::directory_entry dir_entry{ userDir };
		if (!dir_entry.exists()) {
			return std::filesystem::path{};
		}

		auto fullPath = userDir / a_filename;
		fullPath += ".json";

		return fullPath;
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
