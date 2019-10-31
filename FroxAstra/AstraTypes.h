#pragma once

#include "Shared.h"
#include "AstraNative.h"

namespace frox {

enum class EAstraStreamType : uint8_t
{
	STREAM_ANY,
	STREAM_DEPTH, /**< Native stream of depth data produced by RealSense device */
	STREAM_COLOR, /**< Native stream of color data captured by RealSense device */
	STREAM_INFRARED, /**< Native stream of infrared data captured by RealSense device */
};

enum class EAstraFormatType : uint8_t
{
	FORMAT_ANY, /**< When passed to enable stream, librealsense will try to provide best suited format */
	FORMAT_Z16, /**< 16-bit linear depth values. The depth is meters is equal to depth scale * pixel value. */
	FORMAT_DISPARITY16, /**< 16-bit linear disparity values. The depth in meters is equal to depth scale / pixel value. */
	FORMAT_XYZ32F, /**< 32-bit floating point 3D coordinates. */
	FORMAT_YUYV, /**< Standard YUV pixel format as described in https://en.wikipedia.org/wiki/YUV */
	FORMAT_RGB8, /**< 8-bit red, green and blue channels */
	FORMAT_BGR8, /**< 8-bit blue, green, and red channels -- suitable for OpenCV */
	FORMAT_RGBA8, /**< 8-bit red, green and blue channels + constant alpha channel equal to FF */
	FORMAT_BGRA8, /**< 8-bit blue, green, and red channels + constant alpha channel equal to FF */
	FORMAT_Y8, /**< 8-bit per-pixel grayscale image */
	FORMAT_Y16, /**< 16-bit per-pixel grayscale image */
	FORMAT_RAW10, /**< Four 10-bit luminance values encoded into a 5-byte macropixel */
	FORMAT_RAW16, /**< 16-bit raw image */
	FORMAT_RAW8, /**< 8-bit raw image */
	FORMAT_UYVY, /**< Similar to the standard YUYV pixel format, but packed in a different order */
	FORMAT_MOTION_RAW, /**< Raw data from the motion sensor */
	FORMAT_MOTION_XYZ32F, /**< Motion data packed as 3 32-bit float values, for X, Y, and Z axis */
	FORMAT_GPIO_RAW, /**< Raw data from the external sensors hooked to one of the GPIO's */
	FORMAT_6DOF, /**< Pose data packed as floats array, containing translation vector, rotation quaternion and prediction velocities and accelerations vectors */
	FORMAT_DISPARITY32, /**< 32-bit float-point disparity values. Depth->Disparity conversion : Disparity = Baseline*FocalLength/Depth */
};

struct AstraStreamProfile
{
	EAstraStreamType StreamType = EAstraStreamType::STREAM_ANY;
	EAstraFormatType Format = EAstraFormatType::FORMAT_ANY;
	int32_t Width = 640;
	int32_t Height = 480;
	int32_t Rate = 30;
};

struct AstraStreamMode
{
	int32_t Width = 640;
	int32_t Height = 480;
	int32_t Rate = 30;
};

} // End frox.
