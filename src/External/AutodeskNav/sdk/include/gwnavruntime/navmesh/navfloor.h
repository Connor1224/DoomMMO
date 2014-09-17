/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavFloor_H
#define Navigation_NavFloor_H

#include "gwnavruntime/blob/blobref.h"
#include "gwnavruntime/navmesh/blobs/flooraltituderange.h"
#include "gwnavruntime/navmesh/blobs/navfloorblob.h"
#include "gwnavruntime/containers/collection.h"

namespace Kaim
{

class NavCell;
class NavHalfEdgeRawPtr;
class NavFloorToNavGraphLinks;
class Database;
class SpatializedPoint;
class NavFloorLinkInfo;

/// This class is a runtime wrapper of a NavFloorBlob, it gathers all the runtime information associated to a NavFloor
///  such links to other NavFloors, SpatializedPoint spatialized in this NavFloor...
/// A NavFloor defines a contiguous area within a NavCell that is made up of connected, neighboring triangles
///  that share the same NavTag.
/// Different NavFloors within the same NavCell may represent separate floors in 3D that are not connected one
///  to another.
class NavFloor
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
	KY_CLASS_WITHOUT_COPY(NavFloor)

	NavFloor(NavFloorBlob* navFloorBlob);
public:
	// ---------------------------------- Public Member Functions ----------------------------------

	const NavFloorBlob* GetNavFloorBlob() const; ///< Retrieves the NavFloorBlob that contains the static data for this NavFloor. 
	const CellPos& GetCellPos() const;  ///< Returns the position of this NavFloor within the grid of NavData cells.
	bool IsStillLoaded() const; ///< This function returns false if the NavData containing corresponding NavFloorBlob has been removed from the Database.

	/// This function returns true if the NavFloor has been "activated" in term of overlap, dynamic version,... That means this NavFloor
	///  Will be used for Query process, bot pathFollowing, and all other AI components.
	bool IsActive() const;

public: //Internal
	
	const NavFloor* GetAssociatedStaticNavFloor() const; // returns the associated static NavFloor if "this" is a dynamic NavFloor. Else it just returns "this".
	bool IsStaticVersion() const;
	
	const ConnectedComponentId& GetConnectedComponentId() const;  // used in generation
	void SetConnectedComponentId(ConnectedComponentId connectedComponentId);  // used in generation
	NavHalfEdgeRawPtr* GetLinksBuffer() const ;

	bool HasAnUnconnectedLink() const;

	/// Clears all information maintained by this object. 
	void OnRemove();
	void OnDeActivate();

	NavFloorToNavGraphLinks* GetNavFloorToNavGraphLinks() const;

	class CreateConfig
	{
	public:
		CreateConfig() : m_navFloorBlob(KY_NULL), m_database(KY_NULL), m_navCell(KY_NULL), m_navFloorLinkInfo(KY_NULL) {}

		NavFloorBlob* m_navFloorBlob;
		Database* m_database;
		NavCell* m_navCell;
		CellPos m_cellPos;
		const NavFloorLinkInfo* m_navFloorLinkInfo;
	};

	static Ptr<NavFloor> Create(const CreateConfig& navFloorCongig, MemoryHeap* pheap = KY_NULL);

	static KyUInt32 GetNumberOfLinkFromBlob(const NavFloorBlob& navFloorBlob);
	static KyUInt32 ComputeSizeForNavFloor(const NavFloorBlob& navFloorBlob, const NavFloorLinkInfo* navFloorLinkInfo = KY_NULL);

	KyUInt32 GetIndexInCollection() const;
	void SetIndexInCollection(KyUInt32 indexInCollection);

	// SpatializedPoint management
	const Collection<SpatializedPoint*>& GetSpatializedPoints() const;
	void AddSpatializedPoint(SpatializedPoint* spatializedPoint);
	void RemoveSpatializedPoint(SpatializedPoint* spatializedPoint);
	void RemoveAllSpatializedPoints();


private:
	static Ptr<NavFloor> PlaceNavFloorInBuffer(const CreateConfig& navFloorCongig, char*& currentMemoryPlace);

public: // internal
	Database* m_database;
	NavCell* m_navCell;
	const NavFloorLinkInfo* m_navFloorLinkInfo;
	const NavFloorBlob* m_navFloorBlob;
	NavTag* m_navTags; // the navTag presents in the NavFloorBlob are copied in the NavFloorBlob.


	CellPos m_cellPos; // The position of this NavFloor within the grid of NavData cells. Do not modify. 
	FloorAltitudeRange m_floorAltitudeRange;

	ConnectedComponentId m_navFloorConnectedComponent; // used in generation

	Collection<SpatializedPoint*> m_spatializedPoints;

	KyUInt16 m_floorLinksOffset; // bytes offsets to NavHalfEdgeRawPtr buffer
	KyUInt16 m_graphLinksOffset; // bytes offsets to NavFloorToNavGraphLinks buffer
	KyUInt16 m_numberOfLink;
	KyUInt16 m_numberOfUnconnectedLink;
	KyUInt16 m_idxInActiveNavFloorCollection; 
	KyUInt16 m_idxInTheNavCellBuffer; 

private:
	// REFCOUTING
	mutable AtomicInt<int> RefCount;
public: // internal - Lifetime - separate implementation due to Create function
	void AddRef();
	void Release();
	// Debug method only.
	int GetRefCount() const { return RefCount; }
};

KY_INLINE const NavFloorBlob* NavFloor::GetNavFloorBlob() const
{
	KY_DEBUG_ASSERTN(m_navFloorBlob != KY_NULL,("the SizedPtr is not valid"));
	return m_navFloorBlob;
}

KY_INLINE void NavFloor::AddRef() { RefCount++; }
KY_INLINE void NavFloor::Release()
{
	if ((RefCount.ExchangeAdd_NoSync(-1) - 1) == 0)
		KY_FREE(this);
}

KY_INLINE KyUInt32 NavFloor::GetIndexInCollection() const { return m_idxInActiveNavFloorCollection; }
KY_INLINE void NavFloor::SetIndexInCollection(KyUInt32 indexInCollection) { m_idxInActiveNavFloorCollection = (KyUInt16)indexInCollection; }

KY_INLINE bool NavFloor::IsStillLoaded() const { return m_navFloorBlob != KY_NULL;                         }
KY_INLINE bool NavFloor::IsActive()      const { return m_idxInActiveNavFloorCollection != KyUInt16MAXVAL; }

KY_INLINE const CellPos&              NavFloor::GetCellPos()                 const { return m_cellPos;                                                    }
KY_INLINE const ConnectedComponentId& NavFloor::GetConnectedComponentId()    const { return m_navFloorConnectedComponent;                                 }

KY_INLINE NavHalfEdgeRawPtr*          NavFloor::GetLinksBuffer()             const { return       (NavHalfEdgeRawPtr*)((char*)this + m_floorLinksOffset); }
KY_INLINE NavFloorToNavGraphLinks*    NavFloor::GetNavFloorToNavGraphLinks() const { return (NavFloorToNavGraphLinks*)((char*)this + m_graphLinksOffset); }

KY_INLINE bool NavFloor::HasAnUnconnectedLink() const { return m_numberOfUnconnectedLink != 0; }

KY_INLINE void NavFloor::SetConnectedComponentId(ConnectedComponentId connectedComponentId) { m_navFloorConnectedComponent = connectedComponentId; }

KY_INLINE const Collection<SpatializedPoint*>& NavFloor::GetSpatializedPoints() const { return m_spatializedPoints; }
KY_INLINE void NavFloor::AddSpatializedPoint(SpatializedPoint* spatializedPoint)     { m_spatializedPoints.PushBack(spatializedPoint); }
KY_INLINE void NavFloor::RemoveSpatializedPoint(SpatializedPoint* spatializedPoint)  { m_spatializedPoints.RemoveFirstOccurrence(spatializedPoint); }
}

#endif //Navigation_NavFloor_H

