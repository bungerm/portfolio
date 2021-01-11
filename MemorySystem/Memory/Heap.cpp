//---------------------------------------------------------------
// Mike Bunger
// Heap
//---------------------------------------------------------------

#include "Heap.h"
#include "Block.h"
#include <string.h>

namespace Azul
{

	Heap::Type Heap::GetHeapType() const
	{
		return mType;
	}

	// Overflow Heap
	void Heap::SetOverflowHeap(Heap* pHeap)
	{
		pOverflow = pHeap;
	}

	Heap* Heap::GetOverflowHeap() const
	{
		return pOverflow;
	}

	void Heap::GetInfo(Info& retInfo) const
	{
		retInfo.CurrBytesUsed = mInfo.CurrBytesUsed;
		retInfo.CurrNumAlloc = mInfo.CurrNumAlloc;
		retInfo.PeakBytesUsed = mInfo.PeakBytesUsed;
		retInfo.PeakNumAlloc = mInfo.PeakNumAlloc;
		retInfo.StartAddr = mInfo.StartAddr;
		retInfo.EndAddr = mInfo.EndAddr;
		memcpy(retInfo.Name, mInfo.Name, Heap::NameLength);
		retInfo.TotalSize = mInfo.TotalSize;
	};

	Heap* Heap::DebugGetNext() const
	{
		return pNext;
	};

	Heap* Heap::DebugGetPrev() const
	{
		return pPrev;
	};

	Block* Heap::GetTrackingHead() const
	{
		return pBlkHead;
	}

	Azul::Heap::Heap(const char* name, Heap::Type type, Mem* mem, unsigned int size)
	{
		memcpy(mInfo.Name, name, Heap::NameLength);
		mType = type;
		pMem = mem;
		mInfo.TotalSize = size;
		mWinHeapHandle = HeapCreate(0, 0, 0);
		mInfo.StartAddr = (size_t)this;
		mInfo.EndAddr = (size_t)this + size;
		pBlkHead = nullptr;
		pNext = nullptr;
		pPrev = nullptr;

		// Copy the heap header contents as the first allocation
		void* pointer = HeapAlloc(mWinHeapHandle, 0, sizeof(Heap));
		memcpy(pointer, this, sizeof(Heap));
	}

	Azul::Heap::Heap(const char* name, Heap::Type type, Mem* mem, unsigned int numBlocks, unsigned int sizePerBlock, Mem::Align)
	{
		//int a = getValue(align);
		memcpy(mInfo.Name, name, Heap::NameLength);
		mType = type;
		pMem = mem;
		mInfo.TotalSize = numBlocks * sizePerBlock;
		mWinHeapHandle = HeapCreate(0, 0, 0);
		mInfo.StartAddr = (size_t)mWinHeapHandle;
		mInfo.EndAddr = (size_t)mInfo.StartAddr + numBlocks * (sizePerBlock + sizeof(Block)) + sizeof(Heap);
		pBlkHead = nullptr;
		pNext = nullptr;
		pPrev = nullptr;

		// Copy the heap header contents as the first allocation
		void* heap = HeapAlloc(mWinHeapHandle, 0, sizeof(Heap));
		memcpy(heap, this, sizeof(Heap));

		// Stamp all the blocks in
		void* s = HeapAlloc(mWinHeapHandle, 0, numBlocks * (sizePerBlock + sizeof(Block)));
		memset(s, 0x0, numBlocks * (sizePerBlock + sizeof(Block)));
		Block* b = (Block*)s;
		Block* last = nullptr;
		for (unsigned int i = 0; i < numBlocks; i++) {
			memset(b, 0, sizeof(Block));

			// Handle heap DLL
			if (pFixedFreeHead == nullptr) {
				pFixedFreeHead = b;
			}
			else {
				last->hNext = b;
				b->hPrev = last;
			}

			// Set block header stats (the ones we can right now)
			b->mAllocSize = sizePerBlock;
			b->pSecret = b;

			last = b;
			b = (Block*)((size_t)b + sizePerBlock + sizeof(Block));
		}
	}

	void* Azul::Heap::Alloc(size_t inSize, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		size_t a = getValue(align);
		size_t totalBytes = sizeof(Block) + a + inSize;

		// Get allocation from win32
		void* s = HeapAlloc(mWinHeapHandle, 0, totalBytes);
		Block* blockHeader = (Block*) s;
		memset(blockHeader, 0, sizeof(Block));

		// Handle heap DLL
		if (pBlkHead == nullptr) {
			pBlkHead = blockHeader;
		}
		else {
			//// add to end of heap list
			//Block* temp = pBlkHead;
			//while (temp->hNext != nullptr) {
			//	temp = temp->hNext;
			//}
			//temp->hNext = blockHeader;
			//blockHeader->hPrev = temp;

			// add to the front of heap list
			pBlkHead->hPrev = blockHeader;
			blockHeader->hNext = pBlkHead;
			pBlkHead = blockHeader;
		}

		// Handle global allocation DLL
		if (pMem->pGlobalHead == nullptr) {
			pMem->pGlobalHead = blockHeader;
		}
		else {
			// add to front of global list
			blockHeader->gNext = pMem->pGlobalHead;
			pMem->pGlobalHead->gPrev = blockHeader;
			pMem->pGlobalHead = blockHeader;
		}

		// Set block header stats
		memcpy(blockHeader->mFileName, getFileName(inName), Block::NameLength);
		blockHeader->mLineNum = lineNum;
		blockHeader->mAllocIndex = ++pMem->mInfo.CurrAllocIndex;
		blockHeader->mAllocSize = inSize;

		// Adjust Mem stats
		pMem->mInfo.CurrBytesUsed += inSize;
		pMem->mInfo.CurrNumAlloc++;
		if (pMem->mInfo.PeakBytesUsed < pMem->mInfo.CurrBytesUsed) {
			pMem->mInfo.PeakBytesUsed = pMem->mInfo.CurrBytesUsed;
		}
		if (pMem->mInfo.PeakNumAlloc < pMem->mInfo.CurrNumAlloc) {
			pMem->mInfo.PeakNumAlloc = pMem->mInfo.CurrNumAlloc;
		}

		// Adjust Heap stats
		mInfo.CurrBytesUsed += blockHeader->mAllocSize;
		mInfo.CurrNumAlloc++;
		if (mInfo.PeakBytesUsed < mInfo.CurrBytesUsed) {
			mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
		}
		if (mInfo.PeakNumAlloc < mInfo.CurrNumAlloc) {
			mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
		}

		// Get bottom of block header
		void* r = (void*)((size_t)s + sizeof(Block));

		// Do some magic math to get aligned block start
		void* p = (void*)(((size_t)r + (a - 1)) & ~(a - 1));

		// Set that secret pointer to block header
		Heap* secretPtr = (Heap*)((size_t)p - sizeof(Heap*));
		memcpy(secretPtr, &s, sizeof(Block*));

		return p;
	}

	void* Azul::Heap::FixedAlloc(const char* inName, size_t lineNum, size_t inSize)
	{
		// DANGER WILL ROBINSON
		// NO MORE BLOCKS LEFT!
		if (pFixedFreeHead == nullptr) {
			return pOverflow->Alloc(inSize, Mem::Align::Default, inName, lineNum);
		}

		Block* b = pFixedFreeHead;
		pFixedFreeHead = pFixedFreeHead->hNext;
		b->hNext = nullptr;
		b->hPrev = nullptr;

		// Handle heap allocation DLL
		if (pBlkHead == nullptr) {
			pBlkHead = b;
		}
		else {
			// add to front of heap list
			b->hNext = pBlkHead;
			pBlkHead->hPrev = b;
			pBlkHead = b;
		}

		// Handle global allocation DLL
		if (pMem->pGlobalHead == nullptr) {
			pMem->pGlobalHead = b;
		}
		else {
			// add to front of global list
			b->gNext = pMem->pGlobalHead;
			pMem->pGlobalHead->gPrev = b;
			pMem->pGlobalHead = b;
		}

		// Set block header stats
		memcpy(b->mFileName, getFileName(inName), Block::NameLength);
		b->mLineNum = lineNum;
		b->mAllocIndex = ++pMem->mInfo.CurrAllocIndex;

		// Adjust Mem stats
		pMem->mInfo.CurrBytesUsed += b->mAllocSize;
		pMem->mInfo.CurrNumAlloc++;
		if (pMem->mInfo.PeakBytesUsed < pMem->mInfo.CurrBytesUsed) {
			pMem->mInfo.PeakBytesUsed = pMem->mInfo.CurrBytesUsed;
		}
		if (pMem->mInfo.PeakNumAlloc < pMem->mInfo.CurrNumAlloc) {
			pMem->mInfo.PeakNumAlloc = pMem->mInfo.CurrNumAlloc;
		}

		// Adjust Heap stats
		mInfo.CurrBytesUsed += b->mAllocSize;
		mInfo.CurrNumAlloc++;
		if (mInfo.PeakBytesUsed < mInfo.CurrBytesUsed) {
			mInfo.PeakBytesUsed = mInfo.CurrBytesUsed;
		}
		if (mInfo.PeakNumAlloc < mInfo.CurrNumAlloc) {
			mInfo.PeakNumAlloc = mInfo.CurrNumAlloc;
		}

		// Get block header end
		void* r = (void*)((size_t)b + sizeof(Block));

		return r;
	}

	void Azul::Heap::Dealloc(void* p)
	{
		Block** block = (Block**)((size_t)p - sizeof(Block*));

		if (mType == Heap::Type::NORMAL) {
			// Update heap pointers
			if ((*block)->hNext != nullptr) {
				(*block)->hNext->hPrev = (*block)->hPrev;
			}
			if ((*block)->hPrev != nullptr) {
				(*block)->hPrev->hNext = (*block)->hNext;
			}
			if (*block == pBlkHead) {
				pBlkHead = (*block)->hNext;
			}

			// Update global pointers
			if ((*block)->gNext != nullptr) {
				(*block)->gNext->gPrev = (*block)->gPrev;
			}
			if ((*block)->gPrev != nullptr) {
				(*block)->gPrev->gNext = (*block)->gNext;
			}
			if (*block == pMem->pGlobalHead) {
				pMem->pGlobalHead = (*block)->gNext;
			}

			// Update heap stats
			mInfo.CurrBytesUsed -= (*block)->mAllocSize;
			mInfo.CurrNumAlloc--;

			// Update mem stats
			pMem->mInfo.CurrBytesUsed -= (*block)->mAllocSize;
			pMem->mInfo.CurrNumAlloc--;

			// Free it
			HeapFree(mWinHeapHandle, 0, *block);
		}
		else {
			// Update heap pointers FIRST
			if ((*block)->hNext != nullptr) {
				(*block)->hNext->hPrev = (*block)->hPrev;
			}
			if ((*block)->hPrev != nullptr) {
				(*block)->hPrev->hNext = (*block)->hNext;
			}
			if (*block == pBlkHead) {
				pBlkHead = (*block)->hNext;
			}

			if (pFixedFreeHead != nullptr) {
				pFixedFreeHead->hPrev = *block;
				(*block)->hNext = pFixedFreeHead;
				(*block)->hPrev = nullptr;
				pFixedFreeHead = (*block);
			}
			else {
				pFixedFreeHead = (*block);
				(*block)->hNext = nullptr;
				(*block)->hPrev = nullptr;
			}

			// Update global pointers
			if ((*block)->gNext != nullptr) {
				(*block)->gNext->gPrev = (*block)->gPrev;
			}
			if ((*block)->gPrev != nullptr) {
				(*block)->gPrev->gNext = (*block)->gNext;
			}
			if (*block == pMem->pGlobalHead) {
				pMem->pGlobalHead = (*block)->gNext;
			}

			// Update heap stats
			mInfo.CurrBytesUsed -= (*block)->mAllocSize;
			mInfo.CurrNumAlloc--;

			// Update mem stats
			pMem->mInfo.CurrBytesUsed -= (*block)->mAllocSize;
			pMem->mInfo.CurrNumAlloc--;
		}
	}

	unsigned int Azul::Heap::getValue(Azul::Mem::Align align) {
		switch (align) {
		case(Azul::Mem::Align::Byte_4):
			return 4;
			break;
		case(Azul::Mem::Align::Byte_8):
			return 8;
			break;
		case(Azul::Mem::Align::Byte_16):
			return 16;
			break;
		case(Azul::Mem::Align::Byte_32):
			return 32;
			break;
		case(Azul::Mem::Align::Byte_64):
			return 64;
			break;
		case(Azul::Mem::Align::Byte_128):
			return 128;
			break;
		case(Azul::Mem::Align::Byte_256):
			return 256;
			break;
		default:
			return 4;
			break;
		}
	}

	const char* Azul::Heap::getFileName(const char* inName)
	{
		return strrchr(inName, '\\') ? getFileName(strrchr(inName, '\\') + 1) : inName;
	}
}

// ---  End of File ---------------
