#pragma once

namespace PapyrusJson
{
	std::vector<std::string> ListFiles(RE::StaticFunctionTag*);
	bool Exists(RE::StaticFunctionTag*, std::string a_filePath);
	bool Save(RE::StaticFunctionTag*, std::string a_filePath);
	bool Load(RE::StaticFunctionTag*, std::string a_filePath);
	bool Delete(RE::StaticFunctionTag*, std::string a_filePath);

	std::filesystem::path ExpandPath(const std::string& a_filename);

	bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm);
}
