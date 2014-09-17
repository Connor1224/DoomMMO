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


#ifndef PX_PHYSICS_NX_SCENEQUERYREPORT
#define PX_PHYSICS_NX_SCENEQUERYREPORT
/** \addtogroup scenequery
@{
*/
#include "PxPhysXConfig.h"
#include "foundation/PxVec3.h"
#include "foundation/PxFlags.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxShape;
class PxRigidActor;

/**
\brief Scene query specification.

A PxHitFlag parameter to a scene query call determines which optional fields in the output #PxQueryHit are to be filled in.
In #PxQueryHit it is used to inform which optional fields have been filled in.

The PxHitFlag::eINITIAL_OVERLAP_DISABLE controls the query behavior when the swept shape initially overlaps with a shape of the scene.
By default, if the flag is not set, additional overlap tests are performed to detect the initial overlaps. If you can guarantee your swept shape
starts from an empty portion of space, it is best (more efficient) to enable that flag.

In case an initial overlap is detected, the corresponding shape is kept, returned distance is set to zero, and returned normal is set to
the opposite of the sweep direction. You can then filter out these initially overlapping shapes in the post filter callback or post filter shader,
using the PxSweepHit::hadInitialOverlap() function.

@see PxQueryHit PxScene.raycastSingle PxScene.raycastMultiple, PxScene.sweepSingle, PxScene.sweepMultiple
*/
struct PxHitFlag
{
	enum Enum
	{
		ePOSITION					= (1<<0),	//!< "position" member of #PxQueryHit is valid
		eIMPACT						= ePOSITION,//!< Deprecated alias PX_DEPRECATED
		eNORMAL						= (1<<1),	//!< "normal" member of #PxQueryHit is valid
		eDISTANCE					= (1<<2),	//!< "distance" member of #PxQueryHit is valid
		eUV							= (1<<3),	//!< "u" and "v" barycentric coordinates of #PxQueryHit are valid. Not applicable to sweep queries.
		eINITIAL_OVERLAP_DISABLE	= (1<<4),	//!< Enable/disable initial overlap tests in sweeps.
												//!< Disabling the initial test may provide a performance boost,
												//!< but objects which are initially overlapping may not be reported by the sweep.
		eMESH_MULTIPLE				= (1<<5),	//!< Report all hits for meshes rather than just the first.
												//!< On SPU the number of reported hits per mesh is limited to 16 in no specific order.
		eMESH_ANY					= (1<<6),	//!< Report any first hit for meshes. If neither eMESH_MULTIPLE or eMESH_ANY is specified,
												//!< a single closest hit will be reported for meshes.
		eMESH_BOTH_SIDES			= (1<<7),	//!< Report hits with back faces of triangles. Also report hits for raycast
												//!< originating on mesh surface and facing away from the surface normal.
		ePRECISE_SWEEP				= (1<<8),	//!< Use more accurate sweep function, but slower. Ignored on SPU.

		eDEFAULT					= ePOSITION|eNORMAL|eDISTANCE,

		/** \brief Only this subset of flags can be modified by pre-filter. Other modifications will be discarded. */
		eMODIFIABLE_FLAGS			= eMESH_MULTIPLE|eMESH_BOTH_SIDES|eINITIAL_OVERLAP_DISABLE|ePRECISE_SWEEP
	};
};


/**
\brief collection of set bits defined in PxHitFlag.

@see PxHitFlag
*/
PX_FLAGS_TYPEDEF(PxHitFlag, PxU16)

/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryFlag PxHitFlag // PX_DEPRECATED
/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryFlags PxHitFlags // PX_DEPRECATED

/**
\brief Combines a shape pointer and the actor the shape belongs to into one memory location.

Used with PxVolumeCache iterator and serves as a base class for PxQueryHit.

@see PxVolumeCache PxQueryHit
*/
struct PxActorShape
{
	PX_INLINE PxActorShape() : actor(NULL), shape(NULL) {}
	PX_INLINE PxActorShape(PxRigidActor* a, PxShape* s) : actor(a), shape(s) {}

	PxRigidActor*	actor;
	PxShape*		shape;
};


/**
\brief Scene query hit information.

Not all members of the PxQueryHit structure are always valid. For example when the query hits a sphere,
the faceIndex member does not get computed.
*/
struct PxQueryHit : PxActorShape
{
	PX_INLINE			PxQueryHit() : faceIndex(0xffffffff) {}

	/**
	Face index of touched triangle, for triangle meshes, convex meshes and height fields.

	\note This index is not currently filled by overlap queries.
	\note This index is remapped by mesh cooking, use #PxTriangleMesh::getTrianglesRemap() to convert to original mesh index.
	\note For convex meshes use #PxConvexMesh::getPolygonData() to retrieve touched polygon data.
	*/
	PxU32				faceIndex;
};

/** \deprecated Deprecated definition for backwards compatibility with PhysX 3.2 */
#define PxSceneQueryHit PxQueryHit

/**
\brief Scene query hit information for raycasts and sweeps returning hit position and normal information.

::PxHitFlag flags can be passed to scene query functions, as an optimization, to cause the SDK to
only generate specific members of this structure.
*/
struct PxLocationHit : public PxQueryHit
{
	PX_INLINE			PxLocationHit() : flags(0), position(PxVec3(0)), normal(PxVec3(0)), distance(PX_MAX_REAL)	{}

	// the following fields are set in accordance with the #PxHitFlags
	PxHitFlags			flags;		//!< Hit flags specifying which members contain valid values.
	PxVec3				position;	//!< World-space hit position (flag: #PxHitFlag::ePOSITION)
									//!< Formerly named .impact, renamed for clarity. PX_DEPRECATED
	PxVec3				normal;		//!< World-space hit normal (flag: #PxHitFlag::eNORMAL)
	PxF32				distance;	//!< Distance to hit (flag: #PxHitFlag::eDISTANCE)
};

/**
\brief Stores results of sweep queries.

See #PxBatchQuery and #PxScene for sweep methods.
*/
struct PxSweepHit : public PxLocationHit
{
	PX_INLINE			PxSweepHit() {}
	PX_INLINE bool		hadInitialOverlap() const { return (distance == 0.0f); } //!< True for initially overlapping shapes

	PxU32				padTo16Bytes;
};


/**
\brief Stores results of overlap queries.

See #PxBatchQuery and #PxScene for overlap methods.
*/
struct PxOverlapHit: public PxQueryHit { PxU32 padTo16Bytes; };

/**
\brief Stores results of raycast queries.

::PxHitFlag flags can be passed to raycast function, as an optimization, to cause the SDK to only compute specified members of this
structure.

Some members like barycentric coordinates are currently only computed for triangle meshes and convexes, but next versions
might provide them in other cases. The client code should check #flags to make sure returned values are valid.

@see PxBatchQuery.raycastMultiple PxBatchQuery.raycastSingle PxScene.raycastAny PxScene.raycastSingle PxScene.raycastMultiple
*/
struct PxRaycastHit : public PxLocationHit
{
	PX_INLINE			PxRaycastHit() : u(0.0f), v(0.0f)	{}

	// the following fields are set in accordance with the #PxHitFlags

	PxReal	u, v;			//!< barycentric coordinates of hit point, for triangle mesh and height field (flag: #PxHitFlag::eUV)
#ifndef PX_X64
	PxU32	pad[3];
#endif

};

/**
\brief Describes query behavior after returning a partial query result via a callback.

If callback returns true, traversal will continue and callback can be issued again.
If callback returns false, traversal will stop, callback will not be issued again.

@see PxHitCallback
*/
typedef bool PxAgain;

/**
\brief	This callback class is used for reporting scene query hits (intersections) to the user.

User overrides the virtual processTouches function to receive hits in (possibly multiple) fixed size blocks.

\note	PxHitBuffer derives from this class and is used to receive touching hits in a fixed size buffer.
\note	Since the compiler doesn't look in template dependent base classes when looking for non-dependent names
\note	with some compilers it will be necessary to use "this->hasBlock" notation to access a parent variable
\note	in a child callback class.
\note	Pre-made typedef shorthands, such as ::PxRaycastCallback can be used for raycast, overlap and sweep queries.

@see PxHitBuffer PxRaycastHit PxSweepHit PxOverlapHit PxRaycastCallback PxOverlapCallback PxSweepCallback
*/
template<typename HitType>
struct PxHitCallback
{
	HitType		block;			//<! Holds the closest blocking hit result for the query. Invalid if hasBlock is false.
	bool		hasBlock;		//<! Set to true if there was a blocking hit during query.

	HitType*	touches;		//<! User specified buffer for touching hits.

	/**
	\brief	Size of the user specified touching hits buffer.
	\note	If set to 0 all hits will default to PxQueryHitType::eBLOCK, otherwise to PxQueryHitType::eTOUCH
	\note	Hit type returned from pre-filter overrides this default */
	PxU32		maxNbTouches;

	/**
	\brief	Number of touching hits returned by the query. Used with PxHitBuffer.
	\note	If true (PxAgain) is returned from the callback, nbTouches will be reset to 0. */
	PxU32		nbTouches;

	/**
	\brief	Initializes the class with user provided buffer.

	\param[in] aTouches			Optional buffer for recording PxQueryHitType::eTOUCH type hits.
	\param[in] aMaxNbTouches	Size of touch buffer.

	\note	if aTouches is NULL and aMaxNbTouches is 0, only the closest blocking hit will be recorded by the query.
	\note	If PxQueryFlag::eANY_HIT flag is used as a query parameter, hasBlock will be set to true and blockingHit will be used to receive the result.
	\note	Both eTOUCH and eBLOCK hits will be registered as hasBlock=true and stored in PxHitCallback.block when eANY_HIT flag is used.

	@see PxHitCallback.hasBlock PxHitCallback.block */
	PxHitCallback(HitType* aTouches, PxU32 aMaxNbTouches)
		: hasBlock(false), touches(aTouches), maxNbTouches(aMaxNbTouches), nbTouches(0)
	{}

	/**
	\brief virtual callback function used to communicate query results to the user.

	This callback will always be invoked with aTouches as a buffer if aTouches was specified as non-NULL.
	All reported touch hits are guaranteed to be closer than the closest blocking hit.

	\param[in]	buffer	Callback will report touch hits to the user in this buffer. This pointer will be the same as aTouches parameter.
	\param[in]	nbHits	Number of touch hits reported in buffer. This number will not exceed aMaxNbTouches constructor parameter.

	\note	There is a significant performance penalty in case multiple touch callbacks are issued (up to 2x)
	\note	to avoid the penalty use a bigger buffer so that all touching hits can be reported in a single buffer.
	\note	If true (again) is returned from the callback, nbTouches will be reset to 0,
	\note	If false is returned, nbTouched will remain unchanged.
	\note	By the time processTouches is first called, the globally closest blocking hit is already determined,
	\note	values of hasBlock and block are final and all touch hits are guaranteed to be closer than the blocking hit.
	\note	touches and maxNbTouches can be modified inside of processTouches callback.

	\return	true to continue receiving callbacks in case there are more hits or false to stop.

	@see PxAgain PxRaycastHit PxSweepHit PxOverlapHit */
	virtual PxAgain processTouches(const HitType* buffer, PxU32 nbHits) = 0;

	virtual void finalizeQuery() {} //<! Query finalization callback, called after the last processTouches callback.

	virtual ~PxHitCallback() {}
};

/**
\brief	Returns scene query hits (intersections) to the user in a preallocated buffer.

Will clip touch hits to maximum buffer capacity. When clipped, an arbitrary subset of touching hits will be discarded.
Overflow does not trigger a warning or an error. block and hasBlock will be valid in finalizeQuery callback and after completion.
Touching hits are guaranteed to be closer than the globally nearest blocking hit at the time any processTouches()
callback is issued.

\note	Pre-made typedef shorthands, such as ::PxRaycastBuffer can be used for raycast, overlap and sweep queries.

@see PxHitCallback
@see PxRaycastBuffer PxOverlapBuffer PxSweepBuffer PxRaycastBufferN PxOverlapBufferN PxSweepBufferN
*/
template<typename HitType>
struct PxHitBuffer : public PxHitCallback<HitType>
{
	/**
	\brief	Initializes the buffer with user memory.

	The buffer is initialized with 0 touch hits by default => query will only report a single closest blocking hit.
	Use PxQueryFlag::eANY_HIT to tell the query to abort and return any first hit encoutered as blocking.

	\param[in] aTouches			Optional buffer for recording PxQueryHitType::eTOUCH type hits.
	\param[in] aMaxNbTouches	Size of touch buffer.

	@see PxHitCallback */
	PxHitBuffer(HitType* aTouches = NULL, PxU32 aMaxNbTouches = 0) : PxHitCallback<HitType>(aTouches, aMaxNbTouches) {}

	/** \brief Computes the number of any hits in this result, blocking or touching. */
	PX_INLINE PxU32				getNbAnyHits() const				{ return getNbTouches() + PxU32(this->hasBlock); }
	/** \brief Convenience iterator used to access any hits in this result, blocking or touching. */
	PX_INLINE const HitType&	getAnyHit(const PxU32 index) const	{ PX_ASSERT(index < getNbTouches() + PxU32(this->hasBlock));
																		return index < getNbTouches() ? getTouches()[index] : this->block; }

	PX_INLINE PxU32				getNbTouches() const				{ return this->nbTouches; }
	PX_INLINE const HitType*	getTouches() const					{ return this->touches; }
	PX_INLINE const HitType&	getTouch(const PxU32 index) const	{ PX_ASSERT(index < getNbTouches()); return getTouches()[index]; }
	PX_INLINE PxU32				getMaxNbTouches() const				{ return this->maxNbTouches; }

	virtual ~PxHitBuffer() {}

protected:
	// stops after the first callback
	virtual PxAgain processTouches(const HitType* buffer, PxU32 nbHits) { PX_UNUSED(buffer); PX_UNUSED(nbHits); return false; }
};


/** \brief Raycast query callback. */
typedef PxHitCallback<PxRaycastHit> PxRaycastCallback;

/** \brief Overlap query callback. */
typedef PxHitCallback<PxOverlapHit> PxOverlapCallback;

/** \brief Sweep query callback. */
typedef PxHitCallback<PxSweepHit> PxSweepCallback;

/** \brief Raycast query buffer. */
typedef PxHitBuffer<PxRaycastHit> PxRaycastBuffer;

/** \brief Overlap query buffer. */
typedef PxHitBuffer<PxOverlapHit> PxOverlapBuffer;

/** \brief Sweep query buffer. */
typedef PxHitBuffer<PxSweepHit> PxSweepBuffer;

/** \brief	Returns touching raycast hits to the user in a fixed size array embedded in the buffer class. **/
template <int N>
struct PxRaycastBufferN : PxHitBuffer<PxRaycastHit>
{
	PxRaycastHit hits[N];
	PxRaycastBufferN() : PxHitBuffer<PxRaycastHit>(hits, N) {}
};

/** \brief	Returns touching overlap hits to the user in a fixed size array embedded in the buffer class. **/
template <int N>
struct PxOverlapBufferN : PxHitBuffer<PxOverlapHit>
{
	PxOverlapHit hits[N];
	PxOverlapBufferN() : PxHitBuffer<PxOverlapHit>(hits, N) {}
};

/** \brief	Returns touching sweep hits to the user in a fixed size array embedded in the buffer class. **/
template <int N>
struct PxSweepBufferN : PxHitBuffer<PxSweepHit>
{
	PxSweepHit hits[N];
	PxSweepBufferN() : PxHitBuffer<PxSweepHit>(hits, N) {}
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
