/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_GrowingSmallBufferPool_H
#define GwNavGen_GrowingSmallBufferPool_H


#include "gwnavgeneration/common/generatormemory.h"
#include "gwnavgeneration/common/stlvector.h"


namespace Kaim
{


class GrowingSmallBufferPool
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	explicit GrowingSmallBufferPool(MemoryHeap* heap, KyUInt32 byteCountInChunk = 0);

	~GrowingSmallBufferPool();

	KyUInt32 GetWordCount(KyInt32 byteCount);

	void* GetNewBuffer(KyUInt32 byteCount);

	void Clear();

	void Release();

	KyUInt32 ByteCountAllocated() const { return m_byteCountAllocated; }

private:
	KyUInt32 m_maxWordCountInChunk;

	TlsStlVector<KyUInt32*> m_chunks; // TODO use KyArray
	KyUInt32 m_chunkCount;
	KyUInt32 m_wordCountInLastChunk;

	TlsStlVector<KyUInt32*> m_oversizedBuffers; // TODO use KyArray

	KyUInt32 m_byteCountAllocated;

	MemoryHeap* m_heap;
};


}


#endif
