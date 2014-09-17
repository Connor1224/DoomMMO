/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/




// primary contact: GUAL - secondary contact: MAMU
#ifndef Navigation_VisualGeometryBuilder_H
#define Navigation_VisualGeometryBuilder_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/visualsystem/ivisualgeometry.h"
#include "gwnavruntime/basesystem/coordsystem.h"

namespace Kaim
{

class IVisualGeometry;
class VisualColor;
class VisualRepresentation;
class Box3f;
class Vec3f;

/// The VisualGeometryBuilder class offers an API used to push geometrical forms to an object that derives from IVisualGeometry.
/// This class is typically used by implementations of the VisualRepresentation::BuildGeometry() method. An instance of this
/// class is passed to VisualRepresentation::BuildGeometry(), which uses the VisualGeometryBuilder class methods to push 
/// geometrical forms that represent its data. The VisualGeometryBuilder expresses these geometrical forms as triangles in the
/// coordinate system of the game engine, and passes the triangles to an object that derives from IVisualGeometry, specified
/// in the VisualGeometryBuilder constructor. The IVisualGeometry in turn handles the rendering of those triangles within the 
/// game engine.
class VisualGeometryBuilder
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualRep)

public:
	/// Constructs a new VisualGeometryBuilder.
	/// \param geometry			An object derived from IVisualGeometry, which will render the geometry pushed to this VisualGeometryBuilder. 
	VisualGeometryBuilder(IVisualGeometry* geometry);

	/// For internal use by VisualRepresentation. 
	const VisualGeometrySetupConfig& SwitchToFillMode();

public:
	/// Configures the VisualGeometryBuilder to generate normals for the faces of each triangle it passes to the IVisualGeometry.
	/// Normals are disabled by default. 
	void EnableNormalGeneration() { m_setupConfig.m_generateNormals = true; }

	/// Configures the VisualGeometryBuilder not to generate any normals for the faces of each triangle it passes to the IVisualGeometry.
	/// Normals are disabled by default. 
	void DisableNormalGeneration() { m_setupConfig.m_generateNormals = false; }

	/// Sets the current color maintained by the VisualGeometryBuilder. All triangles passed to the IVisualGeometry will be passed
	/// using this color until the next call to SetCurrentVertexColor(). By default, uses Kaim::White. 
	void SetCurrentVertexColor(const VisualColor& vertexColor) { m_currentVertexColor = vertexColor; }

	void SetCurrentAltitudeOffset(KyFloat32 altitudeOffset) { m_currentAltitudeOffset = altitudeOffset; }

	/// Pushes a single triangle. 
	void PushTriangle(const Vec3f& A, const Vec3f& B, const Vec3f& C);

	/// Pushes two triangles that form a quad. 
	void PushQuad(const Vec3f& A, const Vec3f& B, const Vec3f& C, const Vec3f& D);

	/// Pushes an axis-aligned box centered on P. 
	void PushPoint(const Vec3f& P, KyFloat32 halfWidth);

	/// Pushes an axis-aligned tetrahedron centered on P and pointing down. 
	void PushPoint_Tetrahedron(const Vec3f& P, KyFloat32 halfWidth);

	/// Pushes an axis-aligned box whose base is centered on P. 
	void PushColumn(const Vec3f& P, KyFloat32 halfWidth, KyFloat32 height);

	/// Pushes a 2D flat disk centered on P. 
	void PushDisk(const Vec3f& P, KyFloat32 radius, KyUInt32 subdivisionCount = 6);

	/// Pushes a cylinder whose base is centered on P. 
	void PushVerticalCylinder(const Vec3f& P, KyFloat32 radius, KyFloat32 height, KyUInt32 subdivisionCount = 6);

	/// Pushes an axis-aligned box. 
	void PushBox(const Box3f& box);

	/// Pushes a line segment. P and Q are the centers of the two opposite ends, and halfWidth is half the width of the segment. 
	void PushSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	/// Pushes a line segment. P and Q are at a distance of margin inside the two opposite ends,
	/// and halfWidth is half the width of the segment. 
	void PushSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth, KyFloat32 margin);

	/// Push a pyramid composed of a base and four sides. P the is center of the base, Q is the peak. 
	void PushPyramid(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	/// Represents an arrow shape that can be pushed to VisualGeometryBuilder through a call to PushArrow(). 
	struct ArrowShape
	{
		/// Constructs a new ArrowShape.
		explicit ArrowShape(KyFloat32 bodyHalfWidth, KyFloat32 headWidthRatio = 3.0f, KyFloat32 headLengthRatio = 0.33f) :
			m_bodyHalfWidth(bodyHalfWidth), m_headWidthRatio(headWidthRatio), m_headLengthRatio(headLengthRatio) {}

		KyFloat32 m_bodyHalfWidth; ///< Half of the width of the body or trunk of the arrow.

		/// The ratio between the width of the head and the width of the body. In other words, the head or point
		/// of the arrow will be this many times wider than the body or trunk of the arrow.
		/// \defaultvalue    3.0f 

		KyFloat32 m_headWidthRatio;  // = headWidth  / bodyWidth
		/// The ratio between the length of the head and the length of the body. In other words, the head or point
		/// of the arrow will be this many times longer than the body or trunk of the arrow.
		/// \defaultvalue    0.33f 
		KyFloat32 m_headLengthRatio; // = headLength / bodyLength
	};

	/// Pushes an arrow from P to Q using a default arrow shape.
	void PushArrow(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	/// Pushes an arrow from P to Q using a custom arrow shape set by arrowShape.
	void PushArrow(const Vec3f& P, const Vec3f& Q, const ArrowShape& arrowShape);

	/// Pushes a three-dimensional oriented box.
	/// \param P			A point at one end of the box.
	/// \param Q			A point at the opposite end of the box.
	/// \param upHeight		The distance of the top of the box above P and Q.
	/// \param downHeight	The distance of the bottom of the box below P and Q
	/// \param halfWidth	The distance of the sides of the box on either side ofP and Q. 
	void PushWall(const Vec3f& P, const Vec3f& Q, KyFloat32 upHeight, KyFloat32 downHeight, KyFloat32 halfWidth);

	/// Pushes a Christmas tree: two orthogonal triangles with the same central axes. P is the center of the base of
	/// both triangles, and Q is at the top of both triangles. Both triangles extend a distance of bodyHalfWidth
	/// from the central axes.
	void PushChristmasTree(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	/// Pushes a silex from P to Q with the specified bodyHalfWidth. 
	void PushSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	/// Pushes a double silex from P to Q with the specified bodyHalfWidth. Equivalent to a silex from P to the midpoint
	/// between P and Q, and a silex from Q to the midpoint between P and Q. 
	void PushDoubleSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	/// Pushes two orthogonal rectangles with the same central axes. P is the center of one end of both rectangles, and Q 
	/// is at the center of the opposite end of both rectangles. Both rectangles extend a distance of bodyHalfWidth
	/// from the central axes. 
	void PushCrossedRectangles(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	/// Pushes a hollow tube with a square cross-section. P and Q are at the center of two opposite faces, and the width of the
	/// tube is equal to twicehalfWidth. 
	void PushSquareTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	/// Pushes a hollow tube with a triangular cross-section. P and Q are at the center of two opposite faces, and the width of the
	/// tube is equal to twicehalfWidth. 
	void PushTriangleTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	/// Pushes a set of triangles built from the points in the arrays.
	/// \param nbVertices			The number of elements in the vertices array.
	/// \param vertices				An array that contains all vertices that make up the triangles.
	/// \param nbIndices			The number of elements in the indices array.
	/// \param indices				An array that contains an index value for each corner of each triangle. This method constructs triangles
	/// 							by retrieving the indices from the vertices array. For example, indices[0] should indicate the index within
	/// 						    vertices at which the coordinates of the first point of the first triangle can be found, indices[1] should
	/// 							indicate the index within vertices at which the coordinates of the second point of the first triangle can
	/// 							be found, and indices[2] should indicate the index within vertices at which the coordinates of the third point
	/// 							of the first triangle can be found. Then, indices[3], indices[4] and indices[5] are used to retrieve the
	/// 							coordinates of the second triangle from vertices, etc.
	void PushIndexedTriangles(KyUInt32 nbVertices, const Vec3f* vertices, KyUInt32 nbIndices, const KyUInt32* indices);

public: // internal
	void FillTriangle(const Vec3f& A, const Vec3f& B, const Vec3f& C);

	void FillQuad(const Vec3f& A, const Vec3f& B, const Vec3f& C, const Vec3f& D);

	void FillPoint(const Vec3f& P, KyFloat32 halfWidth);

	void FillPoint_Tetrahedron(const Vec3f& P, KyFloat32 halfWidth);

	void FillColumn(const Vec3f& P, KyFloat32 halfWidth, KyFloat32 height);

	void FillDisk(const Vec3f& P, KyFloat32 radius, KyUInt32 subdivisionCount);

	void FillVerticalCylinder(const Vec3f& P, KyFloat32 radius, KyFloat32 height, KyUInt32 subdivisionCount);

	void FillBox(const Box3f& box);

	void FillSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	void FillSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth, KyFloat32 margin);

	void FillPyramid(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	void FillArrow(const Vec3f& P, const Vec3f& Q, const ArrowShape& arrowShape);

	void FillWall(const Vec3f& P, const Vec3f& Q, KyFloat32 upHeight, KyFloat32 downHeight, KyFloat32 halfWidth);

	void FillChristmasTree(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	void FillCrossedRectangles(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	void FillSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	void FillDoubleSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth);

	void FillSquareTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	void FillTriangleTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth);

	void FillIndexedTriangles(KyUInt32 nbVertices, const Vec3f* vertices, KyUInt32 nbIndices, const KyUInt32* indices);

private:
	void IncrementTrianglesCount(KyUInt32 newTrianglesCount);
	void SetCountMode();

private:
	VisualGeometrySetupConfig m_setupConfig;

	enum BuildMode { COUNT, FILL };
	BuildMode m_buildMode;
	KyUInt32 m_currentVertexIdx;
	KyUInt32 m_currentTriangleIdx;
	VisualColor m_currentVertexColor;
	KyFloat32 m_currentAltitudeOffset;

	IVisualGeometry* m_geometry;
	CoordSystem m_coordSystem;
};


inline void VisualGeometryBuilder::IncrementTrianglesCount(KyUInt32 newTrianglesCount)
{
	m_currentTriangleIdx += newTrianglesCount;
	m_currentVertexIdx += newTrianglesCount * 3;
}

inline void VisualGeometryBuilder::PushTriangle(const Vec3f& A, const Vec3f& B, const Vec3f& C)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(1);
	else
		FillTriangle(A, B, C);

}

inline void VisualGeometryBuilder::PushQuad(const Vec3f& A, const Vec3f& B, const Vec3f& C, const Vec3f& D)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(2);
	else
		FillQuad(A, B, C, D);
}

inline void VisualGeometryBuilder::PushPoint(const Vec3f& P, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(12);
	else
		FillPoint(P, halfWidth);
}

inline void VisualGeometryBuilder::PushPoint_Tetrahedron(const Vec3f& P, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(4);
	else
		FillPoint_Tetrahedron(P, halfWidth);
}

inline void VisualGeometryBuilder::PushColumn(const Vec3f& P, KyFloat32 halfWidth, KyFloat32 height)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(12);
	else
		FillColumn(P, halfWidth, height);
}

inline void VisualGeometryBuilder::PushDisk(const Vec3f& P, KyFloat32 radius, KyUInt32 subdivisionCount)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(subdivisionCount);
	else
		FillDisk(P, radius, subdivisionCount);
}

inline void VisualGeometryBuilder::PushVerticalCylinder(const Vec3f& P, KyFloat32 radius, KyFloat32 height, KyUInt32 subdivisionCount)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(subdivisionCount * 4);
	else
		FillVerticalCylinder(P, radius, height, subdivisionCount);
}

inline void VisualGeometryBuilder::PushBox(const Box3f& box)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(12);
	else
		FillBox(box);
}

inline void VisualGeometryBuilder::PushSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(12);
	else
		FillSegment(P, Q, halfWidth);
}

inline void VisualGeometryBuilder::PushSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth, KyFloat32 margin)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(12);
	else
		FillSegment(P, Q, halfWidth, margin);
}

inline void VisualGeometryBuilder::PushPyramid(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(6);
	else
		FillPyramid(P, Q, halfWidth);
}

inline void VisualGeometryBuilder::PushArrow(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth)
{
	PushArrow( P, Q, ArrowShape(bodyHalfWidth));
}

inline void VisualGeometryBuilder::PushArrow(const Vec3f& P, const Vec3f& Q, const ArrowShape& arrowShape)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(18);
	else
		FillArrow(P, Q, arrowShape);
}

inline void VisualGeometryBuilder::PushWall(const Vec3f& P, const Vec3f& Q, KyFloat32 upHeight, KyFloat32 downHeight, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(12);
	else
		FillWall(P, Q, upHeight, downHeight, halfWidth);
}

inline void VisualGeometryBuilder::PushChristmasTree(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(2);
	else
		FillChristmasTree(P, Q, bodyHalfWidth);
}

inline void VisualGeometryBuilder::PushSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(4);
	else
		FillSilex(P, Q, bodyHalfWidth);
}

inline void VisualGeometryBuilder::PushDoubleSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(6);
	else
		FillDoubleSilex(P, Q, bodyHalfWidth);
}

inline void VisualGeometryBuilder::PushCrossedRectangles(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(4);
	else
		FillCrossedRectangles(P, Q, bodyHalfWidth);
}

inline void VisualGeometryBuilder::PushSquareTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(8);
	else
		FillSquareTubeSegment(P, Q, halfWidth);
}

inline void VisualGeometryBuilder::PushTriangleTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(6);
	else
		FillTriangleTubeSegment(P, Q, halfWidth);
}

inline void VisualGeometryBuilder::PushIndexedTriangles(KyUInt32 nbVertices, const Vec3f* vertices, KyUInt32 nbIndices, const KyUInt32* indices)
{
	if (m_buildMode == COUNT)
		IncrementTrianglesCount(nbIndices / 3);
	else
		FillIndexedTriangles(nbVertices, vertices, nbIndices, indices);
}


}


#endif
