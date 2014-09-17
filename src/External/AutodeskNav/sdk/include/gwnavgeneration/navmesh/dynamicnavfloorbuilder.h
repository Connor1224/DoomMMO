/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: LASI - secondary contact: NOBODY
#ifndef GwNavGen_DynamicNavFloorBuilder_H
#define GwNavGen_DynamicNavFloorBuilder_H

#include "gwnavgeneration/generator/generatorsystem.h"
#include "gwnavruntime/base/timeprofiler.h"
#include "gwnavgeneration/boundary/boundaryvertex.h"
#include "gwnavruntime/navmesh/celldesc.h"

#include "gwnavruntime/visualsystem/displaylist.h"

#include "gwnavgeneration/common/stlvector.h"
#include "gwnavgeneration/common/stllist.h"
#include "gwnavruntime/base/memory.h"


namespace Kaim
{

class IndexedTriangleSoup2i;
class IndexedTriangleSoup3f;
class DynamicNavFloor;
class BoundaryPolygon;
class BoundaryGraph;
class BoundarySimplifiedPolygon;
class BoundarySimplifiedContour;
class DynamicNavFloorHeightField;
class DynamicTriangulation;
class DynamicNavCell;
class BoundaryVertex;
class BoundarySimplifiedEdge;

class DynamicNavFloorBuilder
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	class BoundaryContourElement
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public: 
		BoundaryContourElement() : m_boundaryVertex(KY_NULL), m_nextBoundaryEdge(KY_NULL) {}
		BoundaryContourElement(const BoundaryVertex* vertex, const BoundarySimplifiedEdge* nextEdge) 
			: m_boundaryVertex(vertex), m_nextBoundaryEdge(nextEdge) {}

		bool operator == (const BoundaryContourElement& p) const
		{
			return m_boundaryVertex == p.m_boundaryVertex && m_nextBoundaryEdge == p.m_nextBoundaryEdge;
		}
	public: 
		ExclBoundaryPos Pos() const { return m_boundaryVertex->m_exclBoundaryPos; }
		KyFloat32 Altitude() const { return m_boundaryVertex->m_altitude; }
	public: 
		const BoundaryVertex* m_boundaryVertex;
		const BoundarySimplifiedEdge* m_nextBoundaryEdge;
	};

	typedef TlsStlList<BoundaryContourElement> BoundaryVerticeList;
	typedef TlsStlList<BoundaryVerticeList>    BoundaryVerticeListCollection;

protected:
	class IndexedBoundaryContourElement
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public: 
		IndexedBoundaryContourElement() {}
		IndexedBoundaryContourElement(const BoundaryContourElement& element, KyUInt32 index)
			:m_element(element), m_index(index) {}
		bool operator == (const IndexedBoundaryContourElement& p) const
		{
			return m_element == p.m_element && m_index == p.m_index;
		}
	public: 
		ExclBoundaryPos Pos() const { return m_element.Pos(); }
	public:
		BoundaryContourElement m_element;
		KyUInt32 m_index;

	};
	typedef TlsStlList<IndexedBoundaryContourElement> IndexedBoundaryPolygon;

	struct EarCompElem
	{
		IndexedBoundaryPolygon::iterator earIter;
		IndexedBoundaryPolygon::iterator previousIter;
		IndexedBoundaryPolygon::iterator nextIter;

		KyFloat32 earMinAngle;

		bool operator < (const EarCompElem& other) const { return earMinAngle < other.earMinAngle; }
	};

	struct FloorLinkEdge
	{
		BoundaryVertex v1; // SHOULD BE POINTER !
		BoundaryVertex v2; // SHOULD BE POINTER !
		KyUInt32 pairFloorIdx;
		Vec3f v1Pos;
		Vec3f v2Pos;

		FloorLinkEdge(const BoundaryVertex& v1_, const BoundaryVertex& v2_, KyUInt32 pairFloorIdx_) :
			v1(v1_), v2(v2_), pairFloorIdx(pairFloorIdx_) {}

		bool operator == (const FloorLinkEdge& other) const { return v1 == other.v1 && v2 == other.v2; }
	};
	typedef TlsStlList<FloorLinkEdge> FloorLinkList;

	struct CellLinkEdge
	{
		BoundaryVertex v1; // SHOULD BE POINTER !
		BoundaryVertex v2; // SHOULD BE POINTER !
		KyUInt32 cardinalDir;
		Vec3f v1Pos;
		Vec3f v2Pos;

		CellLinkEdge(const BoundaryVertex& v1_, const BoundaryVertex& v2_, KyUInt32 cardinalDir_) :
			v1(v1_), v2(v2_), cardinalDir(cardinalDir_) {}

		bool operator == (const FloorLinkEdge& other) const { return v1 == other.v1 && v2 == other.v2; }
	};
	typedef TlsStlList<CellLinkEdge> CellLinkList;

public:
	DynamicNavFloorBuilder(GeneratorSystem* sys, const CellDesc& cellDesc) 
		: m_sys(sys), m_cellDesc(cellDesc), m_lastBuildDurationMs(0.0f), m_doProfile(false) 
	{
		m_displayLists.SetVisualDebugServer(m_sys->m_visualDebugServer);
	}

	void ConsumeContour(const BoundarySimplifiedContour& contour, FloorLinkList& floorLinkEdges, CellLinkList& cellLinkEdges, BoundaryVerticeList& vertices);
	KyResult BuildNavFloorFromBoundaryPolygons(const BoundaryGraph& inputGraph, const BoundarySimplifiedPolygon& polygon,
		DynamicNavFloorHeightField* heightField, DynamicNavFloor& floor, DynamicNavCell& cell);
	KyResult BuildNavFloorFromIndexedTriangleSoup(const IndexedTriangleSoup2i& soup, DynamicNavFloor& floor, FloorLinkList& floorLinkEdges,
		CellLinkList& cellLinkEdges, DynamicNavCell& cell, bool mightHaveHoles = true);
	void ToggleProfiling(bool activate) {m_doProfile = activate;}
	KyFloat32 GetLastBuildDurationMs() { return m_lastBuildDurationMs; }

private:
	KyResult AbsorbHoles(BoundaryVerticeList& border, BoundaryVerticeListCollection& holes);
	KyResult Triangulate(IndexedTriangleSoup2i& result, const BoundaryVerticeList& inputPolygon);
	bool IsEar(const IndexedBoundaryPolygon& workingPolygon,const IndexedBoundaryPolygon::iterator& currentIter);

	void ReOrderEdges(DynamicNavFloor& navFloorBlob);
	void RemoveDuplicatePointsFromSoup(const IndexedTriangleSoup2i& imputSoup, IndexedTriangleSoup2i& output);

	void RemoveDegeneratedTrianglesFromSoup(const IndexedTriangleSoup2i& imputSoup, IndexedTriangleSoup2i& output);

	KyUInt32 DiagnoseInputPolygon(const BoundaryVerticeList& inputPolygon);

	KyResult MakeVerticallyAccurateTriangulation(DynamicTriangulation& triangulation, DynamicNavFloorHeightField* heightField);

	void RenderDynamicTriangulationInVisualDebug(const DynamicTriangulation& triangulation);
	void RenderHeightFieldInVisualDebug(const DynamicNavFloorHeightField& heightField);

public:
	GeneratorSystem* m_sys;
	CellDesc m_cellDesc;
	KyFloat32 m_lastBuildDurationMs;
	bool m_doProfile;
	DisplayListManager m_displayLists;
};

} // namespace Kaim


#endif //GwNavGen_DynamicNavFloorBuilder_H

