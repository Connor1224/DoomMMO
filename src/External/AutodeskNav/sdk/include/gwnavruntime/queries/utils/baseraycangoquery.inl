/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY


namespace Kaim
{

KY_INLINE void BaseRayCanGoQuery::SetStartTrianglePtr(const NavTrianglePtr& startTrianglePtr)   { m_startTrianglePtr   = startTrianglePtr;   }
KY_INLINE void BaseRayCanGoQuery::SetStartIntegerPos(const WorldIntegerPos& startIntegerPos)    { m_startIntegerPos    = startIntegerPos;    }
KY_INLINE void BaseRayCanGoQuery::SetDestIntegerPos(const WorldIntegerPos& destIntegerPos)      { m_destIntegerPos     = destIntegerPos;     }
KY_INLINE void BaseRayCanGoQuery::SetDynamicOutputMode(DynamicOutputMode dynamicOutputMode)     { m_dynamicOutputMode  = dynamicOutputMode;  }
KY_INLINE void BaseRayCanGoQuery::SetQueryDynamicOutput(QueryDynamicOutput* queryDynamicOutput) { m_queryDynamicOutput = queryDynamicOutput; }
KY_INLINE void BaseRayCanGoQuery::SetResult(RayCanGoQueryResult result)                         { m_result             = result;             }
KY_INLINE void BaseRayCanGoQuery::SetPositionSpatializationRange(const PositionSpatializationRange& positionSpatializationRange)
{
	m_positionSpatializationRange = positionSpatializationRange;
}

KY_INLINE DynamicOutputMode      BaseRayCanGoQuery::GetDynamicOutputMode()  const { return m_dynamicOutputMode;  }
KY_INLINE RayCanGoQueryResult    BaseRayCanGoQuery::GetResult()             const { return m_result;             }
KY_INLINE const Vec3f&           BaseRayCanGoQuery::GetStartPos()           const { return m_startPos3f;         }
KY_INLINE const Vec3f&           BaseRayCanGoQuery::GetDestPos()            const { return m_destPos3f;          }
KY_INLINE const NavTrianglePtr&  BaseRayCanGoQuery::GetStartTrianglePtr()   const { return m_startTrianglePtr;   }
KY_INLINE const NavTrianglePtr&  BaseRayCanGoQuery::GetDestTrianglePtr()    const { return m_destTrianglePtr;    }
KY_INLINE QueryDynamicOutput*    BaseRayCanGoQuery::GetQueryDynamicOutput() const { return m_queryDynamicOutput; }
KY_INLINE const WorldIntegerPos& BaseRayCanGoQuery::GetStartIntegerPos()    const { return m_startIntegerPos;    }
KY_INLINE const WorldIntegerPos& BaseRayCanGoQuery::GetDestIntegerPos()     const { return m_destIntegerPos;     }

KY_INLINE const PositionSpatializationRange& BaseRayCanGoQuery::GetPositionSpatializationRange() const { return m_positionSpatializationRange; }

}
