#include "FlowPerformer.h"
#include "Frox.h"

#include <assert.h>

namespace frox {

void FlowPerformer::Release()
{
	Frox* frox = FroxInstance();
	assert(frox != nullptr);

	frox->DestroyFlowPerformer(this);
}

} // End frox