#pragma once

#include "JContainers/jc_interface.h"

namespace JContainers
{
	using VM = RE::BSScript::IVirtualMachine;

	inline static void* default_domain = nullptr;

	template <class T>
	void obtain_func(const jc::reflection_interface* refl, const char* funcName, const char* className, T& func)
	{
		assert(refl);
		func = reinterpret_cast<T>(refl->tes_function_of_class(funcName, className));
		assert(func);
	}

	using readFromFile_t = int32_t(void*, const char* filePath);
	inline static readFromFile_t* _readFromFile;

	inline int32_t readFromFile(const char* filePath) {
		return _readFromFile(default_domain, filePath);
	}

	struct JMap
	{
		int32_t ID = _object(default_domain);

		operator int32_t() {
			return ID;
		}

		int32_t getInt(const char* key) {
			return _getInt(default_domain, ID, key);
		}

		int32_t getObj(const char* key) {
			return _getObj(default_domain, ID, key);
		}

		void setInt(const char* key, int32_t value) {
			_setInt(default_domain, ID, key, value);
		}

		void setObj(const char* key, int32_t value) {
			_setObj(default_domain, ID, key, value);
		}

		void writeToFile(const char* filePath) {
			_writeToFile(default_domain, ID, filePath);
		}

		static void init(const jc::reflection_interface* refl)
		{
			obtain_func(refl, "object", "JMap", _object);
			obtain_func(refl, "getInt", "JMap", _getInt);
			obtain_func(refl, "getObj", "JMap", _getObj);
			obtain_func(refl, "setInt", "JMap", _setInt);
			obtain_func(refl, "setObj", "JMap", _setObj);
			obtain_func(refl, "writeToFile", "JValue", _writeToFile);
		}

	private:
		using object_t = int32_t(void*);
		inline static object_t* _object;

		using getInt_t = int32_t(void*, int32_t map, const char* key);
		inline static getInt_t* _getInt;

		using getObj_t = int32_t(void*, int32_t map, const char* key);
		inline static getObj_t* _getObj;

		using setInt_t = void(void*, int32_t map, const char* key, int32_t value);
		inline static setInt_t* _setInt;

		using setObj_t = void(void*, int32_t map, const char* key, int32_t value);
		inline static setObj_t* _setObj;

		using writeToFile_t = void(void*, int32_t object, const char* filePath);
		inline static writeToFile_t* _writeToFile;
	};

	struct JFormMap
	{
		int32_t ID = _object(default_domain);

		operator int32_t() {
			return ID;
		}

		int32_t count() {
			return _count(default_domain, ID);
		}

		RE::TESForm** allKeys() {
			return _allKeysPArray(default_domain, ID);
		}

		int32_t getInt(RE::TESForm* key) {
			return _getInt(default_domain, ID, key);
		}

		void setInt(RE::TESForm* key, int32_t value) {
			_setInt(default_domain, ID, key, value);
		}

		static void init(const jc::reflection_interface* refl)
		{
			obtain_func(refl, "object", "JFormMap", _object);
			obtain_func(refl, "count", "JFormMap", _count);
			obtain_func(refl, "allKeysPArray", "JFormMap", _allKeysPArray);
			obtain_func(refl, "getInt", "JFormMap", _getInt);
			obtain_func(refl, "setInt", "JFormMap", _setInt);
		}

	private:
		using object_t = int32_t(void*);
		inline static object_t* _object;

		using count_t = int32_t(void*, int32_t map);
		inline static count_t* _count;

		using allKeysPArray_t = RE::TESForm**(void*, int32_t map);
		inline static allKeysPArray_t* _allKeysPArray;

		using getInt_t = int32_t(void*, int32_t map, RE::TESForm* key);
		inline static getInt_t* _getInt;

		using setInt_t = void(void*, int32_t map, RE::TESForm* key, int32_t value);
		inline static setInt_t* _setInt;
	};

	inline void OnJCAPIAvailable(const jc::root_interface* root)
	{
		logger::info("OnJCAPIAvailable");

		auto refl = root->query_interface<jc::reflection_interface>();

		obtain_func(refl, "readFromFile", "JValue", _readFromFile);
		JMap::init(refl);
		JFormMap::init(refl);

		default_domain = root->query_interface<jc::domain_interface>()->get_default_domain();
	}

	void Save(RE::StaticFunctionTag*, RE::BSString a_filePath);
	void Load(RE::StaticFunctionTag*, RE::BSString a_filePath);

	bool RegisterFuncs(VM* a_vm);
}
