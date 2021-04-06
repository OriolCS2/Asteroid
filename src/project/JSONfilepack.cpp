#include "JSONfilepack.h"
#include "Parson/parson.h"
#include <stack>

JSONfilepack::JSONfilepack(const std::string& path, JSON_Object*& object, JSON_Value*& value) : JSONparser()
{
	this->path = path;
	this->object = object;
	this->value = value;
}

JSONfilepack::~JSONfilepack()
{
	if (!arrays.empty()) {
		std::vector<JSONArraypack*>::iterator item = arrays.begin();
		for (; item != arrays.end(); ++item) {
			delete* item;
		}
		arrays.clear();
	}

	if (value != nullptr) {
		json_value_free(value);
		value = nullptr;
		object = nullptr;
	}
}

void JSONfilepack::StartSave()
{
	value = json_parse_file(path.data());
	object = json_object(value);

	if (!arrays.empty()) {
		JSONparser::StartSaveArrays(object, arrays);
	}
}

void JSONfilepack::FinishSave()
{
	json_serialize_to_file_pretty(value, path.data());
}

void JSONparser::StartSaveArrays(JSON_Object* obj, const std::vector<JSONArraypack*>& arrays)
{
	for (auto item = arrays.begin(); item != arrays.end(); ++item) {
		(*item)->arr = json_object_dotget_array(obj, (*item)->name.data());
		(*item)->value = json_array_get_value((*item)->arr, (*item)->index);
		(*item)->object = json_value_get_object((*item)->value);
		if (!(*item)->arrays.empty()) {
			JSONparser::StartSaveArrays((*item)->object, (*item)->arrays);
		}
	}
}

void JSONparser::SetNumber(const std::string& name, double number)
{
	json_object_dotset_number(object, name.data(), number);
}

double JSONparser::GetNumber(const std::string& name, double def)
{
	JSON_Value* val = json_object_dotget_value(object, name.data());
	return (json_value_get_type(val) == JSONNumber) ? json_value_get_number_unsafe(val) : def;
}

void JSONparser::SetBoolean(const std::string& name, bool boolean)
{
	json_object_dotset_boolean(object, name.data(), boolean);
}

bool JSONparser::GetBoolean(const std::string& name, bool def)
{
	JSON_Value* val = json_object_dotget_value(object, name.data());
	return (json_value_get_type(val) == JSONBoolean) ? json_value_get_boolean_unsafe(val) : def;
}

void JSONparser::SetFloat3(const std::string& name, const float3& numbers)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
	json_array_append_number(arr, numbers.z);
}

float3 JSONparser::GetFloat3(const std::string& name, float3 def)
{
	JSON_Value* val = json_object_dotget_value(object, name.data());
	if (json_value_get_type(val) == JSONArray) {
		JSON_Array* arr = json_value_get_array_unsafe(val);
		float3 numbers = float3::zero;
		numbers.x = json_array_get_number(arr, 0);
		numbers.y = json_array_get_number(arr, 1);
		numbers.z = json_array_get_number(arr, 2);
		return numbers;
	}
	else {
		return def;
	}
}

void JSONparser::SetFloat2(const std::string& name, const float2& numbers)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
}

float2 JSONparser::GetFloat2(const std::string& name, float2 def)
{
	JSON_Value* val = json_object_dotget_value(object, name.data());
	if (json_value_get_type(val) == JSONArray) {
		JSON_Array* arr = json_value_get_array_unsafe(val);
		float2 numbers = float2::zero;
		numbers.x = json_array_get_number(arr, 0);
		numbers.y = json_array_get_number(arr, 1);
		return numbers;
	}
	else {
		return def;
	}
}

void JSONparser::SetQuat(const std::string& name, const Quat& numbers)
{
	JSON_Array* arr = json_object_dotget_array(object, name.data());
	if (arr == nullptr) {
		JSON_Value* new_val = json_value_init_array();
		arr = json_value_get_array(new_val);
		json_object_dotset_value(object, name.data(), new_val);
	}
	else {
		json_array_clear(arr);
	}
	json_array_append_number(arr, numbers.x);
	json_array_append_number(arr, numbers.y);
	json_array_append_number(arr, numbers.z);
	json_array_append_number(arr, numbers.w);
}

Quat JSONparser::GetQuat(const std::string& name, Quat def)
{
	JSON_Value* val = json_object_dotget_value(object, name.data());
	if (json_value_get_type(val) == JSONArray) {
		JSON_Array* arr = json_value_get_array_unsafe(val);
		Quat quat;
		quat.x = json_array_get_number(arr, 0);
		quat.y = json_array_get_number(arr, 1);
		quat.z = json_array_get_number(arr, 2);
		quat.w = json_array_get_number(arr, 3);
		return quat;
	}
	else {
		return def;
	}
}

void JSONparser::SetString(const std::string& name, const std::string& string_parameter)
{
	json_object_dotset_string(object, name.data(), string_parameter.data());
}

const char* JSONparser::GetString(const std::string& name, const char* def)
{
	JSON_Value* val = json_object_dotget_value(object, name.data());
	return (json_value_get_type(val) == JSONString) ? json_value_get_string_unsafe(val) : def;
}

JSONArraypack* JSONparser::InitNewArray(const std::string& name)
{
	JSON_Value* val = json_value_init_array();
	json_object_dotset_value(object, name.data(), val);

	JSONArraypack* array_pack = new JSONArraypack(json_value_get_array(val), json_value_init_object(), name);
	array_pack->depth = depth + 1;
	arrays.push_back(array_pack);

	return array_pack;
}

JSONArraypack* JSONparser::GetArray(const std::string& name)
{
	JSON_Array* arr = nullptr;

	arr = json_object_dotget_array(object, name.data());

	if (arr != nullptr) {
		JSON_Value* value = json_array_get_value(arr, 0);
		if (value != nullptr) {
			JSONArraypack* array_pack = new JSONArraypack(arr, value, name);
			array_pack->depth = depth + 1;
			arrays.push_back(array_pack);
			return array_pack;
		}
	}
	return nullptr;
}

void JSONparser::ClearArrays()
{
	arrays.clear();
}

JSONArraypack::JSONArraypack(JSON_Array* arr, JSON_Value* value, const std::string& name) : JSONparser()
{
	this->name = name;
	this->arr = arr; 
	this->value = value; 
	object = json_value_get_object(value);
}

JSONArraypack::~JSONArraypack()
{
	if (!arrays.empty()) {
		std::vector<JSONArraypack*>::iterator item = arrays.begin();
		for (; item != arrays.end(); ++item) {
			delete* item;
		}
		arrays.clear();
	}
}

bool JSONArraypack::IsFinished()
{
	return index >= json_array_get_count(arr);
}

void JSONArraypack::SetAnotherNode()
{
	// I hope when destroying the core value of the file everything is deleted :) 
	value = json_value_init_object();
	object = json_value_get_object(value);
	json_array_append_value(arr, value);
}

bool JSONArraypack::GetAnotherNode()
{
	++index;
	if (index < json_array_get_count(arr)) {
		value = json_array_get_value(arr, index);
		object = json_value_get_object(value);
		return true;
	}
	else {
		return false;
	}
}

void JSONArraypack::GetFirstNode()
{
	index = 0;
	value = json_array_get_value(arr, index);
	object = json_value_get_object(value);
}

void JSONArraypack::GetNode(const uint& index)
{
	this->index = index;
	value = json_array_get_value(arr, index);
	object = json_value_get_object(value);
}

uint JSONArraypack::GetArraySize()
{
	return json_array_get_count(arr);
}

void JSONArraypack::ClearArrays()
{
	index = 0;
	if (!arrays.empty()) {
		std::vector<JSONArraypack*>::iterator item = arrays.begin();
		for (; item != arrays.end(); ++item) {
			delete* item;
		}
		arrays.clear();
	}
	json_array_clear(arr);
}

JSONfilepack* JSONfilepack::CreateJSON(const char* path)
{
	JSON_Value* value = json_value_init_object();
	JSON_Object* json_object = json_value_get_object(value);

	if (value == nullptr || json_object == nullptr) {
		return nullptr;
	}

	json_serialize_to_file_pretty(value, path);

	return new JSONfilepack(path, json_object, value);
}

JSONfilepack* JSONfilepack::GetJSON(const char* path)
{
	JSON_Value* value = json_parse_file(path);
	JSON_Object* object = json_value_get_object(value);

	if (value == nullptr || object == nullptr) {
		return nullptr;
	}

	return new JSONfilepack(path, object, value);
}

void JSONfilepack::FreeJSON(JSONfilepack* Jsonpack)
{
	delete Jsonpack;
	Jsonpack = nullptr;
}
