//---------------------------------------------------------------
// Mike Bunger
// Heap header
//---------------------------------------------------------------

#ifndef HEAP_H
#define HEAP_H

#include "Mem.h"

void* operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
void* operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
void* operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum);
void operator delete(void* p);
void operator delete[](void* p);

namespace Azul
{

	//-------------------------------------------------------------------------
	// Namespace Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	// Forward declaration
	class Block;

	// Heap class
	class Heap
	{
	public:
		// Constants 
		static const uint32_t NameLength = 40;
		static const uint32_t NameNumChar = (NameLength - 1);

		// Heap Types
		enum class Type : size_t
		{
			NORMAL,
			FIXED
		};

		// Info structure
		struct Info
		{
			char Name[Heap::NameLength];
			size_t    StartAddr;
			size_t    EndAddr;
			size_t    TotalSize;
			size_t	  PeakNumAlloc;
			size_t	  CurrNumAlloc;
			size_t	  PeakBytesUsed;
			size_t	  CurrBytesUsed;
		};

	public:
		// Prevent from calling
		Heap() = delete;
		Heap(const Heap&) = delete;
		const Heap& operator = (const Heap& tmp) = delete;

		// Public Interface (For Customers)
		void GetInfo(Info& info) const;

		// Tracking block 
		Block* GetTrackingHead() const;
		Heap::Type GetHeapType() const;

		// Overflow Heap
		void SetOverflowHeap(Heap*);
		Heap* GetOverflowHeap() const;

		// Needed for Testings (Not for Customers)
		Heap* DebugGetNext() const;
		Heap* DebugGetPrev() const;

	// my private stash
	private:
		friend class Mem;
		friend void* ::operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
		friend void* ::operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
		friend void* ::operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum);
		friend void ::operator delete(void* p);
		friend void ::operator delete[](void* p);

		Heap(const char* name, Heap::Type type, Mem* mem, unsigned int size);
		Heap(const char* name, Heap::Type type, Mem* mem, unsigned int numBlocks, unsigned int sizePerBlock, Mem::Align align = Mem::Align::Default);

		void* Alloc(size_t inSize, Azul::Mem::Align align, const char* inName, size_t lineNum);
		void* FixedAlloc(const char* inName, size_t lineNum, size_t inSize);

		unsigned int getValue(Azul::Mem::Align align);
		const char* getFileName(const char* path);
		void Dealloc(void* p);

		// Heap DLL pointers
		Heap* pNext;
		Heap* pPrev;

	private:
		// data ------------------------------------------------
		Block* pBlkHead;
		Type	mType;
		HANDLE  mWinHeapHandle;
		Info	mInfo;

		// null for Normal, used only in FixedHeap
		Block* pFixedFreeHead;

		// Overflow heap
		Heap* pOverflow;

		// Back link to the memory system
		Mem* pMem;
	};

}


#endif

// ---  End of File ---------------
