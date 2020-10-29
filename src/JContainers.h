#pragma once

#include "JContainers/jc_interface.h"
#include "JValue.h"

namespace JContainers
{
	inline void OnJCAPIAvailable(const jc::root_interface* root)
	{
		logger::info("OnJCAPIAvailable");

		auto refl = root->query_interface<jc::reflection_interface>();

		JValue::init(refl);
		JMap::init(refl);
		JFormMap::init(refl);

		default_domain = root->query_interface<jc::domain_interface>()->get_default_domain();
	}

	bool IsAvailable(RE::StaticFunctionTag*);
	void Save(RE::StaticFunctionTag*, RE::BSString a_filePath);
	void Load(RE::StaticFunctionTag*, RE::BSString a_filePath);

	bool RegisterFuncs(RE::BSScript::IVirtualMachine* a_vm);
}
