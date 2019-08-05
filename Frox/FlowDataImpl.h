#pragma once

#include "Common.h"
#include "ComputeFrame.h"
#include "ComputeData.h"
#include "Variant.h"
#include "Guid.h"

#include <memory>

namespace frox {

using FlowDataImplPtr = std::shared_ptr<class FlowDataImpl>;
class FlowDataImpl : public std::enable_shared_from_this<FlowDataImpl>
{
public:
	virtual ~FlowDataImpl() {}

	std::shared_ptr<FlowDataImpl> GetPtr()
	{
		return shared_from_this();
	}

	virtual void SetValue(const char* name, Variant value) = 0;
	virtual Variant GetValue(const char* name) const = 0;

	virtual void SetValue(const Guid& guid, Variant value) = 0;
	virtual Variant GetValue(const Guid& guid) const = 0;

	virtual void SetFrame(const char* name, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const char* name) const = 0;

	virtual void SetFrame(const Guid& guid, ComputeFramePtr frame) = 0;
	virtual ComputeFramePtr GetFrame(const Guid& guid) const = 0;

	virtual void SetData(const char* name, ComputeDataPtr data) = 0;
	virtual ComputeDataPtr GetData(const char* name) const = 0;

	virtual void SetData(const Guid& guid, ComputeDataPtr data) = 0;
	virtual ComputeDataPtr GetData(const Guid& guid) const = 0;
};

} // End frox