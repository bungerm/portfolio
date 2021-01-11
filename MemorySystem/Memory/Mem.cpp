//---------------------------------------------------------------
// Mike Bunger
// Mem
//---------------------------------------------------------------

#include "Mem.h"
#include "Block.h"
#include "Heap.h"

// initialize the static variable "instance" outside class declaration (for reasons I dont understand)
Azul::Mem* Azul::Mem::instance = nullptr;

namespace Azul
{
	// Create the heap.
	Mem::Code Mem::CreateNormalHeap(Heap*& newHeap, unsigned int heapSize, const char* const Name)
	{
		if (instance == nullptr) {
			newHeap = nullptr;
			return Mem::Code::ERROR_Mem_Not_Initialized;
		}

		newHeap = new Heap(Name, Heap::Type::NORMAL, instance, heapSize);

		if (instance->poHead == nullptr) {
			instance->poHead = newHeap;
		}
		else {
			newHeap->pNext = instance->poHead;
			instance->poHead->pPrev = newHeap;
			instance->poHead = newHeap;
		}

		instance->mInfo.CurrHeapCount++;
		if (instance->mInfo.PeakHeapCount < instance->mInfo.CurrHeapCount) {
			instance->mInfo.PeakHeapCount = instance->mInfo.CurrHeapCount;
		}

		return Mem::Code::OK;
	}

	Mem::Code Mem::CreateFixedHeap(Heap*& newHeap, unsigned int numBlocks, int sizePerBlock, const char* const Name)
	{
		if (instance == nullptr) {
			newHeap = nullptr;
			return Mem::Code::ERROR_Mem_Not_Initialized;
		}

		newHeap = new Heap(Name, Heap::Type::FIXED, instance, numBlocks, sizePerBlock);

		if (instance->poHead == nullptr) {
			instance->poHead = newHeap;
		}
		else {
			newHeap->pNext = instance->poHead;
			instance->poHead->pPrev = newHeap;
			instance->poHead = newHeap;
		}

		instance->mInfo.CurrHeapCount++;
		if (instance->mInfo.PeakHeapCount < instance->mInfo.CurrHeapCount) {
			instance->mInfo.PeakHeapCount = instance->mInfo.CurrHeapCount;
		}

		return Mem::Code::OK;
	}

	// Initialize the memory system  (Public method)
	Mem::Code Mem::Create(void)
	{
		Destroy();
		if (instance == nullptr) {
			instance = new Mem();
			instance->mInitialized = true;
		}
		return Mem::Code::OK;
	}

	Mem::Code Mem::Destroy(void)
	{
		if (instance == nullptr) {
			return Mem::Code::ERROR_Mem_Not_Initialized;
		}
		delete instance;
		instance = nullptr;
		return Mem::Code::OK;
	}

	Mem::Code Mem::GetInfo(Mem::Info& retInfo)
	{
		if (instance == nullptr) {
			return Mem::Code::ERROR_Mem_Not_Initialized;
		}
		retInfo = instance->mInfo;
		return Mem::Code::OK;
	}

	Heap* Mem::DebugGetHeapHead()
	{
		return instance->poHead;
	}

	Block* Mem::DebugGetGlobalTrackingHead()
	{
		return instance->pGlobalHead;
	}

	Mem::Code Mem::DestroyHeap(Heap* pInHeap)
	{
		if (instance == nullptr) {
			return Mem::Code::ERROR_Mem_Not_Initialized;
		}

		Heap* temp = instance->poHead;
		while (temp != nullptr) {
			if (pInHeap == temp) {
				instance->mInfo.CurrHeapCount--;
				instance->mInfo.CurrBytesUsed -= pInHeap->mInfo.CurrBytesUsed;
				instance->mInfo.CurrNumAlloc -= pInHeap->mInfo.CurrNumAlloc;

				if (pInHeap == instance->poHead) {
					instance->poHead = pInHeap->pNext;
				}
				if (pInHeap->pPrev != nullptr) {
					pInHeap->pPrev->pNext = pInHeap->pNext;
				}
				if (pInHeap->pNext != nullptr) {
					pInHeap->pNext->pPrev = pInHeap->pPrev;
				}

				HeapDestroy(pInHeap->mWinHeapHandle);

				return Mem::Code::OK;
			}
			temp = temp->pNext;
		}
		return Mem::Code::ERROR_Illegal_Heap;
	}

	Mem::Code Mem::GetHeapByAddr(Heap*& pHeap, void* p)
	{
		if (instance == nullptr) {
			return Mem::Code::ERROR_Mem_Not_Initialized;
		}

		Heap* heaps = instance->poHead;
		if (heaps == nullptr) {
			return Mem::Code::ERROR_Invalid_Addr;
		}
		Block* blocks = heaps->pBlkHead;
		Block** check = (Block**)((size_t)p-sizeof(Block*));

		while (heaps != nullptr) {
			while (blocks != nullptr) {
				// if we have found our block
				if (*check == blocks) {
					pHeap = heaps;
					return Mem::Code::OK;
				}
				blocks = blocks->GetHeapNext();
			}
			heaps = heaps->pNext;
			if (heaps != nullptr) {
				blocks = heaps->pBlkHead;
			}
		}
		return Mem::Code::ERROR_Invalid_Addr;
	}

	Mem::Mem() : poHead(nullptr), pGlobalHead(nullptr), mInfo{0}, mInitialized(false), pad{0}
	{
		// daves not here mannnnn
	}

	Azul::Mem::~Mem()
	{
		Heap* h = this->poHead;
		Heap* d = h;
		while (h != nullptr) {
			h = h->pNext;
			DestroyHeap(d);
			d = h;
		}
	}
}

// ---  End of File ---------------
