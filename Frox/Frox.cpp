#include "Frox.h"
#include "FroxImpl.h"
#include <assert.h>

namespace frox {

Frox* gFrox = nullptr;
Frox* FroxInit(const FroxDesc& desc)
{
	if (gFrox == nullptr)
	{
		gFrox = new FroxImpl(desc);
	}

	return gFrox;
}

void FroxShutdown(Frox* frox)
{
	assert(gFrox == frox);

	delete gFrox;
	gFrox = nullptr;
}

Frox* FroxInstance()
{
	return gFrox;
}

int FroxGetVersion()
{
	return FROX_VERSION;
}

} // End frox