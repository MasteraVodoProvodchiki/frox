#pragma once

#include "Common.h"
#include <memory>

namespace frox {

struct ComputeDataInitializer
{};

class ComputeData
{
public:
	ComputeData(const ComputeDataInitializer& initializer)
	{}

	virtual ~ComputeData() {}

	virtual const char* GetType() const = 0;

	template<typename T>
	bool IsA() const
	{
		// TODO. Add type class
		return strcmp(this->GetType(), T::GetTypeStatic()) == 0;
	}

	template<typename T>
	const T* As() const
	{
		return IsA<T>() ? reinterpret_cast<T*>(this) : nullptr;
	}

	template<typename T>
	T* As()
	{
		return IsA<T>() ? reinterpret_cast<T*>(this) : nullptr;
	}
};
using ComputeDataPtr = std::shared_ptr<ComputeData>;

} // End frox