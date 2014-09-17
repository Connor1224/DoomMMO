/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: MAMU - secondary contact: 

#ifndef Navigation_ActiveDataBlob_H
#define Navigation_ActiveDataBlob_H

#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/database/database.h"
#include "gwnavruntime/database/navmeshelementmanager.h"
#include "gwnavruntime/visualdebug/messages/stringblob.h"
#include "gwnavruntime/navdata/navdata.h"

namespace Kaim
{

class ActiveSectorBlob
{
public:
	ActiveSectorBlob() {}

	KyGuid m_guid;
	StringBlob m_sectorName;
	StringBlob m_sectorRelativeDir;
};

inline void SwapEndianness(Endianness::Target e, ActiveSectorBlob& self)
{
	SwapEndianness(e, self.m_guid);
	SwapEndianness(e, self.m_sectorName);
	SwapEndianness(e, self.m_sectorRelativeDir);
}

class ActiveSectorBlobBuilder : public BaseBlobBuilder<ActiveSectorBlob>
{
public:
	ActiveSectorBlobBuilder(const KyGuid& guid, const String& sectorName, const String& sectorRelativeDir)
		: m_guid(guid), m_sectorName(sectorName), m_sectorRelativeDir(sectorRelativeDir) {}

	void DoBuild()
	{
		BLOB_SET(m_blob->m_guid, m_guid);
		BLOB_BUILD(m_blob->m_sectorName, StringBlobBuilder(m_sectorName));
		BLOB_BUILD(m_blob->m_sectorRelativeDir, StringBlobBuilder(m_sectorRelativeDir));
	}

	KyGuid m_guid;
	String m_sectorName;
	String m_sectorRelativeDir;
};

class ActiveDataBlob
{
	KY_ROOT_BLOB_CLASS(NavData, ActiveDataBlob, 0)
	KY_CLASS_WITHOUT_COPY(ActiveDataBlob)

public:
	ActiveDataBlob() {}

	BlobArray<ActiveSectorBlob> m_activeSectors;
};

inline void SwapEndianness(Endianness::Target e, ActiveDataBlob& self)
{
	SwapEndianness(e, self.m_activeSectors);
}

class ActiveDataBlobBuilder : public BaseBlobBuilder<ActiveDataBlob>
{
public:
	ActiveDataBlobBuilder(const Database* db) : m_db(db) {}

	void DoBuild()
	{
		m_navDataIndicesToBuild.Clear();

		if (m_db != NULL)
		{
			for (KyUInt32 i = 0; i < m_db->m_navDatas.GetCount(); ++i)
			{
				Kaim::NavData* navData = m_db->m_navDatas[i];
				if (navData->GetSectorName() != "" && navData->GetMainGuid() != KY_NULL)
				{
					m_navDataIndicesToBuild.PushBack(i);
				}
			}
		}

		ActiveSectorBlob* activeSectors = BLOB_ARRAY(m_blob->m_activeSectors, m_navDataIndicesToBuild.GetCount());
		for (KyUInt32 i = 0; i < m_navDataIndicesToBuild.GetCount(); ++i)
		{
			if (m_db != NULL)
			{
				NavData* navData = m_db->m_navDatas[i];
				BLOB_BUILD(activeSectors[i], ActiveSectorBlobBuilder(*navData->GetMainGuid(), navData->m_sectorName, navData->m_generatorRelativeOutputDirectory));
			}
		}
	}

	const Database* m_db;
	KyArray<KyUInt32> m_navDataIndicesToBuild;
};

class ActiveGuidsBlob
{
	KY_ROOT_BLOB_CLASS(NavData, ActiveGuidsBlob, 0)
	KY_CLASS_WITHOUT_COPY(ActiveGuidsBlob)

public:
	ActiveGuidsBlob() {}

	BlobArray<KyGuid> m_guids;
};

inline void SwapEndianness(Endianness::Target e, ActiveGuidsBlob& self)
{
	SwapEndianness(e, self.m_guids);
}

class ActiveGuidsBlobBuilder : public BaseBlobBuilder<ActiveGuidsBlob>
{

public:
	ActiveGuidsBlobBuilder(KyArray<KyGuid, MemStat_NavData>* activeGuids) : m_activeGuids(activeGuids) {}

	void DoBuild()
	{
		if (m_activeGuids == NULL)
			return;

		BLOB_ARRAY_COPY_2(m_blob->m_guids, (*m_activeGuids));
	}

	KyArray<KyGuid, MemStat_NavData>* m_activeGuids;
};



}

#endif