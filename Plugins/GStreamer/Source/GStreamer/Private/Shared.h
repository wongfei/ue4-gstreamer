#pragma once

#include <stdint.h>
#include <stddef.h>

// CONFIG

//#define GST_ENABLE_DEBUG_LOG

// LOG

namespace EGstVerbosity
{
	enum Type : uint8_t
	{
		Error = 0,
		Warning,
		Debug
	};
}

#define GST_ERR_VERBOSITY EGstVerbosity::Error
#define GST_DBG_VERBOSITY EGstVerbosity::Debug

void GstLogA(const char* File, int Line, EGstVerbosity::Type Verbosity, const char* Format, ...);
void GstLogW(const char* File, int Line, EGstVerbosity::Type Verbosity, const wchar_t* Format, ...);

#define GST_LOG_ERR_A(format, ...) GstLogA(__FILE__, __LINE__, GST_ERR_VERBOSITY, format, ##__VA_ARGS__)
#define GST_LOG_ERR(format, ...) GstLogW(__FILE__, __LINE__, GST_ERR_VERBOSITY, format, ##__VA_ARGS__)

#if defined(GST_ENABLE_DEBUG_LOG)
	#define GST_LOG_DBG_A(format, ...) GstLogA(__FILE__, __LINE__, GST_DBG_VERBOSITY, format, ##__VA_ARGS__)
	#define GST_LOG_DBG(format, ...) GstLogW(__FILE__, __LINE__, GST_DBG_VERBOSITY, format, ##__VA_ARGS__)
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
