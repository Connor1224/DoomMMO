/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: BRGR - secondary contact: NOBODY
#ifndef Navigation_RawDataBlob_H
#define Navigation_RawDataBlob_H


#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/blob/iblobcategorybuilder.h"
#include "gwnavruntime/basesystem/version.h"

namespace Kaim
{


enum AggregateBlobCategory
{
	// DO NOT remove, comment or change order in this enum
	Blob_Navigation_Aggregate_VersionBlob         = 0,
	Blob_Navigation_Aggregate_NavDataRawBlob      = 1,
	Blob_Navigation_Aggregate_ClientInputRawBlob  = 2,

	Blob_Navigation_Aggregate_Count
};
class AggregateBlobCategoryBuilder : public IBlobCategoryBuilder
{
public:
	AggregateBlobCategoryBuilder() : IBlobCategoryBuilder(Blob_Navigation, Blob_Navigation_Aggregate, Blob_Navigation_Aggregate_Count) {}
	virtual void Build(BlobCategory* category) const;
};

class VersionBlob
{
public:
	KY_ROOT_BLOB_CLASS(Aggregate, VersionBlob, 0)
public:
	Kaim::BlobArray<char> m_data;
};
inline void SwapEndianness(Kaim::Endianness::Target e, VersionBlob& self)
{
	SwapEndianness(e, self.m_data);
}


class NavDataRawBlob
{
public:
	KY_ROOT_BLOB_CLASS(Aggregate, NavDataRawBlob, 0)
public:
	Kaim::BlobArray<char> m_data;
};
inline void SwapEndianness(Kaim::Endianness::Target e, NavDataRawBlob& self)
{
	SwapEndianness(e, self.m_data);
}


class ClientInputRawBlob
{
public:
	KY_ROOT_BLOB_CLASS(Aggregate, ClientInputRawBlob, 0)
public:
	Kaim::BlobArray<char> m_data;
};
inline void SwapEndianness(Kaim::Endianness::Target e, ClientInputRawBlob& self)
{
	SwapEndianness(e, self.m_data);
}

class VersionBlobBuilder : public BaseBlobBuilder<VersionBlob>
{
public:
	VersionBlobBuilder() {}

private:
	virtual void DoBuild()
	{
		const char* versionStr = Version::GetVersion();
		KyUInt32 versionSize = (KyUInt32) SFstrlen(versionStr);
		BLOB_ARRAY_COPY(this->m_blob->m_data, versionStr, versionSize);
	}
};

template<class T>
class RawDataBlobBuilder : public BaseBlobBuilder<T>
{
public:
	RawDataBlobBuilder(const char* data, KyUInt32 dataSize) : m_data(data), m_dataSize(dataSize) {}

private:
	virtual void DoBuild()
	{
		BLOB_ARRAY_COPY(this->m_blob->m_data, this->m_data, this->m_dataSize);
	}

	const char* m_data;
	KyUInt32 m_dataSize;
};


}

#endif