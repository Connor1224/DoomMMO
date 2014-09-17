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


#ifndef PX_INTERSECTION_RAY_PLANE_H
#define PX_INTERSECTION_RAY_PLANE_H

#include "GuPlane.h"

namespace physx
{
namespace Gu
{
	// Returns true if line and plane are not parallel
	PX_INLINE bool intersectRayPlane(const PxVec3& orig, const PxVec3& dir, const PxPlane& plane, float& distanceAlongLine, PxVec3* pointOnPlane = NULL)
	{
		const float dn = dir.dot(plane.n);
		if(-1E-7 < dn && dn < 1E-7)	return false; // parallel

		distanceAlongLine = -plane.distance(orig)/dn;

		if(pointOnPlane)
			*pointOnPlane = orig + distanceAlongLine * dir;

		return true;
	}
       
	PX_INLINE Ps::aos::BoolV intersectRayPlane(const Ps::aos::Vec3VArg orig, const Ps::aos::Vec3VArg dir, const Gu::PlaneV& plane, Ps::aos::FloatV& distanceAlongLine, Ps::aos::Vec3V& pointOnPlane)
	{
		using namespace Ps::aos;
		const FloatV eps = FloatV_From_F32(1E-7f);
		const Vec3V n = Vec3V_From_Vec4V(plane.nd);
		const FloatV dn = V3Dot(dir, n);
		const FloatV absDn = FAbs(dn);
		const BoolV con = FIsGrtrOrEq(absDn, eps); // not parallel

		distanceAlongLine = FNeg(FDiv(plane.distance(orig), dn));
		pointOnPlane = V3MulAdd(dir, distanceAlongLine, orig);

		return con;
	}
} // namespace Gu

}

#endif