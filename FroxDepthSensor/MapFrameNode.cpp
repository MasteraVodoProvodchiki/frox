#include "MapFrameNode.h"
#include "DepthSensorModule.h"
#include "SensorDevice.h"

#include <ComputeTask.h>
#include <ComputeTaskHelper.h>

#include <assert.h>

namespace frox {

FROX_COMPUTENODE_IMPL(MapDepthToColorNode)

MapDepthToColorNode::MapDepthToColorNode(const ComputeNodeInitializer& initializer)
	: Super(initializer)
	, _depthFrame("depthFrame")
	, _colorFrame("colorFrame")
	, _output("output")
{
}

MapDepthToColorNode::~MapDepthToColorNode()
{}

void MapDepthToColorNode::AllocateDefaultPins()
{
	RegisterInput(&_depthFrame);
	RegisterInput(&_colorFrame);

	RegisterOutput(&_output);
}

bool MapDepthToColorNode::IsValid() const
{
	return true;
}

ComputeTask* MapDepthToColorNode::CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData)
{
	SensorDevice* device = IDepthSensorModule::Get().GetDefaultDevice();

	auto depthFrame = _depthFrame.GetValue(inputData);
	auto colorFrame = _colorFrame.GetValue(inputData);

	auto output = _output.GetValue(outputData);
	return
		ComputeTaskHelper::UnPackProps(depthFrame, colorFrame)
		// .Validate
		// .UnPackOutputs
		// .Invoke
		.MakeTask(
			[device](ComputeFramePtr depthFrame, ComputeFramePtr colorFrame) {
				return (depthFrame != nullptr && depthFrame->IsValid()) && 
					(colorFrame != nullptr && colorFrame->IsValid()) &&
					device != nullptr;
			},
			[output, device](ComputeFramePtr depthFrame, ComputeFramePtr colorFrame) {
				output.SetValue(device->MapDepthFrameToColorFrame(depthFrame, colorFrame));
			}
			);
}

}
