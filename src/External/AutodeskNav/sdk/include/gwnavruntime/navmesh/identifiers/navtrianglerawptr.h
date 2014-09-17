/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavTriangleRawPtr_H
#define Navigation_NavTriangleRawPtr_H

#include "gwnavruntime/navmesh/identifiers/navfloorrawptr.h"
#include "gwnavruntime/math/triangle3f.h"

namespace Kaim
{

/// Each instance of this class uniquely identifies a single NavTriangle in a NavFloor.
/// This pointer is guaranteed to be valid only in the frame in which it was retrieved.
/// Never store a NavTriangleRawPtr for use in subsequent frames, because it has no protection against data
/// streaming. Use NavTrianglePtr instead.
/// Actually, this class is used internally for some performance and working memory usage friendliness reason
/// (no ref-counting increment/decrement, no need to call constructor/destructor), but unless you really know what you do
/// prefer NavTrianglePtr which is safer.
class NavTriangleRawPtr
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)

public:
	/// Creates an invalid NavTriangleRawPtr. 
	NavTriangleRawPtr();

	/// Constructs a new NavTriangleRawPtr referring to the provided NavTriangleIdx in the provided NavFloorRawPtr.
	/// \param navFloorRawPtr    The NavFloor that contains the triangle this object should refer to. 
	/// \param triangleIdx       The index of this triangle within the NavFloor. 
	NavTriangleRawPtr(const NavFloorRawPtr& navFloorRawPtr, NavTriangleIdx triangleIdx);

	/// Constructs a new NavTriangleRawPtr referring to the provided NavTriangleIdx in the provided NavFloor.
	/// \param navFloor       The NavFloor that contains the triangle this object should refer to. 
	/// \param triangleIdx    The index of this triangle within the NavFloor. 
	NavTriangleRawPtr(NavFloor* navFloor, NavTriangleIdx triangleIdx);


	/// Clears all information maintained by this object.
	/// \param navFloorRawPtr    The NavFloor that contains the triangle this object should refer to. 
	/// \param triangleIdx       The index of this triangle within the NavFloor. 
	void Set(const NavFloorRawPtr& navFloorRawPtr, NavTriangleIdx triangleIdx);

	/// Clears all information maintained by this object.
	/// \param navFloor       The NavFloor that contains the triangle this object should refer to. 
	/// \param triangleIdx    The index of this triangle within the NavFloor. 
	void Set(NavFloor* navFloor, NavTriangleIdx triangleIdx);

	/// Returns true if this object refers to a valid edge: i.e. an edge in a validNavFloor. see NavFloorPtr::IsValid()
	bool IsValid() const;

	void Invalidate(); ///< Invalidates this object.

	bool operator==(const NavTriangleRawPtr& rhs) const; ///< Returns true if this object identifies the same triangle as rhs, or if both are invalid. 
	bool operator!=(const NavTriangleRawPtr& rhs) const; ///< Returns true if this object identifies a different triangle from rhs. 

	NavTriangleIdx GetTriangleIdx() const; ///< Retrieves the index of this triangle within its NavFloorBlob.

	// ---------------------------------- Member Functions for valid instance ----------------------------------
	/// All these function should be called only if IsValid() returns true
	/// \pre    This object must be valid. Its validity is not checked. 

	/// Returns a reference to the NavFloor that contains this triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	NavFloor* GetNavFloor() const;

	/// Returns a reference to the NavFloorBlob that contains this triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	const NavFloorBlob* GetNavFloorBlob() const;

	/// Returns a const reference to the CellPos that indicates the position of the cell that contains this triangle
	/// within the grid of NavData cells.
	/// \pre    This object must be valid. Its validity is not checked. 
	const CellPos& GetCellPos() const;

	/// Retrieves the NavTag associated to the NavFloor that contains this triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	const NavTag& GetNavTag() const;

	/// Updates the parameters to identify the vertices in this triangle.
	/// \param[out] v0Pos3f    The 3D position of the first vertex in the triangle.
	/// \param[out] v1Pos3f    The 3D position of the second vertex in the triangle.
	/// \param[out] v2Pos3f    The 3D position of the third vertex in the triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetVerticesPos3f(Vec3f& v0Pos3f, Vec3f& v1Pos3f, Vec3f& v2Pos3f) const;

	/// Updates the parameters to identify the vertices in this triangle.
	/// \param[out] triangle3f    The 3 3D positions of the vertices in the triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetVerticesPos3f(Triangle3f& triangle3f) const;

	/// Updates the parameters to identify the vertices in this triangle.
	/// \param[out] v0CoordPos64    The 2D integer position of the first vertex in the triangle.
	/// \param[out] v1CoordPos64    The 2D integer position of the second vertex in the triangle.
	/// \param[out] v2CoordPos64    The 2D integer position of the third vertex in the triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetVerticesCoordPos64(CoordPos64& v0CoordPos64, CoordPos64& v1CoordPos64, CoordPos64& v2CoordPos64) const;

	/// Updates the parameters to identify the vertices in this triangle.
	/// \param[out] v0CoordPos    The 2D integer position in cell of the first vertex in the triangle.
	/// \param[out] v1CoordPos    The 2D integer position in cell of the second vertex in the triangle.
	/// \param[out] v2CoordPos    The 2D integer position in cell of the third vertex in the triangle.
	/// \pre    This object must be valid. Its validity is not checked. 
	void GetVerticesCoordPos(CoordPos& v0CoordPos, CoordPos& v1CoordPos, CoordPos& v2CoordPos) const;

	/// Returns the altitude of the point on the plane defined by this triangle that has the same (X,Y) coordinates as
	/// the specified position. 
	/// \param pos    The 3D position of the point
	/// \pre    This object must be valid. Its validity is not checked. 
	KyFloat32 GetAltitudeOfPointInTriangle(const Vec3f& pos) const;

public:
	NavFloorRawPtr m_navFloorRawPtr; ///< The NavFloor that contains this triangle. 
	CompactNavTriangleIdx m_triangleIdx; ///< The index of this triangle within its NavFloor. 
};

KY_INLINE NavTriangleRawPtr::NavTriangleRawPtr() : m_navFloorRawPtr(), m_triangleIdx(CompactNavTriangleIdx_MAXVAL) {}

KY_INLINE NavTriangleRawPtr::NavTriangleRawPtr(const NavFloorRawPtr& navFloorRawPtr, NavTriangleIdx triangleIdx) :
	m_navFloorRawPtr(navFloorRawPtr), m_triangleIdx((CompactNavTriangleIdx)triangleIdx) {}

KY_INLINE void NavTriangleRawPtr::Set(const NavFloorRawPtr& navFloorRawPtr, NavTriangleIdx triangleIdx)
{
	m_navFloorRawPtr = navFloorRawPtr;
	m_triangleIdx = (CompactNavTriangleIdx)triangleIdx;
}

KY_INLINE NavTriangleRawPtr::NavTriangleRawPtr(NavFloor* navFloor, NavTriangleIdx triangleIdx) :
	m_navFloorRawPtr(navFloor), m_triangleIdx((CompactNavTriangleIdx)triangleIdx) {}

KY_INLINE void NavTriangleRawPtr::Set(NavFloor* navFloor, NavTriangleIdx triangleIdx)
{
	m_navFloorRawPtr.Set(navFloor);
	m_triangleIdx = (CompactNavTriangleIdx)triangleIdx;
}

KY_INLINE bool NavTriangleRawPtr::IsValid() const { return m_navFloorRawPtr.IsValid() && m_triangleIdx != CompactNavTriangleIdx_MAXVAL; }
KY_INLINE void NavTriangleRawPtr::Invalidate() { m_navFloorRawPtr.Invalidate(); m_triangleIdx = CompactNavTriangleIdx_MAXVAL; }

KY_INLINE bool NavTriangleRawPtr::operator==(const NavTriangleRawPtr& rhs) const { return m_navFloorRawPtr == rhs.m_navFloorRawPtr && m_triangleIdx == rhs.m_triangleIdx;}
KY_INLINE bool NavTriangleRawPtr::operator!=(const NavTriangleRawPtr& rhs) const { return !(*this == rhs); }

KY_INLINE NavTriangleIdx NavTriangleRawPtr::GetTriangleIdx() const { return (NavTriangleIdx)m_triangleIdx; }

KY_INLINE NavFloor*           NavTriangleRawPtr::GetNavFloor()       const { return m_navFloorRawPtr.GetNavFloor();              }
KY_INLINE const NavFloorBlob* NavTriangleRawPtr::GetNavFloorBlob()   const { return m_navFloorRawPtr.GetNavFloorBlob();          }
KY_INLINE const CellPos&      NavTriangleRawPtr::GetCellPos()        const { return m_navFloorRawPtr.GetCellPos();               }
KY_INLINE const NavTag&       NavTriangleRawPtr::GetNavTag()         const { return GetNavFloorBlob()->GetNavTag(m_triangleIdx); }

KY_INLINE void NavTriangleRawPtr::GetVerticesPos3f(Triangle3f& triangle3f) const
{
	GetVerticesPos3f(triangle3f.A, triangle3f.B, triangle3f.C);
}

}

#endif //Navigation_NavTriangleRawPtr_H

