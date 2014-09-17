/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_CheckHeaderLibDesync_H
#define Navigation_CheckHeaderLibDesync_H


#include "gwnavruntime/base/types.h"


namespace Kaim
{


class CheckHeaderLibDesync
{
public:
	static void Check()
	{
	#if defined(KY_BUILD_DEBUG)
		Check_DEBUG();
	#elif defined(KY_BUILD_RELEASE)
		Check_RELEASE();
	#else
		Check_SHIPPING();
	#endif
	}

private:
	static void Check_DEBUG();
	static void Check_RELEASE();
	static void Check_SHIPPING();
	static KyUInt32 s_dummy;
};

}

#endif
