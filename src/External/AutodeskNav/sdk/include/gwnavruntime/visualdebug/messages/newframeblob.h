/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: BRGR - secondary contact: NOBODY
#ifndef Navigation_NewFrameBlob_H
#define Navigation_NewFrameBlob_H


#include "gwnavruntime/visualdebug/visualdebugblobcategory.h"

namespace Kaim
{

	// This class is sent on VisualDebug::Update() calls.
	// This is an internal blob expected to be handled by the user of the VisualDebugClient
	class NewFrameBlob
	{
		KY_ROOT_BLOB_CLASS(VisualDebugMessage, NewFrameBlob, 0)
	public:

		NewFrameBlob() : m_frameNumber(0), m_frameSendTime(0) {}

		KyUInt32 m_frameNumber;
		KyUInt32 m_frameSendTime;
	};

	inline void SwapEndianness(Endianness::Target e, NewFrameBlob& self)
	{
		SwapEndianness(e, self.m_frameNumber);
		SwapEndianness(e, self.m_frameSendTime);
	}

} // namespace Kaim

#endif // Navigation_NewFrameBlob_H
