#include "Common.h"

namespace frox {

struct Guid
{
	static Guid NewGuid();

	uint64_t A;

	friend bool operator == (const Guid& left, const Guid& right)
	{
		return left.A == right.A;
	}

	friend bool operator != (const Guid& left, const Guid& right)
	{
		return left.A != right.A;
	}
};

} // End frox