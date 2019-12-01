#include "Tests.h"
#include "Shared.h"

using namespace frox;

Frox* gFrox = nullptr;

int main(int argc, char *argv[])
{
	// Init
	gFrox = FroxInit();

	Tests::ComputeFrameTests();
	Tests::MainTest();
	Tests::ComputeDataTest();
	Tests::DepthSensorTest();
	Tests::AstraTest();
#ifdef WITH_KINECT2
	Tests::Kinect2Test();
#endif // WITH_KINECT2

	// Shutdown
	FroxShutdown(gFrox);

	// Wait
    getchar();
	return 0;
}
