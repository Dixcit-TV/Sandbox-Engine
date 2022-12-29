#pragma once
#include <string>

#include "Core/Log/Logger.h"

//FOR POD ONLY, DOESN'T HANDLE NON POD RELEASE 
namespace SDBX
{
	namespace Memory
	{
		class StackAllocator final
		{
		public:
			StackAllocator(const StackAllocator& other) = delete;
			StackAllocator(StackAllocator&& other) noexcept = delete;
			StackAllocator& operator=(const StackAllocator& other) = delete;
			StackAllocator& operator=(StackAllocator&& other) noexcept = delete;

			explicit StackAllocator(size_t size);
			~StackAllocator();

			using Marker = char*;

			// Get a marker to the current top of the stack.
			inline Marker GetMarker() const { return m_pCurrent; }

			// Free the memory up to the given marker
			void FreeToMarker(Marker marker);

			template<typename Typename, typename... Arg_Type, typename = std::enable_if_t<std::is_trivially_destructible_v<Typename>>>
			Typename* Acquire(Arg_Type&&... args)
			{
				size_t nbBytes = sizeof(Typename);

				SDBX_ASSERT_MSG(m_FreeSpace >= nbBytes, "Allocator out of memory")

					m_FreeSpace -= nbBytes;
				auto acquiredMemory{ m_pCurrent };
				m_pCurrent += nbBytes;

				new (acquiredMemory) Typename(std::forward<Arg_Type>(args)...);

				return (Typename*)acquiredMemory;
			}

			void* Acquire(size_t);

			// Reset the complete stack
			void Reset();

			// Get the amount of free memory that is left on the stack.
			inline size_t GetFreeSpaceAmount() const { return m_FreeSpace; }

		private:
			char* m_pCurrent;
			size_t m_FreeSpace;
			size_t m_BufferSize;
		};
	}
}