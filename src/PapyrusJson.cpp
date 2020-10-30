#include "PapyrusJson.h"
#include "Json.h"

namespace PapyrusJson
{
	std::vector<RE::BSFixedString> ListFiles(RE::StaticFunctionTag*)
	{
		std::vector<RE::BSFixedString> files;
		auto userDir = Json::GetUserDirectory();
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
		auto fullPath = Json::GetUserDirectory() / std::string_view{ a_filePath };
		fullPath += ".json";

		std::filesystem::directory_entry dir_entry{ fullPath };

		return dir_entry.exists();
	}

	bool Save(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto userDir = Json::GetUserDirectory();
		auto fullPath = userDir / std::string_view{ a_filePath };
		fullPath += ".json";

		return Json::Save(fullPath);
	}

	bool Load(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto userDir = Json::GetUserDirectory();
		auto fullPath = userDir / std::string_view{ a_filePath };
		fullPath += ".json";

		return Json::Load(fullPath);
	}

	bool Delete(RE::StaticFunctionTag*, RE::BSString a_filePath)
	{
		auto userDir = Json::GetUserDirectory();
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
