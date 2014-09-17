/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


namespace Kaim
{

template <class T>
KY_INLINE WorkingMemArray<T>::WorkingMemArray() : m_itemCount(0), m_capacity(0) {}

template <class T>
KY_INLINE WorkingMemArray<T>::WorkingMemArray(WorkingMemory* workingMemory)
{
	Init(workingMemory);
}

template <class T>
KY_INLINE void WorkingMemArray<T>::Init(WorkingMemory* workingMemory)
{
	if (KY_SUCCEEDED(m_workingMemContainerBase.Init(workingMemory)))
		m_capacity = m_workingMemContainerBase.GetBufferSize() / sizeof(T);
	else
		m_capacity = 0;

	m_itemCount = 0;
}

template <class T>
KY_INLINE T*       WorkingMemArray<T>::GetBuffer()     const { return (T*)m_workingMemContainerBase.GetBuffer(); }
template <class T>
KY_INLINE bool     WorkingMemArray<T>::IsInitialized() const { return m_workingMemContainerBase.IsInitialized(); }
template <class T>
KY_INLINE KyUInt32 WorkingMemArray<T>::GetCount()      const { return m_itemCount;                          }
template <class T>
KY_INLINE KyUInt32 WorkingMemArray<T>::GetCapacity()   const { return m_capacity;                           }
template <class T>
KY_INLINE bool     WorkingMemArray<T>::IsEmpty()       const { return m_itemCount == 0;                     }

template <class T>
KY_INLINE void WorkingMemArray<T>::MakeEmpty() { m_itemCount = 0; }
template <class T>
KY_INLINE bool WorkingMemArray<T>::IsFull() { return m_itemCount == m_capacity && KY_FAILED(TryToResize()); }


template <class T>
KY_INLINE KyResult WorkingMemArray<T>::PushBack(const T& x)
{
	KY_DEBUG_ASSERTN(IsInitialized(), ("You are trying to add an item wheareas the buffer is not valid"));

	if (IsFull() == false)
	{
		GetBuffer()[m_itemCount++] = x;
		return KY_SUCCESS;
	}

	return KY_ERROR;
}

template <class T>
KY_INLINE void WorkingMemArray<T>::PushBack_UnSafe(const T& x)
{
	KY_ASSERT(IsFull() == false);
	GetBuffer()[m_itemCount++] = x;
}

template <class T>
KY_INLINE KyResult WorkingMemArray<T>::PopBack()
{
	KY_ASSERT(IsEmpty() == false);
	--m_itemCount;
	return KY_SUCCESS;
}


template <class T>
KY_INLINE const T& WorkingMemArray<T>::operator[](KyUInt32 idx) const
{
	KY_DEBUG_ASSERTN(IsInitialized(), ("You are trying to add an item wheareas the buffer is not valid, it does not contains any item "));
	KY_DEBUG_ASSERTN(idx < GetCount(), ("Invalid input idx. It must be lower than GetSize() == %d", GetCount()));

	return GetBuffer()[idx];
}

template <class T>
KY_INLINE T& WorkingMemArray<T>::operator[](KyUInt32 idx)
{
	KY_DEBUG_ASSERTN(IsInitialized(), ("You are trying to add an item wheareas the buffer is not valid, it does not contains any item "));
	KY_DEBUG_ASSERTN(idx < GetCount(), ("Invalid input idx. It must be lower than GetSize() == %d", GetCount()));

	return GetBuffer()[idx];
}

template <class T>
KY_INLINE KyResult WorkingMemArray<T>::SetMinimumCapacity(KyUInt32 minCapacity)
{
	KY_DEBUG_ASSERTN(IsInitialized(), ("You are trying to change the capacity wheareas the buffer is not valid"));
	while (m_capacity < minCapacity)
	{
		if (KY_FAILED(TryToResize()))
			return KY_ERROR;
	}

	return KY_SUCCESS;
}

template <class T>
KY_INLINE KyResult WorkingMemArray<T>::TryToResize()
{
	KY_DEBUG_ASSERTN(IsInitialized(), ("You are trying to change the capacity wheareas the buffer is not valid"));

	T* oldBuffer = GetBuffer();
	T* newBuffer = (T*)m_workingMemContainerBase.Resize();

	if (newBuffer == KY_NULL)
		return KY_ERROR;

	if (oldBuffer != KY_NULL)
	{
		memcpy(newBuffer, oldBuffer, m_itemCount * sizeof(T));
		m_workingMemContainerBase.ReleaseMemory(oldBuffer);
	}

	m_capacity = m_workingMemContainerBase.GetBufferSize() / sizeof(T);
	return KY_SUCCESS;
}

}

