/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY

#ifndef Navigation_WorldStatistics_H
#define Navigation_WorldStatistics_H

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/math/fastmath.h"
#include "gwnavruntime/world/floatstat.h"

namespace Kaim
{

class World;


enum WorldUpdateStep
{
	WorldUpdateStep_WorldUpdate                       = 0,
	WorldUpdateStep_WorldUpdateAllButVisualDebugServer,
	WorldUpdateStep_UpdateVisualDebugServer           ,
	WorldUpdateStep_ProcessQueuesInWorldUpdate        ,
	WorldUpdateStep_FlushQueries                      ,
	WorldUpdateStep_UpdateDatabases                   ,
	WorldUpdateStep_UpdateDynamicNavMesh              ,
	WorldUpdateStep_UpdateSpatializations             ,
	WorldUpdateStep_UpdateBotsPathFollowing           ,
	WorldUpdateStep_FlushDisplayListManager           ,
	WorldUpdateStep_SendVisualDebugData               ,

	WorldUpdateStep_QueryQueueProcess                 ,

	WorldUpdateStepIndex_Count
};

class WorldStatistics
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_WorldFwk)

public:
	WorldStatistics()
		: m_world(KY_NULL)
		, m_navdataCount(0)
		, m_botsCount(0)
		, m_boxObstaclesCount(0)
		, m_cylinderObstaclesCount(0)
		, m_pointsOfInterestCount(0)
		, m_tagVolumesToBeIntegratedCount(0)
		, m_tagVolumesIntegratedCount(0)
		, m_tagVolumesToBeRemovedCount(0)
	{}

	void Initialize(World* world);

	const FloatStat& GetWorldUpdateStepStat(WorldUpdateStep worldStat) const { return m_worldUpdateStepStats[worldStat]; }


	void Update();
	void ResetStatValues();

	void SendVisualDebug();

	KyFloat32* GetWorldUpdateStepTimeToUpdate(WorldUpdateStep worldStat);

	void SendSummaryBlob();
	void SendProfilingStatsBlob();
	void SendCountersBlob();
	void SendMemoryStatBlob();

	void Report(StringBuffer* report) const;

public:
	World* m_world;

	KyArray<FloatStat> m_worldUpdateStepStats;

	KyUInt32 m_navdataCount; // total in all databases
	KyUInt32 m_botsCount; 
	KyUInt32 m_boxObstaclesCount;
	KyUInt32 m_cylinderObstaclesCount;
	KyUInt32 m_pointsOfInterestCount;
	KyUInt32 m_tagVolumesToBeIntegratedCount;
	KyUInt32 m_tagVolumesIntegratedCount;
	KyUInt32 m_tagVolumesToBeRemovedCount;
};

// to use only in World::Update() "sub functions"
// we may divide the number of Timer calls in WorldUpdate by 2 
#define KY_PROFILE_WORLD_UPDATE_STEP(x) \
ScopedProfilerMs scopedProfilerMs(this->m_statistics.GetWorldUpdateStepTimeToUpdate(WorldUpdateStep_##x)); \
KY_SCOPED_PERF_MARKER("World::" #x);


} // namespace Kaim

#endif

