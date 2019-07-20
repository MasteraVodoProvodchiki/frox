#pragma once

#include <ComputeNodeImpl.h>
#include <ComputeNodeFactory.h>
#include <ComputeFrame.h>

#include "Shared.h"
#include "SensorInspector.h"

namespace frox {

enum class ESensorFrameType
{
	Depth,
	Color,
	Infrared
};

class ComputeTask;
class SensorFrameNode : public ComputeNodeImpl
{
	FROX_COMPUTENODE(SensorFrameNode, "sensorframe")
	using Super = ComputeNodeImpl;

public:
	SensorFrameNode(const ComputeNodeInitializer& initializer);
	virtual ~SensorFrameNode() override;

	// ComputeNode overrides
	virtual void AllocateDefaultPins() override;
	virtual bool IsValid() const override;
	virtual ComputeTask* CreateComputeTask(FlowDataImplPtr inputData, FlowDataImplPtr outputData) override;

	FROXDS_API void SetSensorSerial(const char* serial);
	FROXDS_API void SetSensorFrameType(ESensorFrameType type);

protected:
	std::string _sensorSerial;
	ESensorFrameType _sensorFrameType;
	TOutputFrame<ComputeFramePtr> _output;

private:
	SensorInspectorPtr _sensorInspector;
};


class SensorDepthFrameNode : public SensorFrameNode
{
	FROX_COMPUTENODE(SensorDepthFrameNode, "sensordepthframe")
	using Super = SensorFrameNode;

public:
	SensorDepthFrameNode(const ComputeNodeInitializer& initializer);
};

class SensorColorFrameNode : public SensorFrameNode
{
	FROX_COMPUTENODE(SensorColorFrameNode, "sensorcolorframe")
	using Super = SensorFrameNode;

public:
	SensorColorFrameNode(const ComputeNodeInitializer& initializer);
};

class SensorInfraredFrameNode : public SensorFrameNode
{
	FROX_COMPUTENODE(SensorInfraredFrameNode, "sensorinfraredframe")
	using Super = SensorFrameNode;

public:
	SensorInfraredFrameNode(const ComputeNodeInitializer& initializer);
};

} // End frox