#include "Kinect2Module.h"
#include "Kinect2ModuleImpl.h"

namespace frox {

IKinect2Module& IKinect2Module::Get()
{
	static Kinect2Module kinect2Module;
	return kinect2Module;
}

} // End frox.