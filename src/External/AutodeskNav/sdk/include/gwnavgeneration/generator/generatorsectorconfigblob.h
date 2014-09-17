/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_GeneratorSectorConfigBlob_H
#define GwNavGen_GeneratorSectorConfigBlob_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/blob/blobfieldarray.h"
#include "gwnavruntime/base/kyguid.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"
#include "gwnavgeneration/generator/generatorinputoutput.h"

namespace Kaim
{

class BlobAggregate;
class GeneratorSectorConfig;
class GeneratorSectorList;
class GeneratorSector;


class GeneratorSectorConfigBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, GeneratorSectorConfigBlob, 0)
	KY_CLASS_WITHOUT_COPY(GeneratorSectorConfigBlob)
public:
	GeneratorSectorConfigBlob() {}
	BlobFieldArray m_fields;
	CellBox m_inputCellBox;
	BlobArray<BlobArray<char> > m_inputFilenames;
};
inline void SwapEndianness(Endianness::Target e, GeneratorSectorConfigBlob& self)
{
	SwapEndianness(e, self.m_fields);
	SwapEndianness(e, self.m_inputCellBox);
	SwapEndianness(e, self.m_inputFilenames);
}

class GeneratorSectorConfigBlobBuilder : public BaseBlobBuilder<GeneratorSectorConfigBlob>
{
public:
	GeneratorSectorConfigBlobBuilder(Ptr<GeneratorSector> sector, GenFlags::GenIoFilter filter)
		: m_sector(sector), m_filter(filter) {}
private:
	virtual void DoBuild();
private:
	Ptr<GeneratorSector> m_sector;
	GenFlags::GenIoFilter m_filter;
};

}


#endif

