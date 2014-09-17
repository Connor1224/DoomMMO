/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_DynamicFloorsAtCellPosBlob_H
#define Navigation_DynamicFloorsAtCellPosBlob_H

#include "gwnavruntime/base/guidcompoundblobbuilder.h"
#include "gwnavruntime/navmesh/navcell.h"
#include "gwnavruntime/world/runtimeblobcategory.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/database/database.h"

namespace Kaim
{

/// TODO
class DynamicNavFloorsInCellBlob
{
	KY_CLASS_WITHOUT_COPY(DynamicNavFloorsInCellBlob)
public:
	DynamicNavFloorsInCellBlob() {}

public:
	GuidCompound m_navMeshGuidCompound;
	NavCellIdx m_cellIdxInNavMesh;
	BlobArray<BlobRef<NavFloorBlob> > m_floors;
};

/// Swaps the endianness of the data in the specified object. This function allows this type of object to be serialized through the blob framework.
/// The first parameter specifies the target endianness, and the second parameter provides the object whose data is to be swapped.
inline void SwapEndianness(Endianness::Target e, DynamicNavFloorsInCellBlob& self)
{
	SwapEndianness(e, self.m_navMeshGuidCompound);
	SwapEndianness(e, self.m_cellIdxInNavMesh);
	SwapEndianness(e, self.m_floors);
}


class DynamicNavFloorsBlobBuilder : public BaseBlobBuilder<DynamicNavFloorsInCellBlob>
{
public:
	DynamicNavFloorsBlobBuilder(NavCell* navCell) : m_navCell(navCell) {}

private:
	virtual void DoBuild()
	{
		const GuidCompound& navMeshGuidCompound = m_navCell->GetGuidCompound();
		GuidCompoundBlobBuilder guidCompoundBuilder(navMeshGuidCompound.m_guids.GetValues(), navMeshGuidCompound.m_guids.GetCount(), navMeshGuidCompound.m_timeStamp);
		BLOB_BUILD(m_blob->m_navMeshGuidCompound, guidCompoundBuilder);

		BLOB_SET(m_blob->m_cellIdxInNavMesh, m_navCell->m_navCellIdxInNavMesh);
		const KyUInt32 numberOfFloors = m_navCell->m_numberOfFloors;
		BlobRef<NavFloorBlob>* floors = BLOB_ARRAY(m_blob->m_floors, numberOfFloors);
		
		for (KyUInt32 idx = 0; idx < numberOfFloors; ++idx)
		{
			const Ptr<BaseBlobHandler>& blobHandler = m_navCell->m_blobHandlerOfDynamicNavFloor[idx];
			if (blobHandler != KY_NULL)
			{
				COPY_REFERENCED_BLOB(floors[idx], blobHandler->VoidBlob(), blobHandler->GetShallowBlobSize(), blobHandler->GetDeepBlobSize());
			}
		}
	}

private:
	NavCell* m_navCell;
};


/// Creates a DynamicNavFloorsInCellBlob to remove in a database all dynamic navcells registered in another DynamicNavFloorsInCellBlob.
/// INTERNAL: Used for the visualDebugging of dynamic NavMesh in Lab.
/// Implementation: Creates a DynamicNavFloorsInCellBlob from another one, and forces all navcells to be empty.
class DynamicNavFloorsRemoverBlobBuilder : public BaseBlobBuilder<DynamicNavFloorsInCellBlob>
{
public:
	DynamicNavFloorsRemoverBlobBuilder(const DynamicNavFloorsInCellBlob* dynamicNavFloorsToRemove) : m_dynamicNavFloorsToRemove(dynamicNavFloorsToRemove) {}

private:
	virtual void DoBuild()
	{
		const GuidCompound& navMeshGuidCompound = m_dynamicNavFloorsToRemove->m_navMeshGuidCompound;
		GuidCompoundBlobBuilder guidCompoundBuilder(navMeshGuidCompound.m_guids.GetValues(), navMeshGuidCompound.m_guids.GetCount(), navMeshGuidCompound.m_timeStamp);
		BLOB_BUILD(m_blob->m_navMeshGuidCompound, guidCompoundBuilder);

		BLOB_SET(m_blob->m_cellIdxInNavMesh, m_dynamicNavFloorsToRemove->m_cellIdxInNavMesh);

		const KyUInt32 numberOfFloors = m_dynamicNavFloorsToRemove->m_floors.GetCount();
		BLOB_ARRAY(m_blob->m_floors, numberOfFloors);
	}

private:
	const DynamicNavFloorsInCellBlob* m_dynamicNavFloorsToRemove;
};


/// The NavCellBlob contains the static data for a NavCell: a single cell within the grid of cells maintained by a NavMesh.  
class DynamicFloorsAtCellPosBlob
{
	KY_ROOT_BLOB_CLASS(Runtime, DynamicFloorsAtCellPosBlob, 0)
	KY_CLASS_WITHOUT_COPY(DynamicFloorsAtCellPosBlob)

public:
	
	DynamicFloorsAtCellPosBlob() {}

public:
	CellPos m_cellPos;
	BlobArray<DynamicNavFloorsInCellBlob > m_dynamicNavFloorsInCellBlobs; 
};

/// Swaps the endianness of the data in the specified object. This function allows this type of object to be serialized through the blob framework.
/// The first parameter specifies the target endianness, and the second parameter provides the object whose data is to be swapped.
inline void SwapEndianness(Endianness::Target e, DynamicFloorsAtCellPosBlob& self)
{
	SwapEndianness(e, self.m_cellPos);
	SwapEndianness(e, self.m_dynamicNavFloorsInCellBlobs);
}


class DynamicFloorsAtCellPosBuilder : public BaseBlobBuilder<DynamicFloorsAtCellPosBlob>
{
public:
	DynamicFloorsAtCellPosBuilder(NavCell** navCells, KyUInt32 navCellCount) :
		m_navCells(navCells), m_navCellCount(navCellCount) {}

private:
	virtual void DoBuild()
	{
		KyUInt32 activeCellCount = 0;
		for (KyUInt32 idx = 0; idx < m_navCellCount; ++idx)
		{
			if (m_navCells[idx]->IsNavCellActive())
				++activeCellCount;
		}

		DynamicNavFloorsInCellBlob* dynamicNavFloorsBlob = BLOB_ARRAY(m_blob->m_dynamicNavFloorsInCellBlobs, activeCellCount);
		KyUInt32 currentActiveCellCount = 0;
		for (KyUInt32 idx = 0; idx < m_navCellCount; ++idx)
		{
			if (m_navCells[idx]->IsNavCellActive() == false)
				continue;

			DynamicNavFloorsBlobBuilder builder(m_navCells[idx]);
			BUILD_BLOB(dynamicNavFloorsBlob[currentActiveCellCount], builder);
			++currentActiveCellCount;
		}

		BLOB_SET(m_blob->m_cellPos, m_navCells[0]->GetCellPos());
	}

private:
	NavCell** m_navCells;
	KyUInt32 m_navCellCount;
};



class DynamicFloorsAtCellPosRemoverBlobBuilder : public BaseBlobBuilder<DynamicFloorsAtCellPosBlob>
{
public:
	DynamicFloorsAtCellPosRemoverBlobBuilder(const DynamicFloorsAtCellPosBlob* src) : m_src(src) {}

private:
	virtual void DoBuild()
	{
		KyUInt32 activeCellCount = m_src->m_dynamicNavFloorsInCellBlobs.GetCount();
		const DynamicNavFloorsInCellBlob* srcDynamicNavFloorsBlobArray = m_src->m_dynamicNavFloorsInCellBlobs.GetValues();
		DynamicNavFloorsInCellBlob* dynamicNavFloorsBlob = BLOB_ARRAY(m_blob->m_dynamicNavFloorsInCellBlobs, activeCellCount);
		for (KyUInt32 idx = 0; idx < activeCellCount; ++idx)
		{
			DynamicNavFloorsRemoverBlobBuilder builder(&srcDynamicNavFloorsBlobArray[idx]);
			BUILD_BLOB(dynamicNavFloorsBlob[idx], builder);
		}

		BLOB_SET(m_blob->m_cellPos, m_src->m_cellPos);
	}

private:
	const DynamicFloorsAtCellPosBlob* m_src;
};


}

#endif //Navigation_DynamicFloorsAtCellPosBlob_H

