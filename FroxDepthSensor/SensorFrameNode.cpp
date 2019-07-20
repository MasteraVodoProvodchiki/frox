#include "SensorFrameNode.h"
#include "DepthSensorModule.h"
#include "SensorDevice.h"

#include <ComputeTask.h>
#include <ComputeTaskHelper.h>
#include <Utils.h>

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(SensorFrameNode)
FROX_COMPUTENODE_IMPL(SensorDepthFrameNode)
FROX_COMPUTENODE_IMPL(SensorColorFrameNode)
FROX_COMPUTENODE_IMPL(SensorInfraredFrameNode)

namespace utils {

EInspectorType SensorFrameTypeToInspectorType(ESensorFrameType type)
{
	switch (type)
	{
	case ESensorFrameType::Depth:
		return EInspectorType::Depth;
	case ESensorFrameType::Color:
		return EInspectorType::Color;
	case ESensorFrameType::Infrared:
		return EInspectorType::Infrared;
	default:
		assert(false);
	}

	return EInspectorType::Depth;
}

} // End utils

SensorFrameNode::SensorFrameNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _sensorFrameType(ESensorFrameType::Depth)
	, _output("output")
{}

SensorFrameNode::~SensorFrameNode()
{}

void SensorFrameNode::AllocateDefaultPins()
{
	RegisterOutput(&_output);
}

bool SensorFrameNode::IsValid() const
{
	return true;
}

ComputeTask* SensorFrameNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	// TODO. Save in PrivateData
	if (!_sensorInspector)
	{
		SensorDevice* device = !_sensorSerial.empty() ?
			IDepthSensorModule::Get().FindDevice(_sensorSerial.data()) :
			IDepthSensorModule::Get().GetDefaultDevice();

		if (device != nullptr)
		{
			_sensorInspector = device->CreateInpector(utils::SensorFrameTypeToInspectorType(_sensorFrameType));
		}		
	}

	auto sensorInspector = _sensorInspector; // How by _sensorSerial and _sensorFrameType ?

	auto output = _output.GetValue(outputData);
	return
		ComputeTaskHelper::UnPackProps(sensorInspector)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[](SensorInspectorPtr sensorInspector) {
				return sensorInspector != nullptr && sensorInspector->IsValid();
			},
			[output](SensorInspectorPtr sensorInspector) {

				ComputeFramePtr frame = sensorInspector->ReadFrame();
				output.SetValue(
					frame->GetSize(),
					frame->GetType(),
					[frame](ComputeFramePtr output) {
						utils::Copy(frame, output);
					}
				);
			}
		);
	
	return nullptr;
}

void SensorFrameNode::SetSensorSerial(const char* serial)
{
	assert(serial != nullptr);
	_sensorSerial = serial;
}

void SensorFrameNode::SetSensorFrameType(ESensorFrameType type)
{
	_sensorFrameType = type;
}

/// SensorDepthFrameNode
SensorDepthFrameNode::SensorDepthFrameNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{
	SetSensorFrameType(ESensorFrameType::Depth);
}

/// SensorColorFrameNode
SensorColorFrameNode::SensorColorFrameNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{
	SetSensorFrameType(ESensorFrameType::Color);
}

/// SensorInfraredFrameNode
SensorInfraredFrameNode::SensorInfraredFrameNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
{
	SetSensorFrameType(ESensorFrameType::Infrared);
}

} // End frox