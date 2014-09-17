// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  


#ifndef PX_LEGACY_STREAM_H
#define PX_LEGACY_STREAM_H

/** \addtogroup common
  @{
*/

#include "common/PxPhysXCommonConfig.h"
#include "foundation/PxAssert.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

#define PX_SERIAL_DEFAULT_ALIGN	16

//used to fix multi-byte characters warning from gcc for situations like: PxU32 foo = 'CCTS';
#define PX_MAKE_FOURCC(a, b, c, d) ( (a) | ((b)<<8) | ((c)<<16) | ((d)<<24) )

PX_INLINE PxU32 PxGetPadding(size_t value, PxU32 alignment)
{
	const PxU32 mask = alignment-1;
	const PxU32 overhead = PxU32(value) & mask;
	return (alignment - overhead) & mask;
}

PX_INLINE char* PxAlignStream(char* buffer, PxU32& total, PxU32 alignment = PX_SERIAL_DEFAULT_ALIGN)
{
	if(!alignment)
		return buffer;

	const PxU32 padding = PxGetPadding(size_t(buffer), alignment);
	PX_ASSERT(!PxGetPadding(size_t(buffer + padding), alignment));
	total += padding;
	return buffer + padding;
}


/**
\brief Callback class for the new serialization framework.
*/
class PxSerialStream
{
public:
							PxSerialStream()			{}
	virtual					~PxSerialStream()			{}

	virtual		void		storeBuffer(const void* buffer, PxU32 size)		= 0;
	virtual		PxU32		getTotalStoredSize()							= 0;
	virtual		void		align(PxU32 alignment = PX_SERIAL_DEFAULT_ALIGN) = 0;
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

 /** @} */
#endif
