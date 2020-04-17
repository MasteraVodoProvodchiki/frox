#pragma once

#include "Common.h"

#include <cassert>
#include <string>

namespace frox {

struct Guid
{
public:

	/**
	 * Default constructor.
	 */
	Guid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{ }

	/**
	 * Creates and initializes a new GUID from the specified components.
	 *
	 * @param InA - The first component.
	 * @param InB - The second component.
	 * @param InC - The third component.
	 * @param InD - The fourth component.
	 */
	Guid(uint32_t InA, uint32_t InB, uint32_t InC, uint32_t InD)
		: A(InA), B(InB), C(InC), D(InD)
	{ }

	/**
	 * Get GUID string.
	 *
	 * @return GUID string.
	 */
	std::string ToString() const;

	/**
	 * Create GUID from string.
	 *
	 * @param string - GUID string.
	 * @param GUID - guid object.
	 *
	 * @return true on success, false otherwise.
	 */
	static bool FromString(const std::string& string, Guid& GUID);

public:

	/**
	 * Compares two GUIDs for equality.
	 *
	 * @param X - The first GUID to compare.
	 * @param Y - The second GUID to compare.
	 *
	 * @return true if the GUIDs are equal, false otherwise.
	 */
	friend bool operator==(const Guid& X, const Guid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}

	/**
	 * Compares two GUIDs for inequality.
	 *
	 * @param X - The first GUID to compare.
	 * @param Y - The second GUID to compare.
	 *
	 * @return true if the GUIDs are not equal, false otherwise.
	 */
	friend bool operator!=(const Guid& X, const Guid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}

	/**
	 * Compares two GUIDs.
	 *
	 * @param X - The first GUID to compare.
	 * @param Y - The second GUID to compare.
	 *
	 * @return true if the first GUID is less than the second one.
	 */
	friend bool operator<(const Guid& X, const Guid& Y)
	{
		return ((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
				((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
					((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false))))))));
	}

	/**
	 * Provides access to the GUIDs components.
	 *
	 * @param Index - The index of the component to return (0...3).
	 *
	 * @return The component.
	 */
	uint32_t& operator[](int32_t Index)
	{
		assert(Index >= 0);
		assert(Index < 4);

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	/**
	 * Provides read-only access to the GUIDs components.
	 *
	 * @param Index - The index of the component to return (0...3).
	 *
	 * @return The component.
	 */
	const uint32_t& operator[](uint32_t Index) const
	{
		assert(Index >= 0);
		assert(Index < 4);

		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

public:

	/**
	 * Checks whether this GUID is valid or not.
	 *
	 * A GUID that has all its components set to zero is considered invalid.
	 *
	 * @return true if valid, false otherwise
	 *
	 * @see Invalidate
	 */
	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}

	/**
	 * Invalidates the GUID.
	 *
	 * @see IsValid
	 */
	void Invalidate()
	{
		A = B = C = D = 0;
	}

public:

	/**
	 * Returns a new GUID.
	 *
	 * @return A new GUID.
	 */
	static Guid NewGuid();
	static Guid Convert(uint32_t a, uint32_t b, uint32_t c, uint32_t d);

public:

	// Holds the first component.
	uint32_t A;

	// Holds the second component.
	uint32_t B;

	// Holds the third component.
	uint32_t C;

	// Holds the fourth component.
	uint32_t D;
};

} // End frox