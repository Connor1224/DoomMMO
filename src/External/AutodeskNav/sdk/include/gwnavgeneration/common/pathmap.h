/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_PathMap_H
#define GwNavGen_PathMap_H


#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/kernel/SF_String.h"

namespace Kaim
{


class PathMap // class with static functions only
{
public:
	static String CellFileName(
		const char* containingDirectory,
		const CellPos& cellPos,
		const char* extension,
		bool inCellSubDir);
};


}


#endif

