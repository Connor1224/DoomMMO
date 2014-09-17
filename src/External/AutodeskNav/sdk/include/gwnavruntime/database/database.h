/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: JUBA - secondary contact: LAPA

#ifndef Navigation_Database_H
#define Navigation_Database_H

#include "gwnavruntime/world/worldelement.h"
#include "gwnavruntime/database/databasevisualrepresentation.h"
#include "gwnavruntime/database/databasegenmetrics.h"
#include "gwnavruntime/database/ieventbroadcaster.h"
#include "gwnavruntime/containers/collection.h"
#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/database/positionspatializationrange.h"
#include "gwnavruntime/kernel/SF_Std.h"


namespace Kaim
{

class NavMeshElementManager;
class NavGraphManager;
class WorkingMemory;
class ActiveData;
class NavCellGrid;
class NavData;
class Bot;

/// This class is a runtime container for all NavData that represents the world from the point of view of a specific type of Bots.
///  Databases are created once and for all during the World construction.
/// Through some sub-classes (NavMeshElementManager, NavGraphManager, NavCellGrid, ...), the Database performs the additions and removals
///  of NavData, maintaining a coherent state of "active" navigable data on which Bots are moving, Queries are performed, in term
///  of overlaps, dynamic version of NavFloors, stitching of the NavCells, etc...
/// The Database also maintains an instance of WorkingMemory, which is used by default by the Queries that are not processed within
///  a QueryQueue. 
class Database : public WorldElement
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_WorldFwk)
	KY_CLASS_WITHOUT_COPY(Database)

public:
	static  WorldElementType GetStaticType()       { return TypeDatabase; }
	virtual WorldElementType GetType()       const { return TypeDatabase; }
	virtual const char*      GetTypeName()   const { return "Database";   }
	virtual void DoSendVisualDebug(VisualDebugServer& visualDebugServer, VisualDebugSendChangeEvent changeEvent); 

	Database(World* world, KyUInt32 databaseIndex);
	virtual ~Database();

	void Clear(); // internal. RemoveAll navData, bots, clear memory of cell grid and activeData

	// ---------------------------------- Public Member Functions ----------------------------------

	/// Retrieves the index of the Database in the array of Databases maintained by the World.
	KyUInt32 GetDatabaseIndex() const;

	/// Retrieves the DatabaseGenMetrics, which offers services for converting between floating-point coordinates
	///  and the integer coordinate system used internally by the NavData.
	const DatabaseGenMetrics& GetDatabaseGenMetrics() const;

	DatabasePositionSpatializationRange&       GetDatabasePositionSpatializationRange();
	const DatabasePositionSpatializationRange& GetDatabasePositionSpatializationRange() const;
	const PositionSpatializationRange&         GetPositionSpatializationRange()         const;

	/// Returns true if the Database is not associated to any set of generation parameters. It false is returned, it means that
	///  only NavData compatible with current generation parameters can be added. In this case, you can use the function
	///  IsCompatibleWith to check that.
	/// Note that Databases are cleared only during a call to World::RemoveAndCancellAll()
	bool IsClear() const;

	/// Indicates whether or not the specified NavData object was created with the same generation parameters
	/// as the NavData objects that have already been loaded into this Database so that it can be added too.
	bool IsCompatibleWith(const NavData& navData) const;

	/// Remove all the NavData from this Database.
	/// \pre This function cannot be called if the Database in updating
	void RemoveAllNavData();
	KyUInt32 GetAliveNavDataCount() const;

	void RemoveAllBots();
	KyUInt32 GetBotsCount() const;
	Bot* GetBot(KyUInt32 index) const;

	/// The WorkingMemory used by default by Queries when processed outside a QueryQueue. You should not need
	//  to access this explicitely except for modifying the memory size limit of this WorkingMemory.
	WorkingMemory* GetWorkingMemory() const;

	/// Sends to the IVisualGeometry a set of triangles that express the data maintained by this object for rendering.
	void ComputeVisualGeometry();

public: // internal
	void SetEventBroadcaster(IEventBroadCaster* eventBroadCaster);
	void UpdateWithDynamicNavFloors(const CellBox& enlargeUpdateCellBox);

	ActiveData* GetActiveData() const;

	void UpdateImmediate();

	/// Sets the instance of IVisualGeometry to which this object will send its triangles for rendering when
	/// ComputeVisualGeometry() is called. 
	void SetVisualGeometry(IVisualGeometry* geometry);

	/// Retrieves the instance of IVisualGeometry to which this object sends its triangles for rendering,
	/// or KY_NULL if none has been set. 
	IVisualGeometry* GetVisualGeometry() const;

	/// Retrieves the instance of the ActiveDataVisualRepresentation maintained by this object, which describes
	/// the way its data is expressed as triangles for rendering.
	DatabaseVisualRepresentation& GetVisualRepresentation();

	bool DoesDatabaseNeedToBeUpdated() const; // call by World::Update
	bool IsDatabaseUpdatingNavData() const; // call by World::Update

	enum UpdateMode { TimeSlicedUpdate, ImmediateUpdate };
	void TakeSnapshotAndPrapareActiveDataUpdate(UpdateMode updateMode = TimeSlicedUpdate);
	void UpdateActiveDataAndClearSnapshot(); // call by World::Update
	void UpdateBotsPathFollowing(KyFloat32 simulationTimeInSeconds); // call by World::Update

	void SetInGameRenderNode(IRenderNode* renderNode);

private:
	void IncrementNavDataChangeIndex();

	void RemoveNavGraphsOfNavDataToBeRemoved();
	void AddNavGraphsOfNavDataToBeAdded();

	void SnapShotOfNavDataToUpdate();
	void ClearSnapShotOfNavDataToUpdate();

	void PrepareNavMeshRemovalOfNavDataBeeingRemoved();
	void PrepareNavMeshInsertionOfNavDataToBeeingAdded();

	void RemoveNavMeshOfNavDataToBeeingRemoved();
	void UpdateCellActiveNessStatus(UpdateMode updateMode);
	void UpdateActiveData();

	void ForceSendAllDynamicFloorsAtCellPos(VisualDebugServer &visualDebugServer);

public: // internal
	KyUInt32 m_navdataChangeIdx; // 0 in an invalid Value
	KyUInt32 m_frameIdx;

	DatabasePositionSpatializationRange m_positionSpatializationRange;
	DatabaseGenMetrics m_generationMetrics;          ///< generation parameters. Set by the NavMeshElementManager. Do Not Modify

	ActiveData*             m_activeData;            ///< all the data that are active
	NavCellGrid*            m_navCellGrid;           ///< bootkeeping, stitching, spatialization, NavCells version management, ...
	NavMeshElementManager*  m_navMeshElementManager; ///< NavMesh container + GUID manager/sorter
	NavGraphManager*        m_navGraphManager;       ///< NavGraph container
	WorkingMemory*          m_workingMemory;

	TrackedCollection<Ptr<Bot>, MemStat_WorldFwk> m_bots;

	TrackedCollection<Ptr<NavData>, MemStat_WorldFwk> m_navDataToBeAdded;
	TrackedCollection<Ptr<NavData>, MemStat_WorldFwk> m_navDataBeeingAdded;
	TrackedCollection<Ptr<NavData>, MemStat_WorldFwk> m_navDatas;
	TrackedCollection<Ptr<NavData>, MemStat_WorldFwk> m_navDataToBeRemoved;
	TrackedCollection<Ptr<NavData>, MemStat_WorldFwk> m_navDataBeeingRemoved;

	Ptr<IEventBroadCaster> m_eventBroadCaster;

	DatabaseVisualRepresentation m_databaseVisualRepresentation;
	DisplayListManager m_displayLists;   ///< We can use a displayList, and VisualDebug is handled specifically (i.e. as for the Bots in the world).
	bool m_needToSendActiveGuids;        ///< (VisualDebug) Avoids sending all the active KyGuid each frame. Accessed by the NavCellGrid.
	Collection<Ptr<BaseBlobHandler> > m_visualDebugBlobCollection; ///< (VisualDebug) store the blob ot be send during the DoSendVisualDebug call.

private:
	KyUInt32 m_databaseIndex;            // Index of the Database in the world, set once and for all in the constructor

	// internal for update
	CellBox m_currentUpdateCellBox;
	KyUInt32 m_navDataWithNavMeshElementCount;
};

KY_INLINE bool           Database::IsClear()                  const { return m_generationMetrics.IsClear(); }
KY_INLINE KyUInt32       Database::GetDatabaseIndex()         const { return m_databaseIndex;               }
KY_INLINE KyUInt32       Database::GetBotsCount()             const { return m_bots.GetCount();             }
KY_INLINE Bot*           Database::GetBot(KyUInt32 index)     const { return m_bots[index];                 }
KY_INLINE WorkingMemory* Database::GetWorkingMemory()         const { return m_workingMemory;               }
KY_INLINE ActiveData*    Database::GetActiveData()            const { return m_activeData;                  }

KY_INLINE bool     Database::DoesDatabaseNeedToBeUpdated() const { return m_navDataToBeAdded.GetCount()   + m_navDataToBeRemoved.GetCount()   != 0; }
KY_INLINE bool     Database::IsDatabaseUpdatingNavData()   const { return m_navDataBeeingAdded.GetCount() + m_navDataBeeingRemoved.GetCount() != 0; }
KY_INLINE KyUInt32 Database::GetAliveNavDataCount()        const { return m_navDatas.GetCount() + m_navDataToBeRemoved.GetCount() + m_navDataBeeingRemoved.GetCount(); }

KY_INLINE DatabaseVisualRepresentation&              Database::GetVisualRepresentation()                      { return m_databaseVisualRepresentation;                                 }
KY_INLINE IVisualGeometry*                           Database::GetVisualGeometry()                      const { return m_databaseVisualRepresentation.m_geometry;                      }
KY_INLINE const DatabaseGenMetrics&                  Database::GetDatabaseGenMetrics()                  const { return m_generationMetrics;                                            }
KY_INLINE const DatabasePositionSpatializationRange& Database::GetDatabasePositionSpatializationRange() const { return m_positionSpatializationRange;                                  }
KY_INLINE DatabasePositionSpatializationRange&       Database::GetDatabasePositionSpatializationRange()       { return m_positionSpatializationRange;                                  }
KY_INLINE const PositionSpatializationRange&         Database::GetPositionSpatializationRange()         const { return m_positionSpatializationRange.GetPositionSpatializationRange(); }

KY_INLINE void Database::SetInGameRenderNode(IRenderNode* renderNode) { m_displayLists.SetInGameRenderNode(renderNode);       }
KY_INLINE void Database::SetVisualGeometry(IVisualGeometry* geometry) { m_databaseVisualRepresentation.m_geometry = geometry; }
KY_INLINE void Database::ComputeVisualGeometry()                      { GetVisualRepresentation().ComputeGeometry();          }
KY_INLINE void Database::IncrementNavDataChangeIndex() { m_navdataChangeIdx = m_navdataChangeIdx != KyUInt32MAXVAL ? m_navdataChangeIdx + 1 : 1; }


} // namespace Kaim

#endif // Navigation_Database_H
