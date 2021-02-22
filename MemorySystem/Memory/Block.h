//---------------------------------------------------------------
// Mike Bunger
// Block header
//
// The Block class keeps track of block statistics and links
// in the heap and global linked lists
//---------------------------------------------------------------

#ifndef BLOCK_H
#define BLOCK_H

namespace Azul
{
	class Block
	{
	public:
		// defines for the heap name length
		static const uint32_t NameLength = 40;
		static const uint32_t NameNumChar = (Block::NameLength - 1);

	public:
		// getters
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
		// next and previous in the heap
		Block* hNext;
		Block* hPrev;
		// next and previous globally
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
