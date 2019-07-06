#pragma once

#include "ComputeNodePin.h"
#include "FlowDataImpl.h"
#include "Frox.h"

namespace frox {

template<typename T> //, typename ...ArgsT>
struct TLazyValue
{
	FlowDataImplPtr Data;
	Guid Id;

	TLazyValue(FlowDataImplPtr data, Guid Id)
		: Data(data)
		, Id(Id)
	{}

	TLazyValue(const TLazyValue& other)
		: Data(other.Data)
		, Id(other.Id)
	{}

	void SetValue(T value)
	{
		Data->SetValue(Id, value);
	}

	template<typename CallbackT, typename ...ArgsT>
	inline void SetValue(CallbackT callback, ArgsT... args)
	{
		callback(Id, Data, args...);
	}
};

/*
template<typename CallbackT>
void TLazyValue<ComputeFramePtr>::SetValue<Size, ComputeFrameType>(CallbackT callback, Size size, ComputeFrameType type)
{
	ComputeFramePtr frame = Data->GetFrame(Id);
	if (!frame)
	{
		frame = FroxInstance()->CreateComputeFrame(size, type);
	}

	callback(frame);

	SetValue(frame);
}
*/

struct TLazyFrame
{
	FlowDataImplPtr Data;
	Guid Id;

	TLazyFrame(FlowDataImplPtr data, Guid Id)
		: Data(data)
		, Id(Id)
	{}

	TLazyFrame(const TLazyFrame& other)
		: Data(other.Data)
		, Id(other.Id)
	{}

	void SetValue(ComputeFramePtr value) const
	{
		Data->SetFrame(Id, value);
	}

	template<typename CallbackT>
	void SetValue(const Size& size, ComputeFrameType type, CallbackT callback) const
	{
		ComputeFramePtr frame = Data->GetFrame(Id);
		if (!frame || frame->GetSize() != size || frame->GetType() != type)
		{
			frame = FroxInstance()->CreateComputeFrame(size, type);
		}

		callback(frame);

		Data->SetFrame(Id, frame);
		// SetValue(frame);
	}
};

template<typename T>
struct TOutput : public OutputPin
{
	TOutput(const char* name)
		: OutputPin(name)
	{}

	inline TLazyValue<T> GetValue(FlowDataImplPtr data) const
	{
		return TLazyValue<T>(data, Id);
	}
};

template<typename T>
struct TOutputFrame : public OutputPin
{
	TOutputFrame(const char* name)
		: OutputPin(name)
	{}

	inline TLazyFrame GetValue(FlowDataImplPtr data) const
	{
		return TLazyFrame(data, Id);
	}
};

/*
template<>
inline TLazyValue<ComputeFramePtr, Size, ComputeFrameType> TOutput<ComputeFramePtr>::GetValue(FlowDataImplPtr data) const
{
	return TLazyValue<ComputeFramePtr, Size, ComputeFrameType>(data, Id);
	// Expr ? Expr->GetValue<T>(data) : DefaultValue;
}
*/


} // End frox
