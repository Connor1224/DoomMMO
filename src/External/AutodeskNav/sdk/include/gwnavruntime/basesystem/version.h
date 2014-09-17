/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// authors: GUAL
#ifndef Navigation_Version_H
#define Navigation_Version_H

#include "gwnavruntime/base/types.h"


namespace Kaim
{

/// This class provides information about this Gameware Navigation version.
class Version
{
public:
	// ---------------------------------- Public Member Functions ----------------------------------
	static const char* GetVersion();
	static const char* GetBuildDate();
	static bool IsEvaluationVersion();

private:
	static const char* GetSpecificVersion();
	static bool IsGamewareBuild();

public: // internal
	static void RegisterVersion();
};

}

#endif

