/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: JUBA - secondary contact: NOBODY

#ifndef Navigation_PathValidityInterval_H
#define Navigation_PathValidityInterval_H

#include "gwnavruntime/navmesh/identifiers/navtriangleptr.h"
#include "gwnavruntime/path/patheventlist.h"
#include "gwnavruntime/path/positiononpath.h"
#include "gwnavruntime/queries/raycastquery.h"
#include "gwnavruntime/queries/trianglefromposquery.h"
#include "gwnavruntime/queries/raycangoonsegmentquery.h"
#include "gwnavruntime/queries/raycastonsegmentquery.h"
#include "gwnavruntime/navgraph/navgraphlink.h"

namespace Kaim
{

class LivePath;

/// Enumerate the different validity status of a LivePath.
enum PathValidityStatus
{
	/// No Path has been set yet.
	PathValidityStatus_NoPath = 0,

	/// The Path has been completely checked and is valid.
	PathIsValid,

	/// NavData have changed around the Path, its validation is about to start.
	ValdityIntervalMustBeRecomputed,

	/// The Path is currently under validation. The check is not finished but no
	/// dirtiness has been found so far.
	ValidityIsBeingChecked,

	/// The validation can not be processed at all. The target on path is in an
	/// invalid area; it is either:
	/// - outside navMesh,
	/// - on navMesh but on a non-traversable NavTag,
	/// - was on a NavGraph that has been unloaded.
	Dirty_TargetInvalid,

	/// The Path validity around target on path is limited:
	/// - browsing the path backward from target on path failed
	/// - forward browsing has not yet reach the Path end.
	Dirty_InvalidBeforeTarget_ProcessingAfter,

	/// The Path validity around target on path is limited:
	/// - browsing the path backward from target on path failed
	/// - forward browsing completed successfully up to Path end.
	Dirty_InvalidBeforeTarget_ValidAfter,

	/// The Path validity around target on path is limited:
	/// - backward browsing completed successfully up to Path start
	/// - browsing the path forward failed.
	Dirty_ValidBeforeTarget_InvalidAfterTarget,

	/// The Path validity around target on path is limited: bot backward and
	/// forward validation browsing failed.
	Dirty_InvalidBeforeAndAfterTarget
};


/// Maintains runtime validity information on a given Path.
class PathValidityInterval
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	PathValidityInterval();
	~PathValidityInterval();
	void Clear();
	void ClearBounds();
	void ClearBoundsAndEventList();

	Path* GetPath() const;
	PathValidityStatus GetPathValidityStatus() const;

	void SetPathValidityStatus(PathValidityStatus status);
	void SetPath(Path* path);

	template<class NavTagTraversePredicate>
	KyResult ReStartIntervalComputation(const PositionOnPath& targetOnPath, KyFloat32 minDistanceFromTargetOnPathBackward, KyFloat32 minDistanceFromTargetOnPathFrowaard, NavTagTraversePredicate* predicate);

	template<class NavTagTraversePredicate>
	KyResult ValidateForward(KyFloat32 minimumDistanceAdvancement, NavTagTraversePredicate* predicate);


private:
	KyResult ValidateTargetOnPath(PositionOnPath& targetOnPath);

	template<class NavTagTraversePredicate>
	KyResult ValidateBackWard(KyFloat32 minimumdrawBackDistance, NavTagTraversePredicate* predicate);

	template<class NavTagTraversePredicate>
	KyResult TestCanGoToPrevPathNode(NavTagTraversePredicate* predicate);
	template<class NavTagTraversePredicate>
	KyResult TestCanGoToNextPathNode(NavTagTraversePredicate* predicate);

	template<class NavTagTraversePredicate>
	void FindLastValidPositionForward(NavTagTraversePredicate* predicate);
	template<class NavTagTraversePredicate>
	void FindLastValidPositionBackward(NavTagTraversePredicate* predicate);

	// No need for predicate since we already check navTags going backwards from the targetOnPath
	KyResult BuildEventListFromLowerBoundToUpperBound(); 

	void CreateIntervalForNavTagSubSegment(KyUInt32 pathEdgeIdxToMoveOn)
	{
		CreateIntervalsForNavTagSubsegmentButLast(pathEdgeIdxToMoveOn);
		const PathEventType lastEventType = GetEventTypeForLastNavTagSubSegment(pathEdgeIdxToMoveOn);
		CreateIntervalForLastNavTagSubSegment(pathEdgeIdxToMoveOn, lastEventType);
		m_needToCheckLastEventForRemoval = (lastEventType == PathEventType_OnNavMeshPathNode);
	}
	void CreateIntervalsForNavTagSubsegmentButLast(KyUInt32 pathEdgeIdxToMoveOn);
	void CreateIntervalForLastNavTagSubSegment(KyUInt32 pathEdgeIdxToMoveOn, PathEventType typeOfLastEvent);
	void CheckIfLastEventShouldBeRemoved(const NavTagSubSegment& incomingNavTagSubSegment);

	PathEventType GetEventTypeForLastNavTagSubSegment(KyUInt32 pathEdgeIdxToMoveOn);
	void CreateIntervalForGraphEdge(KyUInt32 pathEdgeIdxToMoveOn);
	void CreateIntervalForFromOutsidePathEdge(KyUInt32 pathEdgeIdxToMoveOn);
	void CreateIntervalForToOutsidePathEdge(KyUInt32 pathEdgeIdxToMoveOn);

	PathEventType GetEventTypeFromLowerBound(const PositionOnPath& posOnPath);
	void SetPathEventStatusForLastEvent(PathEventStatusInList status);


public: // internal
	PathEventList m_pathEventList;

	PositionOnPath m_upperBound;
	PositionOnPath m_lowerBound;

	NavTrianglePtr m_upperTrianglePtr;
	NavTrianglePtr m_lowerTrianglePtr;

	PathValidityStatus m_pathValidityStatus;

	bool m_needToCheckLastEventForRemoval;
	Ptr<QueryDynamicOutput> m_queryDynamicOutput;
};

}

#include "gwnavruntime/path/pathvalidityinterval.inl"

#endif
