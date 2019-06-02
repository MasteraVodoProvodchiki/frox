#include "ComputeNodeFactory.h"
#include <unordered_map>
#include <string>

using namespace std;

namespace frox {

unordered_map<string, ComputeNodeFactory*>& GetFactories()
{
	static unordered_map<string, ComputeNodeFactory*> factories;
	return factories;
}

ComputeNode* ComputeNodeFactory::Create(const char* type, const ComputeNodeInitializer& initializer)
{
	auto it = GetFactories().find(type);
	return it != GetFactories().end() ? it->second->CreateNode(initializer) : nullptr;
}

void ComputeNodeFactory::RegisterFactory(const char* type, ComputeNodeFactory* factory)
{
	GetFactories().insert(std::make_pair(type, factory));
}

} // End frox