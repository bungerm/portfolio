//---------------------------------------------------------------
// Mike Bunger
// Global New
//---------------------------------------------------------------

#include "Mem.h"
#include "GlobalNew.h"

//-------------------------------------------------------------------------
// Namespace Weird thing:
//   Don't use namespace here...
//   Since new/delete isn't allowed namespace
//   do all declaraton with full name Azul::Heap... or Azul::Mem::Align...
//-------------------------------------------------------------------------

// --------------------------------------------
// For Normal Heap
// --------------------------------------------

	void* operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		void* p = pHeap->Alloc(inSize, align, inName, lineNum);
		return p;
	}

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

	void* operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum)
	{
		void* p = pHeap->Alloc(inSize, align, inName, lineNum);
		return p;
	}

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

	void* operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum)
	{
		void* p = pHeap->FixedAlloc(inName, lineNum, inSize);
		return p;
	}

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

	void* operator new(size_t inSize)
	{
		void* p = malloc(inSize);
		return p;
	}

	void operator delete(void* p)
	{
		Azul::Heap* h;
		if (Azul::Mem::GetHeapByAddr(h, p) == Azul::Mem::Code::OK) {
			h->Dealloc(p);
		}
		else {
			free(p);
		}
	}

	void* operator new[](size_t inSize)
	{
		void* p = malloc(inSize);
		return p;
	}

	void operator delete[](void* p)
	{
		Azul::Heap* h;
		if (Azul::Mem::GetHeapByAddr(h, p) == Azul::Mem::Code::OK) {
			h->Dealloc(p);
		}
		else {
			free(p);
		}
	}


// ---  End of File ---------------
