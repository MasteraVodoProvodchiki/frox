#include "SensorFrameNode.h"

#include <ComputeTask.h>
#include <ComputeTaskHelper.h>
#include <Utils.h>

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(SensorFrameNode)
FROX_COMPUTENODE_IMPL(SensorDepthFrameNode)
FROX_COMPUTENODE_IMPL(SensorColorFrameNode)
FROX_COMPUTENODE_IMPL(SensorInfraredFrameNode)

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