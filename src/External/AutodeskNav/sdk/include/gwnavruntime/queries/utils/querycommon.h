/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_RayQueryUtils_H
#define Navigation_RayQueryUtils_H

#include "gwnavruntime/queries/querydynamicoutput.h"
#include "gwnavruntime/queries/trianglefromposandtriangleseedquery.h"
#include "gwnavruntime/navmesh/intersections.h"
#include "gwnavruntime/database/database.h"

namespace Kaim
{

class Box2f;
class NavTrianglePtr;
class NavTriangleRawPtr;
class NavHalfEdgePtr;
class NavHalfEdgeRawPtr;
class TriangleFromPosAndTriangleSeedQuery;
class WorldIntegerPos;
class LocalStoreActiveDataWorkspaceContext;
class RawNavTagSubSegment;
class WorkingMemory;
template <class T> class WorkingMemArray;


/// Enumerates possible ways of storing crossed data from a query in a QueryDynamicOutput object.
enum DynamicOutputMode
{
	QUERY_SAVE_NOTHING                 = 0,      ///< Indicates that QueryDynamicOutput will not be used to store anything.
	QUERY_SAVE_FLOORS                  = 1 << 1, ///< Indicates that QueryDynamicOutput will be used to store NavFloorPtr.
	QUERY_SAVE_TRIANGLES               = 1 << 2, ///< Indicates that QueryDynamicOutput will be used to store NavFloorTrianglePtr.
	QUERY_SAVE_NAVTAGSUBSEGMENTS       = 1 << 3, ///< Indicates that QueryDynamicOutput will be used to store NavTagSubSegment.
	QUERY_SAVE_HALFEDGEINTERSECTIONS   = 1 << 4, ///< Indicates that QueryDynamicOutput will be used to store Vec3f.

	QUERY_SAVE_TRIANGLES_AND_SUBSEGMENTS = QUERY_SAVE_TRIANGLES | QUERY_SAVE_NAVTAGSUBSEGMENTS,
};


/// This class is an helper used internaly to factorize code of Ray queries (RayCanGoQuery, RayCastGoQuery, ...)
class RayQueryUtils
{
	RayQueryUtils();
public:
	RayQueryUtils(const Vec3f& startPos3f, const Vec3f& destPos3f,
		DynamicOutputMode savingMode = QUERY_SAVE_NOTHING,
		WorkingMemArray<NavTriangleRawPtr>* crossedTriangles = KY_NULL,
		WorkingMemArray<RawNavTagSubSegment>* navTagSubSegments = KY_NULL,
		WorkingMemArray<RawHalfEdgeIntersection>* halfEdgeIntersections = KY_NULL) :
		m_startPos3f(startPos3f),
		m_destPos3f(destPos3f),
		m_savingMode(savingMode),
		m_crossedTriangles(crossedTriangles),
		m_navTagSubSegments(navTagSubSegments),
		m_rawHalfEdgeIntersections(halfEdgeIntersections) {}

	KyResult StorePropagationData(KyFloat32 integerPrecision, const NavHalfEdgeRawPtr& currentPairHalfEdgeRawPtr, RawNavTagSubSegment& navTagSubSegment,
	    const CoordPos64& cellOrigin, const NavFloorBlob* navFloorBlob);

	KyInt32 GetfirstEdgeToCrossInStartTriangle(const NavTriangleRawPtr& startTriangleRawPtr, const CoordPos64& startCoordPos, const CoordPos64& destCoordPos);

	bool IsEdgeBeyondDestPos(const CoordPos64& cellOrigin, const NavFloorBlob& navFloorBlob, NavHalfEdgeIdx navHalfEdgeIdx,
	    const CoordPos64& segmentStartCoordPos, const CoordPos64& segmentDestCoordPos, const CoordPos64& destinationCoordPos, const KyInt64& startToDestSquareDist);

	Vec3f m_startPos3f;
	Vec3f m_destPos3f;
	DynamicOutputMode m_savingMode;
	WorkingMemArray<NavTriangleRawPtr>* m_crossedTriangles;
	WorkingMemArray<RawNavTagSubSegment>* m_navTagSubSegments;
	WorkingMemArray<RawHalfEdgeIntersection>* m_rawHalfEdgeIntersections;
};

/// This class is an helper used internaly by the Queries to factorize Code that is used in many Queries.
class QueryUtils
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	QueryUtils() : m_database(KY_NULL), m_workingMemory(KY_NULL) {}
	QueryUtils(Database* database, WorkingMemory* workingMemory);
	void Init(Database* database, WorkingMemory* workingMemory);

	// ------------------------------ Accessors -----------------------------------
	Database*      GetDatabase()      { return m_database;      }
	WorkingMemory* GetWorkingMemory() { return m_workingMemory; }
public:

	// ------------------------------ Location functions -----------------------------------

	/// Checks the validity of the specified triangle. If the triangle is not valid, this method tries to retrieve a 
	/// valid triangle ID from the specified position. If a valid triangle is found, it updatesnavTrianglePtr.
	/// \param [in,out] navTrianglePtr		The triangle to test. If the triangle is invalid and a valid triangle is
	/// 									found, this parameter will be updated with the ID of the new triangle.
	/// \param pos							The position used to retrieve a new triangle, if needed.
	/// \param positionSpatializationRange	The maximum difference in altitude betweenpos and the returned triangle.
	/// \return true if the input triangle is valid or ifnavTrianglePtr was updated, otherwisefalse. 
	bool FindTriangleFromPositionIfNotValid(NavTrianglePtr& navTrianglePtr, const Vec3f& pos, const PositionSpatializationRange& positionSpatializationRange);

	/// Checks the validity of the specified triangle. If the triangle is not valid, this method tries to retrieve a 
	/// valid triangle ID from the specified position. If a valid triangle is found, it updatesnavTrianglePtr.
	/// \param [in,out] navTrianglePtr		The triangle to test. If the triangle is invalid and a valid triangle is
	/// 									found, this parameter will be updated with the ID of the new triangle.
	/// \param pos							The position used to retrieve a new triangle, if needed.
	/// \param positionSpatializationRange	The maximum difference in altitude betweenpos and the returned triangle.
	/// \param [out] outputIntegerPos		Stores the integer position of the returned triangle, or ofpos if the
	/// 									original triangle was valid.
	/// \return true if the input triangle is valid or ifnavTrianglePtr was updated, otherwisefalse. 
	bool FindTriangleFromPositionIfNotValid(NavTrianglePtr& navTrianglePtr, const Vec3f& pos, const PositionSpatializationRange& positionSpatializationRange, WorldIntegerPos& outputIntegerPos);

	/// Checks the validity of the specified triangle. If the triangle is not valid, this method tries to retrieve a 
	/// valid triangle ID from the specified position. If a valid triangle is found, it updatesnavTrianglePtr.
	/// \param [in,out] navTrianglePtr		The triangle to test. If the triangle is invalid and a valid triangle is
	/// 									found, this parameter will be updated with the ID of the new triangle.
	/// \param pos							The position used to retrieve a new triangle, if needed. Only the altitude
	/// 									of this position is used.
	/// \param [in] integerPos				Provides the (x,y) coordinates of the desired position.
	/// \param positionSpatializationRange	The maximum difference in altitude betweenpos and the returned triangle.
	/// \return true if the input triangle is valid or ifnavTrianglePtr was updated, otherwisefalse. 
	bool FindTriangleFromPositionIfNotValid(NavTrianglePtr& navTrianglePtr, const Vec3f& pos, const WorldIntegerPos& integerPos, const PositionSpatializationRange& positionSpatializationRange);

public : // internal
	template<class NavTagTraversePredicate>
	KyResult FindValidPositionFromIntegerIntersection(const Vec3f& collisionPos3f, const NavHalfEdgePtr& halfEdgePtrHit,
		WorldIntegerPos& arrivalIntegerPos, NavTrianglePtr& arrivalTrianglePtr, NavTagTraversePredicate* predicate);

private :
	class CheckIfCurrentIntegerPositionIsValidParam
	{
	public:
		CoordPos64 m_start;
		CoordPos64 m_end;
		CoordPos64 m_third;
		CoordPos64 m_startToEnd;
		CoordPos64 m_endToThird;
		CoordPos64 m_thirdTostart;
		WorldIntegerPos m_currentArrivalIntegerPos;
		
	};

	template<class NavTagTraversePredicate>
	KyResult  CheckIfCurrentIntegerPositionIsValid(const CheckIfCurrentIntegerPositionIsValidParam& params, TriangleFromPosAndTriangleSeedQuery& triangleFromPosAndTriangleSeedQuery,
		NavTagTraversePredicate* predicate, WorldIntegerPos& arrivalIntegerPos, NavTrianglePtr& arrivalTrianglePtr);

public:
	Database* m_database; ///< The Database taken into account by queries made through this object. Do not modify.
	WorkingMemory* m_workingMemory; ///< Contains memory buffers used for performing queries and storing results. For internal use. Do not modify.
};

template<class NavTagTraversePredicate>
inline KyResult QueryUtils::FindValidPositionFromIntegerIntersection(const Vec3f& collisionPos3f,
	const NavHalfEdgePtr& halfEdgePtrHit, WorldIntegerPos& arrivalIntegerPos, NavTrianglePtr& arrivalTrianglePtr, NavTagTraversePredicate* predicate)
{
	const DatabaseGenMetrics& genMetrics = m_database->GetDatabaseGenMetrics();

	const NavTrianglePtr lastTrianglePtr(halfEdgePtrHit.m_navFloorPtr, NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(halfEdgePtrHit.GetHalfEdgeIdx()));
	const KyInt32 stepMax = 2;
	KyInt32 currentSign = 1;

	TriangleFromPosAndTriangleSeedQuery triangleFromPosAndTriangleSeedQuery;
	triangleFromPosAndTriangleSeedQuery.Initialize(m_database, lastTrianglePtr, collisionPos3f);


	const NavHalfEdgeIdx currentEdgeIdx = halfEdgePtrHit.GetHalfEdgeIdx();
	const NavHalfEdgeIdx nextEdgeIdx = NavFloorBlob::NavHalfEdgeIdxToNextNavHalfEdgeIdx(halfEdgePtrHit.GetHalfEdgeIdx());
	const NavHalfEdgeIdx prevEdgeIdx = NavFloorBlob::NavHalfEdgeIdxToPrevNavHalfEdgeIdx(halfEdgePtrHit.GetHalfEdgeIdx());

	const NavFloorBlob& lastFloorBlob = *halfEdgePtrHit.GetNavFloorBlob_Unsafe();
	const NavHalfEdge* const navHalfEdges = lastFloorBlob.m_navHalfEdges.GetValues();

	const NavVertexIdx startIdx = navHalfEdges[currentEdgeIdx].GetStartVertexIdx();
	const NavVertexIdx endIdx   = navHalfEdges[nextEdgeIdx].GetStartVertexIdx();
	const NavVertexIdx thirdIdx = navHalfEdges[prevEdgeIdx].GetStartVertexIdx();

	const NavVertex* const navVertices = lastFloorBlob.m_navVertices.GetValues();

	const CoordPos64 lastCellOrigin = genMetrics.ComputeCellOrigin(halfEdgePtrHit.GetNavFloor_Unsafe()->GetCellPos());

	CheckIfCurrentIntegerPositionIsValidParam params;
	params.m_currentArrivalIntegerPos = genMetrics.GetWorldIntegerPosFromVec3f(collisionPos3f);

	params.m_start = lastCellOrigin + navVertices[startIdx].GetCoordPos64(); // 31 bits
	params.m_end   = lastCellOrigin + navVertices[endIdx].GetCoordPos64();   // 31 bits
	params.m_third = lastCellOrigin + navVertices[thirdIdx].GetCoordPos64(); // 31 bits

	params.m_startToEnd   = params.m_end   - params.m_start; // 31 bits
	params.m_endToThird   = params.m_third - params.m_end;   // 31 bits
	params.m_thirdTostart = params.m_start - params.m_third; // 31 bits

	KY_LOG_WARNING_IF((  params.m_startToEnd.x >= 1LL << 31 || -  params.m_startToEnd.x >= 1LL << 31), ("Input value requires too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF((  params.m_endToThird.x >= 1LL << 31 || -  params.m_endToThird.x >= 1LL << 31), ("Input value requires too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF((params.m_thirdTostart.x >= 1LL << 31 || -params.m_thirdTostart.x >= 1LL << 31), ("Input value requires too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF((  params.m_startToEnd.y >= 1LL << 31 || -  params.m_startToEnd.y >= 1LL << 31), ("Input value requires too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF((  params.m_endToThird.y >= 1LL << 31 || -  params.m_endToThird.y >= 1LL << 31), ("Input value requires too many bits of precision. Potential computation overflow, result may be incoherent"));
	KY_LOG_WARNING_IF((params.m_thirdTostart.y >= 1LL << 31 || -params.m_thirdTostart.y >= 1LL << 31), ("Input value requires too many bits of precision. Potential computation overflow, result may be incoherent"));

	if (KY_SUCCEEDED(CheckIfCurrentIntegerPositionIsValid<NavTagTraversePredicate>(params, triangleFromPosAndTriangleSeedQuery, predicate, arrivalIntegerPos, arrivalTrianglePtr)))
		return KY_SUCCESS;

	const KyInt32 cellSizeInCoord = m_database->GetDatabaseGenMetrics().m_cellSizeInCoord;
	// spiral loop
	for (KyInt32 i = 1; i < stepMax + 1; ++i)
	{
		for (KyInt32 j = 1; j <= i; ++j)
		{
			currentSign > 0 ? params.m_currentArrivalIntegerPos.Add1OnCoordPosX(cellSizeInCoord) : params.m_currentArrivalIntegerPos.Remove1OnCoordPosX(cellSizeInCoord);

			if (KY_SUCCEEDED(CheckIfCurrentIntegerPositionIsValid<NavTagTraversePredicate>(params, triangleFromPosAndTriangleSeedQuery, predicate, arrivalIntegerPos, arrivalTrianglePtr)))
				return KY_SUCCESS;
		}

		for (KyInt32 j = 1; j <= i; ++j)
		{
			currentSign > 0 ? params.m_currentArrivalIntegerPos.Add1OnCoordPosY(cellSizeInCoord) : params.m_currentArrivalIntegerPos.Remove1OnCoordPosY(cellSizeInCoord);

			if (KY_SUCCEEDED(CheckIfCurrentIntegerPositionIsValid<NavTagTraversePredicate>(params, triangleFromPosAndTriangleSeedQuery, predicate, arrivalIntegerPos, arrivalTrianglePtr)))
				return KY_SUCCESS;
		}

		currentSign = -currentSign;
	}

	for (KyInt32 j = 1; j <= stepMax; ++j)
	{
		params.m_currentArrivalIntegerPos.Add1OnCoordPosX(cellSizeInCoord);

		if (KY_SUCCEEDED(CheckIfCurrentIntegerPositionIsValid<NavTagTraversePredicate>(params, triangleFromPosAndTriangleSeedQuery, predicate, arrivalIntegerPos, arrivalTrianglePtr)))
			return KY_SUCCESS;
	}

	return KY_ERROR;
}


template<class NavTagTraversePredicate>
inline KyResult  QueryUtils::CheckIfCurrentIntegerPositionIsValid(const QueryUtils::CheckIfCurrentIntegerPositionIsValidParam& params, TriangleFromPosAndTriangleSeedQuery& triangleFromPosAndTriangleSeedQuery,
	NavTagTraversePredicate* predicate, WorldIntegerPos& arrivalIntegerPos, NavTrianglePtr& arrivalTrianglePtr)
{
	const CoordPos64 collisionPos64 = m_database->GetDatabaseGenMetrics().GetCoordPos64FromWorldIntegerPos(params.m_currentArrivalIntegerPos);

	if (Intersections::IsStrictlyOnTheLeftOfTheEdge2d(collisionPos64, params.m_start, params.m_startToEnd) == false)
		return KY_ERROR;

	triangleFromPosAndTriangleSeedQuery.Initialize(m_database, triangleFromPosAndTriangleSeedQuery.GetSeedTrianglePtr(), triangleFromPosAndTriangleSeedQuery.GetInputPos());
	triangleFromPosAndTriangleSeedQuery.SetInputIntegerPos(params.m_currentArrivalIntegerPos);

	if (Intersections::IsOnTheLeftOfTheEdge2d(collisionPos64, params.m_end, params.m_endToThird) && 
		Intersections::IsOnTheLeftOfTheEdge2d(collisionPos64, params.m_third, params.m_thirdTostart))
	{
		arrivalTrianglePtr = triangleFromPosAndTriangleSeedQuery.GetSeedTrianglePtr();
		arrivalIntegerPos = triangleFromPosAndTriangleSeedQuery.GetInputIntegerPos();
		return KY_SUCCESS;
	}

	triangleFromPosAndTriangleSeedQuery.PerformQueryWithInputCoordPos(m_workingMemory);

	if (triangleFromPosAndTriangleSeedQuery.GetResult() == NEARESTTRIANGLEFROMSEED_DONE_TRIANGLE_FOUND &&
		predicate->CanTraverse(triangleFromPosAndTriangleSeedQuery.GetResultTrianglePtr().GetNavTag()))
	{
		arrivalTrianglePtr = triangleFromPosAndTriangleSeedQuery.GetResultTrianglePtr();
		arrivalIntegerPos = triangleFromPosAndTriangleSeedQuery.GetInputIntegerPos();
		return KY_SUCCESS;
	}

	return KY_ERROR;
}

}

#endif //Navigation_RayQueryUtils_H

