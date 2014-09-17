/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: JUBA

#ifndef Navigation_PositionOnPath_H
#define Navigation_PositionOnPath_H

#include "gwnavruntime/path/path.h"
#include "gwnavruntime/kernel/SF_RefCount.h"

namespace Kaim
{

///< Enumerates the different status an objectPositionOnPath can have on its Path.
enum PositionOnPathStatus
{
	PositionOnPathStatus_Undefined = 0, ///< The PositionOnPath is not yet defined.
	PositionOnPathStatus_OnPathNode,    ///< The PositionOnPath is on a Path node.
	PositionOnPathStatus_OnPathEdge     ///< The PositionOnPath is on a Path edge.
};


/// This class aggregates all necessary information about a position on a Path,
/// namely:
/// - a Vec3f position,
/// - a #PositionOnPathStatus, and
/// - a Path edge or node index.
class PositionOnPath
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Path)

public:
	// ---------------------------------- Main API Functions ----------------------------------

	PositionOnPath();

	bool operator==(const PositionOnPath& positionOnPath) const;
	bool operator!=(const PositionOnPath& positionOnPath) const;

	void InitOnPathNode     (Path* path, KyUInt32 pathNodeIdx);
	void InitOnPathFirstNode(Path* path);
	void InitOnPathLastNode (Path* path);
	void InitOnPathEdge     (Path* path, const Vec3f& positionOnEdge, KyUInt32 pathEdgeIdx);
	void Clear();

	bool                 IsAtFirstNodeOfPath()     const;
	bool                 IsAtLastNodeOfPath()      const;
	Path*                GetPath()                 const;
	const Vec3f&         GetPosition()             const;
	PositionOnPathStatus GetPositionOnPathStatus() const;
	PathEdgeType         GetPathEdgeType()         const;

	/// Returns true when this PositionOnPath precede or is at same position
	/// that the other one.
	bool DoesPrecede(const PositionOnPath& other) const;

	/// Returns true when this PositionOnPath strictly precede the other one;
	/// returns false if it as as the same position or after.
	bool IsStrictlyBefore(const PositionOnPath& other) const;


	// ---------------------------------- Path Edge Functions ----------------------------------

	/// This function has to be used carefully:
	/// - if this instance is at the first Path node, it returns the first edge index (0),
	/// - if it is on another Path node, it returns the index of the edge ending at this Path node,
	/// - else it returns the index of the edge current instance lies on.
	/// \pre GetPositionOnPathStatus() != PositionOnPathStatus_Undefined
	KyUInt32 GetPathEdgeIndex() const;

	/// If this in on a Path node, it returns the index of the edge starting at this Path node,
	/// else it returns the index of the edge current instance lies on.
	/// \pre GetPositionOnPathStatus() != PositionOnPathStatus_Undefined
	/// \pre Current instance must not be at the last node of the Path.
	KyUInt32 GetEdgeIdxToMoveOnForward()  const;

	/// If this in on a Path node, it returns the index of the edge ending at this Path node,
	/// else it returns the index of the edge current instance lies on.
	/// \pre GetPositionOnPathStatus() != PositionOnPathStatus_Undefined
	/// \pre Current instance must not be at the first node of the Path.
	KyUInt32 GetEdgeIdxToMoveOnBackward() const;


	// ---------------------------------- Move Functions ----------------------------------

	/// Main distance-based forward move method: moves the position along distanceMove
	/// meters measured on the broken-line.
	/// \pre distanceMove >= 0.0f
	void MoveForward(KyFloat32 distanceMove);

	/// Main distance-based backward move method: moves the position along distanceMove
	/// meters measured on the broken-line.
	/// \pre distanceMove >= 0.0f
	void MoveBackward(KyFloat32 distanceMove);

	/// Atomic move method: moves forward the position on the current Path edge it lays on,
	/// up to distanceMove meters.
	/// If the next Path node is at a distance lower than distanceMove this instance position
	/// is set at this Path node, and the distance from initial position to this Path node is returned;
	/// else the position is moved for distanceMove meters on the edge it lays on, and distanceMove is returned.
	/// \pre distanceMove >= 0.0f
	KyFloat32 MoveForward_StopAtPathNode(KyFloat32 distanceMove);

	/// Atomic move method: moves backward the position on the current Path edge it lays on,
	/// up to distanceMove meters.
	/// If the previous Path node is at a distance lower than distanceMove this instance position
	/// is set at this Path node, and the distance from initial position to this Path node is returned;
	/// else the position is moved for distanceMove meters on the edge it lays on, and distanceMove is returned.
	/// \pre distanceMove >= 0.0f
	KyFloat32 MoveBackward_StopAtPathNode(KyFloat32 distanceMove);

	/// Atomic forward move method: must be used with lot of care.
	/// Consider using MoveForward method instead, unless you really need to finely control
	/// the PositionOnPath move.
	void MoveForwardToNextPathNode();

	/// Atomic backward move method: must be used with lot of care.
	/// Consider using MoveBackward method instead, unless you really need to finely control
	/// the PositionOnPath move.
	void MoveBackwardToPrevPathNode();


private:
	friend class PathValidityInterval;
	friend class PathEventList;

	Ptr<Path> m_path;
	Vec3f m_position;

	PositionOnPathStatus m_onPathStatus;
	KyUInt32 m_currentIndexOnPath; // index on the Path edge or index of the Path node. position is on ]EdgeStart, EdgeEnd]
};

KY_INLINE PositionOnPath::PositionOnPath() : m_onPathStatus(PositionOnPathStatus_Undefined), m_currentIndexOnPath(KyUInt32MAXVAL) {}

KY_INLINE bool PositionOnPath::IsAtFirstNodeOfPath() const { return m_onPathStatus == PositionOnPathStatus_OnPathNode && m_currentIndexOnPath == 0; }
KY_INLINE bool PositionOnPath::IsAtLastNodeOfPath()  const { return m_onPathStatus == PositionOnPathStatus_OnPathNode && m_currentIndexOnPath + 1 == m_path->GetNodeCount(); };

KY_INLINE KyUInt32 PositionOnPath::GetPathEdgeIndex() const
{
	KY_LOG_ERROR_IF(m_onPathStatus == PositionOnPathStatus_Undefined, ("Error, The OnPathStatus is undefined"));
	if (m_onPathStatus == PositionOnPathStatus_OnPathNode)
		//return m_currentIndexOnPath == 0 ? 0 :m_currentIndexOnPath - 1;
		return Max<KyUInt32>(1, m_currentIndexOnPath) - 1;

	return m_currentIndexOnPath;
}


KY_INLINE KyUInt32 PositionOnPath::GetEdgeIdxToMoveOnForward()  const
{
	KY_LOG_ERROR_IF(IsAtLastNodeOfPath(), ("Error, you cannot move beyond the end of the path"));
	KY_LOG_ERROR_IF(m_onPathStatus == PositionOnPathStatus_Undefined, ("Error, The OnPathStatus is undefined"));
	return m_currentIndexOnPath;
}
KY_INLINE KyUInt32 PositionOnPath::GetEdgeIdxToMoveOnBackward() const
{
	KY_LOG_ERROR_IF(IsAtFirstNodeOfPath(), ("Error, you cannot move before the start of the path"));
	KY_LOG_ERROR_IF(m_onPathStatus == PositionOnPathStatus_Undefined, ("Error, The OnPathStatus is undefined"));
	return m_onPathStatus == PositionOnPathStatus_OnPathNode ? m_currentIndexOnPath - 1 : m_currentIndexOnPath;
}

KY_INLINE Path*                PositionOnPath::GetPath()                 const { return m_path;                                      }
KY_INLINE const Vec3f&         PositionOnPath::GetPosition()             const { return m_position;                                  }
KY_INLINE PositionOnPathStatus PositionOnPath::GetPositionOnPathStatus() const { return m_onPathStatus;                              }
KY_INLINE PathEdgeType   PositionOnPath::GetPathEdgeType()               const { return m_path->GetPathEdgeType(GetPathEdgeIndex()); }

KY_INLINE bool PositionOnPath::operator!=(const PositionOnPath& other) const { return !(*this == other); }
KY_INLINE bool PositionOnPath::operator==(const PositionOnPath& other) const
{
	return
		m_path               == other.m_path               &&
		m_onPathStatus       == other.m_onPathStatus       &&
		m_currentIndexOnPath == other.m_currentIndexOnPath &&
		m_position           == other.m_position;
}


KY_INLINE void PositionOnPath::InitOnPathFirstNode(Path* path) { InitOnPathNode(path, 0); }
KY_INLINE void PositionOnPath::InitOnPathLastNode(Path* path)
{
	KY_ASSERT(path != KY_NULL);
	InitOnPathNode(path, path->GetNodeCount() - 1);
}

} // namespace Kaim

#endif // Navigation_PositionOnPath_H
