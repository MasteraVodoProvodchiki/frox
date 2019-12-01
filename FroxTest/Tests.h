#pragma once

struct Tests
{
	static void ComputeFrameTests();
	static void MainTest();
	static void ComputeDataTest();
	static void DepthSensorTest();
	static void AstraTest();
#ifdef WITH_KINECT2
	static void Kinect2Test();
#endif // WITH_KINECT2
};
