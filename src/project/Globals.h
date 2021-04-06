#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 ) // Warning that exceptions are disabled

#include <windows.h>
#include <stdio.h>

enum class LogType;

#define LOG(format, ...) log((LogType)0, format, __VA_ARGS__);
#define LOG_W(format, ...) log((LogType)1, format, __VA_ARGS__);
#define LOG_E(format, ...) log((LogType)2, format, __VA_ARGS__);

void log(LogType type, const char* format, ...);


typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};