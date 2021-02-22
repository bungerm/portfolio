//---------------------------------------------------------------
// Mike Bunger
// GlobalNew header
//
// new and delete for normal heaps, fixed heaps, and global
//---------------------------------------------------------------

#ifndef GLOBAL_NEW_H
#define GLOBAL_NEW_H

#include "Mem.h"
#include "Heap.h"

	// For Normal Heap

	// NEW: takes type size, heap pointer, alignment, name, and line number
	void* operator new(size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
	// DELETE: takes void pointer, heap pointer, alignment, name and line number
	void operator delete(void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);

	// ARRAY NEW: same paramters as new
	void* operator new[](size_t inSize, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);
	// ARRAY DELETE: same paramters as delete
	void operator delete[](void* p, Azul::Heap* const pHeap, Azul::Mem::Align align, const char* inName, size_t lineNum);

	// For Fixed Heap

	// Fixed NEW: takes type size, heap pointer, name, and line number
	void* operator new(size_t inSize, Azul::Heap* const pHeap, const char* inName, size_t lineNum);
	// Fixed DELETE: takes void pointer, heap pointer, name, and line number
	void operator delete(void* p, Azul::Heap* const pHeap, const char* inName, size_t lineNum);


	// For Global
	
	void* operator new(size_t inSize);
	void operator delete(void* p);

	void* operator new[](size_t inSize);
	void operator delete[](void* p);

#endif

// ---  End of File ---------------
