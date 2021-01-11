//---------------------------------------------------------------
// Mike Bunger
// Block header
//---------------------------------------------------------------

#ifndef BLOCK_H
#define BLOCK_H

namespace Azul
{

	//-------------------------------------------------------------------------
	// Weird thing
	//   Please declare your forward declarations INSIDE namespace
	//   Outside the namespace cause all kinds of hell
	//-------------------------------------------------------------------------

	class Block
	{
	public:
		// defines for the heap name length
		static const uint32_t NameLength = 40;
		static const uint32_t NameNumChar = (Block::NameLength - 1);

	public:
		// Public methods
		Block* GetHeapNext() const;
		Block* GetHeapPrev() const;
		Block* GetGlobalNext() const;
		Block* GetGlobalPrev() const;
		char* GetFileName();
		size_t  GetLineNum() const;
		size_t  GetAllocIndex() const;
		size_t  GetAllocSize() const;

	private:
		friend class Heap;
		// Add tracking links 
		Block* hNext;
		Block* hPrev;
		Block* gNext;
		Block* gPrev;

		// stats
		char	mFileName[Block::NameLength];
		size_t	mLineNum;
		size_t	mAllocIndex;
		size_t	mAllocSize;

		// back pointer
		Block* pSecret;   // back pointer to the top of the trackingblock
	};

}

#endif

// ---  End of File ---------------
