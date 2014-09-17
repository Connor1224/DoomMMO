/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_BoundaryGraph_H
#define GwNavGen_BoundaryGraph_H

#include "gwnavruntime/navmesh/celldesc.h"
#include "gwnavgeneration/boundary/boundarypixel.h"
#include "gwnavgeneration/boundary/boundaryedge.h"
#include "gwnavgeneration/boundary/boundaryvertex.h"
#include "gwnavgeneration/boundary/boundarysimplifypolyline.h"
#include "gwnavgeneration/boundary/boundarypolygon.h"
#include "gwnavgeneration/boundary/boundarysimplifiedpolygon.h"
#include "gwnavgeneration/common/boxofarrays.h"
#include "gwnavgeneration/common/boxoflists.h"
#include "gwnavgeneration/common/stlvector.h"
#include "gwnavgeneration/common/stllist.h"
#include "gwnavruntime/base/types.h"


namespace Kaim
{

class DynamicNavRasterCell;
class GeneratorSystem;
struct ConnectionSquare;

/*
          exclusive_MaxX
exclusive_MinX     |
       |           |
 +---+---+---+---+---+---+
 |   |   |   |   |   |   |
 +---#################---+
 |   #   |   |   |   #   | exclusive_Max_Y
 +---#---+---+---+---#---+
 |   #   |   |   |   #   |
 +---#---+---+---+---#---+
 |   #   |   |   |   #   |
 +---#---+---+---+---#---+
 |   #   |   |   |   #   | exclusive_Min_Y
 +---#################---+
 |   |   |   |   |   |   |
 +---+---+---+---+---+---+
*/
class BoundaryGraph
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BoundaryGraph(const DynamicNavRasterCell* navRaster);

	~BoundaryGraph();

	KyResult Build();

	KyResult WriteIntermediateFiles();

	KyUInt32 VerticesCount() const { return m_vertices.GetElementCount(); }
	KyUInt32 EdgesCount()    const { return m_edges.GetElementCount();    }
	KyUInt32 ContoursCount() const { return m_contours.GetElementCount(); }
	KyUInt32 PolygonsCount() const { return (KyUInt32)m_polygons.size();  }
	KyUInt32 SimplifyPolylinesCount() const { return(KyUInt32)m_simplifyPolylines.size(); }

	KyUInt32 SimplifiedEdgesCount()    const { return(KyUInt32)m_simplifiedEdges.GetElementCount(); }
	KyUInt32 SimplifiedContoursCount() const { return(KyUInt32)m_simplifiedContours.GetElementCount(); }
	KyUInt32 SimplifiedPolygonsCount() const { return(KyUInt32)m_simplifiedPolygons.size(); }

private:
	BoundaryEdge* AddEdge(Boundary::CardinalDir dir, Boundary::EdgeType type, BoundaryPixel* leftPixel, KyUInt32 leftColor);
	
	// Temporary
	bool IsInputNavRasterValid();

	// 1. Replicate navRaster structure with special treatment on corners
	void BuildPixels();

	// 2. Scan along the X axis and build horizontal edges
	void BuildEdges_AlongX();

	// 3. Scan along the Y axis and build vertical edges
	void BuildEdges_AlongY();

	// 4. Build diagonal edges on the corners ...
	void BuildCornerEdges();
	// ... corner by corner
	void BuildNorthWestCornerEdges();
	void BuildNorthEastCornerEdges();
	void BuildSouthEastCornerEdges();
	void BuildSouthWestCornerEdges();

	// 5. Based on the edges, analyse patterns and build vertices
	KyResult BuildVertices();

	// 6. Follow edges and build contours
	void BuildContours();

	// 7. Group polylines in polygons
	void BuildPolygons();
	
	// 8. Simplify polylines
	void BuildSimplifyPolylines();

	// 9. Reassociate simplified polygons
	void BuildSimplifiedPolygons();

	// 10. Post ear clipping simplification.
	void EarClippingSimplification();

	// Misc support functions ...
	void InitBoundaryPixel(BoundaryPixel& BoundaryPixel);
	KyUInt32 ProcessEdgeColumn_AlongX(NavPixelCoord low_x, NavPixelCoord low_y);
	KyUInt32 ProcessEdgeColumn_AlongY(NavPixelCoord low_x, NavPixelCoord low_y);
	KyUInt32 ProcessEdge_AlongX(NavPixelCoord low_y, BoundaryPixel* lowPixel, BoundaryPixel* highPixel);
	KyUInt32 ProcessEdge_AlongY(NavPixelCoord low_x, BoundaryPixel* lowPixel, BoundaryPixel* highPixel);
	KyResult BuildVerticesColumn(NavBoundaryCoord x, NavBoundaryCoord y);
	void AddVertexColumnToBuild(NavBoundaryCoord x, NavBoundaryCoord y);
	void SetupSquarePixelColumn(KyUInt32 idxInSquare, NavPixelCoord x, NavPixelCoord y);
	void BuildContour(BoundaryEdge* beginEdge);
	
	void BuildSimplifyPolylinesInContour(BoundaryContour& contour);
	void BuildSimplifyPolyline_Cycle(BoundaryContour& contour);
	void BuildSimplifyPolylines_NonCycle(BoundaryContour& contour, BoundaryEdge* firstEdgeWithStaticStart);
	void BuildSimplifiedContour(BoundaryContour& contour, BoundarySimplifiedContour& simplifiedContour);
	void AddSimplifiedEdge(BoundarySimplifiedContour* simplifiedContour, BoundaryEdge* firstEdge, BoundaryEdge* lastEdge);

	bool IsOutsideWest(NavPixelCoord x)  { return x == 0; }
	bool IsOutsideEast(NavPixelCoord x)  { return x == m_navPixelBox.CountX() - 1; }
	bool IsOutsideSouth(NavPixelCoord y) { return y == 0; }
	bool IsOutsideNorth(NavPixelCoord y) { return y == m_navPixelBox.CountY() - 1; }

	bool IsOutside(NavPixelCoord x, NavPixelCoord y) { return IsOutsideWest(x) || IsOutsideEast(x) || IsOutsideSouth(y) || IsOutsideNorth(y); }
	KyResult SimplifyPolylines();

public:
	GeneratorSystem* m_sys;
	const DynamicNavRasterCell* m_navRaster;
	CellDesc m_cellDesc;
	PixelBox m_navPixelBox; // navRaster PixelBox
	PixelBox m_exclusivePixelBox; // exclusive PixelBox (without overlap)

	BoxOfArrays<BoundaryPixel> m_boundaryPixels; // association from NavPixels to BoundaryEdges

	GrowingPool<BoundaryEdge> m_edges;
	GrowingPool<BoundaryEdge> m_outsideEdges;
	GrowingPool<BoundaryVertex> m_vertices;
	GrowingPool<BoundaryContour> m_contours;

	TlsStlVector<BoundaryPolygon> m_polygons;

	TlsStlVector<NavBoundaryPos> m_vertexColumnsToBuild;
	ConnectionSquare* m_connectionSquare;

	TlsStlVector<BoundarySimplifyPolyline*> m_simplifyPolylines;

	GrowingPool<BoundarySimplifiedEdge> m_simplifiedEdges;
	GrowingPool<BoundarySimplifiedContour> m_simplifiedContours;
	TlsStlVector<BoundarySimplifiedPolygon> m_simplifiedPolygons;
};


}


#endif
