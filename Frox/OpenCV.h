#pragma once

#include <opencv2/opencv.hpp>
#include "Types.h"

namespace cv {

// Type frox Point
template<> class DataType<frox::Point>
{
public:
	typedef frox::Point value_type;
	typedef frox::Point	work_type;
	typedef int         channel_type;

	enum {
		generic_type = 0,
		channels = 2,
		fmt = traits::SafeFmt<channel_type>::fmt + ((channels - 1) << 8)
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
		, depth = DataType<channel_type>::depth
		, type = CV_MAKETYPE(depth, channels)
#endif
	};

	typedef Vec<channel_type, channels> vec_type;
};

namespace traits {
template<>
struct Depth<frox::Point> { enum { value = Depth<int>::value }; };
template<>
struct Type<frox::Point> { enum { value = CV_MAKETYPE(Depth<int>::value, 2) }; };
} // Endtraits

// Type frox Vec4i
template<> class DataType<frox::uint4>
{
public:
    typedef frox::uint4 value_type;
    typedef frox::uint4 work_type;
    typedef int channel_type;
    typedef value_type vec_type;

    enum { generic_type = 0,
           channels     = 4,
           fmt          = DataType<channel_type>::fmt + ((channels - 1) << 8),
#ifdef OPENCV_TRAITS_ENABLE_DEPRECATED
           depth        = DataType<channel_type>::depth,
           type         = CV_MAKETYPE(depth, channels),
#endif
           _dummy_enum_finalizer = 0
         };
};

namespace traits {
template<>
struct Depth<frox::uint4> { enum { value = Depth<int>::value }; };
template<>
struct Type<frox::uint4> { enum { value = CV_MAKETYPE(Depth<int>::value, 4) }; };
} // namespace

} // End cv
