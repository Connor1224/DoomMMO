/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_World_H
#define Navigation_World_H

#include "gwnavruntime/world/databasebinding.h"
#include "gwnavruntime/world/visualdebugregistry.h"
#include "gwnavruntime/world/worldstatistics.h"

#include "gwnavruntime/path/livepath.h"
#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/visualdebug/visualdebugclientregistry.h"
#include "gwnavruntime/visualdebug/visualdebugserver.h"

#include "gwnavruntime/pathfollower/basepathfollower.h"
#include "gwnavruntime/pathfollower/ipositiononpathvalidator.h"
#include "gwnavruntime/pathfollower/velocityobstacleworkspace.h"
#include "gwnavruntime/pathfollower/velocityobstaclesolver.h"
#include "gwnavruntime/querysystem/queryqueuearray.h"
#include "gwnavruntime/world/asyncquerydispatcher.h"


namespace Kaim
{

class Database;
class DatabaseBinding;
class WorldElementSpatializer;
class PointOfInterest;
class CylinderObstacle;
class BoxObstacle;
class TagVolume;
class TagVolumeMerger;
class LivePath;
class FileOpenerBase;
class VisualDebugServer;
class PathFollowerConfig;
class ITrajectory;
class IAvoidanceComputer;
class QueryQueueArray;
class QueryQueue;
class AsyncQueryDispatcher;

/// This class is a runtime container for Gameware Navigation WorldElements such
/// as NavData, Bots, BoxObstacles, TagVolumes...
class World : public RefCountBaseNTS<World, MemStat_WorldFwk>
{
	KY_CLASS_WITHOUT_COPY(World)

public:
	// ---------------------------------- Main API Functions ----------------------------------

	/// \param databaseCount The number of databases added to this world, must be greater than or equal to 1.
	explicit World(KyUInt32 databaseCount = 1);

	virtual ~World();


	// ---------------------------------- Update ----------------------------------

	/// Updates the World.
	/// It is possible to use Gameware Navigation as a NavMesh/NavGraph Query
	/// toolbox without calling World::Update(). But to easily support Bots,
	/// obstacles, DynamicNavMesh, etc., a World::Update() call is required
	/// every frame.
	void Update(KyFloat32 simulationTimeInSeconds = 0.016f);

	/// Retrieves the number of times Update() has been called.
	KyUInt32 GetFrameIndex() const { return m_frameIndex; }

	/// Returns statistics on CPU consumption during Update() and its steps.
	const WorldStatistics& GetWorldStatistics() const { return m_statistics; }


	// ---------------------------------- VisualDebug ----------------------------------

	/// Starts the visual debug server. You can start it at any time.
	KyResult StartVisualDebug(const VisualDebugServerConfig& visualDebugServerConfig);

	void StopVisualDebug();
	VisualDebugServer* GetVisualDebugServer() { return m_visualDebugServer; }

	/// Relevant and required if (displayListDestination & DisplayListSentToInGameRender)
	void SetInGameRenderNode(IRenderNode* renderNode);

	/// Get the DisplayListManager that is required when creating a ScopedDisplayList
	DisplayListManager* GetDisplayListManager() { return m_displayListManager; }

	/// Returns the registry in this World of instances of WorldElement that can be
	/// visually debugged in Gameware Navigation Lab.
	VisualDebugRegistry* GetElementRegistry() { return m_visualDebugRegistry; }


	// ---------------------------------- WorldElement Getters ----------------------------------

	KyUInt32 GetDatabasesCount() const;
	Database* GetDatabase(KyUInt32 index); ///< \pre index must be valid, we don't check it there.

	KyUInt32 GetPointsOfInterestCount() const;
	PointOfInterest* GetPointOfInterest(KyUInt32 index); ///< \pre index must be valid, we don't check it there.

	KyUInt32 GetCylinderObstaclesCount() const;
	CylinderObstacle* GetCylinderObstacle(KyUInt32 index); ///< \pre index must be valid, we don't check it there.

	KyUInt32 GetBoxObstaclesCount() const;
	BoxObstacle* GetBoxObstacle(KyUInt32 index); ///< \pre index must be valid, we don't check it there.

	KyUInt32 GetTagVolumesCount() const;
	KyUInt32 GetTagVolumesToBeIntegratedCount() const;
	KyUInt32 GetTagVolumesIntegratedCount() const;
	KyUInt32 GetTagVolumesToBeRemovedCount() const;
	TagVolume* GetTagVolumesToBeIntegrated(KyUInt32 index); ///< \pre index must be valid, we don't check it there.
	TagVolume* GetTagVolumesIntegrated(KyUInt32 index); ///< \pre index must be valid, we don't check it there.
	TagVolume* GetTagVolumesToBeRemoved(KyUInt32 index); ///< \pre index must be valid, we don't check it there.

	KyUInt32 GetQueryQueueArraysCount() const;
	QueryQueueArray* GetQueryQueueArray(KyUInt32 index); ///< \pre index must be valid, we don't check it there.

	KyUInt32 GetQueryPendingOperationCount() const;


	// ---------------------------------- Path Following Shared Components ----------------------------------

	/// By default all Bots use this BasePathFollower instance. See BotInitConfig
	/// to make your Bot use another one.
	const Ptr<BasePathFollower>& GetDefaultPathFollower() const { return m_defaultPathFollower; }

	/// By default all Bots use this IPositionOnPathValidator instance. See BotInitConfig
	/// to make your Bot use another one.
	const Ptr<IPositionOnPathValidator>& GetDefaultTargetOnPathCheckPointValidator() const { return m_defaultTargetOnPathCheckPointValidator; }

	/// By default all Bots use this PathFollowerConfig instance. You can call
	/// Bot::SetPathFollowerConfig at any time to make your Bot use another one.
	const Ptr<PathFollowerConfig>& GetDefaultPathFollowerConfig() const { return m_defaultPathFollowerConfig; }

	/// Returns the IAvoidanceComputer instance aggregated by this World. This is
	/// namely used as default IAvoidanceComputer by Trajectory instances when
	/// no IAvoidanceComputer has been specified.
	/// See Trajectory class to set a custom IAvoidanceComputer.
	const Ptr<IAvoidanceComputer>& GetDefaultAvoidanceComputer() const { return m_defaultAvoidanceComputer; }

	/// Returns the IVelocityObstacleSolver instance aggregated by this World. This is
	/// namely used as default IVelocityObstacleSolver by AvoidanceComputer instances when
	/// no IVelocityObstacleSolver has been specified.
	/// See AvoidanceComputer class to set a custom IVelocityObstacleSolver.
	const Ptr<IVelocityObstacleSolver>& GetDefaultVelocityObstacleSolver() const { return m_defaultVelocityObstacleSolver; }


	// ---------------------------------- Async Queries ----------------------------------

	/// Set a new IAsyncQueryDispatcher instance to be used by this World:
	/// - first: calls IAsyncQueryDispatcher::RemoveQueryQueuesFromWorld() on
	///   the current World's IAsyncQueryDispatcher,
	/// - then: replaces World's IAsyncQueryDispatcher,
	/// - finally: calls IAsyncQueryDispatcher::AddQueryQueuesToWorld() on the
	///   new World's IAsyncQueryDispatcher.
	/// Passing NULL does nothing.
	void SetAsyncQueryDispatcher(IAsyncQueryDispatcher* asyncQueryDispatcher);

	IAsyncQueryDispatcher* GetAsyncQueryDispatcher();

	/// Pushes an IQuery in the right QueryQueue accordingly to World's IAsyncQueryDispatcher.
	void PushAsyncQuery(IQuery* query, AsyncQueryDispatchId asyncQueryDispatchId = AsyncQueryDispatchId_Default, Bot* bot = KY_NULL);

	void CancelAsyncQuery(IQuery* query);

	void ProcessQueuesOutsideWorldUpdate();

	void SetWorldUpdateThreadId(ThreadId threadId);
	ThreadId GetWorldUpdateThreadId() const;


	// ---------------------------------- RemoveAll... ----------------------------------

	/// Remove all Bots, PointOfInterest, CylinderObstacles, BoxObstacles,
	/// TagVolumes, and NavData; and cancel all queries.
	void RemoveAndCancellAll();

	void RemoveAllBots();
	void RemoveAllPointsOfInterest();
	void RemoveAllCylinderObstacles();
	void RemoveAllBoxObstacles();
	void RemoveAllTagVolumes();
	// RemoveAllQueryQueueArrays function is not relevant.


private:
	// World::Update() steps
	void UpdateVisualDebugServer();
	void FlushCommandsInWorldUpdate();
	void ProcessQueuesInWorldUpdate();
	void FlushQueries();
	void UpdateDatabases();
	void UpdateDynamicNavMesh();
	void UpdateSpatializations();
	void UpdateBotsPathFollowing(KyFloat32 simulationTimeInSeconds);
	void FlushDisplayListManager();
	void SendVisualDebugData();
	void UpdateStatistics(KyFloat32 worldUpdateMs);
	void FlushCommandsOutOfWorldUpdate();

	// World::RemoveAndCancellAll() steps
	void CancelAllQueriesAndClearCommandsInAllQueryQueueArray();
	void ClearAllDatabases();


public: // internal

	// frameIndex
	KyUInt32 m_frameIndex;

	// databases
	KyUInt32 m_databaseCount;
	KyArray<Ptr<Database>, MemStat_WorldFwk> m_databases;
	Ptr<DatabaseBinding> m_allDatabaseBinding;

	// WorldElements
	TrackedCollection<Ptr<PointOfInterest >, MemStat_PointOfInterest>  m_pointsOfInterest;
	TrackedCollection<Ptr<CylinderObstacle>, MemStat_CylinderObstacle> m_cylinderObstacles;
	TrackedCollection<Ptr<BoxObstacle     >, MemStat_BoxObstacle>      m_boxObstacles;
	
	TrackedCollection<Ptr<QueryQueueArray> , MemStat_QuerySystem> m_queryQueueArrays;

	TrackedCollection<Ptr<TagVolume>, MemStat_TagVolume> m_tagVolumesToBeIntegrated;
	TrackedCollection<Ptr<TagVolume>, MemStat_TagVolume> m_tagVolumesIntegrated;
	TrackedCollection<Ptr<TagVolume>, MemStat_TagVolume> m_tagVolumesToBeRemoved;

	// Spatialization
	WorldElementSpatializer* m_worldElementSpatializer;

	// Default configs
	Ptr<PathFollowerConfig> m_defaultPathFollowerConfig;
	Ptr<BasePathFollower> m_defaultPathFollower;
	Ptr<IPositionOnPathValidator> m_defaultTargetOnPathCheckPointValidator;
	Ptr<IAvoidanceComputer> m_defaultAvoidanceComputer;
	Ptr<IVelocityObstacleSolver> m_defaultVelocityObstacleSolver;

	// Visual Debug
	VisualDebugServerConfig m_visualDebugServerConfig;
	VisualDebugServer* m_visualDebugServer;
	bool m_wasConnected; // to detect a connection
	DisplayListManager* m_displayListManager;
	VisualDebugRegistry* m_visualDebugRegistry;

	// WorldStatistics
	WorldStatistics m_statistics;

	// DynamicNavMesh
	TagVolumeMerger* m_tagVolumeMerger;

	// Shared VelocityObstacles workspace
	VelocityObstacleWorkspace m_velocityObstacleWorkspace;

	// Multi Threading
	Ptr<IAsyncQueryDispatcher> m_asyncQueryDispatcher;
	bool m_isInWorldUpdate;
	ThreadId m_worldUpdateThreadId;

	// internal state to synchoronize the update of the Databases. This avoid to be sure the integration state of a TagVolume is the same in 
	// all the Databases
	enum DatabasesUpdateStatus { Idle, Updating };
	DatabasesUpdateStatus m_databasesUpdateStatus;
};


KY_INLINE KyUInt32 World::GetDatabasesCount() const                    { return m_databases.GetCount(); }
KY_INLINE Database* World::GetDatabase(KyUInt32 index)                 { return m_databases[index]; }

KY_INLINE KyUInt32 World::GetPointsOfInterestCount() const             { return m_pointsOfInterest.GetCount(); }
KY_INLINE PointOfInterest* World::GetPointOfInterest(KyUInt32 index)   { return m_pointsOfInterest[index]; }

KY_INLINE KyUInt32 World::GetCylinderObstaclesCount() const            { return m_cylinderObstacles.GetCount(); }
KY_INLINE CylinderObstacle* World::GetCylinderObstacle(KyUInt32 index) { return m_cylinderObstacles[index]; }

KY_INLINE KyUInt32 World::GetBoxObstaclesCount() const                 { return m_boxObstacles.GetCount(); }
KY_INLINE BoxObstacle* World::GetBoxObstacle(KyUInt32 index)           { return m_boxObstacles[index]; }

KY_INLINE KyUInt32 World::GetTagVolumesCount() const
{
	return m_tagVolumesToBeIntegrated.GetCount() + m_tagVolumesIntegrated.GetCount() + m_tagVolumesToBeRemoved.GetCount();
}
KY_INLINE KyUInt32 World::GetTagVolumesToBeIntegratedCount() const      { return m_tagVolumesToBeIntegrated.GetCount(); }
KY_INLINE KyUInt32 World::GetTagVolumesIntegratedCount() const          { return m_tagVolumesIntegrated.GetCount(); }
KY_INLINE KyUInt32 World::GetTagVolumesToBeRemovedCount() const         { return m_tagVolumesToBeRemoved.GetCount(); }
KY_INLINE TagVolume* World::GetTagVolumesToBeIntegrated(KyUInt32 index) { return m_tagVolumesToBeIntegrated[index]; }
KY_INLINE TagVolume* World::GetTagVolumesIntegrated(KyUInt32 index)     { return m_tagVolumesIntegrated[index]; }
KY_INLINE TagVolume* World::GetTagVolumesToBeRemoved(KyUInt32 index)    { return m_tagVolumesToBeRemoved[index]; }

KY_INLINE KyUInt32 World::GetQueryQueueArraysCount() const           { return m_queryQueueArrays.GetCount(); }
KY_INLINE QueryQueueArray* World::GetQueryQueueArray(KyUInt32 index) { return m_queryQueueArrays[index]; }

KY_INLINE IAsyncQueryDispatcher* World::GetAsyncQueryDispatcher() { return m_asyncQueryDispatcher; }

KY_INLINE void World::SetWorldUpdateThreadId(ThreadId threadId) { m_worldUpdateThreadId = threadId; }
KY_INLINE ThreadId World::GetWorldUpdateThreadId() const { return m_worldUpdateThreadId; }


} // namespace Kaim

#endif
