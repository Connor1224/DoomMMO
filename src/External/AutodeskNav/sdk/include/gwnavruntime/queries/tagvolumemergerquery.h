/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: LASI - secondary contact: NOBODY
#ifndef Navigation_TagVolumeQuery_H
#define Navigation_TagVolumeQuery_H

#include "gwnavruntime/querysystem/iquery.h"
#include "gwnavruntime/queries/utils/querycommon.h"
#include "gwnavruntime/world/tagvolume.h"
#include "gwnavruntime/navmesh/blobs/navhalfedge.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/navmesh/blobs/navfloorblob.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"
#include "gwnavruntime/querysystem/workingmemcontainers/workingmemarray.h"





namespace Kaim
{
class RawPolygonEdgeIntersections;
class TagVolumeSweepInput;
class TriangulatorOutput;
class ScopedDisplayList;
class TagVolume;
class TagVolumeSweepResult;
class TriangulatorOutput;
class DynamicNavFloor;
class EarClippingTriangulatorInputPolygonWithHoles;
class TagVolumeMergerQueryInputExtractor;

typedef Collection<Ptr<TagVolume>, MemStat_NavData> TagVolumeCollection;

enum MergerDebugDisplayListType
{
	MERGER_DEBUG_DISPLAYLIST_INPUT_TAGVOLUMES,
	MERGER_DEBUG_DISPLAYLIST_ROUND_SNAPPING,
	MERGER_DEBUG_DISPLAYLIST_EDGE_CULLING,
	MERGER_DEBUG_DISPLAYLIST_POLYGON_SCAN_INPUT,
	MERGER_DEBUG_DISPLAYLIST_POLYGON_SCAN_OUTPUT, 
	MERGER_DEBUG_DISPLAYLIST_TRIANGULATION,
	MERGER_DEBUG_DISPLAYLIST_COUNT
};

/// Enumerates the possible results of a TagVolumeMergerQuery.
enum TagVolumeMergerQueryResult
{
	TAGVOLUME_MERGE_NOT_INITIALIZED = QUERY_NOT_INITIALIZED, ///< Indicates the query has not yet been initialized. 
	TAGVOLUME_MERGE_NOT_PROCESSED   = QUERY_NOT_PROCESSED,   ///< Indicates the query has not yet been launched. 

	TAGVOLUME_MERGE_PROCESSING,                              ///< Indicates that the query is to process a NavFloor

	TAGVOLUME_MERGE_NAVFLOOR_INVALID,                        ///< Indicates that the navfloor currently being processed is no longer valid. 
	TAGVOLUME_MERGE_LACK_OF_WORKING_MEMORY,                  ///< Indicates that insufficient working memory caused the query to stop.
	TAGVOLUME_MERGE_INTERSECTION_COMPUTATION_ERROR,          ///< Indicates an error occurred while computing the intersections. 
	TAGVOLUME_MERGE_SWEEPLINE_COMPUTATION_ERROR,             ///< Indicates an error occurred while culling edges. 
	TAGVOLUME_MERGE_POLYGON_COMPUTATION_ERROR,               ///< Indicates an error occurred while building polygons from culled edges. 
	TAGVOLUME_MERGE_TRIANGULATION_COMPUTATION_ERROR,         ///< Indicates an error occurred while triangulating the newly obtained polygons. 
	TAGVOLUME_MERGE_ALTITUDE_COMPUTATION_ERROR,              ///< Indicates an error occurred while computing the altitude of the dynamic navmesh. 
	TAGVOLUME_MERGE_UNKNOWN_ERROR,                           ///< Indicates an unknown error occurred during the query processing. 

	TAGVOLUME_MERGE_DONE_SUCCESS,                            ///< Indicates the query was successfully processed.
};

/// The TagVolumeMergerQuery is used by the TagVolumeMerger to recompute a NavCell impacted by newly projected TagVolumes
/// This query can be timesliced and each step of the Query computes one of the NavFloor included in the cell.
class TagVolumeMergerQuery : public ITimeSlicedQuery
{
friend class TagVolumeMergerQueryInputExtractor;
public:
	static  QueryType GetStaticType() { return TypeTagVolumeMerger; }
	virtual QueryType GetType() const { return TypeTagVolumeMerger; }

	TagVolumeMergerQuery();
	virtual ~TagVolumeMergerQuery();

	/// Sets up an instance of this class with any required components and configuration parameters.
	/// Initializes the query with minimum input requirements, and sets all other parameters to their default value. They
	/// may be changed by calling the corresponding write accessor.
	void Initialize(Database* database, NavCell* navCell, const TagVolumeCollection* integratedTagVolumesAtCellPos, const TagVolumeCollection* newTagVolumes);

	virtual void Advance(WorkingMemory* workingMemory);
	virtual void ReleaseWorkingMemoryOnCancelDuringProcess(WorkingMemory* /*workingMemory = KY_NULL*/) { ReleaseWorkingMemory(); }
	/// Read accessor for #m_result. 
	TagVolumeMergerQueryResult GetResult() const { return m_result; };

	// ---------------------------------- Accessors ----------------------------------
	const PixelPos& GetCellOriginPixel() const { return m_cellOriginPixel; }
	 
	//debug and profile
	void SetDebugDisplayLists(KyArray<ScopedDisplayList*> displayLists) { m_debugDisplayLists = displayLists; }
	const TagVolumeMergerQueryStatistics& GetStats() const { return m_stats; }

	void SetResult(TagVolumeMergerQueryResult result) { m_result = result; };

private:
	// Main function
	KyResult ProcessCurrentNavFloor(WorkingMemory* workingMemory);
	// does the following: 
	// 0. ExtractInputDataFromNavFloor
	// 1. BreakEdgeIntersections
	// 2. TagAndCullEdges
	// 3. TriangulatePolygonWithHoles
	// 4. ComputeVerticesAltitudesFromNavFloor
	// 5. BuildNavFloorFromTriangulatorOutput

	// And here are the signatures
	
	// Core functions
	KyResult BreakEdgeIntersections();
	KyResult TagAndCullEdges(TagVolumeSweepResult& result);
	KyResult BuildPolygons(WorkingMemory* workingMemory, const TagVolumeSweepResult& taggedEdges, KyArray<KyArray<MergedPolygonWithHoles> >& polygons);
	KyResult TriangulatePolygonWithHoles(const EarClippingTriangulatorInputPolygonWithHoles& polygon, TriangulatorOutput& output);
	KyResult ComputeVerticesAltitudes(DynamicNavFloor& dynafloor, const Kaim::NavFloor& staticNavfloor);
	
	// Output build
	KyResult BuildNavFloorFromTriangulatorOutput(const KyArray<TriangulatorOutput>& input, const WorkingMemArray<KyUInt32>& polygonNavtagIndexes, DynamicNavFloor& dynafloor);
	KyResult BuildFinalBlobFromDynamicNavFloor(DynamicNavFloor& dynafloor);

	// local accessors and utilities
	KyResult CollectTagVolumesImpactingNavCell();

	ScopedDisplayList* GetDisplayList(KyUInt32 index) const
	{ 
		return m_debugDisplayLists.GetCount() > index  ? m_debugDisplayLists[index] : KY_NULL; 
	}

	void SetFinish();
	void ReleaseWorkingMemory();

public:
	/// Input Data
	NavCell* m_navCell;
	const TagVolumeCollection* m_integratedTagVolumesAtCellPos;
	const TagVolumeCollection* m_newTagVolumes;

	/// Output NavFloors with TagVolumes integrated
	KyArray<Ptr<BlobHandler<NavFloorBlob> > > m_resultNavfloorHandlers;
	
private:
	/// locals
	WorkingMemArray<TagVolume*> m_allTagVolumes;
	WorkingMemArray<TagVolume*> m_tagVolumesImpactingCurrentNavFloor;
	NavFloorIdx m_currentNavFloorIdxInProcess;

	/// PRIMARY input: set during Initialize()
	const NavFloor* m_staticNavFloor;

	// SECONDARY INPUT:
	/// extracted by the TagVolumeMergerQueryInputExtractor
	TagVolumeMergerQueryInputExtractor* m_inputExtractor;
	PixelPos m_cellOriginPixel; ///< The pixel position of the lower left corner of the navcell that contains the processed navfloor. 
	KyArray<MergerRawInputEdge> m_rawInputEdges;

	/// Computed by the edge intersector
	KyArray<MergerRawInputEdgePiece> m_intersectedInputEdges;

	TagVolumeMergerQueryResult m_result; ///< Updated during processing to indicate the result of the query. 
	KyArray<DynamicNavTag> m_navTags; ///< Aggregates navtags found in the static floors and adds them to the TagVolumes projected in this floor.
									/// first n elements (n == navfloor.m_connexNavTag.GetCount()) are the navTag from the static navfloor. 
									/// The remainer come from the TagVolumes; their index can be found in m_tagVolumesNavTagIndexes.
	KyArray<KyUInt32> m_tagVolumesNavTagIndexes; ///< Indicates where the navtag of a particular tagvolume can be found in m_navTags. size == nb of projectedTagVolumes in floor.
	
	//debug and profile
	KyArray<ScopedDisplayList*> m_debugDisplayLists;
	TagVolumeMergerQueryStatistics m_stats;
};

KY_INLINE void TagVolumeMergerQuery::SetFinish()
{
	m_processStatus = QueryDone; // TODO - CHANGE THIS!
	ReleaseWorkingMemory();
}

KY_INLINE void TagVolumeMergerQuery::ReleaseWorkingMemory()
{
	m_allTagVolumes.ReleaseWorkingMemoryBuffer();
	m_tagVolumesImpactingCurrentNavFloor.ReleaseWorkingMemoryBuffer();
}



}





#endif //Navigation_RayCanGoQuery_H

