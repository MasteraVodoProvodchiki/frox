#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "ComputeData.h"
#include "ComputeNode.h"
#include "Guid.h"
#include "Variant.h"

#include <map>
#include <memory>

namespace frox {

class DebugFlowData
{
public:
	virtual void SetValue(const Guid& node, Variant value) = 0;
	virtual Variant GetValue(const Guid& node) const = 0;

	virtual void SetFrame(const Guid& node, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const Guid& node) const = 0;

	virtual void SetData(const Guid& node, ComputeDataPtr data) = 0;
	virtual ComputeDataPtr GetData(const Guid& node) const = 0;
};
using DebugFlowDataPtr = std::unique_ptr<class DebugFlowData>;

} // End frox
