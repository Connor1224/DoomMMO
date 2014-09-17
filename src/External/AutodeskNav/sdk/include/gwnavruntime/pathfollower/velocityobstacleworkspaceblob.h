/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: MUBI - secondary contact: LAPA

#ifndef Navigation_VelocityObstacleWorkspaceBlob_H
#define Navigation_VelocityObstacleWorkspaceBlob_H

#include "gwnavruntime/visualdebug/visualdebugblobcategory.h"
#include "gwnavruntime/pathfollower/velocityobstaclecollider.h"
#include "gwnavruntime/pathfollower/velocityobstacle.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"

namespace Kaim
{
class VelocityObstacleWorkspaceBlob
{
	KY_ROOT_BLOB_CLASS(VisualDebug, VelocityObstacleWorkspaceBlob, 0)

public:
	VelocityObstacleCollider             m_rootCollider;
	BlobArray<VelocityObstacleCollider>  m_colliders;
	BlobArray<VelocityObstacle>          m_velocityObstacles;
};

inline void SwapEndianness(Endianness::Target e, VelocityObstacleWorkspaceBlob& self)
{
	SwapEndianness(e, self.m_rootCollider);
	SwapEndianness(e, self.m_colliders);
	SwapEndianness(e, self.m_velocityObstacles);
}

class VelocityObstacleWorkspace;

class VelocityObstacleWorkspaceBlobBuilder: public BaseBlobBuilder<VelocityObstacleWorkspaceBlob>
{
	KY_CLASS_WITHOUT_COPY(VelocityObstacleWorkspaceBlobBuilder)

public:
	VelocityObstacleWorkspaceBlobBuilder(const VelocityObstacleWorkspace& velocityObstacleWorkspace, bool bBuildGrid)
		: m_velocityObstacleWorkspace(velocityObstacleWorkspace)
		, m_bBuildGrid( bBuildGrid )
	{}
	~VelocityObstacleWorkspaceBlobBuilder() {}

private:
	virtual void DoBuild();

	const VelocityObstacleWorkspace& m_velocityObstacleWorkspace;
	bool m_bBuildGrid;
};

class VelocityObstacleWorkspaceBlobDisplayListBuilder: public IDisplayListBuilder
{
public:
	virtual void Init() { m_doBuild = false; }
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_Other; }
};

} // namespace Kaim

#endif // Navigation_VelocityObstacleWorkspaceBlob_H
