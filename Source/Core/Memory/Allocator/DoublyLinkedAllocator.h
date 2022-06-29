 #pragma once
#include <string>

#include "Core/Log/Logger.h"

namespace SDBX
{
	static const size_t DEFAULT_DL_BLOCKSIZE = 16;

	template<size_t BLOCKSIZE = DEFAULT_DL_BLOCKSIZE>
	class DoublyLinkedAllocator final
	{
	public:

		struct Header
		{
			size_t isFree: 1;
			size_t blockCount: sizeof(size_t) - 1;
		};

		struct Block : Header
		{
			struct Links
			{
				Block* next;
				Block* prev;
			};
			union
			{
				Links link;
				char data[BLOCKSIZE - sizeof(Header)];
			};
		};

		explicit DoublyLinkedAllocator(size_t nbrBlocks = 0);
		DoublyLinkedAllocator(const DoublyLinkedAllocator& other) = delete;
		DoublyLinkedAllocator(DoublyLinkedAllocator&& other) noexcept = delete;
		DoublyLinkedAllocator& operator=(const DoublyLinkedAllocator& other) = delete;
		DoublyLinkedAllocator& operator=(DoublyLinkedAllocator&& other) noexcept = delete;
		~DoublyLinkedAllocator();

		template<typename Typename, typename... Arg_Type>
		Typename* Acquire(Arg_Type&&... args);

		template<typename Typename>
		void Release(Typename* pData);

	private:
		Block* m_pHead;
		size_t m_BufferSize;

		void InsertAfter(Block& firstBlock, Block& secondBlock);
		void UnLink(Block& block);
	};
}

template<size_t BLOCKSIZE>
SDBX::DoublyLinkedAllocator<BLOCKSIZE>::DoublyLinkedAllocator(size_t nbrBlocks)
	: m_pHead(nullptr)
	, m_BufferSize()
{
	m_BufferSize = nbrBlocks;

	//allocate one extra block for the head
	m_pHead = new Block[nbrBlocks + 1];

	if (m_pHead)
	{
		m_pHead->blockCount = 0;
		m_pHead->isFree = false;
		Block* pNext{ m_pHead + 1 };
		pNext->blockCount = m_BufferSize;
		pNext->link.next = pNext->link.prev = m_pHead;
		pNext->isFree = true;
		m_pHead->link.next = m_pHead->link.prev = pNext;
	}
}

template<size_t BLOCKSIZE>
SDBX::DoublyLinkedAllocator<BLOCKSIZE>::~DoublyLinkedAllocator()
{
	delete[] m_pHead;
	m_pHead = nullptr;
}

template<size_t BLOCKSIZE>
template<typename Typename, typename... Arg_Type>
Typename* SDBX::DoublyLinkedAllocator<BLOCKSIZE>::Acquire(Arg_Type&&... args)
{
	SDBX_ASSERT(m_pHead, "SDBX::DoublyLinkedAllocator<" + std::to_string(BLOCKSIZE) + ">::Acquire(" + std::to_string(sizeof(Typename)) + ") : m_pHead is NULL")

	const auto nbBlocks = (sizeof(Typename) + sizeof(Header) + sizeof(Block) - 1) / sizeof(Block);

	Block* pCurrent = m_pHead;
	while (pCurrent->blockCount < nbBlocks && (pCurrent = pCurrent->link.next) != m_pHead) {
		Block* pNextBlock = pCurrent + pCurrent->blockCount;
		if (pNextBlock < (m_pHead + m_BufferSize + 1) && pNextBlock->isFree)
		{
			UnLink(*pNextBlock);
			pNextBlock->isFree = false;
			pCurrent->blockCount += pNextBlock->blockCount;

		}
	}

	SDBX_ASSERT(pCurrent != m_pHead, "SDBX::DoublyLinkedAllocator<" + std::to_string(BLOCKSIZE) + ">::Acquire(" + std::to_string(sizeof(Typename)) + ") : Allocator out of memory")

	if (pCurrent->blockCount > nbBlocks)
	{
		Block* newBlock = pCurrent + nbBlocks;
		newBlock->blockCount = pCurrent->blockCount - nbBlocks;
		newBlock->isFree = true;
		pCurrent->blockCount = nbBlocks;
		InsertAfter(*pCurrent, *newBlock);
	}

	UnLink(*pCurrent);
	pCurrent->isFree = false;

	new (pCurrent->data) Typename(std::forward<Arg_Type>(args)...);

	return (Typename*)pCurrent->data;
}

template<size_t BLOCKSIZE>
template<typename Typename>
void SDBX::DoublyLinkedAllocator<BLOCKSIZE>::Release(Typename* pData)
{
	SDBX_ASSERT(m_pHead, "SDBX::DoublyLinkedAllocator<" + std::to_string(BLOCKSIZE) + ">::Acquire(" + std::to_string(sizeof(Typename)) + ") : m_pHead is NULL")

	Block* pBlock = reinterpret_cast<Block*>(reinterpret_cast<Header*>(pData) - 1);

	SDBX_ASSERT(pBlock > m_pHead && pBlock < m_pHead + m_BufferSize + 1, "")

	pData->~Typename();
	InsertAfter(*m_pHead, *pBlock);
	pBlock->isFree = true;
}

template<size_t BLOCKSIZE>
void SDBX::DoublyLinkedAllocator<BLOCKSIZE>::UnLink(Block& block)
{
	block.link.next->link.prev = block.link.prev;
	block.link.prev->link.next = block.link.next;
}

template<size_t BLOCKSIZE>
void SDBX::DoublyLinkedAllocator<BLOCKSIZE>::InsertAfter(Block& firstBlock, Block& secondBlock)
{
	secondBlock.link.next = firstBlock.link.next;
	secondBlock.link.prev = &firstBlock;
	firstBlock.link.next = &secondBlock;
	secondBlock.link.next->link.prev = &secondBlock;
}