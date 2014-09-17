/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_RuntimeBlobRegistry_H
#define Navigation_RuntimeBlobRegistry_H


#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/blob/iblobcategorybuilder.h"


namespace Kaim
{


enum RuntimeCategory
{
	// DO NOT remove, comment or change order in this enum
	Blob_Navigation_Runtime_BoxObstacleBlob            =  0,
	Blob_Navigation_Runtime_BoxObstacleDetailsBlob     =  1,
	Blob_Navigation_Runtime_BoxObstaclesCollectionBlob =  2,
	Blob_Navigation_Runtime_CylinderObstacleBlob       =  3,
	Blob_Navigation_Runtime_PathBlob                   =  4,
	Blob_Navigation_Runtime_PositionOnPathBlob         =  5,
	Blob_Navigation_Runtime_TrajectoryBlob             =  6,
	Blob_Navigation_Runtime_BotBlob                    =  7,
	Blob_Navigation_Runtime_SpatializedPointBlob       =  8,
	Blob_Navigation_Runtime_SpatializedCylinderBlob    =  9,
	Blob_Navigation_Runtime_PointOfInterestBlob        = 10,
	Blob_Navigation_Runtime_DynamicFloorsAtCellPosBlob = 11,
	Blob_Navigation_Runtime_PositionOnLivePathBlob     = 12,
	Blob_Navigation_Runtime_LivePathBlob               = 13,
	Blob_Navigation_Runtime_LodMaxBlob                 = 14,

	Blob_Navigation_Runtime_Count
};

class RuntimeCategoryBuilder : public IBlobCategoryBuilder
{
public:
	RuntimeCategoryBuilder() : IBlobCategoryBuilder(Blob_Navigation, Blob_Navigation_Runtime, Blob_Navigation_Runtime_Count) {}
	virtual void Build(BlobCategory* category) const;
};


}


#endif

