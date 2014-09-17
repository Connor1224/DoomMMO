/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: GUAL - secondary contact: NOBODY


namespace Kaim
{

KY_INLINE void Path::AddRef() { ++RefCount; }
KY_INLINE void Path::Release()
{
	if (RefCount-- == 1)
	{
		ClearBuffers();
		KY_FREE(this);
		return;
	}
}

KY_INLINE void Path::SetNodePosition       (KyUInt32 nodeIdx, const Vec3f& position                     ) { m_nodePositions[nodeIdx]          = position;               }
KY_INLINE void Path::SetNodeNavTrianglePtr (KyUInt32 nodeIdx, const NavTrianglePtr& navTrianglePtr      ) { m_nodetrianglePtrs[nodeIdx]       = navTrianglePtr;         }
KY_INLINE void Path::SetEdgeNavGraphEdgePtr(KyUInt32 pathEdgeIdx, const NavGraphEdgePtr& navGraphEdgePtr) { m_edgeNavGraphEdgePtr[pathEdgeIdx] = navGraphEdgePtr;       }
KY_INLINE void Path::SetPathEdgeType       (KyUInt32 pathEdgeIdx, PathEdgeType pathEdgeType             ) { m_edgeTypes[pathEdgeIdx]           = (KyUInt8)pathEdgeType; }

KY_INLINE KyUInt32               Path::GetNodeCount()                              const { return m_nodesCount;                           }
KY_INLINE KyUInt32               Path::GetEdgeCount()                              const { return m_edgesCount;                           }
KY_INLINE const Vec3f&           Path::GetNodePosition(KyUInt32 nodeIdx)            const { return m_nodePositions[nodeIdx];               }
KY_INLINE Vec3f&                 Path::GetNodePosition(KyUInt32 nodeIdx)                  { return m_nodePositions[nodeIdx];               }
KY_INLINE const NavTrianglePtr&  Path::GetNodeNavTrianglePtr(KyUInt32 nodeIdx)      const { return m_nodetrianglePtrs[nodeIdx];            }
KY_INLINE NavTrianglePtr&        Path::GetNodeNavTrianglePtr(KyUInt32 nodeIdx)            { return m_nodetrianglePtrs[nodeIdx];            }
KY_INLINE const NavGraphEdgePtr& Path::GetEdgeNavGraphEdgePtr(KyUInt32 pathEdgeIdx) const { return m_edgeNavGraphEdgePtr[pathEdgeIdx];     }
KY_INLINE NavGraphEdgePtr&       Path::GetEdgeNavGraphEdgePtr(KyUInt32 pathEdgeIdx)       { return m_edgeNavGraphEdgePtr[pathEdgeIdx];     }
KY_INLINE PathEdgeType           Path::GetPathEdgeType(KyUInt32 pathEdgeIdx)        const { return (PathEdgeType)m_edgeTypes[pathEdgeIdx]; }
KY_INLINE const Vec3f&           Path::GetPathEdgeStartPosition(KyUInt32 edgeIdx)   const { return GetNodePosition(edgeIdx);               }
KY_INLINE const Vec3f&           Path::GetPathEdgeEndPosition(KyUInt32 edgeIdx)     const { return GetNodePosition(edgeIdx + 1);           }
KY_INLINE const Vec3f&           Path::GetPathStartPosition()                       const { return GetNodePosition(0);                     }
KY_INLINE const Vec3f&           Path::GetPathEndPosition()                         const { return GetNodePosition(GetNodeCount() - 1);   }
KY_INLINE const Vec3f*           Path::GetNodePositionBuffer()                      const { return m_nodePositions;                        }

template <class T>
KY_INLINE void Path::ClearBufferCPP(T*& buffer, KyUInt32 count)
{
	ConstructorCPP<T>::DestructArray(buffer, count);
	buffer = KY_NULL;
}

template <class T>
KY_INLINE void Path::ClearBufferPOD(T*& buffer, KyUInt32 count)
{
	ConstructorPOD<T>::DestructArray(buffer, count);
	buffer = KY_NULL;

}

template <class T>
KY_INLINE void Path::InitBufferCPP(T*& buffer, KyUInt32 count, char*& memory)
{
	if (count != 0)
	{
		buffer = (T*)memory;
		memory += count * sizeof(T);
		ConstructorCPP<T>::ConstructArray(buffer, count);
	}
	else
	{
		buffer = KY_NULL;
	}
}

template <class T>
KY_INLINE void Path::InitBufferPOD(T*& buffer, KyUInt32 count, char*& memory)
{
	if (count != 0)
	{
		buffer = (T*)memory;
		memory += count * sizeof(T);
		ConstructorPOD<T>::ConstructArray(buffer, count);
	}
	else
	{
		buffer = KY_NULL;
	}
}

}
