#pragma once

#include <stdint.h>
#include <stddef.h>

// CONFIG

#define GST_ENABLE_DEBUG_LOG

// LOG

namespace EGstVerbosity
{
	enum Type : uint8_t
	{
		Error = 0,
		Warning,
		Info,
		Debug,
		_COUNT
	};
}

void GstSetVerbosity(EGstVerbosity::Type Verbosity);
EGstVerbosity::Type GstGetVerbosity();

void GstLogA(const char* File, int Line, EGstVerbosity::Type Verbosity, const char* Format, ...);
void GstLogW(const char* File, int Line, EGstVerbosity::Type Verbosity, const wchar_t* Format, ...);

#define GST_LOG_ERR_A(format, ...) GstLogA(__FILE__, __LINE__, EGstVerbosity::Error, format, ##__VA_ARGS__)
#define GST_LOG_ERR(format, ...) GstLogW(__FILE__, __LINE__, EGstVerbosity::Error, format, ##__VA_ARGS__)

#define GST_LOG_WARN_A(format, ...) GstLogA(__FILE__, __LINE__, EGstVerbosity::Warning, format, ##__VA_ARGS__)
#define GST_LOG_WARN(format, ...) GstLogW(__FILE__, __LINE__, EGstVerbosity::Warning, format, ##__VA_ARGS__)

#define GST_LOG_INFO_A(format, ...) GstLogA(__FILE__, __LINE__, EGstVerbosity::Info, format, ##__VA_ARGS__)
#define GST_LOG_INFO(format, ...) GstLogW(__FILE__, __LINE__, EGstVerbosity::Info, format, ##__VA_ARGS__)

#if defined(GST_ENABLE_DEBUG_LOG)
	#define GST_LOG_DBG_A(format, ...) GstLogA(__FILE__, __LINE__, EGstVerbosity::Debug, format, ##__VA_ARGS__)
	#define GST_LOG_DBG(format, ...) GstLogW(__FILE__, __LINE__, EGstVerbosity::Debug, format, ##__VA_ARGS__)
#else
	#define GST_LOG_DBG_A(format, ...)
	#define GST_LOG_DBG(format, ...)
#endif

// PROFILER

#define INIT_PROFILER
#define SHUT_PROFILER
#define SCOPED_PROFILER
#define NAMED_PROFILER(name)

// UTILS

void* SysLoadLibrary(const wchar_t* Name);
void SysFreeLibrary(void* Handle);
void* SysGetProcAddress(void* Handle, const char* Name);

template<typename T>
inline void SafeDestroy(T*& Obj) { if (Obj) { Obj->Destroy(); Obj = nullptr; } }
