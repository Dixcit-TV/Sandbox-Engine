#pragma once
#include <string>

#include "Core/Log/Logger.h"

namespace SDBX
{
	static const size_t DEFAULT_BLOCKSIZE = 16;

	template<size_t BLOCKSIZE = DEFAULT_BLOCKSIZE>
	class SinglyLinkedAllocator final
	{
	public:

		struct Block
		{
			size_t blockCount;
			union {
				Block* pNext;
				char data[BLOCKSIZE - sizeof(blockCount)];
			};
		};

		explicit SinglyLinkedAllocator(size_t nbBlocks);
		SinglyLinkedAllocator(const SinglyLinkedAllocator& other) = delete;
		SinglyLinkedAllocator(SinglyLinkedAllocator&& other) noexcept = delete;
		SinglyLinkedAllocator& operator=(const SinglyLinkedAllocator& other) = delete;
		SinglyLinkedAllocator& operator=(SinglyLinkedAllocator&& other) noexcept = delete;
		~SinglyLinkedAllocator();

		template<typename Typename, typename... Arg_Type>
		Typename* Acquire(Arg_Type&&... args);

		template<typename Typename>
		void Release(Typename* pData);

	private:
		Block* m_pHead;
		size_t m_BufferSize;
	};
}

template<size_t BLOCKSIZE>
SDBX::SinglyLinkedAllocator<BLOCKSIZE>::SinglyLinkedAllocator(size_t nbBlocks)
	: m_pHead(nullptr)
	, m_BufferSize()
{
	//adjust memory block to allocate if it doesn't respect memory alignment
	m_BufferSize = nbBlocks;

	//allocate one extra block for the head
	m_pHead = new Block[nbBlocks + 1];

	if (m_pHead)
	{
		m_pHead->blockCount = 0;
		Block* pNext{ m_pHead + 1 };
		pNext->blockCount = nbBlocks;
		pNext->pNext = nullptr;
		m_pHead->pNext = pNext;
	}
}

template<size_t BLOCKSIZE>
SDBX::SinglyLinkedAllocator<BLOCKSIZE>::~SinglyLinkedAllocator()
{
	delete[] m_pHead;
	m_pHead = nullptr;
}

template<size_t BLOCKSIZE>
template<typename Typename, typename... Arg_Type>
Typename* SDBX::SinglyLinkedAllocator<BLOCKSIZE>::Acquire(Arg_Type&&... args)
{
	SDBX_ASSERT(m_pHead, "SDBX::SinglyLinkedAllocator<" + std::to_string(BLOCKSIZE) + ">::Acquire(" + std::to_string(sizeof(Typename)) + ") : m_pHead is NULL")

	//calculate the number of blocks required to store the Typename object (requires extra space to store the block count) 
	const auto nbBlocks = (sizeof(Typename) + sizeof(Block::blockCount) + BLOCKSIZE - 1) / BLOCKSIZE;

	Block* pPreviousBlock = m_pHead;
	Block* pNextBlock = m_pHead->pNext;
	while (pNextBlock != nullptr && pNextBlock->blockCount < nbBlocks) {
		pPreviousBlock = pNextBlock;
		pNextBlock = pNextBlock->pNext;
	}

	//Reached end of the buffer without finding enough space
	SDBX_ASSERT(pNextBlock, "SDBX::SinglyLinkedAllocator<" + std::to_string(BLOCKSIZE) + ">::Acquire(" + std::to_string(sizeof(Typename)) + ") : Allocator out of memory")

	//if the free block is larger than the requested number of blocks, need to split the free block to only acquire the minimum requested number of block and create a free block with the rest
	if (pNextBlock->blockCount > nbBlocks)
	{
		Block* newBlock = pNextBlock + nbBlocks;
		newBlock->blockCount = pNextBlock->blockCount - nbBlocks;
		newBlock->pNext = pNextBlock->pNext;
		pNextBlock->blockCount = nbBlocks;
		pNextBlock->pNext = newBlock;
	}

	pPreviousBlock->pNext = pNextBlock->pNext;

	//call Typename default constructor, buffer overrun warning can be ignored because, if it happens, we already "reserved" the blocks that will be overwritten
	new (pNextBlock->data) Typename(std::forward<Arg_Type>(args)...);

	return (Typename*)pNextBlock->data;
}

template<size_t BLOCKSIZE>
template<typename Typename>
void SDBX::SinglyLinkedAllocator<BLOCKSIZE>::Release(Typename* pData)
{
	assert(m_pHead);

	Block* pBlock = reinterpret_cast<Block*>(reinterpret_cast<char*>(pData) - sizeof(Block::blockCount));

	SDBX_ASSERT(pBlock > m_pHead && pBlock < m_pHead + m_BufferSize + 1, "")

	Block* pFreeBlock{ m_pHead };
	while (pFreeBlock->pNext != nullptr && pFreeBlock->pNext < pBlock)
	{
		pFreeBlock = pFreeBlock->pNext;
	}

	pData->~Typename();
	pBlock->pNext = pFreeBlock->pNext;
	pFreeBlock->pNext = pBlock;

	Block* pBlockNeighbor{ pBlock + pBlock->blockCount };
	if (pBlock->pNext == pBlockNeighbor) //check if the right adjacent block are free, merge it with the current node if it is
	{
		pBlock->blockCount += pBlockNeighbor->blockCount;
		pBlock->pNext = pBlockNeighbor->pNext;
	}

	if (pFreeBlock == m_pHead)
		return;

	Block* pFreeBlockNeighbor{ pFreeBlock + pFreeBlock->blockCount };
	if (pBlock == pFreeBlockNeighbor)  //check if the current block being released is the right neighbor of the previous free block, merge them if it is
	{
		pFreeBlock->blockCount += pBlock->blockCount;
		pFreeBlock->pNext = pBlock->pNext;
	}
}