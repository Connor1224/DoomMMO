/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_PositionOnLivePathBlob_H
#define Navigation_PositionOnLivePathBlob_H

#include "gwnavruntime/path/positiononpathblob.h"
#include "gwnavruntime/path/positiononlivepath.h"

namespace Kaim
{

class PositionOnLivePathBlob
{
	KY_ROOT_BLOB_CLASS(Runtime, PositionOnLivePathBlob, 0)

public:
	PositionOnLivePathBlob() : m_onEventLListIndex(KyUInt32MAXVAL), m_onEventListStatus((KyUInt32)OnEventListStatus_Undefined) {}

	PositionOnPathStatus GetPositionStatus() const { return m_positionOnPathBlob.GetPositionStatus(); }
	OnEventListStatus GetOnEventListStatus() const { return (OnEventListStatus)m_onEventListStatus; }
public:
	PositionOnPathBlob m_positionOnPathBlob;
	KyUInt32 m_onEventLListIndex;
	KyUInt32 m_onEventListStatus;
};
inline void SwapEndianness(Endianness::Target e, PositionOnLivePathBlob& self)
{
	SwapEndianness(e, self.m_positionOnPathBlob);
	SwapEndianness(e, self.m_onEventLListIndex);
	SwapEndianness(e, self.m_onEventListStatus);
}


class PositionOnLivePathBlobBuilder: public BaseBlobBuilder<PositionOnLivePathBlob>
{
	KY_CLASS_WITHOUT_COPY(PositionOnLivePathBlobBuilder)

public:
	PositionOnLivePathBlobBuilder(const PositionOnLivePath* positionOnLivePath)
		: m_positionOnLivePath(positionOnLivePath)
	{}
	~PositionOnLivePathBlobBuilder() {}

private:
	virtual void DoBuild()
	{
		if ((m_positionOnLivePath != NULL) && (m_positionOnLivePath->GetPositionOnPathStatus() != PositionOnPathStatus_Undefined))
		{
			PositionOnPathBlobBuilder builder(&m_positionOnLivePath->GetPositionOnPath());
			BLOB_BUILD(m_blob->m_positionOnPathBlob, builder);
			BLOB_SET(m_blob->m_onEventLListIndex, m_positionOnLivePath->GetOnEventListIndex());
			BLOB_SET(m_blob->m_onEventListStatus, (KyUInt32)m_positionOnLivePath->GetOnEventListStatus());
		}
	}

private:
	const PositionOnLivePath* m_positionOnLivePath;
};


}

#endif
