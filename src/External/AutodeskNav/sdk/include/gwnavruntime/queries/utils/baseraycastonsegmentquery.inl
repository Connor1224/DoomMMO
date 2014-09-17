/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

KY_INLINE void BaseRayCastOnSegmentQuery::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)               { m_startIntegerPos        = startIntegerPos;        }
KY_INLINE void BaseRayCastOnSegmentQuery::SetSegmentStartIntegerPos(const WorldIntegerPos& segmentStartIntegerPos) { m_segmentStartIntegerPos = segmentStartIntegerPos; }
KY_INLINE void BaseRayCastOnSegmentQuery::SetSegmentEndIntegerPos(const WorldIntegerPos& segmentEndIntegerPos)     { m_segmentEndIntegerPos   = segmentEndIntegerPos;   }
KY_INLINE void BaseRayCastOnSegmentQuery::SetResult(RayCastOnSegmentQueryResult result)                            { m_result                 = result;                 }
KY_INLINE void BaseRayCastOnSegmentQuery::SetDynamicOutputMode(DynamicOutputMode dynamicOutputMode)                { m_dynamicOutputMode      = dynamicOutputMode;      }
KY_INLINE void BaseRayCastOnSegmentQuery::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput)            { m_queryDynamicOutput     = queryDynamicOutput;     }
KY_INLINE void BaseRayCastOnSegmentQuery::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	m_positionSpatializationRange = positionSpatializationRange;
}

KY_INLINE RayCastOnSegmentQueryResult BaseRayCastOnSegmentQuery::GetResult ()                const { return m_result;                 }
KY_INLINE DynamicOutputMode           BaseRayCastOnSegmentQuery::GetDynamicOutputMode()      const { return m_dynamicOutputMode;      }
KY_INLINE QueryDynamicOutput*         BaseRayCastOnSegmentQuery::GetQueryDynamicOutput()     const { return m_queryDynamicOutput;     }
KY_INLINE const Vec3f&                BaseRayCastOnSegmentQuery::GetStartPos()               const { return m_startPos3f;             }
KY_INLINE const Vec3f&                BaseRayCastOnSegmentQuery::GetSegmentStartPos()        const { return m_segmentStartPos3f;      }
KY_INLINE const Vec3f&                BaseRayCastOnSegmentQuery::GetSegmentEndPos()          const { return m_segmentEndPos3f;        }
KY_INLINE const Vec3f&                BaseRayCastOnSegmentQuery::GetArrivalPos()             const { return m_arrivalPos3f;           }
KY_INLINE const NavTrianglePtr&       BaseRayCastOnSegmentQuery::GetStartTrianglePtr()       const { return m_startTrianglePtr;       }
KY_INLINE const NavTrianglePtr&       BaseRayCastOnSegmentQuery::GetArrivalTrianglePtr()     const { return m_arrivalTrianglePtr;     }
KY_INLINE KyFloat32                   BaseRayCastOnSegmentQuery::GetMaxDist()                const { return m_maxDist;                }
KY_INLINE const WorldIntegerPos&      BaseRayCastOnSegmentQuery::GetStartIntegerPos()        const { return m_startIntegerPos;        }
KY_INLINE const WorldIntegerPos&      BaseRayCastOnSegmentQuery::GetArrivalIntegerPos()      const { return m_arrivalIntegerPos;      }
KY_INLINE const WorldIntegerPos&      BaseRayCastOnSegmentQuery::GetSegmentStartIntegerPos() const { return m_segmentStartIntegerPos; }
KY_INLINE const WorldIntegerPos&      BaseRayCastOnSegmentQuery::GetSegmentEndIntegerPos()   const { return m_segmentEndIntegerPos;   }

KY_INLINE const PositionSpatializationRange& BaseRayCastOnSegmentQuery::GetPositionSpatializationRange() const { return m_positionSpatializationRange; }

}
