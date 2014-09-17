/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NONE
#ifndef Navigation_Bot_H
#define Navigation_Bot_H


#include "gwnavruntime/world/worldelement.h"
#include "gwnavruntime/world/pathfollowerconfig.h"
#include "gwnavruntime/spatialization/spatializedcylinder.h"
#include "gwnavruntime/path/livepath.h"
#include "gwnavruntime/path/positiononlivepath.h"
#include "gwnavruntime/path/ipatheventlistobserver.h"
#include "gwnavruntime/pathfollower/itrajectory.h"
#include "gwnavruntime/pathfollower/basepathfollower.h"
#include "gwnavruntime/pathfollower/ipositiononpathvalidator.h"
#include "gwnavruntime/pathfollower/avoidancecomputerconfig.h"

namespace Kaim
{

class Database;
class IPathEventListObserver;
class IMovePositionOnPathPredicate;
template<class AStarCustomizer>
class AStarQuery;
class BaseAStarQuery;
class IPathFinderQuery;

/// Enumerates the possible status values of Bot target on path.
enum TargetOnPathStatus
{
	/// Target on path has not been yet initialized at all. Main reason is that
	/// no path has been set yet.
	TargetOnPathNotInitialized = 0,

	/// Target on path is initialized but not validated. This namely occurs when
	/// a new path has been set, the Target on path is at Path start but its
	/// validation is pending.
	TargetOnPathUnknownReachability,

	/// Target on path was on a valid NavData that has been streamed out (due to
	/// explicit streaming or DynamicNavMesh) and no valid NavData has been
	/// found there.
	TargetOnPathInInvalidNavData,

	/// Target on path is no more reachable in straight line. This is namely due
	/// to NavData changes, Target on path is on valid NavData but there is
	/// something in the way.
	TargetOnPathNotReachable,

	/// Target on path is reachable in straight line.
	TargetOnPathReachable
};


/// Class used to provide Bot initialization parameters.
class BotInitConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Bot)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	BotInitConfig() { SetDefaults(); }

	/// Sets all members to their default value. After calling this method, you
	/// must set at least the mandatory m_database before using this BotInitConfig
	/// to initialize a Bot.
	void SetDefaults()
	{
		m_userData = KY_NULL;
		m_database = KY_NULL;
		m_databaseBinding = KY_NULL;

		m_pathFollower = KY_NULL;
		m_targetOnPathCheckPointValidator = KY_NULL;
		m_trajectory = KY_NULL;
		m_pathEventListObserver = KY_NULL;

		m_startPosition.Set(0.0f, 0.0f, 0.0f);
		m_radius = 0.0f;
		m_height = 0.0f;
		m_desiredSpeed = 5.0f;
		m_enableAvoidance = false;
		m_enableShortcut = true;
	}


	// ---------------------------------- Public Data Members ----------------------------------

	void* m_userData;

	/// Mandatory: you must provide a valid Database on which the Bot will move.
	Database* m_database;

	/// The Databases where the Bot will be spatialized. If set to KY_NULL, the
	/// Bot will use World default DatabaseBinding that spatializes on all Databases.
	Ptr<DatabaseBinding> m_databaseBinding;

	/// The BasePathFollower the Bot will use to follow the Path. If let to KY_NULL,
	/// the Bot will use World default PathFollower based on World default NavTag
	/// Predicate.
	Ptr<BasePathFollower> m_pathFollower;

	/// The IPositionOnPathValidator the Bot will use to validate check points
	/// along its Path. If let to KY_NULL, Bot will use the default one provided by
	/// World::GetDefaultTargetOnPathCheckPointValidator().
	Ptr<IPositionOnPathValidator> m_targetOnPathCheckPointValidator;

	/// The ITrajectory the Bot will aggregate. If let to KY_NULL, Bot will
	/// instantiate a Trajectory with the world default NavTag Predicate.
	Ptr<ITrajectory> m_trajectory;

	/// The IPathEventListObserver to be associated to the Bot. If let to KY_NULL,
	/// no observer will be used.
	Ptr<IPathEventListObserver> m_pathEventListObserver;

	Vec3f m_startPosition;

	/// The Bot radius in meter. If set to 0.0f, the Bot will use m_database generation metrics radius.
	/// \defaultvalue 0.0f
	KyFloat32 m_radius;

	/// The Bot height in meter. If set to 0.0f, the Bot will use m_database generation metrics height.
	/// \defaultvalue 0.0f
	KyFloat32 m_height;

	/// The speed desired for the Bot, in meter per second.
	/// \defaultvalue 5.0f
	KyFloat32 m_desiredSpeed;

	/// The avoidance computation enabling flag.
	/// \defaultvalue false
	bool m_enableAvoidance;

	/// The target on path shortcut enabling flag.
	/// \defaultvalue true
	bool m_enableShortcut;
};


/// Class used to provide Bot new position and velocity to be integrated
/// at next update.
class BotUpdateConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Bot)

public:
	// ---------------------------------- Public Member Functions ----------------------------------

	BotUpdateConfig() { SetDefaults(); }

	/// Sets all members to their default value.
	void SetDefaults()
	{
		m_position.Set(0.0f, 0.0f, 0.0f);
		m_velocity.Set(0.0f, 0.0f, 0.0f);
	}


	// ---------------------------------- Public Data Members ----------------------------------
	Vec3f m_position;
	Vec3f m_velocity;
};


/// This class is the world element that represent an active character in
/// Gameware Navigation. It manages character spatialization and path following.
class Bot: public WorldElement
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Bot)
	KY_CLASS_WITHOUT_COPY(Bot)

public:
	static  WorldElementType GetStaticType() { return TypeBot; }
	virtual WorldElementType GetType() const { return TypeBot; }

	virtual const char* GetTypeName() const { return "Bot"; }


	// ---------------------------------- Main API Functions ----------------------------------

	Bot();
	Bot(const BotInitConfig& initConfig);

	virtual ~Bot();

	/// Initiates the Bot, but does not add it to the world.
	/// If m_database is already set, Clear() is called to clean this
	/// instance up and prepare it for re-use.
	void Init(const BotInitConfig& initConfig);

	/// Prepares this instance for destruction and / or reuse: sets all members
	/// to their default values and clears: the spatialization, the LivePath and
	/// the target on path.
	void Clear();

	/// Adds the Bot to the Database (and thus to the World). It becomes
	/// active in the simulation, it is updated, and it is taken into account by other Bots.
	/// This increments the RefCount of this instance.
	/// NOTE: this method has immediate effect: ensure to call this outside of any
	/// Gameware Navigation computation (World::Update() or threaded query
	/// computation). However, the spatialization is not updated immediately, this
	/// will be done in next World::Update().
	void AddToDatabase();

	/// Removes the Bot from the Database (and thus from the World). It is removed
	/// from the simulation, updates stop, and it is ignored by other Bots.
	/// This decrements the RefCount of this instance.
	/// NOTE: this method has immediate effect: ensure to call this outside of any
	/// Gameware Navigation computation (Kaim::World::Update or threaded query
	/// computation). The spatialization information are immediately invalidated too.
	void RemoveFromDatabase();

	/// This method has to be called regularly to update Bot position and velocity.
	/// Ideally it is called each time the corresponding character is updated in
	/// the game engine.
	/// The Bot internals are not updated immediately: the provided BotUpdateConfig
	/// is copied to be used at next World::Update to actually update the Bot internals.
	void SetNextUpdateConfig(const BotUpdateConfig& updateConfig) { m_nextUpdateConfig.m_updateConfig = updateConfig; }


	// ---------------------------------- Getters ----------------------------------

	Database*                   GetDatabase()               const;
	IPathFinderQuery*           GetPathFinderQuery();
	BasePathFollower*           GetPathFollower()           const;
	ITrajectory*                GetTrajectory()             const;

	const PositionOnLivePath&   GetTargetOnLivePath()       const;
	TargetOnPathStatus          GetTargetOnLivePathStatus() const;
	static const char*          GetTargetOnLivePathStatusDescription(TargetOnPathStatus status);

	const SpatializedCylinder&  GetSpatializedCylinder()    const;
	KyFloat32                   GetRadius()                 const;
	KyFloat32                   GetHeight()                 const;
	const Vec3f&                GetPosition()               const;
	NavTrianglePtr              GetNavTrianglePtr()         const;

	/// Returns Bot previous position. This can be used to achieve much more precise
	/// arrival detection and check point validation.
	const Vec3f&                GetPreviousPosition()       const;


	// ---------------------------------- Getters ----------------------------------

	void SetRadius(KyFloat32 radius);
	void SetHeight(KyFloat32 height);

	// ---------------------------------- Basic PathFinding functions ----------------------------------
	/// This functions are used to have the bot finding a Path very easily by only providing your AstarQuery
	///  and then the destination without going into the details of the different available Pathfinder queries
	///  and all their parameters. As an helper, they are typically  used during your first integration steps.

	/// Set an AStarQuery with your own AStarCustomizer to the Bot. This query will be used
	/// each time you call ComputeNewPathToDestination(const Vec3f& destPos).
	template<class AStarCustomizer>
	void SetAStarQuery(AStarQuery<AStarCustomizer>* astarQuery);

	/// Launches an async path computation from the Bot position to #destPos. If the Bot is already computing
	///  a Path, this computation is canceled, and a we try to launch a new path computation with the AStarQuery.
	/// if the AStarQuery cannot be initialized because of the Query Framework (We may have to wait for the next World::Update()
	///  for the query to be flushed from the QueryQueue), this function returns KY_ERROR. In this case, you could try to call it next frame.
	/// it also returns KY_ERROR if you do not have set a valid AStarQuery with the Bot::SetAStarQuery function.
	/// Note that this function calls internally the ComputeNewPathAsync function. If you had previously called ComputeNewPathAsync with an other
	///  IPathFinderQuery than the AStarQuery passed by calling the SetAStarQuery function, it will be canceled and replaced by the AStarQuery.
	KyResult ComputeNewPathToDestination(const Vec3f& destPos);


	// ---------------------------------- Advance PathFinding functions ----------------------------------
	/// These functions should be used for rich path finding solution. With them you can set the IPathFinder query
	///  you want (not only an AStarQuery). You have to initialize your Query by yourself before calling the ComputeNewPathAsync
	///  function and you are responsible for cheching the computation status (See IsComputingNewPath()) before calling ComputeNewPathAsync.

	/// Launches a new path computation with #pathFinderQuery.
	/// You must have initialized it, and your bot  must not be computing a path (See IsComputingNewPath()).
	/// If you want to force a recomputation while this is running, you must explicitely call CancelAsyncPathComputation() first.
	///  \pre The bot must not be computing a Path. See IsComputingNewPath() function.
	void ComputeNewPathAsync(IPathFinderQuery* pathFinderQuery);

	/// Cancel the current path computation. You must call this method each time
	/// you want to compute a new path whereas the previous path computation query
	/// has not been completed.
	void CancelAsynPathComputation();

	/// Call this method each time you want to compute a new Path. If it returns
	/// true you must call ClearFollowedPath() before queriing a new path computation
	/// (or wait for the previous query completion).
	bool IsComputingNewPath() const;

	// ---------------------------------- LivePath Management ----------------------------------

	const LivePath&             GetLivePath()             const;
	const PathEventList&        GetPathEventList()        const;
	const PathValidityInterval& GetPathValidityInterval() const;
	Path*                       GetFollowedPath()         const;
	PathSource                  GetFollowedPathSource()   const;

	bool IsPathEventListBuildCompleted() const;

	/// Clears the current LivePath and associated events.
	/// Sets the PathFollowing component controls to default: target on path and
	/// Trajectory will be computed when a new path will be provided.
	void ClearFollowedPath();

	/// Set a new Path, considered as the path to follow, at the next World::Update.
	/// Note : Your bot must not be computing a new path when this function is
	/// called. If it is, cancel the path computation before calling this function,
	/// to prevent the provided path from being overwritten by the path computation
	/// when it completes.
	/// You can pass KY_NULL to end path following, but non-null paths with 0 nodes
	/// will be rejected.
	void InjectUserPath(Path* newPath);

	/// Returns a pointer to the PositionOnPath of the PathEvent along the path
	/// after the target on path.
	/// If target on path is on a PathEvent, it returns the PositionOnPath of the
	/// next PathEvent.
	/// Note:  if target on path is at the upper bound of the validity interval,
	/// it will return KY_NULL.
	const PositionOnPath* GetUpcomingEventPositionOnPath() const;

	/// Returns a pointer to the navTag along the path after the target on path.
	/// If target on path is on a PathEvent, it returns the NavTag between this
	/// PathEvent and the next.
	/// Note: if target on path is at the upper bound of the validity interval,
	/// it will return KY_NULL.
	const NavTag* GetUpcomingEventNavTag() const;

	/// Returns a pointer to the navTag along the path after positionOnLivePath.
	/// If positionOnLivePath is on a PathEvent, it returns the NavTag between
	/// this PathEvent and the next.
	/// Note: if positionOnLivePath is at the upper bound of the validity interval,
	/// it will return KY_NULL.
	const NavTag* GetUpcomingEventNavTagFromPositionOnPath(const PositionOnLivePath& positionOnLivePath) const;

	/// Returns a pointer to the navTag along the path before the target on path.
	/// If target on path is on a PathEvent, it returns the NavTag between this
	/// PathEvent and the previous.
	/// Note: if target on path is at the lower bound of the validity interval,
	/// it will return KY_NULL.
	const NavTag* GetPreviousEventNavTag() const;

	/// Returns a pointer to the navTag along the path before positionOnLivePath.
	/// If positionOnLivePath is on a PathEvent, it returns the NavTag between this
	/// PathEvent and the previous.
	/// Note: if positionOnLivePath is at the lower bound of the validity interval,
	/// it will return KY_NULL.
	const NavTag* GetPreviousEventNavTagFromPositionOnPath(const PositionOnLivePath& positionOnLivePath) const;


	// ---------------------------------- Path Following Configuration ----------------------------------

	/// Sets the PathFollowerConfig to be used by this Bot.
	/// Note that PathFollowerConfigs can be shared by several Bots.
	/// World has a default PathFollowerConfig but you can instantiate your own(s).
	void SetPathFollowerConfig(const Ptr<PathFollowerConfig>& config) { m_pathFollowerConfig = config; }

	PathFollowerConfig&      GetPathFollowerConfig();
	TargetOnPathConfig&      GetTargetOnPathConfig();
	AvoidanceComputerConfig& GetAvoidanceComputerConfig();

	const TargetOnPathConfig&      GetTargetOnPathConfig()      const;
	const AvoidanceComputerConfig& GetAvoidanceComputerConfig() const;


	// ---------------------------------- Path Following ----------------------------------

	void SetDesiredSpeed(KyFloat32 desiredSpeed);
	KyFloat32 GetDesiredSpeed() const;

	void EnableAvoidance(bool enable);
	bool IsAvoidanceEnabled() const;

	void EnableShortcut(bool enable);
	bool IsShortcutEnabled() const;

	/// When set to false, the PathFollower will no more compute the target on path
	/// nor check its validity against NavData.
	/// You can set this to false typically when the Bot enters a specific NavTag
	/// associated with a custom movement you want to control on your side.
	/// Set to true by default.
	void SetDoComputeTargetOnPath(bool doComputeTargetOnPath);

	/// When set to false, the PathFollower will no more compute the Trajectory.
	/// You can set this to false typically when the Bot enters a specific NavTag
	/// associated with a custom movement you want to control on your side.
	/// Set to true by default.
	void SetDoComputeTrajectory(bool doComputeTrajectory);

	bool DoComputeTargetOnPath() const;
	bool DoComputeTrajectory() const;

	/// The position is reached when the segment between a Bot previous and current
	/// position intersects the cylinder centered on the position with precisionRadius
	/// and database generation height.
	bool HasReachedPosition(const Vec3f& position, KyFloat32 precisionRadius);

	/// This function will move the target on path forward until the predicate returns
	/// false. If it reaches the UpperBound, it stops.
	void MoveTargetOnPathForwardToNextEvent(IMovePositionOnPathPredicate* predicate);

	/// This function will move the target on path backward until the predicate returns
	/// false. If it reaches the LowerBound, it stops.
	void MoveTargetOnPathBackwardToFirstEvent(IMovePositionOnPathPredicate* predicate);

	/// This function will move positionOnLivePath forward until the predicate returns
	/// false. If it reaches the UpperBound, it stops.
	void MovePositionOnPathForwardToNextEvent(PositionOnLivePath& positionOnLivePath, IMovePositionOnPathPredicate* predicate);

	/// This function will move positionOnLivePath backward until the predicate returns
	/// false. If it reaches the LowerBound, it stops.
	void MovePositionOnPathBackwardToFirstEvent(PositionOnLivePath& positionOnLivePath, IMovePositionOnPathPredicate* predicate);

	/// Computes the trajectory. Call this directly when you called SetDoComputeTrajectory()
	/// with false argument value but still want to use the Bot Trajectory to
	/// compute itself. This is useful namely when traversing special NavTags but
	/// still want using usual Trajectory computation namely for Bot avoidance.
	void ComputeTrajectory(KyFloat32 simulationTimeInSeconds);


public: // internal
	virtual void DoSendVisualDebug(VisualDebugServer& server, VisualDebugSendChangeEvent changeEvent); // Inherited from WorldElement

	// Bot spatialization (through its SpatializedCylinder)
	void ComputeNewSpatialization();
	void UpdateNavFloorCollections();

	// LivePath management
	LivePath& GetLivePath();
	PathEventList& GetPathEventList();
	void FlagPathAsPotentiallyDirty(); // Called in EventBroadcaster to trigger a path validity check after NavData has changed

	// Path following
	void UpdatePathFollowing(KyFloat32 simulationTimeInSeconds); // Called in World::Update() to compute the trajectory

private:
	void SetupBotExtents(const BotInitConfig& initConfig, KyFloat32& radius, KyFloat32& height);
	void SetupBotRadiusCapedToGenerationMetrics(const KyFloat32 providedRadius, const KyFloat32 databaseRadius, KyFloat32& radius);
	void SetupBotHeightCapedToGenerationMetrics(const KyFloat32 providedHeight, const KyFloat32 databaseHeight, KyFloat32& height);
	void SetupBotRadiusWithEmptyDatabase(const KyFloat32 providedRadius, KyFloat32& radius);
	void SetupBotHeightWithEmptyDatabase(const KyFloat32 providedHeight, KyFloat32& height);


public: // internal
	class InternalUpdateConfig
	{
	public:
		InternalUpdateConfig()
			: m_radius(0.0f)
			, m_height(0.0f)
		{}

		BotUpdateConfig m_updateConfig;
		KyFloat32 m_radius;
		KyFloat32 m_height;
	};

	Database* m_database;
	LivePath m_livePath;
	SpatializedCylinder m_spatializedCylinder;

	// State
	InternalUpdateConfig m_nextUpdateConfig; // set in SetNextUpdateConfig(), SetRadius() & SetHeight(); used in ComputeNewSpatialization()
	TargetOnPathStatus m_targetOnPathStatus;

	// Path following internals
	Ptr<IPathEventListObserver> m_pathEventListObserver;
	Ptr<BasePathFollower> m_pathFollower;
	Vec3f m_previousPosition; // used for arrival and checkpoint validation

	// Path following config
	Ptr<PathFollowerConfig> m_pathFollowerConfig;
	KyFloat32 m_desiredSpeed;
	bool m_avoidanceEnabled;
	bool m_shortcutEnabled;

	// Target on path internal
	bool m_doComputeTargetOnPath;
	bool m_forceTargetOnPathComputationOnNextUpdate;
	PositionOnLivePath m_targetOnLivePath;
	KyFloat32 m_nextShortcutSqDistTrigger; // for internal use

	// Trajectory internal
	bool m_doComputeTrajectory;
	bool m_forceColliderCollectionOnNextUpdate;
	Vec3f m_lastCollectorPosition;
	Ptr<QueryDynamicOutput> m_collectorOutput;
	Ptr<ITrajectory> m_trajectory;

	// VisualDebug
	Kaim::Path* m_lastPathSentToVisualDebug; // used to send LivePath visual debug only when path changes

	// pathFinding helper
	Ptr<BaseAStarQuery> m_astarQuery;
};


KY_INLINE Database*                      Bot::GetDatabase()                   const { return m_database;                                          }
KY_INLINE const LivePath&                Bot::GetLivePath()                   const { return m_livePath;                                          }
KY_INLINE const PathEventList&           Bot::GetPathEventList()              const { return m_livePath.GetPathEventList();                       }
KY_INLINE const PositionOnLivePath&      Bot::GetTargetOnLivePath()           const { return m_targetOnLivePath;                                  }
KY_INLINE BasePathFollower*              Bot::GetPathFollower()               const { return m_pathFollower;                                      }
KY_INLINE ITrajectory*                   Bot::GetTrajectory()                 const { return m_trajectory;                                        }
KY_INLINE Path*                          Bot::GetFollowedPath()               const { return m_livePath.GetFollowedPath();                        }
KY_INLINE PathSource                     Bot::GetFollowedPathSource()         const { return m_livePath.GetFollowedPathSource();                  }
KY_INLINE const PathValidityInterval&    Bot::GetPathValidityInterval()       const { return m_livePath.m_validityInterval;                       }
KY_INLINE bool                           Bot::IsComputingNewPath()            const { return m_livePath.IsComputingNewPath();                     }
KY_INLINE bool                           Bot::IsPathEventListBuildCompleted() const { return GetPathEventList().IsBuildCompleted();               }
KY_INLINE const SpatializedCylinder&     Bot::GetSpatializedCylinder()        const { return m_spatializedCylinder;                               }
KY_INLINE KyFloat32                      Bot::GetRadius()                     const { return m_spatializedCylinder.GetRadius();                   }
KY_INLINE KyFloat32                      Bot::GetHeight()                     const { return m_spatializedCylinder.GetHeight();                   }
KY_INLINE const Vec3f&                   Bot::GetPosition()                   const { return m_spatializedCylinder.GetPosition();                 }
inline    NavTrianglePtr                 Bot::GetNavTrianglePtr()             const { return m_spatializedCylinder.GetNavTrianglePtr(m_database); }
KY_INLINE const Vec3f&                   Bot::GetPreviousPosition()           const { return m_previousPosition;                                  }
KY_INLINE TargetOnPathStatus             Bot::GetTargetOnLivePathStatus()     const { return m_targetOnPathStatus;                                }
KY_INLINE const TargetOnPathConfig&      Bot::GetTargetOnPathConfig()         const { return m_pathFollowerConfig->m_targetOnPathConfig;          }
KY_INLINE const AvoidanceComputerConfig& Bot::GetAvoidanceComputerConfig()    const { return m_pathFollowerConfig->m_avoidanceComputerConfig;     }

KY_INLINE void                     Bot::SetRadius(KyFloat32 radius)   { m_nextUpdateConfig.m_radius = radius;                   }
KY_INLINE void                     Bot::SetHeight(KyFloat32 height)   { m_nextUpdateConfig.m_height = height;                   }
KY_INLINE void                     Bot::CancelAsynPathComputation()   { m_livePath.CancelAsynPathComputation();                 }
KY_INLINE void                     Bot::InjectUserPath(Path* newPath) { m_livePath.InjectUserPath(newPath);                     }
KY_INLINE void                     Bot::FlagPathAsPotentiallyDirty()  { m_livePath.ForceValidityIntervalRecompute();            }
KY_INLINE LivePath&                Bot::GetLivePath()                 { return m_livePath;                                      }
KY_INLINE PathEventList&           Bot::GetPathEventList()            { return m_livePath.GetPathEventList();                   }
KY_INLINE PathFollowerConfig&      Bot::GetPathFollowerConfig()       { return *m_pathFollowerConfig;                           }
KY_INLINE TargetOnPathConfig&      Bot::GetTargetOnPathConfig()       { return m_pathFollowerConfig->m_targetOnPathConfig;      }
KY_INLINE AvoidanceComputerConfig& Bot::GetAvoidanceComputerConfig()  { return m_pathFollowerConfig->m_avoidanceComputerConfig; }
KY_INLINE IPathFinderQuery*        Bot::GetPathFinderQuery()          { return m_livePath.GetPathFinderQuery();                 }

KY_INLINE void Bot::ComputeNewPathAsync(IPathFinderQuery* pathFinderQuery) { m_livePath.ComputeNewPathAsync(pathFinderQuery); }

template<class AStarCustomizer>
KY_INLINE void Bot::SetAStarQuery(AStarQuery<AStarCustomizer>* astarQuery) { m_astarQuery = (BaseAStarQuery*)astarQuery; }

KY_INLINE void Bot::SetDesiredSpeed(KyFloat32 desiredSpeed) { m_desiredSpeed = desiredSpeed; }
KY_INLINE KyFloat32 Bot::GetDesiredSpeed() const { return m_desiredSpeed; }

KY_INLINE void Bot::EnableAvoidance(bool enable) { m_avoidanceEnabled = enable; }
KY_INLINE bool Bot::IsAvoidanceEnabled() const { return m_avoidanceEnabled; }

KY_INLINE void Bot::EnableShortcut(bool enable) { m_shortcutEnabled = enable; }
KY_INLINE bool Bot::IsShortcutEnabled() const { return m_shortcutEnabled; }

KY_INLINE void Bot::SetDoComputeTargetOnPath(bool doComputeTargetOnPath)
{
	m_doComputeTargetOnPath = doComputeTargetOnPath;
	if (doComputeTargetOnPath)
		m_forceTargetOnPathComputationOnNextUpdate = true;
}

KY_INLINE void Bot::SetDoComputeTrajectory(bool doComputeTrajectory)
{
	m_doComputeTrajectory = doComputeTrajectory;
	if (m_doComputeTrajectory)
		m_forceColliderCollectionOnNextUpdate = true;
}

KY_INLINE bool Bot::DoComputeTargetOnPath() const { return m_doComputeTargetOnPath; }
KY_INLINE bool Bot::DoComputeTrajectory()   const { return m_doComputeTrajectory;   }

KY_INLINE void Bot::UpdateNavFloorCollections() { m_spatializedCylinder.UpdateNavFloorCollections(); }
KY_INLINE void Bot::ComputeNewSpatialization()
{
	m_spatializedCylinder.SetRadius(m_nextUpdateConfig.m_radius);
	m_spatializedCylinder.SetHeight(m_nextUpdateConfig.m_height);
	m_spatializedCylinder.UpdateSpatialization(m_nextUpdateConfig.m_updateConfig.m_position);
	m_spatializedCylinder.SetVelocity(m_nextUpdateConfig.m_updateConfig.m_velocity);
}


} // namespace Kaim

#endif // Navigation_Bot_H
