#pragma once

namespace Json
{
	std::vector<RE::BSFixedString> ListFiles(RE::StaticFunctionTag*);
	bool Exists(RE::StaticFunctionTag*, RE::BSString a_filePath);
	bool Save(RE::StaticFunctionTag*, RE::BSString a_filePath);
	bool Load(RE::StaticFunctionTag*, RE::BSString a_filePath);
	bool Delete(RE::StaticFunctionTag*, RE::BSString a_filePath);

	bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm);
}
