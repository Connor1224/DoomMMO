/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_GeneratorCategory_H
#define GwNavGen_GeneratorCategory_H


#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/blob/iblobcategorybuilder.h"


namespace Kaim
{

enum GeneratorCategory
{
	// DO NOT remove, comment or change order in this enum
	Blob_Navigation_Generator_BoundaryGraphBlob             = 0,
	Blob_Navigation_Generator_RasterCell                    = 1,
	Blob_Navigation_Generator_NavRasterCell                 = 2,
	Blob_Navigation_Generator_PdgInputCell                  = 3,
	Blob_Navigation_Generator_ClientInputTagVolumeBlob      = 4,
	Blob_Navigation_Generator_PdgInputTileIndex             = 5,
	Blob_Navigation_Generator_SeedPointArray                = 6,
	Blob_Navigation_Generator_PdgInputTile                  = 7,
	Blob_Navigation_Generator_GeneratorParametersBlob       = 8,
	Blob_Navigation_Generator_GeneratorSectorConfigBlob     = 9,
	Blob_Navigation_Generator_ClientInputChunk              = 10,
	Blob_Navigation_Generator_ClientInputDescriptorBlob     = 11,
	Blob_Navigation_Generator_GeneratorSectorsConfigBlob    = 12,
	Blob_Navigation_Generator_GeneratorRunOptionsBlob       = 13,

	Blob_Navigation_Generator_Count
};


class GeneratorCategoryBuilder : public IBlobCategoryBuilder
{
public:
	GeneratorCategoryBuilder() : IBlobCategoryBuilder(Blob_Navigation, Blob_Navigation_Generator, Blob_Navigation_Generator_Count) {}
	virtual void Build(BlobCategory* category) const;
};


}


#endif

