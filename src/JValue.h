#pragma once

namespace JContainers
{
	inline static void* default_domain = nullptr;

	template <class T>
	void obtain_func(const jc::reflection_interface* refl, const char* funcName, const char* className, T& func)
	{
		assert(refl);
		func = reinterpret_cast<T>(refl->tes_function_of_class(funcName, className));
		assert(func);
	}

	using JString = const char*;

	class JValue
	{
	public:
		int32_t ID;

		operator int32_t() {
			return ID;
		}

		void writeToFile(JString filePath)
		{
			_writeToFile(default_domain, ID, filePath);
		}

		static void init(const jc::reflection_interface* refl)
		{
			obtain_func(refl, "readFromFile", "JValue", _readFromFile);
			obtain_func(refl, "writeToFile", "JValue", _writeToFile);
		}

	protected:
		JValue(int32_t id) : ID(id) { }

		JValue(JString filePath) : ID(_readFromFile(default_domain, filePath)) { }

	private:
		using readFromFile_t = int32_t(void*, JString filePath);
		inline static readFromFile_t* _readFromFile;

		using writeToFile_t = void(void*, int32_t object, JString filePath);
		inline static writeToFile_t* _writeToFile;
	};

	class JMap : public JValue
	{
	public:
		JMap() : JValue(_object(default_domain)) { }
		JMap(int32_t id) : JValue(id) { }
		JMap(JString filePath) : JValue(filePath) { }

		operator int32_t() {
			return ID;
		}

		int32_t getInt(JString key) {
			return _getInt(default_domain, ID, key);
		}

		int32_t getObj(JString key) {
			return _getObj(default_domain, ID, key);
		}

		void setInt(JString key, int32_t value) {
			_setInt(default_domain, ID, key, value);
		}

		void setObj(JString key, JValue value) {
			_setObj(default_domain, ID, key, value.ID);
		}

		static void init(const jc::reflection_interface* refl)
		{
			obtain_func(refl, "object", "JMap", _object);
			obtain_func(refl, "getInt", "JMap", _getInt);
			obtain_func(refl, "getObj", "JMap", _getObj);
			obtain_func(refl, "setInt", "JMap", _setInt);
			obtain_func(refl, "setObj", "JMap", _setObj);
		}

	private:
		using object_t = int32_t(void*);
		inline static object_t* _object;

		using getInt_t = int32_t(void*, int32_t map, JString key);
		inline static getInt_t* _getInt;

		using getObj_t = int32_t(void*, int32_t map, JString key);
		inline static getObj_t* _getObj;

		using setInt_t = void(void*, int32_t map, JString key, int32_t value);
		inline static setInt_t* _setInt;

		using setObj_t = void(void*, int32_t map, JString key, int32_t value);
		inline static setObj_t* _setObj;

		using writeToFile_t = void(void*, int32_t object, JString filePath);
		inline static writeToFile_t* _writeToFile;
	};

	class JFormMap : public JValue
	{
	public:
		JFormMap() : JValue(_object(default_domain)) { }
		JFormMap(int32_t id) : JValue(id) { }

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
}
