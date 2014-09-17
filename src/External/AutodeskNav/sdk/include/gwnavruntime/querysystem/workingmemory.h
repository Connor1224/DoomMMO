/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_WorkingMemory_H
#define Navigation_WorkingMemory_H

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_Debug.h"

namespace Kaim
{

typedef KyUInt32 BufferIndex;
static const BufferIndex BufferIndex_Invalid = KyUInt32MAXVAL;
static const BufferIndex BufferIndex_0 = 0;
static const BufferIndex BufferIndex_1 = 1;
static const BufferIndex BufferIndex_2 = 2;
static const BufferIndex BufferIndex_3 = 3;
static const BufferIndex BufferIndex_4 = 4;
static const BufferIndex BufferIndex_Count = 5;

class AStarTraversalContext;
class PathRefinerContext;
class PathClamperContext;
class NavDataChangeIndexInGrid;
class SpatializedPointCollectorContext;

class WorkingMemory
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_WorldFwk)
	KY_CLASS_WITHOUT_COPY(WorkingMemory)

public:
	struct MyLimitHandler : public MemoryHeap::LimitHandler
	{
		virtual ~MyLimitHandler() {}
		virtual bool OnExceedLimit(MemoryHeap* /*heap*/, UPInt /*overLimit*/) { return false; };
		virtual void OnFreeSegment(MemoryHeap* /*heap*/, UPInt /*freeingSize*/) { };
	};

	struct WorkingBuffer
	{
		WorkingBuffer() : m_memory(KY_NULL), m_memorySize(0), m_inUse(false) {}

		void* m_memory;
		KyUInt32 m_memorySize;
		bool m_inUse;
	};

	/*
		\param memoryLimitSize				specifies the local heap memory allocation limit
		\param granularityOfBufferResize	specifies how the buffer size increase if more memory is needed
	*/
	WorkingMemory(KyUInt32 memoryLimitSize = 4 * 1024 * 1024 /* 4 Mb */, KyUInt32 granularityOfBufferResize = 5 * 1024/* 5 Kb */);
	~WorkingMemory();

	void Init(KyUInt32 memoryLimitSize, KyUInt32 granularityOfBufferResize);
	void InitMemoryHeap(KyUInt32 memoryLimitSize);

	void SetNewLimit(KyUInt32 memoryLimitSize);
	void ReleaseAllMemoryBuffer();

	void* ResizeBuffer(BufferIndex bufferIdx);
	void ReleaseMemory(void* memory)
	{
		/*KY_DEBUG_MESSAGEN(("release %x\n", (int)memory));*/
		KY_HEAP_FREE(m_memoryHeap, memory);
	}

	void InitBuffersToNull();

	BufferIndex TakeUsageOfFirstUnusedBufferIdx();
	void ReleaseBuffer(BufferIndex bufferIdx);

	void* GetBuffer(BufferIndex bufferIdx) const;
	KyUInt32 GetBufferSize(BufferIndex bufferIdx) const;

	KyUInt32 GetTotalAlloctedSize() const;

	// Used by astarQuery
	AStarTraversalContext* GetAStarTraversalContext() const;
	PathRefinerContext* GetPathRefinerContext() const;
	PathClamperContext* GetPathClamperContext() const;
	NavDataChangeIndexInGrid* GetNavDataChangeIndexInGrid() const;
public:
	MemoryHeap* m_memoryHeap;
	MyLimitHandler m_myLimitHandler;
	KyUInt32 m_granularityOfBufferResize;
	WorkingBuffer m_workingBuffer[BufferIndex_Count];

public:
	AStarTraversalContext* m_astarContext;
	PathRefinerContext* m_refinerContext;
	PathClamperContext* m_clamperContext;
	NavDataChangeIndexInGrid* m_navDataChangeIndexInGrid;
	SpatializedPointCollectorContext* m_collectorContext;
};


KY_INLINE void WorkingMemory::ReleaseAllMemoryBuffer()
{
	for(BufferIndex bufferIndex = BufferIndex_0; bufferIndex < BufferIndex_Count; ++bufferIndex)
	{
		KY_ASSERT(m_workingBuffer[bufferIndex].m_inUse == false);

		if (m_workingBuffer[bufferIndex].m_memory != KY_NULL)
			ReleaseMemory(m_workingBuffer[bufferIndex].m_memory);
	}
	InitBuffersToNull();
}

KY_INLINE BufferIndex WorkingMemory::TakeUsageOfFirstUnusedBufferIdx()
{
	for(BufferIndex bufferIdx = BufferIndex_0; bufferIdx < BufferIndex_Count; ++bufferIdx)
	{
		if(m_workingBuffer[bufferIdx].m_inUse == false)
		{
			m_workingBuffer[bufferIdx].m_inUse = true;
			return bufferIdx;
		}
	}

	return BufferIndex_Invalid;
}

KY_INLINE void WorkingMemory::ReleaseBuffer(BufferIndex bufferIdx) { m_workingBuffer[bufferIdx].m_inUse = false; }
KY_INLINE void* WorkingMemory::GetBuffer(BufferIndex bufferIdx) const
{
	KY_ASSERT(bufferIdx < BufferIndex_Count);
	return m_workingBuffer[bufferIdx].m_memory;
}

KY_INLINE KyUInt32 WorkingMemory::GetBufferSize(BufferIndex bufferIdx) const
{
	KY_ASSERT(bufferIdx < BufferIndex_Count);
	const WorkingBuffer* workingBuffer = m_workingBuffer + bufferIdx;
	return workingBuffer->m_memorySize;
}

KY_INLINE KyUInt32 WorkingMemory::GetTotalAlloctedSize() const
{
	KyUInt32 total = 0;
	for(BufferIndex bufferIdx = BufferIndex_0; bufferIdx < BufferIndex_Count; ++bufferIdx)
	{
		total += GetBufferSize(bufferIdx);
	}

	return total;
}

KY_INLINE void WorkingMemory::InitBuffersToNull()
{
	for(BufferIndex bufferIdx = BufferIndex_0; bufferIdx < BufferIndex_Count; ++bufferIdx)
	{
		m_workingBuffer[bufferIdx].m_memory = KY_NULL;
		m_workingBuffer[bufferIdx].m_memorySize = 0;
		m_workingBuffer[bufferIdx].m_inUse = false;
	}
}

KY_INLINE AStarTraversalContext*    WorkingMemory::GetAStarTraversalContext()    const { return m_astarContext;             }
KY_INLINE PathRefinerContext*       WorkingMemory::GetPathRefinerContext()       const { return m_refinerContext;           }
KY_INLINE PathClamperContext*       WorkingMemory::GetPathClamperContext()       const { return m_clamperContext;           }
KY_INLINE NavDataChangeIndexInGrid* WorkingMemory::GetNavDataChangeIndexInGrid() const { return m_navDataChangeIndexInGrid; }
}

#endif //Navigation_NavMeshQueryBuffer_H

