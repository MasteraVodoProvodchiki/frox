#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "ComputeData.h"

namespace frox {

enum ELogType
{
	Info = 0,
	Warning,
	Error
};

class ILogListerner
{
public:
	virtual void OnFroxLog(ELogType type, const char* message, const char* space = nullptr) = 0;
};

struct FroxDesc
{
	class ILogListerner* LogListerner = nullptr;
};

enum class EFrameLoaderType
{
	Default = 0,
	Gamex = 1
};

class Frox
{
public:
	virtual ~Frox() {}

	virtual ComputeFlow* CreateComputeFlow() = 0;
	virtual void DestroyComputeFlow(ComputeFlow* computeFlow) = 0;

	virtual FlowData* CreateFlowData() = 0;
	virtual void DestroyFlowData(FlowData* flowData) = 0;

	virtual FlowPerformer* CreateFlowPerformer(IComputeFlowListerner* listerner = nullptr) = 0;
	virtual void DestroyFlowPerformer(FlowPerformer* performer) = 0;

	virtual ComputeFramePtr CreateComputeFrame(Size size = Size{ 0, 0 }, ComputeFrameType type = ComputeFrameType{ ECFT_None, 0 }, const void* data = nullptr) = 0;
	virtual ComputeFramePtr LoadComputeFrame(const char* path, EFrameLoaderType loader = EFrameLoaderType::Default) = 0;
	virtual void DestroyComputeFrame(ComputeFramePtr computeFlow) = 0;

	ComputeFramePtr CreateComputeFrame(Size size, EComputeFrameType type, const void* data = nullptr)
	{
		return this->CreateComputeFrame(size, ComputeFrameType{ type, 1}, data);
	}

	ComputeFramePtr CreateComputeFrame(Size size, EComputeFrameType type, uint32_t channels, const void* data = nullptr)
	{
		return this->CreateComputeFrame(size, ComputeFrameType{ type, channels }, data);
	}

	ComputeFramePtr CreateScalar(uint8_t value)
	{
		return CreateComputeFrame(Size{ 1, 1 }, ComputeFrameType{ ECFT_UInt8, 1 }, &value);
	}

	ComputeFramePtr CreateScalar(uint16_t value)
	{
		return CreateComputeFrame(Size{ 1, 1 }, ComputeFrameType{ ECFT_UInt16, 1 }, &value);
	}

	ComputeFramePtr CreateScalar(uint32_t value)
	{
		return CreateComputeFrame(Size{ 1, 1 }, ComputeFrameType{ ECFT_UInt32, 1 }, &value);
	}

	ComputeFramePtr CreateScalar(float value)
	{
		return CreateComputeFrame(Size{ 1, 1 }, ComputeFrameType{ ECFT_Float, 1 }, &value);
	}

	virtual ComputeDataPtr CreateComputeData(const char* type) = 0;

	template<typename ComputeDataT>
	ComputeDataPtr CreateComputeData()
	{
		return this->CreateComputeData(ComputeDataT::GetTypeStatic());
	}
};

FROX_API Frox* FroxInit(const FroxDesc& desc = FroxDesc());
FROX_API void FroxShutdown(Frox* frox);
FROX_API Frox* FroxInstance();

/**
 * Get library version number
 */
FROX_API int FroxGetVersion();

} // End frox