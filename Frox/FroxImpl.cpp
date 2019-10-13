#include "FroxImpl.h"
#include "Log.h"
#include "BasicComputeFlow.h"
#include "BasicFlowPerformer.h"
#include "BasicFlowData.h"

#ifndef WITHOUT_OPENCV
#include "OpenCVComputeFrameImpl.h"
#else
#include "ComputeFrameImplV1.h"
#endif

#include "ComputeDataFactory.h"

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <fstream>

namespace frox {

FroxImpl::FroxImpl(const FroxDesc& desc)
{
	Log::SetListerner(desc.LogListerner);
}

FroxImpl::~FroxImpl()
{
	assert(_flows.empty());
	assert(_datas.empty());
	assert(_performers.empty());
}

ComputeFlow* FroxImpl::CreateComputeFlow()
{
	ComputeFlow* flow =  BasicComputeFlow::Create();
	_flows.push_back(flow);
	return flow;
}

void FroxImpl::DestroyComputeFlow(ComputeFlow* computeFlow)
{
	auto it = std::remove_if(_flows.begin(), _flows.end(), [computeFlow](ComputeFlow* other) {
		return other == computeFlow;
	});
	_flows.erase(it);

	delete computeFlow;
}

FlowData* FroxImpl::CreateFlowData()
{
	FlowData* data = BasicFlowData::Create();
	_datas.push_back(data);
	return data;
}

void FroxImpl::DestroyFlowData(FlowData* flowData)
{
	auto it = std::remove_if(_datas.begin(), _datas.end(), [flowData](FlowData* other) {
		return other == flowData;
	});
	_datas.erase(it);

	delete flowData;
}

FlowPerformer* FroxImpl::CreateFlowPerformer(IComputeFlowListerner* listerner)
{
	FlowPerformer* performer = BasicFlowPerformer::Create(listerner);
	_performers.push_back(performer);
	return performer;
}

void FroxImpl::DestroyFlowPerformer(FlowPerformer* performer)
{
	auto it = std::remove_if(_performers.begin(), _performers.end(), [performer](FlowPerformer* other) {
		return other == performer;
	});
	_performers.erase(it);

	delete performer;
}

ComputeFramePtr FroxImpl::CreateComputeFrame(Size size, ComputeFrameType type, const void* data)
{
	assert(size.Width > 0 && size.Height > 0);
	assert(type.Type != EComputeFrameType::ECFT_None && type.Channels > 0);

#ifndef WITHOUT_OPENCV
	return OpenCVComputeFrameImpl::Create(size, type, data);
#else
	return ComputeFrameImplV1::Create(size, type, data);
#endif
}

ComputeFramePtr FroxImpl::LoadComputeFrame(const char* path, EFrameLoaderType loader)
{
	std::ifstream file(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		Log::Error((std::string("Unable to open file + ") + path).c_str(), "LoadComputeFrame");
		return nullptr;
	}

	// Version
	static int gVersion = 1;
	int version;
	file.read(reinterpret_cast<char*>(&version), sizeof(int));

	if (version != gVersion)
	{
		Log::Error((std::string("Unknown version of file + ") + path).c_str(), "LoadComputeFrame");
		return nullptr;
	}

	// Read name and ignore
	std::vector<uint16_t> text;
	uint32_t nbSymbols = 0;
	file.read(reinterpret_cast<char*>(&nbSymbols), sizeof(uint32_t));
	text.resize(nbSymbols);

	file.read(reinterpret_cast<char*>(text.data()), sizeof(uint16_t) * nbSymbols);

	// Read Info
	uint32_t rows, cols = 0;
	int type = 0;
	file.read(reinterpret_cast<char*>(&rows), sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(&cols), sizeof(uint32_t));
	file.read(reinterpret_cast<char*>(&type), sizeof(int));

	auto magicType2FroxType = [](int type) {
		static std::map<int, ComputeFrameType> types = {
			// 8 unsigned bits per channel
			std::make_pair(CV_8UC1, ComputeFrameType{ECFT_UInt8, 1}),
			std::make_pair(CV_8UC2, ComputeFrameType{ECFT_UInt8, 2}),
			std::make_pair(CV_8UC3, ComputeFrameType{ECFT_UInt8, 3}),
			std::make_pair(CV_8UC4, ComputeFrameType{ECFT_UInt8, 4}),

			// 16 unsigned bits per channel
			std::make_pair(CV_16UC1, ComputeFrameType{ECFT_UInt16, 1}),
			std::make_pair(CV_16UC2, ComputeFrameType{ECFT_UInt16, 2}),
			std::make_pair(CV_16UC3, ComputeFrameType{ECFT_UInt16, 3}),
			std::make_pair(CV_16UC4, ComputeFrameType{ECFT_UInt16, 4}),

			// 16 signed bits per channel
			// TODO. Add ECFT_Int16
			std::make_pair(CV_16SC1, ComputeFrameType{ECFT_UInt16, 1}),
			std::make_pair(CV_16SC2, ComputeFrameType{ECFT_UInt16, 2}),
			std::make_pair(CV_16SC3, ComputeFrameType{ECFT_UInt16, 3}),
			std::make_pair(CV_16SC4, ComputeFrameType{ECFT_UInt16, 4}),

			// 32 signed bits per channel
			// TODO. Add ECFT_Int32
			std::make_pair(CV_32SC1, ComputeFrameType{ECFT_UInt32, 1}),
			std::make_pair(CV_32SC2, ComputeFrameType{ECFT_UInt32, 2}),
			std::make_pair(CV_32SC3, ComputeFrameType{ECFT_UInt32, 3}),
			std::make_pair(CV_32SC4, ComputeFrameType{ECFT_UInt32, 4}),

			// 32 real bits per channel
			std::make_pair(CV_32FC1, ComputeFrameType{ECFT_Float, 1}),
			std::make_pair(CV_32FC2, ComputeFrameType{ECFT_Float, 2}),
			std::make_pair(CV_32FC3, ComputeFrameType{ECFT_Float, 3}),
			std::make_pair(CV_32FC4, ComputeFrameType{ECFT_Float, 4}),
		};

		auto it = types.find(type);
		return it != types.end() ? it->second : ComputeFrameType{ ECFT_None, 1 };
	};

	ComputeFrameType frameType = magicType2FroxType(type);
	if (frameType.Type == ECFT_None)
	{
		Log::Error((std::string("Unknown data type of file + ") + path).c_str(), "LoadComputeFrame");
		return nullptr;
	}

	// Read Data
	size_t elemSize = utils::FrameTypeToSize(frameType.Type) * frameType.Channels;;
	size_t rowSize = cols * elemSize;
	size_t nbBytes = rowSize * rows;

	uint8_t* frameData = new uint8_t[nbBytes];
	for (uint32_t row = 0; row < rows; ++row)
	{
		auto rowData = frameData + rowSize * row;
		file.read(reinterpret_cast<char*>(rowData), rowSize);
	}

	// Make new frame
	ComputeFramePtr frame = CreateComputeFrame(Size{ cols, rows }, frameType, frameData);

	delete[] frameData;
	return frame;
}

void FroxImpl::DestroyComputeFrame(ComputeFramePtr computeFrame)
{}

ComputeDataPtr FroxImpl::CreateComputeData(const char* type)
{
	ComputeDataInitializer initializer;
	return ComputeDataPtr(ComputeDataFactory::Create(type, initializer));
}

} // End frox