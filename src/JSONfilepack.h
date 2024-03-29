#pragma once

#include <string>
#include "MathGeoLib/include/MathGeoLib.h"
#include "SDL/include/SDL_scancode.h"
#include "Color.h"
#include "Parson/parson.h"

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

	static JSONfilepack* CreateJSON(const char* path);
	static JSONfilepack* GetJSON(const char* path);
	static void FreeJSON(JSONfilepack* Jsonpack);

	void SetNumber(const std::string& name, double number);
	double GetNumber(const std::string& name, double def = 0);

	void SetStringArray(const std::string& name, std::string* strings, int size);
	std::string* GetStringArray(const std::string& name, std::string* strings, int* size);

	JSONArraypack* InitNewArray(const std::string& name);
	JSONArraypack* GetArray(const std::string& name);
	virtual void ClearArrays();

private:

	static void StartSaveArrays(JSON_Object* obj, const std::vector<JSONArraypack*>& arrays);

public:
	std::vector<JSONArraypack*> arrays;
	JSON_Object* object = nullptr;
	JSON_Value* value = nullptr;
};

class JSONfilepack : public JSONparser {
public:

	JSONfilepack(const std::string& path, JSON_Object*& object, JSON_Value*& value);
	JSONfilepack(JSON_Object*& object, JSON_Value*& value);
	~JSONfilepack();

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

	void SetAnotherNode();
	bool GetAnotherNode();
	uint GetArraySize();

	void ClearArrays() override;

public:

	std::string name;

private:

	JSON_Array* arr = nullptr;
	uint index = 0;
};