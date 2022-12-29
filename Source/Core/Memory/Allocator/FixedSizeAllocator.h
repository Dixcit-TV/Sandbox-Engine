#pragma once
#include "Core/Log/Logger.h"

namespace SDBX
{
	namespace Memory
	{
		template<typename Typename>
		class FixedSizeAllocator final
		{
		public:

			class iterator
			{
			public:
				explicit iterator(Typename* ptr) : m_pElem{ ptr } {};

				Typename& operator*() { return *m_pElem; }
				Typename* operator->() { return m_pElem; }

				iterator& operator++() { ++m_pElem; return *this; }
				iterator operator++(int) { const auto temp(*this); ++* this; return temp; }
				iterator& operator--() { --m_pElem; return *this; }
				iterator operator--(int) { const auto temp(*this); --* this; return temp; }

				bool operator== (const iterator& other) const { return m_pElem == other.m_pElem; }
				bool operator!= (const iterator& other) const { return !(*this == other); }

			private:
				Typename* m_pElem;
			};

			class const_iterator
			{
			public:
				explicit const_iterator(const Typename* ptr) : m_pElem{ ptr } {};

				const Typename& operator*() const { return *m_pElem; }
				const Typename* operator->() const { return m_pElem; }

				iterator& operator++() { ++m_pElem; return *this; }
				iterator operator++(int) { const auto temp(*this); ++* this; return temp; }
				iterator& operator--() { --m_pElem; return *this; }
				iterator operator--(int) { const auto temp(*this); --* this; return temp; }

				bool operator== (const iterator& other) const { return m_pElem == other.m_pElem; }
				bool operator!= (const iterator& other) const { return !(*this == other); }

			private:
				const Typename* m_pElem;
			};

			explicit FixedSizeAllocator(size_t size);
			FixedSizeAllocator(const FixedSizeAllocator& other) = delete;
			FixedSizeAllocator(FixedSizeAllocator&& other) noexcept = delete;
			FixedSizeAllocator& operator=(const FixedSizeAllocator& other) = delete;
			FixedSizeAllocator& operator=(FixedSizeAllocator&& other) noexcept = delete;
			~FixedSizeAllocator();

			template<typename... Arg_Type>
			Typename* Acquire(Arg_Type&&... args);
			void Release(Typename*);
			void Release(iterator it);
			void Clear();

			size_t Size() const { return m_InUseCount; };
			size_t Capacity() const { return m_BufferSize; };

			iterator begin() { return iterator{ m_pBegin }; };
			iterator end() { return iterator{ (m_pBegin + m_InUseCount) }; };
			const_iterator cbegin() { return const_iterator{ m_pBegin }; };
			const_iterator cend() { return const_iterator{ (m_pBegin + m_InUseCount) }; };

		private:
			Typename* m_pBegin;
			size_t m_BufferSize;
			size_t m_InUseCount;
		};
	}
}

template<typename Typename>
SDBX::Memory::FixedSizeAllocator<Typename>::FixedSizeAllocator(size_t maxElementCount)
	: m_pBegin(new Typename[maxElementCount]())
	, m_BufferSize(maxElementCount)
	, m_InUseCount(0)
{}

template<typename Typename>
SDBX::Memory::FixedSizeAllocator<Typename>::~FixedSizeAllocator()
{ 
	Clear();
	delete[] m_pBegin;
}

template<typename Typename>
void SDBX::Memory::FixedSizeAllocator<Typename>::Clear()
{
	for (auto it{ begin() }; it != end(); ++it)
	{
		it->~Typename();
	}
}

template<typename Typename>
template<typename... Arg_Type>
Typename* SDBX::Memory::FixedSizeAllocator<Typename>::Acquire(Arg_Type&&... args)
{
	SDBX_ASSERT_MSG(m_InUseCount < m_BufferSize, "Allocator out of memory")

	Typename* acquiredElement{ (m_pBegin + m_InUseCount) };
	++m_InUseCount;
	new (acquiredElement) Typename(std::forward<Arg_Type>(args)...);

	return acquiredElement;
}

template<typename Typename>
void SDBX::Memory::FixedSizeAllocator<Typename>::Release(Typename* pElement)
{
	pElement->~Typename();

	std::swap(*pElement, *(m_pBegin + m_InUseCount - 1));
	--m_InUseCount;
}

template<typename Typename>
void SDBX::Memory::FixedSizeAllocator<Typename>::Release(iterator it)
{
	it->~Typename();

	std::swap(*it, *(m_pBegin + m_InUseCount - 1));
	--m_InUseCount;
}

