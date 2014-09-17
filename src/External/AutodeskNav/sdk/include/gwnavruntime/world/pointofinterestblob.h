/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: MAMU
#ifndef Navigation_PointOfInterestBlob_H
#define Navigation_PointOfInterestBlob_H

#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/world/runtimeblobcategory.h"
#include "gwnavruntime/spatialization/spatializedpointblob.h"


namespace Kaim
{

class PointOfInterest;


//////////////////////////////////////////////////////////////////////////
// PointOfInterestBlob
class PointOfInterestBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Blob)
	KY_ROOT_BLOB_CLASS(Runtime, PointOfInterestBlob, 0)
	//KY_CLASS_WITHOUT_COPY(PointOfInterestBlob)

public:
	SpatializedPointBlob m_spatializedPoint;
	KyUInt32 m_poiType;
	KyUInt32 m_visualDebugId;
};

inline void SwapEndianness(Endianness::Target e, PointOfInterestBlob& self)
{
	SwapEndianness(e, self.m_spatializedPoint);
	SwapEndianness(e, self.m_poiType);
	SwapEndianness(e, self.m_visualDebugId);
}


//////////////////////////////////////////////////////////////////////////
// PointOfInterestBlobBuilder
class PointOfInterestBlobBuilder : public BaseBlobBuilder<PointOfInterestBlob>
{
	KY_CLASS_WITHOUT_COPY(PointOfInterestBlobBuilder)

public:
	PointOfInterestBlobBuilder(const PointOfInterest* pointOfInterest, VisualDebugLOD visualDebugLOD)
		: m_pointOfInterest(pointOfInterest)
		, m_visualDebugLOD(visualDebugLOD)
	{}

	virtual void DoBuild();

private:
	const PointOfInterest* m_pointOfInterest;
	VisualDebugLOD m_visualDebugLOD;
};

} // namespace Kaim

#endif // Navigation_PointOfInterestBlob_H
