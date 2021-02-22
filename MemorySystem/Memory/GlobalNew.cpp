//---------------------------------------------------------------
// Mike Bunger
// Global New
//
// new and delete for normal heaps, fixed heaps, and global
//---------------------------------------------------------------

#include "Mem.h"
#include "GlobalNew.h"

// --------------------------------------------
// For Normal Heap
// --------------------------------------------

	// normal heap new
	void* operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		// Our Heap Alloc, return pointer
		void* p = pHeap->Alloc(inSize, align, inName, lineNum);
		return p;
	}

	// normal heap delete
	void operator delete(void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		// -----------------------------------------
		// You don't need to implement this... 
		// its here to shut up compiler warnings
		// just this function
		//------------------------------------------
		REPLACE_ME(p);
		REPLACE_ME(pHeap);
		REPLACE_ME(align);
		REPLACE_ME(inName);
		REPLACE_ME(lineNum);
	}

	// normal heap array new
	void* operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		// Our Heap Alloc, return pointer
		void* p = pHeap->Alloc(inSize, align, inName, lineNum);
		return p;
	}

	// normal heap array delete
	void operator delete[](void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		// -----------------------------------------
		// You don't need to implement this... 
		// its here to shut up compiler warnings
		// just this function
		//------------------------------------------
		REPLACE_ME(p);
		REPLACE_ME(pHeap);
		REPLACE_ME(align);
		REPLACE_ME(inName);
		REPLACE_ME(lineNum);
	}

// --------------------------------------------
// For Fixed Heap
// --------------------------------------------

	// fixed heap new
	void* operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum)
	{
		// Our FixedAlloc, return pointer
		void* p = pHeap->FixedAlloc(inName, lineNum, inSize);
		return p;
	}

	// fixed heap delete
	void operator delete(void* p, Azul::Heap* const pHeap, const char* inName, size_t lineNum)
	{
		// -----------------------------------------
		// You don't need to implement this... 
		// its here to shut up compiler warnings
		// just this function
		//------------------------------------------
		REPLACE_ME(p);
		REPLACE_ME(pHeap);
		REPLACE_ME(inName);
		REPLACE_ME(lineNum);
	}

// --------------------------------------------
// For Global Standard new/delete
// --------------------------------------------

	// global new
	void* operator new(size_t inSize)
	{
		// standard malloc
		void* p = malloc(inSize);
		return p;
	}

	// global delete
	void operator delete(void* p)
	{
		// since this is a global delete, we need to check if p belongs to one of our heaps
		// if it does, Dealloc it, otherwise use standard free
		Azul::Heap* h;
		if (Azul::Mem::GetHeapByAddr(h, p) == Azul::Mem::Code::OK) {
			h->Dealloc(p);
		}
		else {
			free(p);
		}
	}

	// global array new
	void* operator new[](size_t inSize)
	{
		// standard malloc
		void* p = malloc(inSize);
		return p;
	}

	// global array delete
	void operator delete[](void* p)
	{
		// since this is a global delete, we need to check if p belongs to one of our heaps
		// if it does, Dealloc it, otherwise use standard free
		Azul::Heap* h;
		if (Azul::Mem::GetHeapByAddr(h, p) == Azul::Mem::Code::OK) {
			h->Dealloc(p);
		}
		else {
			free(p);
		}
	}


// ---  End of File ---------------
