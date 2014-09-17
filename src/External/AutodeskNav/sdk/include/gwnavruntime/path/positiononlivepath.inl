/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: GUAL - secondary contact: NOBODY

namespace Kaim
{
KY_INLINE void PositionOnLivePath::ClearEventListlocation()
{
	m_currentOnEventListIdx = KyUInt32MAXVAL;
	m_onEventListStatus = OnEventListStatus_Undefined;
}
KY_INLINE bool                  PositionOnLivePath::IsAtFirstNodeOfPath()        const { return m_positionOnPath.IsAtFirstNodeOfPath();        }
KY_INLINE bool                  PositionOnLivePath::IsAtLastNodeOfPath()         const { return m_positionOnPath.IsAtLastNodeOfPath();         }
KY_INLINE KyUInt32              PositionOnLivePath::GetPathEdgeIndex()           const { return m_positionOnPath.GetPathEdgeIndex();           }
KY_INLINE KyUInt32              PositionOnLivePath::GetEdgeIdxToMoveOnForward()  const { return m_positionOnPath.GetEdgeIdxToMoveOnForward();  }
KY_INLINE KyUInt32              PositionOnLivePath::GetEdgeIdxToMoveOnBackward() const { return m_positionOnPath.GetEdgeIdxToMoveOnBackward(); }
KY_INLINE const Path*           PositionOnLivePath::GetPath()                    const { return m_pathEventList->m_path;                       }
KY_INLINE const Vec3f&          PositionOnLivePath::GetPosition()                const { return m_positionOnPath.GetPosition();                }
KY_INLINE PositionOnPathStatus  PositionOnLivePath::GetPositionOnPathStatus()    const { return m_positionOnPath.GetPositionOnPathStatus();    }
KY_INLINE PathEdgeType          PositionOnLivePath::GetPathEdgeType()            const { return m_positionOnPath.GetPathEdgeType();            }
KY_INLINE const PositionOnPath& PositionOnLivePath::GetPositionOnPath()          const { return m_positionOnPath;                              }
KY_INLINE KyUInt32              PositionOnLivePath::GetOnEventListIndex()        const { return m_currentOnEventListIdx;                       }
KY_INLINE OnEventListStatus     PositionOnLivePath::GetOnEventListStatus()       const { return m_onEventListStatus;                           }

KY_INLINE bool PositionOnLivePath::IsAtLowerBoundOfLivePath() const
{
	return m_onEventListStatus == OnEventListStatus_OnEvent && GetOnEventListIndex() == 0;
}
KY_INLINE bool PositionOnLivePath::IsAtUpperBoundOfLivePath() const
{
	return m_onEventListStatus == OnEventListStatus_OnEvent && (GetOnEventListIndex() + 1) == m_pathEventList->GetPathEventCount();
}


KY_INLINE Path* PositionOnLivePath::GetPath() { return m_pathEventList->m_path; }

KY_INLINE bool PositionOnLivePath::DoesPrecede(const PositionOnLivePath& other)      const { return m_positionOnPath.DoesPrecede(other.m_positionOnPath);      }
KY_INLINE bool PositionOnLivePath::IsStrictlyBefore(const PositionOnLivePath& other) const { return m_positionOnPath.IsStrictlyBefore(other.m_positionOnPath); }

KY_INLINE bool PositionOnLivePath::operator!=(const PositionOnLivePath& other) const { return !(*this == other); }
KY_INLINE bool PositionOnLivePath::operator==(const PositionOnLivePath& other) const
{
	return
		m_bot                   == other.m_bot                   &&
		m_positionOnPath        == other.m_positionOnPath        &&
		m_currentOnEventListIdx == other.m_currentOnEventListIdx &&
		m_onEventListStatus     == other.m_onEventListStatus;
}
KY_INLINE void PositionOnLivePath::InitOnPathFirstNode()
{
	ClearEventListlocation();
	m_positionOnPath.InitOnPathFirstNode(GetPath());
}

KY_INLINE void PositionOnLivePath::InitOnPathLastNode()
{
	ClearEventListlocation();
	m_positionOnPath.InitOnPathLastNode(GetPath());
}

KY_INLINE void PositionOnLivePath::InitOnPathNode(KyUInt32 pathNodeIdx)
{
	ClearEventListlocation();
	m_positionOnPath.InitOnPathNode(GetPath(), pathNodeIdx);
}

KY_INLINE void PositionOnLivePath::InitOnPathEdge(const Vec3f& posOnEdge, KyUInt32 pathEdgeIdx)
{
	ClearEventListlocation();
	m_positionOnPath.InitOnPathEdge(GetPath(), posOnEdge, pathEdgeIdx);
}

KY_INLINE void PositionOnLivePath::MoveForward_StopAtEvent(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveForward(distanceMove);
	StopAtEventAfterMovingForward();
}

KY_INLINE void PositionOnLivePath::MoveBackward_StopAtEvent(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveBackward(distanceMove);
	StopAtEventAfterMovingBackwrad();
}

KY_INLINE void PositionOnLivePath::MoveForward_StopAtEventOrAtPathNode(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveForward_StopAtPathNode(distanceMove);
	StopAtEventAfterMovingForward();
}

KY_INLINE void PositionOnLivePath::MoveBackward_StopAtEventOrAtPathNode(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveBackward_StopAtPathNode(distanceMove);
	StopAtEventAfterMovingBackwrad();
}

KY_INLINE void PositionOnLivePath::MoveForward(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveForward(distanceMove);
	FixLocationOnEventListAfterMovingForward();
}

KY_INLINE void PositionOnLivePath::MoveBackward(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveBackward(distanceMove);
	FixLocationOnEventListAfterMovingBackward();
}

KY_INLINE void PositionOnLivePath::MoveForward_StopAtPathNode(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveForward_StopAtPathNode(distanceMove);
	FixLocationOnEventListAfterMovingForward();
}

KY_INLINE void PositionOnLivePath::MoveBackward_StopAtPathNode(KyFloat32 distanceMove)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath.MoveBackward_StopAtPathNode(distanceMove);
	FixLocationOnEventListAfterMovingBackward();
}

KY_INLINE void PositionOnLivePath::SetNewPositionOnPathForward(const PositionOnPath& newPositionOnPathForward)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath = newPositionOnPathForward;
	FixLocationOnEventListAfterMovingForward();
}

KY_INLINE void PositionOnLivePath::SetNewPositionOnPathBackward(const PositionOnPath& newPositionOnPathBackward)
{
	KY_LOG_ERROR_IF(GetOnEventListStatus() == OnEventListStatus_Undefined, ("You should call this function if current PositionOnLivePath is not located on the PathEventList"));
	m_positionOnPath = newPositionOnPathBackward;
	FixLocationOnEventListAfterMovingBackward();
}

} // namespace Kaim

