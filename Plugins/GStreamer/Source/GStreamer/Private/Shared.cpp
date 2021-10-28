#include "Shared.h"
#include "SharedUnreal.h"
#include "HAL/PlatformProcess.h"

#include <stdarg.h>
#include <stdio.h>

#define LOG_BUF_SIZE 2048

static EGstVerbosity::Type _GstVerbosity = EGstVerbosity::Error;

void GstSetVerbosity(EGstVerbosity::Type Verbosity)
{
	_GstVerbosity = Verbosity;
}

EGstVerbosity::Type GstGetVerbosity()
{
	return _GstVerbosity;
}

static ELogVerbosity::Type ConvertVerbosity(EGstVerbosity::Type GstVerbosity)
{
	switch (GstVerbosity)
	{
		case EGstVerbosity::Error: return ELogVerbosity::Error;
		case EGstVerbosity::Warning: return ELogVerbosity::Warning;
		case EGstVerbosity::Info: return ELogVerbosity::Display;
		case EGstVerbosity::Debug: return ELogVerbosity::Display;
	}
	return ELogVerbosity::Display;
}

void GstLogA(const char* File, int Line, EGstVerbosity::Type Verbosity, const char* Format, ...)
{
#if !NO_LOGGING
	if (Verbosity > _GstVerbosity)
		return;

	char Buffer[LOG_BUF_SIZE];
	va_list Argptr;
	va_start(Argptr, Format);
	const int n = vsnprintf(Buffer, LOG_BUF_SIZE - 1, Format, Argptr);
	va_end(Argptr);

	if (n > 0)
	{
		Buffer[n] = 0;
		FString Message(ANSI_TO_TCHAR(Buffer));
		FMsg::Logf_Internal(File, Line, LogGStreamer.GetCategoryName(), ConvertVerbosity(Verbosity), TEXT("%s"), *Message);
	}
#endif
}

void GstLogW(const char* File, int Line, EGstVerbosity::Type Verbosity, const wchar_t* Format, ...)
{
#if !NO_LOGGING
	if (Verbosity > _GstVerbosity)
		return;

	wchar_t Buffer[LOG_BUF_SIZE];
	va_list Argptr;
	va_start(Argptr, Format);
	#if PLATFORM_WINDOWS
		const int n = _vsnwprintf_s(Buffer, LOG_BUF_SIZE - 1, Format, Argptr);
	#else
		const int n = vswprintf(Buffer, LOG_BUF_SIZE - 1, Format, Argptr);
	#endif
	va_end(Argptr);

	if (n > 0)
	{
		Buffer[n] = 0;
		FMsg::Logf_Internal(File, Line, LogGStreamer.GetCategoryName(), ConvertVerbosity(Verbosity), TEXT("%s"), Buffer);
	}
#endif
}

void* SysLoadLibrary(const wchar_t* Name)
{
	return FPlatformProcess::GetDllHandle(Name);
}

void SysFreeLibrary(void* Handle)
{
	FPlatformProcess::FreeDllHandle(Handle);
}

void* SysGetProcAddress(void* Handle, const char* Name)
{
	return FPlatformProcess::GetDllExport(Handle, ANSI_TO_TCHAR(Name));
}
