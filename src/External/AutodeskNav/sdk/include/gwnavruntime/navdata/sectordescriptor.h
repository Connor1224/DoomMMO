/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: GUAL
#ifndef Navigation_SectorDescriptor_H
#define Navigation_SectorDescriptor_H

#include "gwnavruntime/blob/blobfieldarray.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/blob/blobaggregate.h"

namespace Kaim
{

class SectorDescriptorBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
	KY_CLASS_WITHOUT_COPY(SectorDescriptorBlob)
	KY_ROOT_BLOB_CLASS(NavData, SectorDescriptorBlob, 0)
public:
	SectorDescriptorBlob() {}
	BlobFieldArray m_fields;
};
inline void SwapEndianness(Endianness::Target e, SectorDescriptorBlob& self)
{
	SwapEndianness(e, self.m_fields);
}


class SectorDescriptor
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)

public:
	KyResult ReadFromAggregate(const BlobAggregate& aggregate)
	{
		BlobAggregate::Collection<SectorDescriptorBlob> sectorDescs = aggregate.GetCollection<SectorDescriptorBlob>();
		if (sectorDescs.GetCount() != 1)
			return KY_ERROR; // 1 and only 1 SectorDescriptorBlob
		return ReadFromBlob(*sectorDescs.GetHandler(0)->Blob());
	}

	void AddMapping(BlobFieldsMapping& mapping)
	{
		mapping.AddString("SectorName", m_sectorName);
		mapping.AddString("GeneratorRelativeOutputDirectory", m_generatorRelativeOutputDirectory);
	}

	KyResult ReadFromBlob(const SectorDescriptorBlob& blob)
	{
		BlobFieldsMapping mapping;
		AddMapping(mapping);
		return mapping.ReadFromBlobFieldArray(blob.m_fields);
	}

public:
	String m_sectorName;
	String m_generatorRelativeOutputDirectory; // relative directory used by the generator to output files related to this sector.
};

} // namespace Kaim

#endif
