/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_StitchDataManagerUtils_H
#define Navigation_StitchDataManagerUtils_H

#include "gwnavruntime/base/types.h"


namespace Kaim
{

class MemoryManagerUtils
{
public:

	template<class T>
	static bool IsMemoryAlignedForClass(T* memory)
	{
		KyUInt32 sizeOfClass = (KyUInt32)sizeof(T);

		if ((sizeOfClass & 0x1) != 0)
			// no alignement required for this class
			return true;

		const UPInt address = (UPInt)memory;

		if ((sizeOfClass & 0x2) != 0)
			// memory for class must be aligned on 2 bytes
			return (address & 0x1) == 0;

#ifndef KY_64BIT_POINTERS
		// memory for class must be aligned on 4 bytes
		return (address & 0x3) == 0;
#else
		if ((sizeOfClass & 0x4) != 0)
			// memory for class must be aligned on 4 bytes
			return (address & 0x3) == 0;

		// memory for class must be aligned on 8 bytes
		return (address & 0x7) == 0;
#endif
	}

	template<class T>
	static KyUInt32 GetPaddingForBytesCount(KyUInt32 currentBytesCount)
	{
		KyUInt32 sizeOfClass = (KyUInt32)sizeof(T);

		if ((sizeOfClass & 0x1) != 0)
			// no padding
			return 0;

		if ((sizeOfClass & 0x2) != 0)
			// padding to be 2 bytes aligned
			return (KyUInt32)currentBytesCount & 0x1;

#ifndef KY_64BIT_POINTERS
		// padding to be 4 bytes aligned
		return (4 - (currentBytesCount & 0x3)) & 0x3;
#else
		if ((sizeOfClass & 0x4) != 0)
			// padding to be 4 bytes aligned
			return (4 - (currentBytesCount & 0x3)) & 0x3;

		// padding to be 8 bytes aligned
		return (8 - (currentBytesCount & 0x7)) & 0x7;
#endif
	}

	template<class T>
	static  KyUInt32 GetPaddingForMemory(char* memory)
	{
		KyUInt32 sizeOfClass = (KyUInt32)sizeof(T);

		if ((sizeOfClass & 0x1) != 0)
			return 0;

		const UPInt address = (UPInt)memory;

		if ((sizeOfClass & 0x2) != 0)
			// padding to be 2 bytes aligned
			return (KyUInt32)(address & 0x1);

#ifndef KY_64BIT_POINTERS
		// padding to be 4 bytes aligned
		return (4 - (address & 0x3)) & 0x3;
#else
		if ((sizeOfClass & 0x4) != 0)
			// padding to be 4 bytes aligned
			return (KyUInt32)(4 - (address & 0x3)) & 0x3;

		// padding to be 8 bytes aligned
		return (KyUInt32)(8 - (address & 0x7)) & 0x7;
#endif
	}
};


}

#endif //Navigation_StitchDataManagerUtils_H

