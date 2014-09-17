/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_BotBlob_H
#define Navigation_BotBlob_H

#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/world/runtimeblobcategory.h"
#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/path/livepathblob.h"
#include "gwnavruntime/spatialization/spatializedcylinderblob.h"
#include "gwnavruntime/path/positiononlivepathblob.h"


namespace Kaim
{

class Bot;

class BotBlob
{
	KY_ROOT_BLOB_CLASS(Runtime, BotBlob, 0)

public:
	SpatializedCylinderBlob m_spatializedCylinder;
	PositionOnLivePathBlob m_targetOnPath;
	KyUInt32 m_visualDebugId;
	KyUInt32 m_targetOnPathStatus;
	KyUInt32 m_doComputeTargetOnPath;
	KyUInt32 m_doComputeTrajectory;
	KyUInt32 m_avoidanceState;
	KyUInt32 m_pathValidityStatus;
	KyUInt32 m_pathFinderResult;
};

inline void SwapEndianness(Endianness::Target e, BotBlob& self)
{
	SwapEndianness(e, self.m_spatializedCylinder);
	SwapEndianness(e, self.m_targetOnPath);
	SwapEndianness(e, self.m_visualDebugId);
	SwapEndianness(e, self.m_targetOnPathStatus);
	SwapEndianness(e, self.m_doComputeTargetOnPath);
	SwapEndianness(e, self.m_doComputeTrajectory);
	SwapEndianness(e, self.m_avoidanceState);
	SwapEndianness(e, self.m_pathValidityStatus);
	SwapEndianness(e, self.m_pathFinderResult);
}


class BotBlobBuilder: public BaseBlobBuilder<BotBlob>
{
	KY_CLASS_WITHOUT_COPY(BotBlobBuilder)

public:
	BotBlobBuilder(const Bot* bot): m_bot(bot) {}
	~BotBlobBuilder() {}

private:
	virtual void DoBuild();

	const Bot* m_bot;
};

} // namespace Kaim

#endif // Navigation_BotBlob_H
