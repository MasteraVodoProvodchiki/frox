#include "Guid.h"

namespace frox {

Guid Guid::NewGuid()
{
	static uint64_t NextId = 0;

	Guid guid;
	guid.A = NextId++;
	return guid;
}

} // End frox