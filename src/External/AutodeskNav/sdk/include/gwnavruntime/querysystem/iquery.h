/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: JAPA - secondary contact: NOBODY
#ifndef Navigation_IQuery_H
#define Navigation_IQuery_H

#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_Debug.h"
#include "gwnavruntime/basesystem/logger.h"


namespace Kaim
{

class World;
class Database;
class WorkingMemory;
class BaseBlobHandler;
class QueryQueue;

/// Enumerates all the type of query.
/// If you write your own query, you have to add a corresponding QueryType here.
enum QueryType
{
	TypeRayCanGo = 0,
	TypeRayCast,
	TypeDiskCanGo,
	TypeDiskCast,
	TypeDiskCollision,
	TypeAStar,
	TypeTriangleFromPos,
	TypeMoveOnNavMesh,
	TypeSpatializedPointCollectorInAABB,
	TypeTriangleFromPosAndTriangleSeed,
	TypeNearestBorderHalfEdgeFromPos,
	TypeSegmentCastQuery,
	TypeSegmentCanGo,
	TypeDiskExpansion,
	TypeInsidePosFromOutsidePos,
	TypeMultipleFloorTrianglesFromPos,
	TypeRayCanGoOnSegment,
	TypeRayCastOnSegment,
	TypeTagVolumeMerger,
	TypeFindFirstVisiblePositionOnPath,

	QueryType_FirstCustom
};

/// Enumerates all the processing status a query can have.
enum QueryProcessStatus
{
	QueryNotStarted = 0,
	QueryInProcess,
	QueryDone,
	QueryCanceled,
	QueryResultMax,
};

/// Enumerates the different states a query may have in a QueryQueue.
enum QueryStatusInQueue
{
	QueryNotInQueue,
	QueryPushedAsCommand,
	QueryInQueryQueue,
};

/// Small interface class that you have to derive from and set to an IQuery to
/// get the IOnDone::OnDone() function called during the World::Update() (in
/// FlushQueries() step) if the query has been completed.
/// Note that this function is not called if the query is canceled.
class IOnDone : public RefCountBase<IOnDone, MemStat_Query>
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------
	virtual ~IOnDone() {}
	virtual void OnDone() = 0;
};

/// Abstract class for all queries.
class IQuery : public RefCountBase<IQuery, MemStat_Query>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Query)
public:
	IQuery();
	virtual ~IQuery();

	// ---------------------------------- Main API Functions ----------------------------------

	/// Should be called by the derived class before trying to perform the query
	/// or to push it in a QueryQueue.
	/// It sets #m_database to the provided value and #m_processStatus to
	/// Kaim::QueryNotStarted. As some query framework components (such as QueryQueue)
	/// relies on these members, you cannot call Initialize() unless CanBeInitialized()
	/// returns true.
	void Initialize(Database* database);

	/// In the case of a query processed in a QueryQueue, this must be tested before
	/// initializing the query, to prevent modifying the query during the process.
	bool CanBeInitialized() const;

	/// Returns true if the query is currently registered in a QueryQueue.
	/// If true, you must not push it again in QueryQueue (even the same one).
	/// If false, you must not try to cancel it from the QueryQueue.
	bool IsInAQueryQueue() const;

	/// Process the query at once whether it is time-sliced or not.
	/// \param workingMemory    Sandbox memory used to perform queries. If let to KY_NULL,
	///                         the #m_database internal WorkingMemory is used.
	void PerformQueryBlocking(WorkingMemory* workingMemory = KY_NULL);

	/// Calls the OnDone function of the IOnDone object if provided.
	/// Called automatically by the QueryQueue during the World::Update()
	/// (in FlushQueries() step) when this query has been completed.
	/// Not called if this query is canceled.
	/// Note that the member #m_onDone is reset to KY_NULL at the end of this function.
	void OnDone();


	// ---------------------------------- Pure virtual functions - Query framework contract ----------------------------------

	virtual QueryType GetType() const = 0;

	/// This function is called by the QueryQueue to process one step on the query.
	/// It MUST NEVER, under any circumstances, take too long. As a guideline, consider 0.1ms as too long.
	/// It is under the responsibility of this function to update #m_processStatus. Obviously, if the
	/// query is atomic (i.e. not time-sliced), a call to Advance() will fully perform the query and set
	/// #m_processStatus to QueryDone. If the query is time-sliced, several calls to Advance() will be needed
	/// to complete the query; the first call should set #m_processStatus to Kaim::QueryInProcess and the last one
	/// to Kaim::QueryDone.
	/// \param workingMemory    Sandbox memory used to perform queries.
	virtual void Advance(WorkingMemory* workingMemory) = 0;


	// ---------------------------------- Internal virtual functions for Visual Debug and NavigationLab Remote Query processing ----------------------------------

	virtual void BuildQueryBlob(BaseBlobHandler* /*blobHandler*/)    {}
	virtual void InitFromQueryBlob(World* /*world*/, void* /*blob*/) {}
	virtual BaseBlobHandler* CreateQueryBlobHandler() { return KY_NULL; }

public:
	// ---------------------------------- Public Data Members ----------------------------------

	QueryProcessStatus m_processStatus; ///< Internal use. Modified by the query within Advance().
	Ptr<IOnDone> m_onDone;              ///< The optional IOnDone instance to be called. Must be set manually by the user. See IOnDone.

public: // Internal
	Database* m_database;               ///< The database on which the query will be performed. Set in Initialize(), do not directly modify.
	QueryQueue* m_queue;                ///< Updated by the QueryQueue. Do not modify.
	QueryStatusInQueue m_inQueueStatus; ///< Used by the QueryQueue. Do not modify.
	KyUInt32 m_queryInfoId;
};


/// Base class for all the queries that do not need to be time-sliced.
class IAtomicQuery : public IQuery
{
public:
	virtual ~IAtomicQuery() {}
};


/// Base class for all the queries that need to be time-sliced.
class ITimeSlicedQuery : public IQuery
{
public:
	ITimeSlicedQuery();
	virtual ~ITimeSlicedQuery();

	// ---------------------------------- Public Member Functions ----------------------------------

	/// Should be called by the derived class before trying to perform the query
	/// or to push it in a QueryQueue.
	/// This function calls IQuery::Initialize().
	void Initialize(Database* database);


	// ---------------------------------- Pure virtual functions  ----------------------------------

	/// Called when a query is canceled in FlushCommands while its status is QueryInProcess
	/// to make sure that the WorkingMemory is left in valid state (typically, no
	/// WorkingMemory buffer should be in used after this call).
	virtual void ReleaseWorkingMemoryOnCancelDuringProcess(WorkingMemory* workingMemory) = 0;

public:
	KyUInt32 m_advanceCount;
	KyUInt32 m_lastAdvanceFrameIdx;
};

/// Enumerates the result codes that are common to all queries.
enum CommonQueryResult
{
	QUERY_NOT_INITIALIZED = 0, ///< Indicates that the query has not yet been initialized. 
	QUERY_NOT_PROCESSED = 1    ///< Indicates that the query has not yet been launched. 
};



KY_INLINE IQuery::IQuery() :
	m_processStatus(QueryNotStarted),
	m_database(KY_NULL),
	m_queue(KY_NULL),
	m_inQueueStatus(QueryNotInQueue),
	m_queryInfoId(0)
{}

KY_INLINE IQuery::~IQuery() {}

KY_INLINE void IQuery::Initialize(Database* database)
{
	KY_ASSERT(database != KY_NULL);
	KY_LOG_ERROR_IF(CanBeInitialized() == false, ("You cannot initialized a query while it is registered in a QueryQueue."));
	m_database = database;
	m_processStatus = QueryNotStarted;
}

KY_INLINE bool IQuery::CanBeInitialized() const { return m_inQueueStatus != QueryInQueryQueue; }
KY_INLINE bool IQuery::IsInAQueryQueue()  const { return m_inQueueStatus != QueryNotInQueue;   }

KY_INLINE void IQuery::OnDone()
{
	if (m_onDone != KY_NULL)
		m_onDone->OnDone();

	m_onDone = KY_NULL;
}

KY_INLINE ITimeSlicedQuery::ITimeSlicedQuery() : m_advanceCount(0), m_lastAdvanceFrameIdx(0) {}
KY_INLINE ITimeSlicedQuery::~ITimeSlicedQuery() {}
KY_INLINE void ITimeSlicedQuery::Initialize(Database* database)
{
	KY_ASSERT(m_processStatus != QueryInProcess);
	IQuery::Initialize(database);
	m_advanceCount = 0;
	m_lastAdvanceFrameIdx = 0;
}


}

#endif
