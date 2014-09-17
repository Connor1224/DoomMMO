/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_DiskCastQueryBlob_H
#define Navigation_DiskCastQueryBlob_H

#include "gwnavruntime/queries/blobs/queryblobcategory.h"
#include "gwnavruntime/queries/blobs/querydynamicoutputblob.h"
#include "gwnavruntime/queries/blobs/navhalfedgeblob.h"
#include "gwnavruntime/queries/utils/basediskcastquery.h"

namespace Kaim
{

class DiskCastQueryOutputBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	DiskCastQueryOutputBlob() : m_result(0) {}

	DiskCastQueryResult GetResult() const { return (DiskCastQueryResult)m_result; }
public:
	KyUInt32 m_result;
	NavTriangleBlob m_startTriangle;
	NavTriangleBlob m_arrivalTriangle;
	Vec3f m_arrivalPos3f;
	Vec3f m_collisionPos3f;
	NavHalfEdgeBlob m_collisionHalfEdge;
	BlobRef<QueryDynamicOutputBlob> m_queryDynamicOutputBlobRef;
};
inline void SwapEndianness(Endianness::Target e, DiskCastQueryOutputBlob& self)
{
	SwapEndianness(e, self.m_result                   );
	SwapEndianness(e, self.m_startTriangle            );
	SwapEndianness(e, self.m_arrivalTriangle          );
	SwapEndianness(e, self.m_arrivalPos3f             );
	SwapEndianness(e, self.m_collisionPos3f           );
	SwapEndianness(e, self.m_collisionHalfEdge        );
	SwapEndianness(e, self.m_queryDynamicOutputBlobRef);
}


class DiskCastQueryOutputBlobBuilder : public BaseBlobBuilder<DiskCastQueryOutputBlob>
{
public:
	DiskCastQueryOutputBlobBuilder(BaseDiskCastQuery* query) { m_query = query; }
	virtual void DoBuild()
	{
		BLOB_SET(m_blob->m_result, (KyUInt32)m_query->GetResult());

		BLOB_BUILD(m_blob->m_startTriangle    , NavTriangleBlobBuilder(m_query->GetStartTrianglePtr()));
		BLOB_BUILD(m_blob->m_arrivalTriangle  , NavTriangleBlobBuilder(m_query->GetArrivalTrianglePtr()));
		BLOB_SET(m_blob->m_arrivalPos3f       , m_query->GetArrivalPos());
		BLOB_SET(m_blob->m_collisionPos3f     , m_query->GetCollisionPos());
		BLOB_BUILD(m_blob->m_collisionHalfEdge, NavHalfEdgeBlobBuilder(m_query->GetCollisionNavHalfEdgePtr()));

		QueryDynamicOutput* queryDynamicOutput = m_query->GetQueryDynamicOutput();
		if (queryDynamicOutput != KY_NULL && queryDynamicOutput->IsEmpty() == false)
		{
			BUILD_REFERENCED_BLOB(m_blob->m_queryDynamicOutputBlobRef, QueryDynamicOutputBlobBuilder(queryDynamicOutput));
		}
	}

private:
	BaseDiskCastQuery* m_query;
};

class DiskCastQueryBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Query, DiskCastQueryBlob, 0)
public:
	DiskCastQueryBlob() : m_queryInfoId(KyUInt32MAXVAL), m_databaseIdx(KyUInt32MAXVAL) {}

	DynamicOutputMode GetDynamicOutputMode() const { return (DynamicOutputMode)m_dynamicOutputMode; }

public:
	KyUInt32 m_queryInfoId;

	KyUInt32 m_databaseIdx;
	Vec3f m_startPos3f;
	KyFloat32 m_radius;
	Vec2f m_normalizedDir2d;
	KyFloat32 m_maxDist;
	PositionSpatializationRange m_positionSpatializationRange;
	KyFloat32 m_safetyDist;
	KyUInt32 m_dynamicOutputMode; ///< this member is used to tell the Query what it should store into QueryDynamicOutput.

	BlobRef<DiskCastQueryOutputBlob> m_queryOutput;
};
inline void SwapEndianness(Endianness::Target e, DiskCastQueryBlob& self)
{
	SwapEndianness(e, self.m_queryInfoId);
	SwapEndianness(e, self.m_databaseIdx);
	SwapEndianness(e, self.m_startPos3f);
	SwapEndianness(e, self.m_radius);
	SwapEndianness(e, self.m_normalizedDir2d);
	SwapEndianness(e, self.m_maxDist);
	SwapEndianness(e, self.m_positionSpatializationRange);
	SwapEndianness(e, self.m_safetyDist);
	SwapEndianness(e, self.m_dynamicOutputMode);

	SwapEndianness(e, self.m_queryOutput);
}



class DiskCastQueryBlobBuilder : public BaseBlobBuilder<DiskCastQueryBlob>
{
public:
	DiskCastQueryBlobBuilder(BaseDiskCastQuery* query) { m_query = query; }
	virtual void DoBuild()
	{
		BLOB_SET(m_blob->m_queryInfoId, m_query->m_queryInfoId);

		if (m_query->m_database != KY_NULL)
		{
			BLOB_SET(m_blob->m_databaseIdx                , m_query->m_database->GetDatabaseIndex());
			BLOB_SET(m_blob->m_startPos3f                 , m_query->GetStartPos());
			BLOB_SET(m_blob->m_radius                     , m_query->GetRadius());
			BLOB_SET(m_blob->m_normalizedDir2d            , m_query->GetNormalizedDir2d());
			BLOB_SET(m_blob->m_maxDist                    , m_query->GetMaxDist());
			BLOB_SET(m_blob->m_positionSpatializationRange, m_query->GetPositionSpatializationRange());
			BLOB_SET(m_blob->m_safetyDist                 , m_query->GetSafetyDist());
			BLOB_SET(m_blob->m_dynamicOutputMode          , (KyUInt32)m_query->GetDynamicOutputMode());

			switch (m_query->GetResult())
			{
			case DISKCAST_NOT_INITIALIZED :
			case DISKCAST_NOT_PROCESSED :
				break;
			default:
				BUILD_REFERENCED_BLOB(m_blob->m_queryOutput, DiskCastQueryOutputBlobBuilder(m_query));
				break;
			}
		}
	}

private:
	BaseDiskCastQuery* m_query;
};
}

#endif // Kaim_DiskCastQueryInputBlob_H
