/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef Navigation_TagVolumeMerger_H
#define Navigation_TagVolumeMerger_H

#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/math/box2f.h"
#include "gwnavruntime/containers/collection.h"

namespace Kaim
{

class World;
class Database;
class TagVolume;
class TagVolumeMergerQuery;
class TagVolumeMergerQueryOnDone;
typedef Collection<Ptr<TagVolume>, MemStat_NavData> TagVolumeCollection;
class NavCell;

class TagVolumeMerger
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_DynamicNavMesh)

public:
	explicit TagVolumeMerger(World* world);
	~TagVolumeMerger();

	enum Status
	{
		Idle,
		UpdatingTagVolumeIntegration,
		UpdatingNavDataInDatabases
	};


	void Update();

	void CancelUpdate();

	// call on World::RemoveAllTagvolumes
	void PerformAllTagVolumeRemovals();
public: 
	void Clear();

	void ToggleDebugRender(bool toggle);
	void RenderDisplayLists();
	void ClearDebugRender();
	
	void DebugTagVolumeInUpdateDisplayList();

	enum QueryProcessMode {QUERY_SYNC, QUERY_ASYNC};
	void CreateQuery(QueryProcessMode queryProcessMode, Database* database, NavCell* navCell, const TagVolumeCollection* integratedTagVolumesAtCellPos, const TagVolumeCollection* newTagVolumes);

	KyUInt32 GetNumberOfWaitingQueries() const { return m_tagMergerQueriesInProcess; }
	KyUInt32 GetNumberOfQueries() const { return m_totalQueryCount; }

private:
	void SnapshotOfTagVolumesToIntegrateAndToRemove();
	void MarkAllCellsConcernedByUpdate(TagVolume* tagVolume);
	void MarkAllCellsConcernedByUpdateInDatabase(Database* database, const CellBox& tagVolumeCellBox);

	void PushNewIntegratedTagVolumeInCellBox(TagVolume* tagVolume, Database* database, const CellBox& cellBoxInDatabase);
	void RemoveDeIntegratedTagVolumeFromCellBox(TagVolume* tagVolume, Database* database, const CellBox& cellBoxInDatabase);

	// return the number of queries creeated;
	void CreateTagVolumeMergerQueriesForDatabase(KyUInt32 databaseIdx, QueryProcessMode queryProcessMode = QUERY_ASYNC);
	void PostProcessTagVolumeMergerQueriesForDatabase(KyUInt32 databaseIdx);
	void PostProcessIntegratedTagVolumes();
	void PostProcessDeIntegratedTagVolumes();

public:
	World* m_world;

	Status m_status;

	KyArray<CellBox, MemStat_DynamicNavMesh> m_cellBoxOfUpdateInDatabase; // indexed on databaseIdx
	KyArray<KyArray<Ptr<TagVolumeMergerQuery>, MemStat_DynamicNavMesh>, MemStat_DynamicNavMesh> m_queriesPerDatabase; // indexed on databaseIdx
	KyUInt32 m_totalQueryCount;
	AtomicInt<int> m_tagMergerQueriesInProcess;

	Ptr<TagVolumeMergerQueryOnDone> m_OnDoneForQueries;

	TagVolumeCollection m_tagVolumesInDeIntegration;
	TagVolumeCollection m_tagVolumesInIntegration;

	// for visualDebugManager (debug)
	bool m_debugDisplayListsEnabled;
	KyArray<ScopedDisplayList*> m_debugDisplayLists;
};

} // namespace Kaim

#endif //Navigation_TagVolumeMerger_H
