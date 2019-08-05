#pragma once

#include "ComputeData.h"

namespace frox {

class FROX_API ComputeDataFactory
{
public:
	virtual ~ComputeDataFactory() {}
	virtual ComputeData* CreateData(const ComputeDataInitializer& initializer) = 0;

	static ComputeData* Create(const char* type, const ComputeDataInitializer& initializer);
	static void RegisterFactory(const char* type, ComputeDataFactory* factory);
};


/**
 * Tempalate node factory
 */
template<typename ComputeDataT>
class TComputeDataFactory : public ComputeDataFactory
{
public:
	virtual ComputeData* CreateData(const ComputeDataInitializer& initializer) override
	{
		return new ComputeDataT(initializer);
	}
};

template<typename ComputeDataT>
class TDataFactoryRegistrar
{
public:
	TDataFactoryRegistrar()
	{
		ComputeDataFactory::RegisterFactory(
			ComputeDataT::GetTypeStatic(),
			new TComputeDataFactory<ComputeDataT>()
		);
	}
};

#define FROX_COMPUTEDATA(classname, textName) \
private: \
	static const TDataFactoryRegistrar<classname> creator##classname; \
public: \
	virtual const char* GetType() const { return GetTypeStatic(); } \
	static const char* GetTypeStatic() { return textName; } \
private:

#define FROX_COMPUTEDATA_IMPL(classname) \
	const TDataFactoryRegistrar<classname> classname::creator##classname;

} // End frox