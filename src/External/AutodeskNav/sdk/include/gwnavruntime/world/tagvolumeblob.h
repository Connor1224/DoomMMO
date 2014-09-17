/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_TagVolumeBlob_H
#define Navigation_TagVolumeBlob_H

#include "gwnavruntime/world/tagvolume.h"


namespace Kaim
{

class TagVolumeBlob
{
	KY_CLASS_WITHOUT_COPY(TagVolumeBlob)
	KY_ROOT_BLOB_CLASS(NavData, TagVolumeBlob, 105)

public:
	TagVolumeBlob() : m_navTagIdx(KyUInt32MAXVAL) {}

	bool operator==(const TagVolumeBlob& other) const
	{
		if ((m_points.GetCount() != other.m_points.GetCount()) ||
			(m_aabb              != other.m_aabb) ||
			(m_obstacleType      != other.m_obstacleType) ||
			(m_navTag            != other.m_navTag))
		{
			return false;
		}

		const Vec2f* thisPoints = m_points.GetValues();
		const Vec2f* otherPoints = other.m_points.GetValues();
		for (KyUInt32 i = 0; i < m_points.GetCount(); ++i)
		{
			if (thisPoints[i] != otherPoints[i])
				return false;
		}

		return true;
	}
	bool operator!=(const TagVolumeBlob& other) const { return !operator==(other); }

public:
	KyUInt32 m_visualDebugId;
	BlobArray<Vec2f> m_points;
	Box3f m_aabb;
	KyUInt32 m_obstacleType;
	NavTag m_navTag;
	mutable KyUInt32 m_navTagIdx;
};

inline void SwapEndianness(Endianness::Target e, TagVolumeBlob& self)
{
	SwapEndianness(e, self.m_visualDebugId);
	SwapEndianness(e, self.m_points);
	SwapEndianness(e, self.m_aabb);
	SwapEndianness(e, self.m_obstacleType);
	SwapEndianness(e, self.m_navTag);
	SwapEndianness(e, self.m_navTagIdx);
}

class TagVolumeBlobBuilder : public BaseBlobBuilder<TagVolumeBlob>
{
public:
	TagVolumeBlobBuilder(const TagVolume* tagVolume) : m_tagVolume(tagVolume) {}

private:
	void DoBuild()
	{
		if (m_tagVolume)
		{
			BLOB_SET(m_blob->m_visualDebugId, m_tagVolume->GetVisualDebugId());
			BLOB_SET(m_blob->m_aabb, m_tagVolume->GetAABB());
			BLOB_SET(m_blob->m_obstacleType, (KyUInt32)m_tagVolume->GetObstacleType());
			BLOB_ARRAY_COPY(m_blob->m_points, m_tagVolume->GetPoints(), m_tagVolume->GetPointCount());
			BLOB_BUILD(m_blob->m_navTag, NavTagBlobBuilder(m_tagVolume->GetNavTag()));
		}
	}

	const TagVolume* m_tagVolume;
};

class TagVolumeContextBlob
{
	KY_CLASS_WITHOUT_COPY(TagVolumeContextBlob)
	KY_ROOT_BLOB_CLASS(NavData, TagVolumeContextBlob, 0)

public:
	TagVolumeContextBlob() {}

public:
	KyUInt32 m_visualDebugId;
	KyUInt32 m_integrationStatus; // cast this into IntegratonStatus
	KyUInt32 m_worldStatus; // cast this into WorldStatus;
	Vec3f m_topCenter;
};

inline void SwapEndianness(Endianness::Target e, TagVolumeContextBlob& self)
{
	SwapEndianness(e, self.m_visualDebugId);
	SwapEndianness(e, self.m_integrationStatus);
	SwapEndianness(e, self.m_worldStatus);
	SwapEndianness(e, self.m_topCenter);
}

class TagVolumeContextBlobBuilder : public BaseBlobBuilder<TagVolumeContextBlob>
{
public:
	TagVolumeContextBlobBuilder(const TagVolume* tagVolume) : m_tagVolume(tagVolume) {}

private:
	void DoBuild()
	{
		if (m_tagVolume)
		{
			BLOB_SET(m_blob->m_visualDebugId, m_tagVolume->GetVisualDebugId());
			BLOB_SET(m_blob->m_integrationStatus, m_tagVolume->GetIntegrationStatus());
			BLOB_SET(m_blob->m_worldStatus, m_tagVolume->GetWorldStatus());

			Vec3f boxTopCenter = m_tagVolume->GetAABB().Center();
			boxTopCenter.z = m_tagVolume->GetAABB().m_max.z;
			BLOB_SET(m_blob->m_topCenter, boxTopCenter);
		}
	}

	const TagVolume* m_tagVolume;
};

} // namespace Kaim

#endif // Navigation_TagVolumeBlob_H
