#include "pch.h"
#include "StackAllocator.h"

SDBX::StackAllocator::StackAllocator(const size_t size)
	: m_pCurrent((char*)malloc(size))
	, m_BufferSize(size)
	, m_FreeSpace(size)
{}

SDBX::StackAllocator::~StackAllocator()
{
	Reset();
	free(m_pCurrent);
}

void SDBX::StackAllocator::FreeToMarker(const Marker marker)
{
	// Reset the stack till the marker
	m_FreeSpace += size_t(m_pCurrent - marker);
	m_pCurrent = marker;
}

void* SDBX::StackAllocator::Acquire(size_t nbBytes)
{
	SDBX_ASSERT(m_FreeSpace >= nbBytes, "SDBX::StackAllocator::Acquire(" + std::to_string(nbBytes) + ") : Allocator out of memory")

	m_FreeSpace -= nbBytes;
	void* acquiredMemory{ static_cast<void*>(m_pCurrent) };
	m_pCurrent += nbBytes;
	return acquiredMemory;
}

void SDBX::StackAllocator::Reset()
{
	// Free all memory of the stack
	m_pCurrent -= (m_BufferSize - m_FreeSpace);
	m_FreeSpace = m_BufferSize;
}
