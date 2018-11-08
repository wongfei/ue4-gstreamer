#pragma once

#include "Shared.h"
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGStreamer, Log, All);

template<typename T>
T* NewUniqueObject(const wchar_t* Name)
{
	auto Outer = (UObject*)GetTransientPackage();
	FName UniqName = MakeUniqueObjectName(Outer, T::StaticClass(), Name);
	auto Object = NewObject<T>(Outer, UniqName);
	return Object;
}
