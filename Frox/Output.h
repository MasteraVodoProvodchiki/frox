#pragma once

#include "ComputeNodePin.h"
#include "FlowDataImpl.h"
#include "Frox.h"

#include "ComputeFrame.h"
#include "ComputeData.h"

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

	void SetValue(T value) const
	{
		Data->SetValue(Id, value);
	}

	template<typename CallbackT, typename ...ArgsT>
	inline void SetValue(CallbackT callback, ArgsT... args) const
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

// Data
struct TLazyData
{
	FlowDataImplPtr Data;
	Guid Id;

	TLazyData(FlowDataImplPtr data, Guid Id)
		: Data(data)
		, Id(Id)
	{}

	TLazyData(const TLazyData& other)
		: Data(other.Data)
		, Id(other.Id)
	{}

	void SetValue(ComputeDataPtr value) const
	{
		Data->SetData(Id, value);
	}

	template<typename DataT, typename CallbackT>
	void SetValue(CallbackT callback) const
	{
		ComputeDataPtr data = Data->GetData(Id);
		if (!data || !data->IsA<DataT>())
		{
			data = FroxInstance()->CreateComputeData<DataT>();
		}

		callback(data);

		Data->SetData(Id, data);
	}
};

/*
namespace utils {

template<typename ...TypesT>
struct TLazyDataComposer
{
	template<typename CallbackT, typename FirstT, typename ...ArgsT>
	void SetValues(FirstT first, ArgsT... args, CallbackT callback) const
	{
	}
};

} // End utils
*/

template<typename T>
struct TOutputData : public OutputPin
{
	TOutputData(const char* name)
		: OutputPin(name)
	{}

	inline TLazyData GetValue(FlowDataImplPtr data) const
	{
		return TLazyData(data, Id);
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
