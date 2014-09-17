/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_VisualDebugBlobRegistry_H
#define Navigation_VisualDebugBlobRegistry_H


#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/blob/iblobcategorybuilder.h"


namespace Kaim
{


enum VisualDebugMessageCategory
{
	// DO NOT remove, comment or change order in this enum
	Blob_Navigation_VisualDebugMessage_NewFrameBlob                = 0,
	Blob_Navigation_VisualDebugMessage_LogBlob                     = 1,
	Blob_Navigation_VisualDebugMessage_ServerMemoryLimitBlob       = 2,
	Blob_Navigation_VisualDebugMessage_VisualDebugDataRemovalBlob  = 3,
	Blob_Navigation_VisualDebugMessage_TextualCommandBlob          = 4,
	Blob_Navigation_VisualDebugMessage_FrameDumpBlob               = 5,
	Blob_Navigation_VisualDebugMessage_LoadedFilesBlob             = 6,

	Blob_Navigation_VisualDebugMessage_Count
};

class VisualDebugMessageCategoryBuilder : public IBlobCategoryBuilder
{
public:
	VisualDebugMessageCategoryBuilder() : IBlobCategoryBuilder(Blob_Navigation, Blob_Navigation_VisualDebugMessage, Blob_Navigation_VisualDebugMessage_Count) {}
	virtual void Build(BlobCategory* category) const;
};


enum VisualDebugCategory
{
	// DO NOT remove, comment or change order in this enum
	Blob_Navigation_VisualDebug_DisplayListBlob                 = 0,
	Blob_Navigation_VisualDebug_VelocityObstacleWorkspaceBlob   = 1,

	Blob_Navigation_VisualDebug_Count
};

class VisualDebugCategoryBuilder : public IBlobCategoryBuilder
{
public:
	VisualDebugCategoryBuilder() : IBlobCategoryBuilder(Blob_Navigation, Blob_Navigation_VisualDebug, Blob_Navigation_VisualDebug_Count) {}
	virtual void Build(BlobCategory* category) const;
};

}


#endif

