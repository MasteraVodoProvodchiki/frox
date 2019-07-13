#pragma once

#include <Frox.h>

#include <limits>
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <algorithm>
#include <cmath>

namespace std {

template<> class numeric_limits<frox::float4> {
public:
	static frox::float4 lowest() { return frox::float4(std::numeric_limits<float>::lowest()); };
	static frox::float4 max() { return frox::float4(std::numeric_limits<float>::max()); };
};

} // End std