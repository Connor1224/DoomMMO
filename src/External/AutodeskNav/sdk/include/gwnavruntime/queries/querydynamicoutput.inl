/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


namespace Kaim
{


template <class T>
KY_INLINE void QueryOutputBuffer_Impl<T>::ClearBuffer()
{
	ConstructorCPP<T>::DestructArray(m_buffer, m_count);
	m_buffer = KY_NULL;
	m_count = 0;
}

template <class T>
KY_INLINE void QueryOutputBuffer_Impl<T>::InitBuffer(KyUInt32 count, char*& memory)
{
	if (count != 0)
	{
		m_count = count;
		m_buffer = (T*)memory;
		memory += count * sizeof(T);
		ConstructorCPP<T>::ConstructArray(m_buffer, count);
	}
	else
	{
		m_buffer = KY_NULL;
		m_count = 0;
	}
}

template <class T>
KY_INLINE KyUInt32 QueryOutputBuffer_Impl<T>::GetCount() const { return m_count; }

template <class T>
KY_INLINE const T& QueryOutputBuffer_Impl<T>::Get(KyUInt32 index) const
{
	KY_ASSERT(index < m_count);
	return m_buffer[index];
}


template <class T>
KY_INLINE T& QueryOutputBuffer_Impl<T>::Get(KyUInt32 index)
{
	KY_ASSERT(index < m_count);
	return m_buffer[index];
}

template <class T>
KY_INLINE void QueryOutputBuffer_Impl<T>::Set(KyUInt32 index, const T& object)
{
	KY_ASSERT(index < m_count);
	m_buffer[index] = object;
}


KY_INLINE QueryDynamicOutput::QueryDynamicOutput() : m_byteSize(0), RefCount(1) {}

KY_INLINE QueryDynamicOutput::Config::Config() :
	m_floorCount(0),
	m_triangleCount(0),
	m_subSegmentCount(0),
	m_halfEdgeIntersectionCount(0),
	m_spatializedPointCount(0) {}

KY_INLINE bool QueryDynamicOutput::IsEnoughMemoryForStoringDynamicOutput(const Config& config)
{
	return ComputeByteSize(config) <= m_byteSize;
}

KY_INLINE KyUInt32 QueryDynamicOutput::GetNavFloorPtrCount()          const { return m_navFloorPtrs.GetCount();           }
KY_INLINE KyUInt32 QueryDynamicOutput::GetNavTrianglePtrCount()       const { return m_navTrianglePtrs.GetCount();        }
KY_INLINE KyUInt32 QueryDynamicOutput::GetNavTagSubSegmentCount()     const { return m_navTagSubSegments.GetCount();      }
KY_INLINE KyUInt32 QueryDynamicOutput::GetHalfEdgeIntersectionCount() const { return m_halfEgdeIntersections.GetCount();  }
KY_INLINE KyUInt32 QueryDynamicOutput::GetSpatializedPointCount()     const { return m_spatializedPoints.GetCount();      }

KY_INLINE const NavFloorPtr&           QueryDynamicOutput::GetNavFloorPtr(KyUInt32 floorPtrIndex)                      const { return m_navFloorPtrs.Get(floorPtrIndex);                      }
KY_INLINE       NavFloorPtr&           QueryDynamicOutput::GetNavFloorPtr(KyUInt32 floorPtrIndex)                            { return m_navFloorPtrs.Get(floorPtrIndex);                      }
KY_INLINE const NavTrianglePtr&        QueryDynamicOutput::GetNavTrianglePtr(KyUInt32 trianglePtrIndex)                const { return m_navTrianglePtrs.Get(trianglePtrIndex);                }
KY_INLINE       NavTrianglePtr&        QueryDynamicOutput::GetNavTrianglePtr(KyUInt32 trianglePtrIndex)                      { return m_navTrianglePtrs.Get(trianglePtrIndex);                }
KY_INLINE const NavTagSubSegment&      QueryDynamicOutput::GetNavTagSubSegment(KyUInt32 navTagSubSegmentIndex)         const { return m_navTagSubSegments.Get(navTagSubSegmentIndex);               }
KY_INLINE       NavTagSubSegment&      QueryDynamicOutput::GetNavTagSubSegment(KyUInt32 navTagSubSegmentIndex)               { return m_navTagSubSegments.Get(navTagSubSegmentIndex);               }
KY_INLINE const HalfEdgeIntersection&  QueryDynamicOutput::GetHalfEdgeIntersection(KyUInt32 halfEdgeIntersectionIndex) const { return m_halfEgdeIntersections.Get(halfEdgeIntersectionIndex); }
KY_INLINE       HalfEdgeIntersection&  QueryDynamicOutput::GetHalfEdgeIntersection(KyUInt32 halfEdgeIntersectionIndex)       { return m_halfEgdeIntersections.Get(halfEdgeIntersectionIndex); }
KY_INLINE const Ptr<SpatializedPoint>& QueryDynamicOutput::GetSpatializedPoint(KyUInt32 spatializedPointIndex)         const { return m_spatializedPoints.Get(spatializedPointIndex);         }
KY_INLINE       Ptr<SpatializedPoint>& QueryDynamicOutput::GetSpatializedPoint(KyUInt32 spatializedPointIndex)               { return m_spatializedPoints.Get(spatializedPointIndex);         }

KY_INLINE void QueryDynamicOutput::SetNavFloorPtr(KyUInt32 floorPtrIndex, const NavFloorPtr& navFloorPtr)                                         { m_navFloorPtrs.Set(floorPtrIndex, navFloorPtr);                               }
KY_INLINE void QueryDynamicOutput::SetNavTrianglePtr(KyUInt32 trianglePtrIndex, const NavTrianglePtr& navTrianglePtr)                             { m_navTrianglePtrs.Set(trianglePtrIndex, navTrianglePtr);                      }
KY_INLINE void QueryDynamicOutput::SetNavTagSubSegment(KyUInt32 navTagSubSegmentIndex, const NavTagSubSegment& navTagSubSegment)                  { m_navTagSubSegments.Set(navTagSubSegmentIndex, navTagSubSegment);                      }
KY_INLINE void QueryDynamicOutput::SetHalfEdgeIntersection(KyUInt32 halfEdgeIntersectionIndex, const HalfEdgeIntersection& halfEdgeIntersection)  { m_halfEgdeIntersections.Set(halfEdgeIntersectionIndex, halfEdgeIntersection); }
KY_INLINE void QueryDynamicOutput::SetSpatializedPoint(KyUInt32 spatializedPointIndex, const Ptr<SpatializedPoint>& spatializedPoint)             { m_spatializedPoints.Set(spatializedPointIndex, spatializedPoint);             }

KY_INLINE ScopeAutoSaveDynamicOutput::ScopeAutoSaveDynamicOutput(Ptr<QueryDynamicOutput>& dynamicOutput) :
	m_queryDynamicOutput(&dynamicOutput),
	m_navFloorRawPtrs(KY_NULL),
	m_navTriangleRawPtrs(KY_NULL),
	m_rawNavTagSubSegments(KY_NULL),
	m_rawHalfEdgeIntersections(KY_NULL),
	m_spatializedPoints(KY_NULL) {}

KY_INLINE void ScopeAutoSaveDynamicOutput::SetNavFloorRawPtrs(WorkingMemArray<NavFloorRawPtr>* navFloorRawPtrs)                            { m_navFloorRawPtrs          = navFloorRawPtrs;          }
KY_INLINE void ScopeAutoSaveDynamicOutput::SetNavTriangleRawPtrs(WorkingMemArray<NavTriangleRawPtr>* navTriangleRawPtrs)                   { m_navTriangleRawPtrs       = navTriangleRawPtrs;       }
KY_INLINE void ScopeAutoSaveDynamicOutput::SetRawNavTagSubSegments(WorkingMemArray<RawNavTagSubSegment>* rawNavTagSubSegments)             { m_rawNavTagSubSegments     = rawNavTagSubSegments;     }
KY_INLINE void ScopeAutoSaveDynamicOutput::SetRawHalfEdgeIntersections(WorkingMemArray<RawHalfEdgeIntersection>* rawHalfEdgeIntersections) { m_rawHalfEdgeIntersections = rawHalfEdgeIntersections; }
KY_INLINE void ScopeAutoSaveDynamicOutput::SetSpatializedPoints(WorkingMemArray<SpatializedPoint*>* spatializedPoints)                     { m_spatializedPoints        = spatializedPoints;        }

KY_INLINE ScopeAutoSaveDynamicOutput::~ScopeAutoSaveDynamicOutput()
{
	if (m_navFloorRawPtrs    != KY_NULL || m_navTriangleRawPtrs       != KY_NULL ||
		m_rawNavTagSubSegments  != KY_NULL || m_rawHalfEdgeIntersections != KY_NULL ||
		m_spatializedPoints  != KY_NULL)
	{
		CopyCrossedDataIntoDynamicOutput();
	}
}

} // namespace Kaim
