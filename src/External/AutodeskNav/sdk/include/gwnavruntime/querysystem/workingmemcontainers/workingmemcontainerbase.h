/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_WorkingMemContainerBase_H
#define Navigation_WorkingMemContainerBase_H

#include "gwnavruntime/querysystem/workingmemory.h"

namespace Kaim
{

class WorkingMemContainerBase
{
public:
	WorkingMemContainerBase() : m_workingMemory(KY_NULL), m_bufferIdx(BufferIndex_Invalid) {}
	WorkingMemContainerBase(WorkingMemory* workingMemory);

	KyResult Init(WorkingMemory* workingMemory);

	~WorkingMemContainerBase();

	bool IsInitialized() const;
	void* GetBuffer() const;
	void* Resize();
	void ReleaseMemory(void* memory);
	void ReleaseBuffer();
	KyUInt32 GetBufferSize() const;

public :
	WorkingMemory* m_workingMemory;
	BufferIndex m_bufferIdx;
};

KY_INLINE WorkingMemContainerBase::WorkingMemContainerBase(WorkingMemory* workingMemory) { Init(workingMemory); }

KY_INLINE KyResult WorkingMemContainerBase::Init(WorkingMemory* workingMemory)
{
	BufferIndex bufferIdx = workingMemory->TakeUsageOfFirstUnusedBufferIdx();

	if(bufferIdx == BufferIndex_Invalid)
		return KY_ERROR;

	m_workingMemory = workingMemory;
	m_bufferIdx = bufferIdx;

	if (workingMemory->GetBuffer(bufferIdx) == KY_NULL)
	{
		if (workingMemory->ResizeBuffer(bufferIdx) == KY_NULL)
		{
			return KY_ERROR;
		}
	}

	return KY_SUCCESS;
}

KY_INLINE bool     WorkingMemContainerBase::IsInitialized() const { return m_workingMemory != KY_NULL;                  }
KY_INLINE void*    WorkingMemContainerBase::GetBuffer()     const { return m_workingMemory->GetBuffer(m_bufferIdx);     }
KY_INLINE KyUInt32 WorkingMemContainerBase::GetBufferSize() const { return m_workingMemory->GetBufferSize(m_bufferIdx); }

KY_INLINE void* WorkingMemContainerBase::Resize() { return m_workingMemory->ResizeBuffer(m_bufferIdx); }

KY_INLINE void WorkingMemContainerBase::ReleaseMemory(void* memory) { m_workingMemory->ReleaseMemory(memory); }

KY_INLINE void WorkingMemContainerBase::ReleaseBuffer()
{
	if(m_workingMemory != KY_NULL)
	{
		m_workingMemory->ReleaseBuffer(m_bufferIdx);

		m_workingMemory = KY_NULL;
		m_bufferIdx = BufferIndex_Invalid;
	}
}

KY_INLINE WorkingMemContainerBase::~WorkingMemContainerBase()
{
	ReleaseBuffer();
}
}


#endif //Navigation_WorkingMemContainerBase_H

