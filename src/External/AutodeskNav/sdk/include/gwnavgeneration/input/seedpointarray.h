/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NOBODY
#ifndef GwNavGen_SeedPointArray_H
#define GwNavGen_SeedPointArray_H

#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/blob/blobaggregate.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"


namespace Kaim
{

class SeedPointArray
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, SeedPointArray, 0)

public:
	void ReadBlob(KyArray<Vec3f>& seedPoints) const
	{
		const Vec3f* blobSeedPoints = m_seedPoints.GetValues();
		for (KyUInt32 i = 0; i < m_seedPoints.GetCount(); ++i)
			seedPoints.PushBack(blobSeedPoints[i]);
	}

	static void ReadAggregate(const BlobAggregate& aggregate, KyArray<Vec3f>& seedPoints)
	{
		BlobAggregate::Collection<SeedPointArray> seedPointArrays = aggregate.GetCollection<SeedPointArray>();
		for (UPInt i = 0; i < seedPointArrays.GetCount(); ++i)
			seedPointArrays.GetBlob(i)->ReadBlob(seedPoints);
	}
public:
	BlobArray<Vec3f> m_seedPoints; 
};
inline void SwapEndianness(Endianness::Target e, SeedPointArray& self)
{
	SwapEndianness(e, self.m_seedPoints);
}


class SeedPointArrayBlobBuilder : public BaseBlobBuilder<SeedPointArray>
{
public:
	SeedPointArrayBlobBuilder(const KyArray<Vec3f>* positions) : m_positions(positions) {}

	virtual void DoBuild()
	{
		BLOB_ARRAY_COPY(m_blob->m_seedPoints, m_positions->GetDataPtr(), m_positions->GetCount());
	}

	const KyArray<Vec3f>* m_positions;
};


}


#endif

