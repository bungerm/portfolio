//---------------------------------------------------------------
// Mike Bunger
// Memory Block
//
// Mostly getters
//---------------------------------------------------------------

#include "Block.h"

namespace Azul
{
	Block* Block::GetHeapNext() const
	{
		return hNext;
	}

	Block* Block::GetHeapPrev() const
	{
		return hPrev;
	}

	Block* Block::GetGlobalNext() const
	{
		return gNext;
	}

	Block* Block::GetGlobalPrev() const
	{
		return gPrev;
	}

	char* Block::GetFileName()
	{
		return mFileName;
	}

	size_t Block::GetLineNum() const
	{
		return mLineNum;
	}

	size_t Block::GetAllocIndex() const
	{
		return mAllocIndex;
	}

	size_t Block::GetAllocSize() const
	{
		return mAllocSize;
	}

}

// ---  End of File ---------------
