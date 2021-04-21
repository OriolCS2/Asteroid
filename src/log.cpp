#pragma once
#include "Globals.h"

#include "Application.h"
#include "ModuleUI.h"

void log(LogType log_type, const char* format, ...)
{
	static char tmp_string[4096];
	static va_list  ap;
	
	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);

	OutputDebugString(std::string('\n' + tmp_string).data());
}