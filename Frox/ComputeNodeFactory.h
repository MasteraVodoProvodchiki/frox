#pragma once

#include "ComputeNode.h"

namespace frox {

class FROX_API ComputeNodeFactory
{
public:
	virtual ~ComputeNodeFactory() {}
	virtual ComputeNode* CreateNode(const ComputeNodeInitializer& initializer) = 0;

	static ComputeNode* Create(const char* type, const ComputeNodeInitializer& initializer);
	static void RegisterFactory(const char* type, ComputeNodeFactory* factory);
};


/**
 * Tempalate node factory
 */
template<typename ComputeNodeT>
class TComputeNodeFactory : public ComputeNodeFactory
{
public:
	virtual ComputeNode* CreateNode(const ComputeNodeInitializer& initializer) override
	{
		return new ComputeNodeT(initializer);
	}
};

template<typename ComputeNodeT>
class TFactoryRegistrar
{
public:
	TFactoryRegistrar()
	{
		ComputeNodeFactory::RegisterFactory(
			ComputeNodeT::GetNodeTypeStatic(),
			new TComputeNodeFactory<ComputeNodeT>()
		);
	}
};

#define FROX_COMPUTENODE(classname, textName) \
private: \
	static const TFactoryRegistrar<classname> creator##classname; \
public: \
	virtual const char* GetNodeType() { return GetNodeTypeStatic(); } \
	static const char* GetNodeTypeStatic() { return textName; } \
private:

#define FROX_COMPUTENODE_IMPL(classname) \
	const TFactoryRegistrar<classname> classname::creator##classname;

} // End frox