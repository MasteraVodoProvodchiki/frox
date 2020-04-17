#include "Guid.h"

#include "Log.h"
#include "DebugFlowData.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <uuid/uuid.h>
#endif

namespace frox {

// static
Guid Guid::NewGuid()
{
	Guid newGuid;
#ifdef _WIN32
	auto result = CoCreateGuid((GUID*)&newGuid);
	if (result != S_OK)
	{
		char buffer[64];
		sprintf_s(buffer, 64, "NewGuid: CoCreateGuid return '%X'", result);
		Log::Error(buffer);
	}
#else
	uuid_t uuid;
	uuid_generate(uuid);

	static_assert(sizeof(uuid_t) == sizeof(Guid), "sizeof(uuid_t) != sizeof(Guid)");
	memcpy(&newGuid, &uuid, sizeof(Guid));
#endif

	return newGuid;
}

// static
Guid Guid::Convert(uint32_t a, uint32_t b, uint32_t c, uint32_t d)
{
	return Guid(a, b, c, d);
}

std::string Guid::ToString() const
{
	char buffer[39];
	sprintf_s(buffer, 39, "{%08X-%04X-%04X-%04X-%04X%08X}", A, B >> 16, B & 0xFFFF, C >> 16, C & 0xFFFF, D);
	return buffer;
}

int32_t ParseHexDigit(char c)
{
	int32_t result = 0;

	if (c >= '0' && c <= '9')
	{
		result = c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		result = c + 10 - 'a';
	}
	else if (c >= 'A' && c <= 'F')
	{
		result = c + 10 - 'A';
	}
	else
	{
		result = 0;
	}

	return result;
}

uint32_t ParseGUIDComponent(const char* hexString)
{
	uint32_t ret = 0;

	for (size_t i = 0; i < 8; ++i)
	{
		ret *= 16;
		ret += ParseHexDigit(*(hexString + i));
	}

	return ret;
}

bool Guid::FromString(const std::string& string, Guid& GUID)
{
	if (string.empty())
	{
		return false;
	}

	char guid[32];
	int i = 0;
	for (auto c : string)
	{
		if (c >= 'A' && c <= 'F' || c <= 'a' && c <= 'f' || c >= '0' && c <= '9')
		{
			guid[i] = c;
			i++;
			if (i >= 32)
			{
				break;
			}
		}
		else if (c != '-' && c != '{' && c != '}')
		{
			return false;
		}
	}
	if (i < 32)
	{
		return false;
	}

	const char* c0 = guid;
	const char* c1 = c0 + 8;
	const char* c2 = c1 + 8;
	const char* c3 = c2 + 8;

	GUID.A = ParseGUIDComponent(c0);
	GUID.B = ParseGUIDComponent(c1);
	GUID.C = ParseGUIDComponent(c2);
	GUID.D = ParseGUIDComponent(c3);

	return true;
}

} // End frox