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


#ifndef PX_PHYSICS_NX_SCENE_QUERY_FILTERING
#define PX_PHYSICS_NX_SCENE_QUERY_FILTERING
/** \addtogroup scenequery
@{
*/

#include "PxPhysXConfig.h"
#include "PxFiltering.h"
#include "PxQueryReport.h"
#include "PxClient.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxShape;
class PxRigidActor;
struct PxQueryHit;


/**
\brief Filtering flags for scene queries.

@see PxSceneQueryFilter.filterFlags
*/
struct PxQueryFlag
{
	enum Enum
	{
		eSTATIC				= (1<<0),	//!< Traverse static shapes

		eDYNAMIC			= (1<<1),	//!< Traverse dynamic shapes

		ePREFILTER			= (1<<2),	//!< Run the pre-intersection-test filter (see #PxQueryFilterCallback::preFilter())

		ePOSTFILTER			= (1<<3),	//!< Run the post-intersection-test filter (see #PxQueryFilterCallback::postFilter())

		eANY_HIT			= (1<<4),	//!< Abort traversal as soon as any hit is found and return it via callback.block.
										//!< Helps query performance. Both eTOUCH and eBLOCK hitTypes are considered hits with this flag.

		eNO_BLOCK			= (1<<5),	//!< Tells traversal not to expect any blocking hits. Helps query performance.
										//!< All hits are reported as touching.

		eRESERVED			= (1<<15),	//!< Reserved for internal use
	};
};
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eSTATIC==(1<<0));
PX_COMPILE_TIME_ASSERT(PxQueryFlag::eDYNAMIC==(1<<1));

/**
\brief Collection of set bits defined in PxQueryFlag.

@see PxSceneQueryFilter
*/
typedef PxFlags<PxQueryFlag::Enum,PxU16> PxQueryFlags;
PX_FLAGS_OPERATORS(PxQueryFlag::Enum,PxU16);

/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryFilterFlag PxQueryFlag // PX_DEPRECATED
/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryFilterFlags PxQueryFlags // PX_DEPRECATED

/**
\brief Classification of scene query hits.

A hit type of eNONE means that the hit should not be reported. 

In the case of raycastMultiple and sweepMultiple queries, hits of type eTOUCH will be returned which are closer than the 
first eBLOCK, together with the closest hit of type eBLOCK. For example, to return all hits in a raycastMultiple, always return eTOUCH.

For raycastSingle/sweepSingle, the closest hit of type eBLOCK is returned.

@see PxSceneQueryFilter.preFilter PxSceneQueryFilter.postFilter
*/
struct PxQueryHitType
{
	enum Enum
	{
		eNONE	= 0,	//!< the query should ignore this shape
		eTOUCH	= 1,	//!< a hit on the shape touches the intersection geometry of the query but does not block it
		eBLOCK	= 2		//!< a hit on the shape blocks the query (does not block overlap queries)
	};
};

/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryHitType PxQueryHitType // PX_DEPRECATED

/**
\brief Scene query filtering data.

When the scene graph traversal determines that a shape intersects, filtering is performed.

Filtering is performed in the following order:

\li For non-batched queries only:<br>If the data field is non-zero, and the bitwise-AND value of data AND the shape's
queryFilterData is zero, the shape is skipped
\li If the filter callbacks are enabled in the flags field (see #PxQueryFlags) they will get invoked accordingly.
\li If neither #PxQueryFlag::ePREFILTER or #PxQueryFlag::ePOSTFILTER is set, the hit is 
assumed to be of type #PxQueryHitType::eBLOCK for single hit queries and of type
#PxQueryHitType::eTOUCH for multi hit queries.

@see PxScene.raycastAny PxScene.raycastSingle PxScene.raycastMultiple PxScene.sweepSingle PxScene.sweepMultiple
*/
struct PxQueryFilterData
{
	/**
	\brief constructor sets to default 
	*/
	explicit PX_INLINE PxQueryFilterData() : flags(PxQueryFlag::eDYNAMIC | PxQueryFlag::eSTATIC), clientId(PX_DEFAULT_CLIENT) {}

	/**
	\brief constructor to set properties
	*/
	explicit PX_INLINE PxQueryFilterData(const PxFilterData& fd, PxQueryFlags f) : data(fd), flags(f), clientId(PX_DEFAULT_CLIENT) {}

	/**
	\brief constructor to set filter flags only
	*/
	explicit PX_INLINE PxQueryFilterData(PxQueryFlags f) : flags(f), clientId(PX_DEFAULT_CLIENT) {}

	PxFilterData	data;		//!< Filter data associated with the scene query
	PxQueryFlags	flags;		//!< Filter flags (see #PxQueryFlags)
	PxClientID		clientId;	//!< ID of the client doing the query (see #PxScene.createClient())
};

/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryFilterData PxQueryFilterData // PX_DEPRECATED

/**
\brief Scene query filtering callbacks.

Custom filtering logic for scene query intersection candidates. If an intersection candidate object passes the data based filter (see #PxQueryFilterData),
the filtering callbacks run on request (see #PxQueryFilterData.flags)

\li If #PxQueryFlag::ePREFILTER is set, the preFilter function runs before precise intersection
testing. If this function returns #PxQueryHitType::eTOUCH or #PxQueryHitType::eBLOCK, precise testing is performed to 
determine intersection point(s).

The prefilter may overwrite the copy of queryFlags passed in the query's function call, in order to specify #PxHitFlag::eMESH_BOTH_SIDES and 
#PxHitFlag::eMESH_MULTIPLE on a per-shape basis. Changes apply only to the shape being filtered, and changes to other flags are ignored.

\li If #PxQueryFlag::ePREFILTER is not set, precise intersection testing is performed with the 
#PxHitFlag::eMESH_BOTH_SIDES and #PxHitFlag::eMESH_MULTIPLE flags from the original queryFlags.

\li If the #PxQueryFlag::ePOSTFILTER flag is set, the postFilter function is called for each intersection
point to determine touch/block status. This overrides any touch/block status returned from the preFilter function for this shape.

Filtering calls are not in order along the query direction, rather they are processed in the order in which
candidate shapes for testing are found by PhysX' scene traversal algorithms.

@see PxScene.raycastAny PxScene.raycastSingle PxScene.raycastMultiple PxScene.sweepSingle PxScene.sweepMultiple PxScene.overlapMultiple
*/
class PxQueryFilterCallback
{
public:

	/**
	\brief Filter callback before precise intersection testing.

	\param[in] filterData The custom filter data of the query
	\param[in] shape The potentially hit shape
	\param[in] actor The shape's actor
	\param[in,out] queryFlags The scene query flags from the query's function call (only the eMODIFIABLE_FLAGS flags can be modified)

	@see PxQueryFilterCallback
	*/
	virtual PxQueryHitType::Enum preFilter(const PxFilterData& filterData, const PxShape* shape, const PxRigidActor* actor, PxHitFlags& queryFlags) = 0;

	/**
	\brief Filter callback after precise intersection testing.

	\param[in] filterData The custom filter data of the query
	\param[in] hit Scene query hit information. For overlap tests the faceIndex member is not valid. For sweepSingle/sweepMultiple and raycastSingle/raycastMultiple the hit information can be casted to #PxSweepHit and #PxRaycastHit respectively
	\return Hit declaration.

	@see PxQueryFilterCallback
	*/
	virtual PxQueryHitType::Enum postFilter(const PxFilterData& filterData, const PxQueryHit& hit) = 0;

	/**
	\brief virtual destructor
	*/	
	virtual ~PxQueryFilterCallback() {}
};

/** \brief Deprecated define for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryFilterCallback PxQueryFilterCallback

/**
\brief Batched query prefiltering shader.

Custom filtering logic for batched query intersection candidates. If an intersection candidate object passes the data based filter (see #PxQueryFilterData),
the filtering shader run on request (see #PxQueryFilterData.flags)

\li If #PxQueryFlag::ePREFILTER is set, the #PxBatchQueryPreFilterShader runs before precise intersection
testing. If this function returns #PxQueryHitType::eTOUCH or #PxQueryHitType::eBLOCK, precise testing is performed to 
determine intersection point(s).

The #PxBatchQueryPreFilterShader may overwrite the copy of queryFlags passed in, in order to specify #PxHitFlag::eMESH_BOTH_SIDES and 
#PxHitFlag::eMESH_MULTIPLE on a per-shape basis. Changes apply only to the shape being filtered, and changes to other flags are ignored.

\li If #PxQueryFlag::ePREFILTER is not set, precise intersection testing is performed with the 
#PxHitFlag::eMESH_BOTH_SIDES and #PxHitFlag::eMESH_MULTIPLE flags from the original queryFlags.

Filtering shaders are not in order along the query direction, rather they are processed in the order in which
candidate shapes for testing are found by PhysX' scene traversal algorithms.

@see PxBatchQueryDesc.preFilterShader PxQueryFilterCallback.preFilter PxBatchQueryPostFilterShader

*/

/**
\param[in] filterData0 The custom filter data of the query
\param[in] filterData1 The custom filter data of the second object
\param[in] constantBlock The constant global filter data (see #PxBatchQuery)
\param[in] constantBlockSize Size of the global filter data (see #PxBatchQuery)
\param[in,out] queryFlags The scene query flags from the query's function call (only the eMODIFIABLE_FLAGS flags can be modified)
\return Hit declaration.

@see PxBatchQueryPostFilterShader 
*/
typedef PxQueryHitType::Enum (*PxBatchQueryPreFilterShader)(	
	PxFilterData filterData0, 
	PxFilterData filterData1,
	const void* constantBlock, PxU32 constantBlockSize,
	PxHitFlags& queryFlags);

/**
\brief Batched query postfiltering shader.

Custom filtering logic for batched query intersection candidates. If an intersection candidate object passes the data based filter (see #PxQueryFilterData),
the filtering shader run on request (see #PxQueryFilterData.flags)

\li If the #PxQueryFlag::ePOSTFILTER flag is set, the #PxBatchQueryPostFilterShader function is called for each intersection
point to determine touch/block status. This overrides any touch/block status returned from the #PxBatchQueryPreFilterShader for this shape.

Filtering shaders are not in order along the query direction, rather they are processed in the order in which
candidate shapes for testing are found by PhysX' scene traversal algorithms.

@see PxBatchQueryDesc.postFilterShader PxQueryFilterCallback.postFilter PxBatchQueryPreFilterShader
*/

/**
\param[in] filterData0 The custom filter data of the query
\param[in] filterData1 The custom filter data of the shape
\param[in] constantBlock The constant global filter data (see #PxBatchQuery)
\param[in] constantBlockSize Size of the global filter data (see #PxBatchQuery)
\param[in] hit declaration.

@see PxBatchQueryPreFilterShader 
*/

typedef PxQueryHitType::Enum (*PxBatchQueryPostFilterShader)(
	PxFilterData filterData0,
	PxFilterData filterData1,
	const void* constantBlock, PxU32 constantBlockSize,
	const PxQueryHit& hit);

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
