#include "ComputeDataFactory.h"
#include <unordered_map>
#include <string>

using namespace std;

namespace frox {

unordered_map<string, ComputeDataFactory*>& GetFactories()
{
	static unordered_map<string, ComputeDataFactory*> factories;
	return factories;
}

ComputeData* ComputeDataFactory::Create(const char* type, const  ComputeDataInitializer& initializer)
{
	auto it = GetFactories().find(type);
	return it != GetFactories().end() ? it->second->CreateData(initializer) : nullptr;
}

void ComputeDataFactory::RegisterFactory(const char* type, ComputeDataFactory* factory)
{
	GetFactories().insert(std::make_pair(type, factory));
}

} // End frox