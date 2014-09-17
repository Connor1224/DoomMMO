/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_BoxObstacleBlob_H
#define Navigation_BoxObstacleBlob_H

#include "gwnavruntime/world/boxobstacle.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/world/runtimeblobcategory.h"
#include "gwnavruntime/world/tagvolumeblob.h"
#include "gwnavruntime/spatialization/spatializedcylinderblob.h"


namespace Kaim
{

//////////////////////////////////////////////////////////////////////////
//
// BoxObstacleDetailsBlob and BoxObstacleDetailsBlobBuilder
//
//////////////////////////////////////////////////////////////////////////
class BoxObstacleDetailsBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Runtime, BoxObstacleDetailsBlob, 0)
public:
	BoxObstacleDetailsBlob() {}

	BlobArray<SpatializedCylinderBlob> m_spatializedCylinderBlobs;
	Vec3f     m_linearVelocity;
	Vec3f     m_angularVelocity;
	KyUInt32  m_rotationMode;
};

inline void SwapEndianness(Endianness::Target e, BoxObstacleDetailsBlob& self)
{
	SwapEndianness(e, self.m_spatializedCylinderBlobs);
	SwapEndianness(e, self.m_linearVelocity);
	SwapEndianness(e, self.m_angularVelocity);
	SwapEndianness(e, self.m_rotationMode);
}

class BoxObstacleDetailsBlobBuilder : public BaseBlobBuilder<BoxObstacleDetailsBlob>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BoxObstacleDetailsBlobBuilder(const BoxObstacle* boxObstacle) { m_boxObstacle = boxObstacle; }
	virtual void DoBuild();

private:
	const BoxObstacle* m_boxObstacle;
};


//////////////////////////////////////////////////////////////////////////
//
// BoxObstacleBlob and BoxObstacleBlobBuilder
//
//////////////////////////////////////////////////////////////////////////
class BoxObstacleBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Runtime, BoxObstacleBlob, 0)
public:
	BoxObstacleBlob() {}

	void GetWorldPosition(Vec3f& worldPosition) const { m_worldTransform.GetRootCoordinates(m_localCenter, worldPosition); }
	void GetWorldAABB(Box3f& worldAABB) const;

	Vec3f m_localCenter;
	Vec3f m_localHalfExtents;
	Transform m_worldTransform;
	BlobRef<BoxObstacleDetailsBlob> m_details;
	KyUInt32 m_tagVolumeIntegrationStatus;
	KyUInt32 m_visualDebugId;
};

inline void SwapEndianness(Endianness::Target e, BoxObstacleBlob& self)
{
	SwapEndianness(e, self.m_localCenter);
	SwapEndianness(e, self.m_localHalfExtents);
	SwapEndianness(e, self.m_worldTransform);
	SwapEndianness(e, self.m_details);
	SwapEndianness(e, self.m_tagVolumeIntegrationStatus);
	SwapEndianness(e, self.m_visualDebugId);
}

class BoxObstacleBlobBuilder : public BaseBlobBuilder<BoxObstacleBlob>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BoxObstacleBlobBuilder(const BoxObstacle* boxObstacle, VisualDebugLOD visualDebugLOD)
		: m_boxObstacle(boxObstacle)
		, m_visualDebugLOD(visualDebugLOD)
	{}

private:
	virtual void DoBuild();

	const BoxObstacle* m_boxObstacle;
	VisualDebugLOD m_visualDebugLOD;
};


//////////////////////////////////////////////////////////////////////////
//
// BoxObstaclesCollectionBlob and BoxObstaclesCollectionBlobBuilder
//
//////////////////////////////////////////////////////////////////////////

/* A type of blob that serializes data for several BoxObstacleBlobs that are managed through the BoxObstaclesCollectionBlobBuilder class.
		*/
class BoxObstaclesCollectionBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Runtime, BoxObstaclesCollectionBlob, 0)
public:
	BlobArray<BoxObstacleBlob> m_boxObstacles;
};
inline void SwapEndianness(Endianness::Target e, BoxObstaclesCollectionBlob& self)
{
	SwapEndianness(e, self.m_boxObstacles);
}

/// A type of blob builder that creates a BoxObstaclesCollectionBlob from a list of box BoxObstacles (Kaim::BoxObstacle).
/// You can use this class to serialize the obstacles to and from a file on disk.
class BoxObstaclesCollectionBlobBuilder : public BaseBlobBuilder<BoxObstaclesCollectionBlob>
{
public:
	/* \param boxObstacles			The List of BoxObstacles serialized by this blob builder. */
	BoxObstaclesCollectionBlobBuilder(const TrackedCollection<Ptr<BoxObstacle>, MemStat_BoxObstacle>& boxObstacles)
		: m_boxObstacles(&boxObstacles)
	{}

	virtual void DoBuild();

private:
	const TrackedCollection<Ptr<BoxObstacle>, MemStat_BoxObstacle>* m_boxObstacles;
};


} // namespace Kaim

#endif // Navigation_BoxObstacleBlob_H
