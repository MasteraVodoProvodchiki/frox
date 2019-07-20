#pragma once

#include <SensorInspector.h>

#include "RealSenseDevice.h"
#include "RealSenseNative.h"
#include "RealSenseTypes.h"

namespace frox {

using RealSenseInspectorPtr = std::shared_ptr<class RealSenseInspector>;
class RealSenseInspector : public SensorInspector
{
public:
	RealSenseInspector(RealSenseDevicePtr device, EInspectorType type);
	virtual ~RealSenseInspector();

	static RealSenseInspectorPtr Create(RealSenseDevicePtr device, EInspectorType type)
	{
		return std::make_shared<RealSenseInspector>(device, type);
	}

	// FDepthSensor overrides
	virtual bool Start();
	virtual void Stop();

	virtual bool IsValid() const override;
	virtual ComputeFramePtr ReadFrame() const override;

	/*
	virtual FFramePtr GetDepthImage() override
	{
		return _depthFrame;
	}
	virtual FFramePtr GetColorImage() override
	{
		return _colorFrame;
	}
	virtual Vector2i GetDepthResolution() const override
	{
		return _depthResoluation;
	}
	virtual Vector2i GetColorResolution() const override
	{
		return _colorResoluation;
	}
	virtual void GetDepthCameraIntrinsics(Vector2f& focalLength, Vector2f& principalPoint, Vector3f& radialDistortion) const override;
	virtual float GetDepthValue(int32 x, int32 y) const override;
	virtual Vector3f MapCameraPointToDepthSpace(const Vector3f& point) const override;
	virtual Vector3f MapDepthPointToCameraSpace(const Vector3f& point) const override;
	virtual Vector3f MapColorPointToCameraSpace(const Vector3f& point) const override;
	virtual Vector2f MapDepthPointToColorSpace(const Vector3f& point) const override;
	// End FDepthSensor overrides
	*/
private:
	int32_t FindProfile(ERealSenseStreamType streamType, ERealSenseFormatType format, RealSenseStreamMode mode, RealSenseStreamProfile& out) const;
	void EnsureProfileSupported(ERealSenseStreamType streamType, ERealSenseFormatType format, RealSenseStreamMode mode) const;

	bool WaitReadFrame() const;
	void ProcessFrameset(rs2::frameset* frameset) const;
	void ReadDepth(rs2::frameset* frameset) const;
	void ReadColor(rs2::frameset* frameset) const;
	void ReadInfrared(rs2::frameset* frameset) const;

protected:
	RealSenseDevicePtr _device; // use weak_ptr
	EInspectorType _type;

	// Depth reader
	std::unique_ptr<class rs2::pipeline> _rsPipeline;
	// std::unique_ptr<class rs2::device> _rsDevice;
	// std::unique_ptr<class rs2::align> _rsAlign;

	std::unique_ptr<class rs2::pointcloud> _rsPointCloud;
	std::unique_ptr<class rs2::points> _rsPoints;

	ComputeFramePtr _frame;

private:
	RealSenseStreamProfile _currenProfile;
	bool _bStartedFlag;
};

} // End frox.
