/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavHalfEdgeRawPtr_H
#define Navigation_NavHalfEdgeRawPtr_H

#include "gwnavruntime/navmesh/identifiers/navfloorptr.h"

namespace Kaim
{

///////////////////////////////////////////////////////////////////////////////////////////
// NavHalfEdgeRawPtr
///////////////////////////////////////////////////////////////////////////////////////////

/// Each instance of this class uniquely identifies a singleNavHalfEdge in a NavFloor.
/// This pointer is guaranteed to be valid only in the frame in which it was retrieved.
/// Never store a NavHalfEdgeRawPtr for use in subsequent frames, because it has no protection against data
/// streaming. Use NavHalfEdgePtr instead.
/// Actually, this class is used internally for some performance and working memory usage friendliness reason
/// (no ref-counting increment/decrement, no need to call constructor/destructor), but unless you really know what you do
/// prefer NavHalfEdgePtr which is safer.
class NavHalfEdgeRawPtr
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)

public:
	/// Creates an invalid NavHalfEdgeRawPtr. 
	NavHalfEdgeRawPtr();

	/// Creates a new NavHalfEdgePtr referring to the provided NavHalfEdgeIdx in the provided NavFloorRawPtr.
	/// \param navFloorRawPtr    The NavFloor that contains the edge this object should refer to. 
	/// \param halfEdgeIdx       The index of this edge within the NavFloor. 
	NavHalfEdgeRawPtr(const NavFloorRawPtr& navFloorRawPtr, NavHalfEdgeIdx halfEdgeIdx);

	/// Creates a new NavHalfEdgePtr referring to the provided NavHalfEdgeIdx in the provided NavFloor.
	/// \param navFloor       The NavFloor that contains the edge this object should refer to. 
	/// \param halfEdgeIdx    The index of this edge within the NavFloor. 
	NavHalfEdgeRawPtr(NavFloor* navFloor, NavHalfEdgeIdx halfEdgeIdx);


	/// Clears all information maintained by this object.
	/// \param navFloorRawPtr    The NavFloor that contains the edge this object should refer to. 
	/// \param halfEdgeIdx       The index of this edge within the NavFloor. 
	void Set(const NavFloorRawPtr& navFloorRawPtr, NavHalfEdgeIdx halfEdgeIdx);

	/// Clears all information maintained by this object.
	/// \param navFloor       The NavFloor that contains the edge this object should refer to. 
	/// \param halfEdgeIdx    The index of this edge within the NavFloor. 
	void Set(NavFloor* navFloor, NavHalfEdgeIdx halfEdgeIdx);

	/// Returns true if this object refers to a valid edge: i.e. an edge in a validNavFloor. see NavFloorPtr::IsValid()
	bool IsValid() const;

	void Invalidate(); ///< Invalidates this object.

	bool operator==(const NavHalfEdgeRawPtr& rhs) const; ///< Returns true if this object identifies the same edge as rhs, or if both are invalid. 
	bool operator!=(const NavHalfEdgeRawPtr& rhs) const; ///< Returns true if this object identifies a different edge from rhs. 
	
	NavHalfEdgeIdx GetHalfEdgeIdx() const;  ///< Retrieves the index of this NavHalfEdge within its NavFloor.

	// ---------------------------------- Member Functions for valid instance ----------------------------------
	/// All these function should be called only if IsValid() returns true
	/// \pre    This object must be valid. Its validity is not checked. 

	/// Returns a reference to the NavFloor that contains this hedge.
	/// \pre    This object must be valid. Its validity is not checked. 
	NavFloor* GetNavFloor() const;

	/// Returns a reference to the NavFloorBlob that contains this edge.
	/// \pre    This object must be valid. Its validity is not checked. 
	const NavFloorBlob* GetNavFloorBlob() const;

	/// Returns a const reference to the CellPos that indicates the position of the cell that contains this edge
	/// within the grid of NavData cells.
	/// \pre    This object must be valid. Its validity is not checked. 
	const CellPos& GetCellPos() const;

	/// Retrieves the NavTag associated to the NavFloor that contains this halfEdge.
	/// \pre    This object must be valid. Its validity is not checked. 
	const NavTag& GetNavTag() const;

	/// Updates the parameters to identify the vertices in the specified triangle.
	/// \param[out] v0Pos3f    The 3D position of the start vertex of the edge.
	/// \param[out] v1Pos3f    The 3D position of the end vertex in the edge.
	/// \param[out] v2Pos3f    The 3D position of the third vertex in the triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetTriangleVerticesPos3f(Vec3f& v0Pos3f, Vec3f& v1Pos3f, Vec3f& v2Pos3f) const;

	/// Updates the parameters to identify the start and end vertices of the specified edge.
	/// \param[out] v0Pos3f    The 3D position of the start vertex of the edge.
	/// \param[out] v1Pos3f    The 3D position of the end vertex in the edge.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetVerticesPos3f(Vec3f& v0Pos3f, Vec3f& v1Pos3f) const;

	/// Returns the coordinates of the starting vertex of this NavHalfEdge
	/// \pre    This object must be valid. Its validity is not checked.
	Vec3f GetStartVertexPos3f() const;

	/// Returns the coordinates of the ending vertex of this NavHalfEdge
	/// \pre    This object must be valid. Its validity is not checked.
	Vec3f GetEndVertexPos3f() const;

	/// Returns the x,y integer coordinates of the starting vertex of this NavHalfEdge
	/// \pre    This object must be valid. Its validity is not checked.
	CoordPos64 GetStartVertexCoordPos64() const;

	/// Returns the x,y integer coordinates of the ending vertex of this NavHalfEdge
	/// \pre    This object must be valid. Its validity is not checked.
	CoordPos64 GetEndVertexCoordPos64() const;

	/// Returns the altitude (z coordinate) of the starting vertex of this NavHalfEdge
	/// \pre    This object must be valid. Its validity is not checked 
	KyFloat32 GetStartVertexAltitude() const;

	/// Returns the altitude (z coordinate) of the ending vertex of this NavHalfEdge
	/// \pre    This object must be valid. Its validity is not checked 
	KyFloat32 GetEndVertexAltitude() const;

	/// Returns the 2D integer coordinates of the starting vertex of this edge in its NavCell.
	/// \pre    This object must be valid. Its validity is not checked. 
	CoordPos GetStartVertexCoordPosInCell() const;

	/// Returns the 2D integer coordinates of the ending vertex of this edge in its NavCell.
	/// \pre    This object must be valid. Its validity is not checked. 
	CoordPos GetEndVertexCoordPosInCell() const;

	/// Returns true if this edge can be traversed according the CanTraverse function in the 
	/// NavTagTraversePredicate.
	/// \param predicate    A pointer to a predicate that has this function implemented : bool CanTraverse(const NavTag& navTag). This typically
	///                     the same predicate that you passed to the a query or to a PathFollower
	/// \pre    This object must be valid. Its validity is not checked.
	template<class NavTagTraversePredicate>
	bool IsHalfEdgeCrossable(NavTagTraversePredicate* predicate) const;

	/// Returns true if this edge is not on an external boundary of the NavMesh and not a internal
	/// edge that is not correctly stitched.
	/// \pre    This object must be valid. Its validity is not checked.
	bool IsHalfEdgeCrossable() const;

	/// Returns true if this edge can be traversed according the CanTraverse function in the
	/// NavTagTraversePredicate, and updates resultRawPtr.
	/// \param[out] resultRawPtr    The edge in the next adjacent triangle that borders this edge.
	/// \param predicate            A pointer to a predicate that has this function implemented : bool CanTraverse(const NavTag& navTag). This typically
	///                             the same predicate that you passed to the a query or to a PathFollower
	/// \pre    This object must be valid. Its validity is not checked.
	template<class NavTagTraversePredicate>
	bool IsHalfEdgeCrossable(NavHalfEdgeRawPtr& resultRawPtr, NavTagTraversePredicate* predicate) const;

	/// Returns true if this edge is not on an external boundary of the NavMesh and not a internal
	/// edge that is not correctly stitched, and updates resultRawPtr.
	/// \param[out] resultRawPtr    The edge in the next adjacent triangle that borders this edge.
	/// \pre    This object must be valid. Its validity is not checked.
	bool IsHalfEdgeCrossable(NavHalfEdgeRawPtr& resultRawPtr) const;

	/// Updates resultRawPtr to identify the edge in the next adjacent triangle that borders this edge.
	/// \param[out] resultRawPtr    Updated to store the next adjacent triangle.
	/// \pre    This object must be valid. Its validity is not checked.
	/// \pre    The type of the edge returned by a call to NavHalfEdge::GetHalfEdgeType() must not be #EDGETYPE_OBSTACLE.
	void GetPairHalfEdgeRawPtr(NavHalfEdgeRawPtr& resultRawPtr) const;

	/// Updates resultRawPtr to identify the next edge in the triangle.
	/// \pre    This object must be valid. Its validity is not checked.
	void GetNextHalfEdgeRawPtr(NavHalfEdgeRawPtr& resultRawPtr) const;

	/// Updates resultRawPtr to identify the previous edge in the triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetPrevHalfEdgeRawPtr(NavHalfEdgeRawPtr& resultRawPtr) const;

	/// Returns the next edge along the border of the NavMesh according to NavTagTraversePredicate.
	/// \param predicate    A pointer to a predicate that has this function implemented : bool CanTraverse(const NavTag& navTag). This typically
	///                      the same predicate that you passed to the a query or to a PathFollower.
	/// \pre    This object must be valid. Its validity is not checked.
	/// \pre    This object must be on the border of an area of the NavMesh according to NavTagTraversePredicate. It may not be paired with
	///         another triangle with a traversable NavTag. That means we must have IsHalfEdgeCrossable<NavTagTraversePredicate>() == false.
	/// \post    The returned edge is not paired with another triangle with a traversable NavTag (IsHalfEdgeCrossable<NavTagTraversePredicate>() == false).
	template<class NavTagTraversePredicate>
	NavHalfEdgeRawPtr GetNextNavHalfEdgeRawPtrAlongBorder(NavTagTraversePredicate* predicate) const;

	/// Returns the previous edge along the border of the given NavTag.
	/// \param predicate    A pointer to a predicate that has this function implemented : bool CanTraverse(const NavTag& navTag). This typically
	///                      the same predicate that you passed to the a query or to a PathFollower.
	/// \pre    This object must be valid. Its validity is not checked.
	/// \pre    This object must be on the border of an area of the NavMesh according to NavTagTraversePredicate. It may not be paired with
	/// 		another triangle with a traversable NavTagv. That means we must have IsHalfEdgeCrossable<NavTagTraversePredicate>() == false.
	/// \post	The returned edge is not paired with another triangle with a traversable NavTag (IsHalfEdgeCrossable<NavTagTraversePredicate>() == false).
	template<class NavTagTraversePredicate>
	NavHalfEdgeRawPtr GetPrevNavHalfEdgeRawPtrAlongBorder(NavTagTraversePredicate* predicate) const;

public: // internal
	NavHalfEdgeRawPtr& GetCorrespondingLink();
	template<class NavTagTraversePredicate>
	bool IsHalfEdgeCrossable(const NavFloorBlob* navFloorBlob, NavHalfEdgeRawPtr& resultRawPtr, NavTagTraversePredicate* predicate) const;
	template<class NavTagTraversePredicate>
	bool IsHalfEdgeCrossable(NavHalfEdge navHalfEdge, const NavFloorBlob* navFloorBlob, NavHalfEdgeRawPtr& resultRawPtr, NavTagTraversePredicate* predicate) const;
	void GetVerticesPos3f(KyFloat32 integerPrecision, const CoordPos64& cellOrigin, const NavFloorBlob* navFloorBlob, Vec3f& v0Pos3f, Vec3f& v1Pos3f) const;
public:
	NavFloorRawPtr m_navFloorRawPtr;     ///< The NavFloor that contains this edge.
	CompactNavHalfEdgeIdx m_halfEdgeIdx; ///< The index of this edge within its NavFloor.

	KyUInt16 m_boundaryEdgeIdx; ///< Une internally to speed up stitching. Do not modify.
};

KY_INLINE NavHalfEdgeRawPtr::NavHalfEdgeRawPtr() : m_navFloorRawPtr(), m_halfEdgeIdx(CompactNavHalfEdgeIdx_MAXVAL), m_boundaryEdgeIdx(KyUInt16MAXVAL) {}

KY_INLINE NavHalfEdgeRawPtr::NavHalfEdgeRawPtr(const NavFloorRawPtr& navFloorRawPtr, NavHalfEdgeIdx halfEdgeIdx) :
	m_navFloorRawPtr(navFloorRawPtr), m_halfEdgeIdx((CompactNavHalfEdgeIdx)halfEdgeIdx), m_boundaryEdgeIdx(KyUInt16MAXVAL) {}

KY_INLINE void NavHalfEdgeRawPtr::Set(const NavFloorRawPtr& navFloorRawPtr, NavHalfEdgeIdx halfEdgeIdx)
{
	m_navFloorRawPtr = navFloorRawPtr;
	m_halfEdgeIdx = (CompactNavHalfEdgeIdx)halfEdgeIdx;
	m_boundaryEdgeIdx = KyUInt16MAXVAL;
}

KY_INLINE NavHalfEdgeRawPtr::NavHalfEdgeRawPtr(NavFloor* navFloor, NavHalfEdgeIdx halfEdgeIdx) : 
	m_navFloorRawPtr(navFloor), m_halfEdgeIdx((CompactNavHalfEdgeIdx)halfEdgeIdx), m_boundaryEdgeIdx(KyUInt16MAXVAL) {}

KY_INLINE void NavHalfEdgeRawPtr::Set(NavFloor* navFloor, NavHalfEdgeIdx halfEdgeIdx)
{
	m_navFloorRawPtr.Set(navFloor);
	m_halfEdgeIdx = (CompactNavHalfEdgeIdx)halfEdgeIdx;
	m_boundaryEdgeIdx = KyUInt16MAXVAL;
}

KY_INLINE bool NavHalfEdgeRawPtr::IsValid() const { return m_navFloorRawPtr.IsValid() && m_halfEdgeIdx != CompactNavHalfEdgeIdx_MAXVAL; }
KY_INLINE void NavHalfEdgeRawPtr::Invalidate() { m_navFloorRawPtr.Invalidate(); m_halfEdgeIdx = CompactNavHalfEdgeIdx_MAXVAL; m_boundaryEdgeIdx = KyUInt16MAXVAL; }

KY_INLINE bool NavHalfEdgeRawPtr::operator==(const NavHalfEdgeRawPtr& navHalfEdgeRawPtr) const { return m_navFloorRawPtr == navHalfEdgeRawPtr.m_navFloorRawPtr && m_halfEdgeIdx == navHalfEdgeRawPtr.m_halfEdgeIdx;}
KY_INLINE bool NavHalfEdgeRawPtr::operator!=(const NavHalfEdgeRawPtr& rhs) const { return !(*this == rhs); }

KY_INLINE NavHalfEdgeRawPtr& NavHalfEdgeRawPtr::GetCorrespondingLink()
{
	const NavFloorBlob* navFloorBlob = GetNavFloorBlob();

	KY_DEBUG_ASSERTN(IsHalfEdgeAFloorOrCellBoundary(navFloorBlob->GetHalfEdgeType(m_halfEdgeIdx)), ("Error in halfEdge type"));
	const KyUInt32 linkIdx = navFloorBlob->GetNavHalfEdge(m_halfEdgeIdx).GetBoundaryEdgeIdx();

	return m_navFloorRawPtr.GetNavFloor()->GetLinksBuffer()[linkIdx];
}

KY_INLINE CoordPos NavHalfEdgeRawPtr::GetStartVertexCoordPosInCell() const { return GetNavFloorBlob()->NavHalfEdgeIdxToStartNavVertex(m_halfEdgeIdx).GetCoordPos(); }
KY_INLINE CoordPos NavHalfEdgeRawPtr::GetEndVertexCoordPosInCell()   const { return GetNavFloorBlob()->NavHalfEdgeIdxToEndNavVertex(m_halfEdgeIdx).GetCoordPos(); }

KY_INLINE void NavHalfEdgeRawPtr::GetNextHalfEdgeRawPtr(NavHalfEdgeRawPtr& resultRawPtr) const { resultRawPtr.Set(m_navFloorRawPtr, NavFloorBlob::NavHalfEdgeIdxToNextNavHalfEdgeIdx(m_halfEdgeIdx)); }
KY_INLINE void NavHalfEdgeRawPtr::GetPrevHalfEdgeRawPtr(NavHalfEdgeRawPtr& resultRawPtr) const { resultRawPtr.Set(m_navFloorRawPtr, NavFloorBlob::NavHalfEdgeIdxToPrevNavHalfEdgeIdx(m_halfEdgeIdx)); }

KY_INLINE NavHalfEdgeIdx      NavHalfEdgeRawPtr::GetHalfEdgeIdx()  const { return (NavHalfEdgeIdx)m_halfEdgeIdx;      }
KY_INLINE NavFloor*           NavHalfEdgeRawPtr::GetNavFloor()     const { return m_navFloorRawPtr.GetNavFloor();     }
KY_INLINE const NavFloorBlob* NavHalfEdgeRawPtr::GetNavFloorBlob() const { return m_navFloorRawPtr.GetNavFloorBlob(); }
KY_INLINE const CellPos&      NavHalfEdgeRawPtr::GetCellPos()      const { return m_navFloorRawPtr.GetCellPos();      }

KY_INLINE const NavTag& NavHalfEdgeRawPtr::GetNavTag() const { return GetNavFloorBlob()->GetNavTag(NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(m_halfEdgeIdx)); }

KY_INLINE KyFloat32 NavHalfEdgeRawPtr::GetStartVertexAltitude() const
{
	const NavFloorBlob* navFloorBlob = GetNavFloorBlob();
	return navFloorBlob->GetNavVertexAltitude(navFloorBlob->NavHalfEdgeIdxToStartNavVertexIdx(m_halfEdgeIdx));
}
KY_INLINE KyFloat32 NavHalfEdgeRawPtr::GetEndVertexAltitude() const
{
	const NavFloorBlob* navFloorBlob = GetNavFloorBlob();
	return navFloorBlob->GetNavVertexAltitude(navFloorBlob->NavHalfEdgeIdxToEndNavVertexIdx(m_halfEdgeIdx));
}

template<class NavTagTraversePredicate>
KY_INLINE bool NavHalfEdgeRawPtr::IsHalfEdgeCrossable(NavHalfEdgeRawPtr& resultRawPtr, NavTagTraversePredicate* predicate) const
{
	return IsHalfEdgeCrossable<NavTagTraversePredicate>(GetNavFloorBlob(), resultRawPtr, predicate);
}

template<class NavTagTraversePredicate>
KY_INLINE bool NavHalfEdgeRawPtr::IsHalfEdgeCrossable(const NavFloorBlob* navFloorBlob, NavHalfEdgeRawPtr& resultRawPtr, NavTagTraversePredicate* predicate) const
{
	return IsHalfEdgeCrossable<NavTagTraversePredicate>(navFloorBlob->GetNavHalfEdge(m_halfEdgeIdx), navFloorBlob, resultRawPtr, predicate);
}

template<class NavTagTraversePredicate>
inline bool NavHalfEdgeRawPtr::IsHalfEdgeCrossable(NavTagTraversePredicate* predicate) const
{
	const NavFloorBlob* navFloorBlob = GetNavFloor()->GetNavFloorBlob();
	const NavHalfEdge halfEdge = navFloorBlob->GetNavHalfEdge(m_halfEdgeIdx);
	const NavHalfEdgeType halfEdgeType = halfEdge.GetHalfEdgeType();

	switch (halfEdgeType)
	{
	case EDGETYPE_OBSTACLE:
		return false;

	case EDGETYPE_PAIRED :
		return true;

	case EDGETYPE_CONNEXBOUNDARY : 
		{
			const NavTriangleIdx neihgbourTriangleIdx = NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(halfEdge.GetPairHalfEdgeIdx());
			return predicate->CanTraverse(navFloorBlob->GetNavTag(neihgbourTriangleIdx));
		}
	default :
		{
			const KyUInt32 boundaryEdgeIdx = halfEdge.GetBoundaryEdgeIdx();
			NavFloor& navFloor = *GetNavFloor();
			NavHalfEdgeRawPtr& link = navFloor.GetLinksBuffer()[boundaryEdgeIdx];
			if (link.IsValid() == false)
				return false;

			return predicate->CanTraverse(link.GetNavTag());
		}
	}
}

template<class NavTagTraversePredicate>
inline bool NavHalfEdgeRawPtr::IsHalfEdgeCrossable(NavHalfEdge halfEdge, const NavFloorBlob* navFloorBlob, NavHalfEdgeRawPtr& resultRawPtr, NavTagTraversePredicate* predicate) const
{
	const NavHalfEdgeType halfEdgeType = halfEdge.GetHalfEdgeType();

	switch (halfEdgeType)
	{
	case EDGETYPE_OBSTACLE:
		return false;

	case EDGETYPE_PAIRED :
		{
			resultRawPtr.Set(m_navFloorRawPtr, halfEdge.GetPairHalfEdgeIdx());
			return true;
		}

	case EDGETYPE_CONNEXBOUNDARY : 
		{
			resultRawPtr.Set(m_navFloorRawPtr, halfEdge.GetPairHalfEdgeIdx());
			return predicate->CanTraverse(navFloorBlob->GetNavTag(NavFloorBlob::NavHalfEdgeIdxToTriangleIdx(halfEdge.GetPairHalfEdgeIdx())));
		}

	default :
		{
			const KyUInt32 boundaryEdgeIdx = halfEdge.GetBoundaryEdgeIdx();
			NavFloor* navFloor = GetNavFloor();
			NavHalfEdgeRawPtr& link = navFloor->GetLinksBuffer()[boundaryEdgeIdx];
			resultRawPtr = link;

			if (link.IsValid() == false)
				return false;

			return predicate->CanTraverse(link.GetNavTag());
		}
	}
}


template<class NavTagTraversePredicate>
inline NavHalfEdgeRawPtr NavHalfEdgeRawPtr::GetNextNavHalfEdgeRawPtrAlongBorder(NavTagTraversePredicate* predicate) const
{
	KY_LOG_ERROR_IF(IsValid() == false, ("This function must not be called with an invalid instance of NavHalfEdgeId"));
	KY_DEBUG_ERRORN_IF(IsHalfEdgeCrossable<NavTagTraversePredicate>(predicate), ("Input NavHalfEdgeId is not an Id of obstacle edge. You should not have called this function !"));

	NavHalfEdgeRawPtr resultRawPtr;
	GetNextHalfEdgeRawPtr(resultRawPtr);

	NavHalfEdgeRawPtr pairHalfEdgeRawPtr;
	while (resultRawPtr.IsHalfEdgeCrossable<NavTagTraversePredicate>(pairHalfEdgeRawPtr, predicate))
	{
		pairHalfEdgeRawPtr.GetNextHalfEdgeRawPtr(resultRawPtr);
	}

	return resultRawPtr;
}

template<class NavTagTraversePredicate>
inline NavHalfEdgeRawPtr NavHalfEdgeRawPtr::GetPrevNavHalfEdgeRawPtrAlongBorder(NavTagTraversePredicate* predicate) const
{
	KY_LOG_ERROR_IF(IsValid() == false, ("This function must not be called with an invalid instance of NavHalfEdgeId"));
	KY_DEBUG_ERRORN_IF(IsHalfEdgeCrossable<NavTagTraversePredicate>(predicate), ("Input NavHalfEdgeId is not an Id of obstacle edge. You should not have called this function !"));

	NavHalfEdgeRawPtr resultRawPtr;
	GetPrevHalfEdgeRawPtr(resultRawPtr);

	NavHalfEdgeRawPtr pairHalfEdgeRawPtr;
	while (resultRawPtr.IsHalfEdgeCrossable<NavTagTraversePredicate>(pairHalfEdgeRawPtr, predicate))
	{
		pairHalfEdgeRawPtr.GetPrevHalfEdgeRawPtr(resultRawPtr);
	}

	return resultRawPtr;
}

}

#endif //Navigation_NavHalfEdgeRawPtr_H

