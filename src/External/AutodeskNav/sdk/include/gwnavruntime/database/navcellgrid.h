/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavcellGrid_H
#define Navigation_NavcellGrid_H

#include "gwnavruntime/containers/iterablepool.h"
#include "gwnavruntime/navgraph/identifiers/navgraphvertexptr.h"
#include "gwnavruntime/database/activedata.h"
#include "gwnavruntime/database/navfloorstitcher.h"
#include "gwnavruntime/database/activenavfloorcollection.h"
#include "gwnavruntime/database/smallptrtrackedcollection.h"


namespace Kaim
{

class NavCellBlob;
class NavCell;
class NavMeshElement;
class NavGraphManager;
class NavGraphLinker;
class ActiveGuids;
class Database;
class TagVolumeMergerQuery;
class TagVolume;

typedef Collection<NavGraphVertexRawPtr, MemStat_NavData> NavGraphVertexCollection;
typedef Collection<Ptr<TagVolume>, MemStat_NavData> TagVolumeCollection;

/// This object gather all the data located at a CellPos in the CellPos-bases grid
/// of the NavCellGrid object.
/// For Internal Use.
class NavCellPosInfo
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	NavCellPosInfo();
	~NavCellPosInfo();
	void Clear();
	void CopyFrom(NavCellPosInfo& other);

public:
	SmallPtrTrackedCollection<NavCell> m_navCellVersions;
	Pool<NavGraphVertexCollection>::Key m_navGraphVertexCollectionKey;
	Pool<TagVolumeCollection>::Key m_integratedTagVolumeCollectionKey;
	bool m_activeFloorsChanged;
};

/// This class is an internal class used to manage all the NavCell in a grid. There is 1 NavCellGrid
///  object per Database. the NavCellGrid is responsible for the spreading of NavCells coming from added
///  NavMeshElement within a CellPos-based grid, choosing which NavCell has to be activated in term of overlap, GuidCompound,
///  spatializing, by CellPos, the NavGraphVertex and the TagVolumes, updating the active data according to TagVolumeMergerQuery results, ...
/// You should not need to access and use this class directly.
class NavCellGrid
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_WorldFwk)
	KY_CLASS_WITHOUT_COPY(NavCellGrid)
public:
	NavCellGrid(Database* database);
	~NavCellGrid();

	void Clear();

	void EnlargeGrid(const CellBox& cellBox);

	// This function insert all the navCell of a NavMeshElement in the grid.
	void InsertNavMeshElement(NavMeshElement* navMeshElement);

	// This function remove all the navCell of the NavMeshElement from the grid.
	void RemoveNavMeshElement(NavMeshElement* navMeshElement);

	enum QueryProcessMode { QUERY_SYNC, QUERY_ASYNC };
	void UpdateNavCellActivenessStatusInBox(const CellBox& cellBox, QueryProcessMode queryProcessMode);
	void UpdateAndStitchActiveData(const CellBox& cellBox);

	void InsertNavGraphVertexPtrAtCellPos(const NavGraphVertexRawPtr& navGraphVertexRawPtr, const CellPos& cellPos);
	void RemoveNavGraphVertexPtrFromCellPos(const NavGraphVertexRawPtr& navGraphVertexRawPtr, const CellPos& cellPos);

	void UpdateWithDynamicNavFloors(KyArray<Ptr<TagVolumeMergerQuery> >& queries, const CellBox& enlargeUpdateCellBox);

	TagVolumeCollection* GetTagVolumeCollection(NavCellPosInfo& navCellPosInfo);
	TagVolumeCollection* CreateTagVolumeCollection(NavCellPosInfo& navCellPosInfo);

	void ProcessNavCellToRemoveOrInactivate(const CellBox& cellBox);
	void PostProcessQueries();
	void UpdateAvailableNavFloorsInNavCell(NavCell* navCell, Ptr<BlobHandler<NavFloorBlob> >* resultNavfloorHandlers, KyUInt32 resultNavfloorCount);
	void UpdateActiveNavFloorsInActiveData(const CellBox& cellBox);
	void StitchAllActiveNavFloors(const CellBox& cellBox);
private:
	void InsertNavCell(NavCell* navCell);
	void RemoveNavCell(NavCell* navCell);

	void UpdateNavCellActivenessStatusAndCreateDynamicNavmeshQueries(NavCellPosInfo& navCellPosInfo, QueryProcessMode queryProcessMode);
	void CreateQueryForDynamicNavFloors(NavCellPosInfo& navCellPosInfo, TagVolumeCollection* integratedTagVolumes, QueryProcessMode queryProcessMode);
	void SelectGuidCompoundCompatibleVersions(NavCellPosInfo& navCellPosInfo);
	void SortCompatibleVersionsAndWhichNavCellToActivate(NavCellPosInfo& navCellPosInfo);

	void TryToConnectAllUnConnectedNavGraphVertexInCell(NavCellPosInfo& navCellPosInfo);
	void DeleteNavGraphVertexCollectionOfNavCellPosInfo(NavCellPosInfo& navCellPosInfo);

	void DeleteTagVolumeCollectionOfNavCellPosInfo(NavCellPosInfo& navCellPosInfo);

	void UpdateActiveDataWithNewNavfloors(const CellBox& cellBox);

	void BuildAndStoreDynamicFloorsAtCellPosBlob(NavCell** navCells, KyUInt32 navCellCount);
public:
	Database* m_database;
	NavFloorStitcher m_floorStitcher;
	NavCellPosInfo* m_bufferOfNavCellPosInfo;

private :
	MemoryHeap* m_memoryHeapForActiveNavFloorArray;
	Pool<NavGraphVertexCollection> m_poolOfNavGraphVertexCollection;
	Pool<TagVolumeCollection> m_poolOfTagVolumeCollection;
	BitFieldMemStat<MemStat_NavData> m_stitchedCellIsDiscarded;
	KyArrayPOD<NavCell*, MemStat_NavData> m_stitchedCellVersions;
};

KY_INLINE void NavCellGrid::DeleteNavGraphVertexCollectionOfNavCellPosInfo(NavCellPosInfo& navCellPosInfo)
{
	m_poolOfNavGraphVertexCollection.Delete(navCellPosInfo.m_navGraphVertexCollectionKey);
	navCellPosInfo.m_navGraphVertexCollectionKey.Invalidate();
}

KY_INLINE TagVolumeCollection* NavCellGrid::GetTagVolumeCollection(NavCellPosInfo& navCellPosInfo)
{
	return navCellPosInfo.m_integratedTagVolumeCollectionKey.IsValid() ? &m_poolOfTagVolumeCollection.Get(navCellPosInfo.m_integratedTagVolumeCollectionKey) : KY_NULL;
}

}

#endif //Navigation_NavcellGrid_H

