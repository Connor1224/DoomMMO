/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: LASI
#ifndef Navigation_DynamicNavFloor_H
#define Navigation_DynamicNavFloor_H

#include "gwnavruntime/navmesh/blobs/flooraltituderange.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/math/box2f.h"
#include "gwnavruntime/database/navtag.h"


namespace Kaim
{

// ----------------------------
// ----- DynamicNavVertex -----
// ----------------------------
class DynamicNavVertex
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	DynamicNavVertex() :
		m_pixelPos(InvalidPixelCoord, InvalidPixelCoord),
		m_altitude(KyFloat32MAXVAL),
		m_first(KyUInt32MAXVAL)
	{}

	DynamicNavVertex(const PixelPos& pixelPos, KyFloat32 alt) :
		m_pixelPos(pixelPos),
		m_altitude(alt),
		m_first(KyUInt32MAXVAL)
	{}

	bool operator==(const DynamicNavVertex& other) const
	{
		return m_pixelPos == other.m_pixelPos && m_altitude == other.m_altitude;
	}
	bool operator!=(const DynamicNavVertex& other) const
	{
		return !operator==(other);
	}
public:
	PixelPos m_pixelPos;
	KyFloat32 m_altitude;
	KyUInt32 m_first; // first outgoing edge
};

// ------------------------------
// ----- DynamicNavTriangle -----
// ------------------------------
class DynamicNavTriangle
{
public:
	DynamicNavTriangle() : m_edge(KyUInt32MAXVAL) {}
	DynamicNavTriangle(KyUInt32 edge) : m_edge(edge) {}

public:
	KyUInt32 m_edge;	 // one edge around the face
};

// --------------------------
// ----- DynamicNavHalfEdge -----
// --------------------------

enum DynamicNavHalfEdgeType
{
	DynamicNavHalfEdgeType_Normal = 0,
	DynamicNavHalfEdgeType_Wall = 1,
	DynamicNavHalfEdgeType_Hole = 2,
	DynamicNavHalfEdgeType_FloorBoundary = 3,
	DynamicNavHalfEdgeType_CellBoundary = 4,

	DynamicNavHalfEdgeType_Invalid = 0xFFFFFFFF
};

class DynamicNavHalfEdge
{
public:
	DynamicNavHalfEdge() : m_idx(KyUInt32MAXVAL), m_start(KyUInt32MAXVAL), m_end(KyUInt32MAXVAL), m_face(KyUInt32MAXVAL),
		m_next(KyUInt32MAXVAL), m_type(DynamicNavHalfEdgeType_Invalid), m_pair(KyUInt32MAXVAL), m_cardinalDir(CardinalDir_INVALID) {}
public:
	KyUInt32 m_idx;
	KyUInt32 m_start; // start vertex
	KyUInt32 m_end; // end vertex (added by LS for dynamicFloorBuilder)
	KyUInt32 m_face; // the adjacent face
	KyUInt32 m_next; // next edge
	DynamicNavHalfEdgeType m_type;  // 0 = normal, 1 = wall, 2 = hole,  3 = Floor boundary, 4 = cell boundary
	
	// if m_type == Normal : pair edge idx in the same floor
	// if m_type == FloorBoundary || m_type == CellBoundary, UNUSED
	KyUInt32 m_pair;

	CardinalDir m_cardinalDir; // use for edge of type == 3 (cell boundary) only. The cardinal dir of the side of the cell
};

// --------------------------
// ----- DynamicNavFloor -----
// --------------------------
class DynamicNavFloor
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	enum PixelPosRelativeCoordSystem
	{
		PIXELPOS_ABSOLUTE,
		PIXELPOS_RELATIVE_TO_CELL
	};

public:
	DynamicNavFloor() :
		m_idx(KyUInt32MAXVAL),
		m_triangles(KY_NULL),
		m_vertices(KY_NULL),
		m_edges(KY_NULL),
		m_navTagIndexes(KY_NULL),
		m_navTagArray(KY_NULL),
		m_originalPixelColor(KyUInt32MAXVAL),
		m_cellPixelBoxOffset(PixelPos(InvalidPixelCoord, InvalidPixelCoord)),
		m_pixelPosCoordSystem(PIXELPOS_RELATIVE_TO_CELL)
	{
		Clear();
	}
	DynamicNavFloor(MemoryHeap* memoryHeap) :
		m_idx(KyUInt32MAXVAL),
		m_triangles(memoryHeap),
		m_vertices(memoryHeap),
		m_edges(memoryHeap),
		m_navTagIndexes(memoryHeap),
		m_navTagArray(memoryHeap),
		m_originalPixelColor(KyUInt32MAXVAL),
		m_cellPixelBoxOffset(PixelPos(InvalidPixelCoord, InvalidPixelCoord)),
		m_pixelPosCoordSystem(PIXELPOS_RELATIVE_TO_CELL)
	{
		Clear();
	}

	DynamicNavFloor(MemoryHeap* memoryHeap, const PixelPos& offsetPos) :
		m_idx(KyUInt32MAXVAL),
		m_triangles(memoryHeap),
		m_vertices(memoryHeap),
		m_edges(memoryHeap),
		m_navTagIndexes(memoryHeap),
		m_navTagArray(memoryHeap),
		m_originalPixelColor(KyUInt32MAXVAL),
		m_cellPixelBoxOffset(offsetPos),
		m_pixelPosCoordSystem(PIXELPOS_RELATIVE_TO_CELL)
	{
		Clear();
	}

	// Add a vertex to the pixel position relative to the cellbox
	// this position will later be transformed to navfloor absolute pixelbox relative position
	// using TransformToFloorRelativeCoords
	KyUInt32 AddVertex(const Vec2i& position, KyFloat32 altitude);

	KyUInt32 AddIntVertex(const Vec2i& position, KyFloat32 altitude);
	KyUInt32 AddIntVertex(const Vec2i& position); // Do not forget to call ComputeAltitudeRange() once the altitude has been set for each vertex
	// for legacy. Create a vertex a position but without pixel pos.
	KyUInt32 AddVertex3f(const Vec3f& position);

	KyUInt32 AddTriangle(KyUInt32 idxV1, KyUInt32 idxV2, KyUInt32 idxV3);
	KyUInt32 AddHalfEdge(KyUInt32 start, KyUInt32 end, KyUInt32 face, DynamicNavHalfEdgeType type = DynamicNavHalfEdgeType_Normal);
	KyResult MakeEdgesOpposite(KyUInt32 idxEdge1, KyUInt32 idxEdge2);

	// ---- Edge -----
	KyUInt32 GetNextEdgeIdx(KyUInt32 edgeIdx)     const {  return m_edges[edgeIdx].m_next; }
	KyUInt32 GetPrevEdgeIdx(KyUInt32 edgeIdx)     const {  return GetNextEdgeIdx(GetNextEdgeIdx(edgeIdx)); }
	KyUInt32 GetOppositeEdgeIdx(KyUInt32 edgeIdx) const {  return m_edges[edgeIdx].m_pair; }
	KyUInt32 GetStartVertexIdx(KyUInt32 edgeIdx)  const {  return m_edges[edgeIdx].m_start; }
	KyUInt32 GetFaceIdx(KyUInt32 edgeIdx)         const {  return m_edges[edgeIdx].m_face; }

	// ---- Face -----
	KyUInt32 GetEdgeIdx(KyUInt32 triangleIdx) const { return m_triangles[triangleIdx].m_edge; }

	// ---- Vertex -----
	KyUInt32 GetFirstEdgeIdx(KyUInt32 vertexIdx) const { return m_vertices[vertexIdx].m_first; }
	
	const DynamicNavVertex& GetNavVertex(KyUInt32 vertexIdx) const { return m_vertices[vertexIdx]; }

	KyUInt32 ComputeNextBorderEdgeIdx(KyUInt32 edgeIdx) const;

	void Clear()
	{
		m_vertices.Clear();
		m_triangles.Clear();
		m_edges.Clear();
		m_navTagIndexes.Clear();
		m_navTagArray.Clear();
		m_altitudeRange.m_maxZ = -KyFloat32MAXVAL;
		m_altitudeRange.m_minZ = KyFloat32MAXVAL;
	}

	void BuildAbsolutePixelBox()
	{
		m_absolutePixelBox.Clear();

		for (KyUInt32 i = 0; i < m_vertices.GetCount(); ++i)
			m_absolutePixelBox.ExpandByVec2(m_vertices[i].m_pixelPos + m_cellPixelBoxOffset);
	}

	bool AreIndexesWithinBounds() const;
	// search m_navTagArray for Navtag. 
	// If this particular navtag is not present, It will be added.
	// returns the index in m_navTagArray where tag was found or added. 
	KyUInt32 FindOrAddNavtag(const NavTag* tag); 

	void ComputeAltitudeRange();

public:
	KyUInt32 m_idx;

	KyArrayDH<DynamicNavTriangle> m_triangles;
	KyArrayDH_POD<DynamicNavVertex> m_vertices;
	KyArrayDH_POD<DynamicNavHalfEdge> m_edges;

	FloorAltitudeRange m_altitudeRange;

	KyArrayDH_POD<KyUInt32> m_navTagIndexes;
	KyArrayDH_POD<const NavTag*> m_navTagArray;
	KyUInt32 m_originalPixelColor;

	PixelBox m_absolutePixelBox;   // the pixel bounding box of this floor. In absolute coordinates ie not relative to the cell.
	PixelPos m_cellPixelBoxOffset; // correspond to the cell exclusivePixelBox.Min()
	PixelPosRelativeCoordSystem m_pixelPosCoordSystem; // either absolute or relative to cell or to floor
};

} // namespace Kaim


#endif //Navigation_DynamicNavFloor_H

