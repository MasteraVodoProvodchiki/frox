#include "FlowData.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

void FlowData::Release()
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	frox->DestroyFlowData(this);
}

} // End frox