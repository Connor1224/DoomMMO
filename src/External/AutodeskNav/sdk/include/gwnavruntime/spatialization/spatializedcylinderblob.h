/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: none
#ifndef Navigation_SpatializedCylinderBlob_H
#define Navigation_SpatializedCylinderBlob_H

#include "gwnavruntime/spatialization/spatializedcylinder.h"
#include "gwnavruntime/spatialization/spatializedpoint.h"
#include "gwnavruntime/spatialization/spatializedpointblob.h"
#include "gwnavruntime/world/runtimeblobcategory.h"


namespace Kaim
{

//////////////////////////////////////////////////////////////////////////
// SpatializedCylinderBlob
class SpatializedCylinderBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Runtime, SpatializedCylinderBlob, 0)

public:
	SpatializedPointBlob m_spatializedPoint;
	Vec3f m_velocity;
	KyFloat32 m_radius;
	KyFloat32 m_height;
};

inline void SwapEndianness(Endianness::Target e, SpatializedCylinderBlob& self)
{
	SwapEndianness(e, self.m_spatializedPoint);
	SwapEndianness(e, self.m_velocity);
	SwapEndianness(e, self.m_radius);
	SwapEndianness(e, self.m_height);
}


//////////////////////////////////////////////////////////////////////////
// SpatializedCylinderBlobBuilder
class SpatializedCylinderBlobBuilder : public BaseBlobBuilder<SpatializedCylinderBlob>
{
public:
	SpatializedCylinderBlobBuilder(const SpatializedCylinder* spatializedCylinder, VisualDebugLOD visualDebugLOD)
		: m_spatializedCylinder(spatializedCylinder)
		, m_visualDebugLOD(visualDebugLOD)
	{}

private:
	virtual void DoBuild();

	const SpatializedCylinder* m_spatializedCylinder;
	VisualDebugLOD m_visualDebugLOD;
};

} // namespace Kaim

#endif // Navigation_SpatializedCylinderBlob_H
