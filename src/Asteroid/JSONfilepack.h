#pragma once

#include <string>
#include "MathGeoLib/include/MathGeoLib.h"

typedef unsigned int uint;
typedef struct json_object_t JSON_Object;
typedef struct json_array_t  JSON_Array;
typedef struct json_value_t  JSON_Value;

class JSONfilepack;
class JSONArraypack;

class JSONparser {
	friend class JSONfilepack;
	friend class JSONArraypack;
public:

	JSONparser() {}
	virtual ~JSONparser() {}

	void SetNumber(const std::string& name, double number);
	double GetNumber(const std::string& name, double def = 0);

	void SetBoolean(const std::string& name, bool boolean);
	bool GetBoolean(const std::string& name, bool def = false);

	void SetFloat3(const std::string& name, const float3& numbers);
	float3 GetFloat3(const std::string& name, float3 def = float3::zero);

	void SetFloat2(const std::string& name, const float2& numbers);
	float2 GetFloat2(const std::string& name, float2 def = float2::zero);

	void SetQuat(const std::string& name, const Quat& numbers);
	Quat GetQuat(const std::string& name, Quat def = Quat::identity);

	void SetString(const std::string& name, const std::string& string_parameter);
	const char* GetString(const std::string& name, const char* def = "Default");

	JSONArraypack* InitNewArray(const std::string& name);
	JSONArraypack* GetArray(const std::string& name);
	virtual void ClearArrays();

private:

	static void StartSaveArrays(JSON_Object* obj, const std::vector<JSONArraypack*>& arrays);

public:
	std::vector<JSONArraypack*> arrays;
	int depth = 0;
	JSON_Object* object = nullptr;
	JSON_Value* value = nullptr;
};

class JSONfilepack : public JSONparser {
public:

	JSONfilepack(const std::string& path, JSON_Object*& object, JSON_Value*& value);
	~JSONfilepack();

	static JSONfilepack* CreateJSON(const char* path);
	static JSONfilepack* GetJSON(const char* path);
	static void FreeJSON(JSONfilepack* Jsonpack);

	void StartSave();
	void FinishSave();

private:

	std::string path;
};

class JSONArraypack : public JSONparser {
	friend class JSONparser;
public:

	JSONArraypack(JSON_Array* arr, JSON_Value* value, const std::string& name);
	~JSONArraypack();

	bool IsFinished();
	void SetAnotherNode();
	bool GetAnotherNode();
	void GetFirstNode();
	void GetNode(const uint& index);
	uint GetArraySize();

	void ClearArrays() override;

public:

	std::string name;

private:

	JSON_Array* arr = nullptr;
	uint index = 0;
};