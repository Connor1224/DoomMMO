/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_PathBlob_H
#define Navigation_PathBlob_H

#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/world/runtimeblobcategory.h"
#include "gwnavruntime/path/path.h"
#include "gwnavruntime/blob/baseblobbuilder.h"


namespace Kaim
{

class PathBlob
{
	KY_ROOT_BLOB_CLASS(Runtime, PathBlob, 0)

public:
	KyUInt32 m_visualDebugId;
	BlobArray<Vec3f> m_nodePositions;
};
inline void SwapEndianness(Endianness::Target e, PathBlob& self)
{
	SwapEndianness(e, self.m_visualDebugId);
	SwapEndianness(e, self.m_nodePositions);
}


class PathBlobBuilder: public BaseBlobBuilder<PathBlob>
{
	KY_CLASS_WITHOUT_COPY(PathBlobBuilder)

public:
	PathBlobBuilder(const Path* path): m_path(path), m_visualDebugId(0) {}
	~PathBlobBuilder() {}

private:
	virtual void DoBuild()
	{
		BLOB_SET(m_blob->m_visualDebugId, m_visualDebugId);
		if (m_path != NULL && m_path->GetNodeCount() > 0)
		{
			Vec3f* nodePositions = BLOB_ARRAY(m_blob->m_nodePositions, m_path->GetNodeCount());

			if (IsWriteMode())
			{
				for (KyUInt32 i = 0; i < m_path->GetNodeCount(); ++i)
					nodePositions[i] = m_path->GetNodePosition(i);
			}
		}
		/*else
		{
			BLOB_ARRAY(m_blob->m_nodePositions, 0);
		}*/
	}

public:
	const Path* m_path;
	KyUInt32 m_visualDebugId;
};


}

#endif
