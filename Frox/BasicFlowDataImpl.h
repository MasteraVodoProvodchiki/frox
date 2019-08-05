#pragma once

#include "FlowDataImpl.h"
#include <map>


namespace frox {

class BasicFlowDataImpl : public FlowDataImpl
{
public:
	BasicFlowDataImpl();
	virtual ~BasicFlowDataImpl();

	static FlowDataImplPtr Create()
	{
		return std::make_shared<BasicFlowDataImpl>();
	}

	// FlowDataImpl overrides
	virtual void SetValue(const char* name, Variant value) override;
	virtual Variant GetValue(const char* name) const override;
	virtual void SetValue(const Guid& guid, Variant value) override;
	virtual Variant GetValue(const Guid& guid) const override;
	virtual void SetFrame(const char* name, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetFrame(const char* name) const override;
	virtual void SetFrame(const Guid& guid, ComputeFramePtr frame) override;
	virtual ComputeFramePtr GetFrame(const Guid& guid) const override;
	virtual void SetData(const char* name, ComputeDataPtr data) override;
	virtual ComputeDataPtr GetData(const char* name) const override;
	virtual void SetData(const Guid& guid, ComputeDataPtr data) override;
	virtual ComputeDataPtr GetData(const Guid& guid) const override;

private:
	std::map<std::string, Variant> _valuesByName;
	std::map<Guid, Variant> _valuesByGuid;

	std::map<std::string, ComputeFramePtr> _framesByName;
	std::map<Guid, ComputeFramePtr> _framesByGuid;

	std::map<std::string, ComputeDataPtr> _datasByName;
	std::map<Guid, ComputeDataPtr> _datasByGuid;
};

} // End frox