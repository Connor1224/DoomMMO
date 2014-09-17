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


#ifndef PX_PHYSICS_METADATA_H
#define PX_PHYSICS_METADATA_H
/** \addtogroup physics
@{
*/

#include "foundation/Px.h"
#include "PxMetaDataFlags.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

	class PxOutputStream;

	/**
	\brief Struct to store meta data definitions.

	Note: The individual fields have different meaning depending on the meta data type.
	*/
	struct PxMetaDataEntry
	{
		const char*		mType;			//!< Field type (bool, byte, quaternion, etc)
		const char*		mName;			//!< Field name (appears exactly as in the source file)
		PxU32			mOffset;		//!< Offset from the start of the class (ie from "this", field is located at "this"+Offset)
		PxU32			mSize;			//!< sizeof(Type)
		PxU32			mCount;			//!< Number of items of type Type (0 for dynamic sizes)
		PxU32			mOffsetSize;	//!< Offset of dynamic size param, for dynamic arrays
		PxU32			mFlags;			//!< Field parameters
		PxU32			mAlignment;		//!< Explicit alignment added for DE1340
	};

    #define PX_STORE_ENTRY	stream.write(&tmp, sizeof(PxMetaDataEntry))

	//define a binary metadata entry for the member variable of the class 
	#define PX_DEF_BIN_METADATA_ITEM(Class, type, name, flags)	\
	{ PxMetaDataEntry tmp = { #type, #name, (PxU32)PX_OFFSET_OF(Class, name), PX_SIZE_OF(Class, name), 1, 0, flags, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the member array variable of the class, specify the array lenght.
	#define PX_DEF_BIN_METADATA_ITEMS(Class, type, name, flags, count)	\
	{ PxMetaDataEntry tmp = { #type, #name, (PxU32)PX_OFFSET_OF(Class, name), PX_SIZE_OF(Class, name), count, 0, flags, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the member array variable of the class, automatic detection the array length 
	#define PX_DEF_BIN_METADATA_ITEMS_FIXED_SIZE(Class, type, name, flags)	\
	{ PxMetaDataEntry tmp = { #type, #name, (PxU32)PX_OFFSET_OF(Class, name), PX_SIZE_OF(Class, name), sizeof(((Class*)0)->name)/sizeof(type), 0, flags, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for padding data of the class
	#define PX_DEF_BIN_METADATA_JUNK(Class, count)	\
	{ PxMetaDataEntry tmp = { "PxU8", "Junk", 0, count, count, 0, PxMetaDataFlag::ePADDING, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the class
	#define PX_DEF_BIN_METADATA_CLASS(Class)	\
	{ PxMetaDataEntry tmp = { #Class, 0, 0, sizeof(Class), 0, 0, PxMetaDataFlag::eCLASS, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the virtual class
	#define PX_DEF_BIN_METADATA_VCLASS(Class)	\
	{ PxMetaDataEntry tmp = { #Class, 0, 0, sizeof(Class), 0, 0, PxMetaDataFlag::eCLASS|PxMetaDataFlag::eVIRTUAL, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the type
	#define PX_DEF_BIN_METADATA_TYPEDEF(newType, oldType)	{					\
		PX_COMPILE_TIME_ASSERT(sizeof(newType)==sizeof(oldType));	\
		PxMetaDataEntry tmp = { #newType, #oldType, 0, 0, 0, 0, PxMetaDataFlag::eTYPEDEF, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the base class
	#define PX_DEF_BIN_METADATA_BASE_CLASS(Class, BaseClass)														\
	{																									\
		Class* myClass = reinterpret_cast<Class*>(42);													\
		BaseClass* s = static_cast<BaseClass*>(myClass);												\
		const PxU32 offset = PxU32(size_t(s) - size_t(myClass));										\
		PxMetaDataEntry tmp = { #Class, #BaseClass, offset, sizeof(Class), 0, 0, PxMetaDataFlag::eCLASS, 0 };	\
		PX_STORE_ENTRY;																					\
	}

	//define a binary metadata entry for the union data 
	#define PX_DEF_BIN_METADATA_UNION(Class, name)	\
	{ PxMetaDataEntry tmp = { #Class, 0, (PxU32)PX_OFFSET_OF(Class, name), PX_SIZE_OF(Class, name), 1, 0, PxMetaDataFlag::eUNION, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the union data type
	#define PX_DEF_BIN_METADATA_UNION_TYPE(Class, type, enumValue)	\
	{ PxMetaDataEntry tmp = { #Class, #type, enumValue, 0, 0, 0, PxMetaDataFlag::eUNION, 0 }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the extra data of the class
	#define PX_DEF_BIN_METADATA_EXTRA_ITEM(Class, type, control, align)	\
	{ PxMetaDataEntry tmp = { #type, 0, (PxU32)PX_OFFSET_OF(Class, control), sizeof(type), 0, (PxU32)PX_SIZE_OF(Class, control), PxMetaDataFlag::eEXTRA_DATA|PxMetaDataFlag::eEXTRA_ITEM, align }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the array extra data of the class, with control variable
	#define PX_DEF_BIN_METADATA_EXTRA_ITEMS(Class, type, control, count, flags, align)	\
	{ PxMetaDataEntry tmp = { #type, 0, (PxU32)PX_OFFSET_OF(Class, control), (PxU32)PX_SIZE_OF(Class, control), (PxU32)PX_OFFSET_OF(Class, count), (PxU32)PX_SIZE_OF(Class, count), PxMetaDataFlag::eEXTRA_DATA|PxMetaDataFlag::eEXTRA_ITEMS|flags, align }; PX_STORE_ENTRY;	}
	
	//define a binary metadata entry for the array extra data of the class, specify the flag.
	#define PX_DEF_BIN_METADATA_EXTRA_ITEMS_FLAGED(Class, type, control, controlFlag ,count, flags, align)	\
	{ PxMetaDataEntry tmp = { #type, 0, (PxU32)PX_OFFSET_OF(Class, control), (PxU32)PX_SIZE_OF(Class, control), (PxU32)PX_OFFSET_OF(Class, count), (PxU32)PX_SIZE_OF(Class, count), PxMetaDataFlag::eFLAG_CONTROL|PxMetaDataFlag::eEXTRA_DATA|PxMetaDataFlag::eEXTRA_ITEMS|flags| ((controlFlag << 16) & PxMetaDataFlag::eFLAG_CONTROL_MASK), align }; PX_STORE_ENTRY;	}

	//define a binary metadata entry for the array extra data of the class
	#define PX_DEF_BIN_METADATA_EXTRA_ARRAY(Class, type, dyn_count, align, flags)	\
	{ PxMetaDataEntry tmp = { #type, 0, (PxU32)PX_OFFSET_OF(Class, dyn_count), PX_SIZE_OF(Class, dyn_count), align, 0, PxMetaDataFlag::eEXTRA_DATA|flags, align }; PX_STORE_ENTRY;	}
	
	//define a binary metadata entry for a aligment
	#define PX_DEF_BIN_METADATA_EXTRA_ALIGN(Class, align)	\
	{ PxMetaDataEntry tmp = { "PxU8", "Alignment", 0, 0, 0, 0, PxMetaDataFlag::eEXTRA_DATA|PxMetaDataFlag::eALIGNMENT, align }; PX_STORE_ENTRY;	}

	#define PX_EXPLICIT_PADDING(x)

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
