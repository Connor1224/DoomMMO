/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef KAIM_TAG_VOLUME_MERGER_TYPES_H
#define KAIM_TAG_VOLUME_MERGER_TYPES_H

// primary contact: LASI - secondary contact: NONE
#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/math/vec3i.h"
#include "gwnavruntime/math/geometryfunctions.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/math/integersegmentutils.h"
#include "gwnavruntime/basesystem/logger.h"
#include "gwnavruntime/containers/sharedpoollist.h"



namespace Kaim
{
	static const KyUInt32 SWEEPLINE_CLIPPING_EXTERIOR_POLYGON_BIT = 1 << 30;
	static const KyUInt32 SWEEPLINE_CLIPPING_HOLE_POLYGON_BIT = 1 << 29;
	static const KyUInt32 SWEEPLINE_MAX_TAGVOLUME_POLYGON_INDEX = 1 << 28;

	static const KyUInt32 TAGVOLUME_INDEX_OUT_OF_NAVMESH = KyUInt32MAXVAL;
	static const KyUInt32 TAGVOLUME_INDEX_UNKNOWN = TAGVOLUME_INDEX_OUT_OF_NAVMESH - 1;
	static const KyUInt32 TAGVOLUME_INDEX_LAST_VALID = TAGVOLUME_INDEX_UNKNOWN - 1;

	enum TagVolumeMergerContourWinding
	{
		MERGER_INPUT_CONTOUR_IS_CCW,
		MERGER_INPUT_CONTOUR_IS_CW,
		UNKNOWN_CONTOUR_WINDING
	};

struct MergerRawInputEdgeProperty
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)
public: 
	enum EdgeOrigin
	{
		EDGE_FROM_NAVFLOOR_LINK = 0,
		EDGE_FROM_NAVCELL_LINK = 1,
		EDGE_FROM_NAVFLOOR_OBSTACLE = 2,
		EDGE_FROM_TAGVOLUME = 3,
		EDGE_FROM_UNKNOWN_ORIGIN
	};

public:
	typedef KyUInt32 NavTagIndex;
	MergerRawInputEdgeProperty():
		m_edgeOrigin(EDGE_FROM_UNKNOWN_ORIGIN),
		m_contourWinding(POLYGON_UNKNOWN_WINDING),
		m_ownerIndex(KyUInt32MAXVAL),
		m_navTagIndex(TAGVOLUME_INDEX_UNKNOWN),
		m_index(KyUInt32MAXVAL)
	{}

	bool IsBoundary() const { return IsFromStaticNavfloor() && !IsHole(); }
	bool IsHole() const { return (IsFromStaticNavfloor() && m_contourWinding == POLYGON_IS_CW); }
	bool IsFromStaticNavfloor() const { return (m_edgeOrigin <= EDGE_FROM_NAVFLOOR_OBSTACLE); }
	bool IsFromTagVolume() const { return !IsFromStaticNavfloor(); }
	KyUInt32 GetCompositeIndex() const 
	{ 
		KyUInt32 index = m_ownerIndex;
		index |= IsBoundary() ? SWEEPLINE_CLIPPING_EXTERIOR_POLYGON_BIT : 0;
		index |= IsHole() ? SWEEPLINE_CLIPPING_HOLE_POLYGON_BIT : 0;
		return index;
	}
	bool operator <(const MergerRawInputEdgeProperty& rhs) const { return GetCompositeIndex() < rhs.GetCompositeIndex(); }
	bool operator ==(const MergerRawInputEdgeProperty& rhs) const { return GetCompositeIndex() == rhs.GetCompositeIndex(); }

public:
	KyUInt32 m_edgeOrigin; // EDGE_FROM_NAVFLOOR_XXX  or EDGE_FROM_TAGVOLUME
	PolygonWinding m_contourWinding; //CW or CCW
	KyUInt32 m_ownerIndex; // index of the contour it belong to in Navfloor or TagVolumeIndex
	NavTagIndex m_navTagIndex; // info regarding the navtag associated to this edge
	// internal state
	KyUInt32 m_index; // index of the edge, unique among edges for a particular polygon
};

struct MergerRawInputEdge
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public: 
	
	MergerRawInputEdge() :
		m_start(), 
		m_end(),
		m_property()
	{}

public: 
	Vec2i m_start;
	Vec2i m_end;
	MergerRawInputEdgeProperty m_property;
};

typedef KyArray<Vec2i> EdgeCutList;
typedef KyArray<EdgeCutList> EdgeCuts;

class RawPolygonEdgeIntersections
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	EdgeCuts m_cuts;
	KyArray<const MergerRawInputEdge*> m_edges;
};

// simple "std::pair"-style comparator
struct BasicLessHalfEdge
{
	KY_INLINE bool operator() (const MergerRawInputEdge& edge1, const MergerRawInputEdge& edge2) const
	{
		return edge1.m_start != edge2.m_start ? edge1.m_start < edge2.m_start :edge1.m_end < edge2.m_end;
	}
};

struct MergerRawInputEdgeSubSegment
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)
public:
	KY_INLINE MergerRawInputEdgeSubSegment() {}
	KY_INLINE MergerRawInputEdgeSubSegment(const Kaim::Vec2i& start, const Kaim::Vec2i& end) :
	m_start(start), m_end(end) {}

	KY_INLINE bool operator==(const MergerRawInputEdgeSubSegment& rhs) const { return m_start == rhs.m_start && m_end == rhs.m_end; }
	KY_INLINE bool operator!=(const MergerRawInputEdgeSubSegment& rhs) const { return !operator==(rhs); }

	//idx = 0 or idx = 1
	const Vec2i& operator[](KyUInt32 idx) const { return (&m_start)[idx]; }
public:
	Vec2i m_start;
	Vec2i m_end;
};

struct MergerRawInputEdgePiece
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Kaim::Stat_Default_Mem)
public:
	MergerRawInputEdgePiece(): m_parentEdge(KY_NULL), m_count(0), m_index(0) {}
	MergerRawInputEdgePiece(const MergerRawInputEdgeSubSegment& subSegment, const MergerRawInputEdge& parentEdge) :
	m_edgePiece(subSegment), m_parentEdge(&parentEdge), m_count(0), m_index(0)
	{}

	const MergerRawInputEdgeProperty& ParentProperty() const { return m_parentEdge->m_property; }
	inline bool operator==(const MergerRawInputEdgePiece& otherEdge) const
	{
		return GetEdgeStart() == otherEdge.GetEdgeStart() && GetEdgeEnd() == otherEdge.GetEdgeEnd() && GetCount() == otherEdge.GetCount();
	}
	inline bool operator!=(const MergerRawInputEdgePiece& otherEdge) const { return !((*this) == otherEdge); }
	inline bool operator<(const MergerRawInputEdgePiece& otherEdge) const
	{
		if (GetEdgeStart().x < otherEdge.GetEdgeStart().x)
			return true;
		if (GetEdgeStart().x == otherEdge.GetEdgeStart().x)
		{
			if (GetEdgeStart().y < otherEdge.GetEdgeStart().y)
				return true;
			if (GetEdgeStart().y == otherEdge.GetEdgeStart().y)
			{
				return Kaim::IntegerSegment::LessSlope<KyInt32>(GetEdgeStart().x, GetEdgeStart().y, GetEdgeEnd(), otherEdge.GetEdgeEnd());
			}
		}
		return false;
	}


	bool operator>(const MergerRawInputEdgePiece& edge) const { return edge < (*this); }
	bool operator<=(const MergerRawInputEdgePiece& edge) const { return !((*this) > edge); }
	bool operator>=(const MergerRawInputEdgePiece& edge) const { return !((*this) < edge); }
	KyFloat32 EvalAtX(KyInt32 xIn) const { return Kaim::IntegerSegment::EvalAtXforY_float(xIn, GetEdgeStart(), GetEdgeEnd()); }
	bool IsVertical() const
	{
		return GetEdgeStart().x == GetEdgeEnd().x;
	}

	const Vec2i& GetEdgeStart() const { return m_edgePiece.m_start; }
	const Vec2i& GetEdgeEnd() const { return m_edgePiece.m_end; }
	KyInt32 GetCount() const { return m_count; }
	const Vec2i& GetOriginalStart() const 
	{
		const KyInt32 swappedPropertyCount = Isel(m_edgePiece.m_start.x - m_edgePiece.m_end.x, -m_count , m_count);
		const KyInt32 extremityIdx = Isel(swappedPropertyCount, 1, 0); // 0 for start, 1 for end
		return m_edgePiece[extremityIdx]; 
	}
	const Vec2i& GetOriginalEnd() const 
	{
		const KyInt32 swappedPropertyCount = Isel(m_edgePiece.m_start.x - m_edgePiece.m_end.x, -m_count , m_count);
		const KyInt32 extremityIdx = Isel(swappedPropertyCount, 0, 1); // 0 for start, 1 for end
		return m_edgePiece[extremityIdx];
	}
public:
	MergerRawInputEdgeSubSegment m_edgePiece;
	const MergerRawInputEdge* m_parentEdge;
	KyInt32 m_count;
	KyUInt32 m_index;
};

class MergerRawInputEdgePieceComparatorAtX /*: public std::binary_function<NavFloorSweeplineHalfEdge, NavFloorSweeplineHalfEdge, bool> */
{
public:
	inline MergerRawInputEdgePieceComparatorAtX() : m_x(KY_NULL), m_justBefore(KY_NULL){}
	inline MergerRawInputEdgePieceComparatorAtX(KyInt32* x, KyInt32 *justBefore) : m_x(x), m_justBefore(justBefore){}

	inline bool operator () (const MergerRawInputEdgeSubSegment& elm1, const MergerRawInputEdgeSubSegment& elm2) const
	{
		assert(elm1.m_start.x <= *m_x && elm1.m_end.x >= *m_x);
		assert(elm2.m_start.x <= *m_x && elm2.m_end.x >= *m_x);

		if ((Kaim::Max)(elm1.m_start.y, elm1.m_end.y) < (Kaim::Min)(elm2.m_start.y, elm2.m_end.y))
			return true;
		if ((Kaim::Min)(elm1.m_start.y, elm1.m_end.y) > (Kaim::Max)(elm2.m_start.y, elm2.m_end.y))
			return false;

		//check if either x of elem1 is equal to x_
		KyInt32 localx = *m_x;
		KyInt32 elm1y = 0;
		bool elm1_at_x = false;
		if (localx == elm1.m_start.x)
		{
			elm1_at_x = true;
			elm1y = elm1.m_start.y;
		}
		else if (localx == elm1.m_end.x)
		{
			elm1_at_x = true;
			elm1y = elm1.m_end.y;
		}
		KyInt32 elm2y = 0;
		bool elm2_at_x = false;
		if (localx == elm2.m_start.x)
		{
			elm2_at_x = true;
			elm2y = elm2.m_start.y;
		}
		else if (localx == elm2.m_end.x)
		{
			elm2_at_x = true;
			elm2y = elm2.m_end.y;
		}
		bool retval = false;
		if (!(elm1_at_x && elm2_at_x))
		{
			//at least one of the segments doesn't have an end point a the current x
			//-1 below, 1 above
			int pt1_oab = Kaim::IntegerSegment::OnAboveOrBelow<KyInt64>(elm1.m_start, elm2.m_start, elm2.m_end);
			int pt2_oab = Kaim::IntegerSegment::OnAboveOrBelow<KyInt64>(elm1.m_end, elm2.m_start, elm2.m_end);
			if (pt1_oab == pt2_oab) {
				if (pt1_oab == -1)
					retval = true; //pt1 is below elm2 so elm1 is below elm2
			}
			else
			{
				//the segments can't cross so elm2 is on whatever side of elm1 that one of its ends is
				int pt3_oab = Kaim::IntegerSegment::OnAboveOrBelow<KyInt64>(elm2.m_start, elm1.m_start, elm1.m_end);
				if (pt3_oab == 1)
					retval = true; //elm1's point is above elm1
			}
		}
		else
		{
			if (elm1y < elm2y)
			{
				retval = true;
			}
			else if (elm1y == elm2y)
			{
				if (elm1 == elm2)
				{
					return false;
				}
				retval = Kaim::IntegerSegment::LessSlopeUnit<KyInt64>((KyInt64)elm1.m_end.x - elm1.m_start.x,
					(KyInt64)elm1.m_end.y - elm1.m_start.y,
					(KyInt64)elm2.m_end.x - elm2.m_start.x,
					(KyInt64)elm2.m_end.y - elm2.m_start.y);
				retval = ((*m_justBefore) != 0) ^ retval;
			}
		}
		return retval;
	}
private:
	KyInt32* m_x; //x value at which to apply comparison
	KyInt32* m_justBefore;
};


struct MergerRawInputEdgePieceSorter
{
public:
	bool operator() (const MergerRawInputEdgePiece& i1, const MergerRawInputEdgePiece& i2) const
	{
		if (i1.m_edgePiece.m_start != i2.m_edgePiece.m_start)
			return i1.m_edgePiece.m_start < i2.m_edgePiece.m_start;
		KyInt32 x = i1.m_edgePiece.m_start.x;
		KyInt32 just_before = 0;
		MergerRawInputEdgePieceComparatorAtX lhe(&x, &just_before);
		return lhe(i1.m_edgePiece, i2.m_edgePiece);
	}
};

class MergedPolygon
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	MergedPolygon() {}
public:
	KyArray<Vec2i> m_points;
	KyArray<const MergerRawInputEdgePiece*> m_edges;
};

class MergedPolygonWithHoles
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	MergedPolygonWithHoles() {}

public:
	MergedPolygon m_exterior;
	KyArray<MergedPolygon> m_holes;
};

typedef MergedPolygon TriangulatorInputPolygon;
typedef MergedPolygonWithHoles TriangulatorInputPolygonWithHoles;

class TriangulatorOutput
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	TriangulatorInputPolygon m_referencePolygon;
	KyArray<KyArray<const MergerRawInputEdgePiece*> > m_edgesAtPoint;
	KyArray<KyFloat32> m_verticesAltitudes;
	KyArray<KyUInt32> m_triangleVertices;
};

class TagVolumeMergerQueryStatistics
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	TagVolumeMergerQueryStatistics():
		m_edgeIntersectTime(0.0f), 
		m_sweeplineTime(0.0f),
		m_polygonBuildingTime(0.0f),
		m_triangulationTime(0.0f),
		m_altitudeComputationTime(0.0f),
		m_totalQueryTime(0.0f), 
		m_inputEdgeCount(0){}

public: 
	TagVolumeMergerQueryStatistics& operator+=(const TagVolumeMergerQueryStatistics& rhs) 
	{
		m_edgeIntersectTime += rhs.m_edgeIntersectTime;
		m_sweeplineTime += rhs.m_sweeplineTime;
		m_polygonBuildingTime += rhs.m_polygonBuildingTime;
		m_triangulationTime += rhs.m_triangulationTime;
		m_altitudeComputationTime += rhs.m_altitudeComputationTime;
		m_totalQueryTime += rhs.m_totalQueryTime;
		m_inputEdgeCount += rhs.m_inputEdgeCount;
		return *this;
	} 

	bool operator<(const TagVolumeMergerQueryStatistics& other)
	{
		return m_totalQueryTime < other.m_totalQueryTime;
	}

	void PrintStats() const
	{
		KY_LOG_MESSAGE((" --- Tag Volume Merger Query Stats ---"));
		KY_LOG_MESSAGE((" edgeIntersectTime : %.2f ms (%.1f %%)", m_edgeIntersectTime, (m_edgeIntersectTime/m_totalQueryTime * 100.0f)));
		KY_LOG_MESSAGE((" sweeplineTime : %.2f ms (%.1f %%)", m_sweeplineTime , (m_sweeplineTime/m_totalQueryTime * 100.0f)));
		KY_LOG_MESSAGE((" polygonBuildingTime : %.2f ms (%.1f %%)", m_polygonBuildingTime, (m_polygonBuildingTime/m_totalQueryTime * 100.0f)));
		KY_LOG_MESSAGE((" triangulationTime : %.2f ms (%.1f %%)", m_triangulationTime, (m_triangulationTime/m_totalQueryTime * 100.0f)));
		KY_LOG_MESSAGE((" altitudeComputationTime : %.2f ms (%.1f %%)", m_altitudeComputationTime, (m_altitudeComputationTime/m_totalQueryTime * 100.0f)));
		KY_LOG_MESSAGE((" totalQueryTime : %.2f ms", m_totalQueryTime));
		KY_LOG_MESSAGE((" number of input edges: %i", m_inputEdgeCount));
	}

	void PrintTotalTime() const
	{
		KY_LOG_MESSAGE((" totalQueryTime : %.2f ms", m_totalQueryTime));
	}

public: 
	KyFloat32 m_edgeIntersectTime;
	KyFloat32 m_sweeplineTime;
	KyFloat32 m_polygonBuildingTime;
	KyFloat32 m_triangulationTime;
	KyFloat32 m_altitudeComputationTime;
	KyFloat32 m_totalQueryTime;
	KyUInt32 m_inputEdgeCount;

};
} // namespace Kaim



#endif // KAIM_TAG_VOLUME_MERGER_TYPES_H
