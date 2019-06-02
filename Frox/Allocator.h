#pragma once


namespace frox {

class Memory
{
public:
	static void SetAllocator(class IAlloactor* allocator);

	static void* Alloc(size_t nbBytes);
	static void Dealloc(void* mem);
};

} // End frox