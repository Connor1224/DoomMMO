/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#include "perfmarkerinterface.h"


#if defined(KY_OS_PS3) && defined(KY_PERF_MARKERS_ENABLED)
#include <libsntuner.h>
#endif

void PerfMarkerInterface::Begin(const char* name)
{
	KY_UNUSED(name);
#if defined(KY_OS_PS3) && defined(KY_PERF_MARKERS_ENABLED)
	snPushMarker(name);
#elif defined(KY_OS_XBOX360) && defined(KY_PERF_MARKERS_ENABLED)
	PIXBeginNamedEvent(0, name);
#endif
}

void PerfMarkerInterface::End()
{
#if defined(KY_OS_PS3) && defined(KY_PERF_MARKERS_ENABLED)
	snPopMarker();
#elif defined(KY_OS_XBOX360) && defined(KY_PERF_MARKERS_ENABLED)
	PIXEndNamedEvent();
#endif

}

