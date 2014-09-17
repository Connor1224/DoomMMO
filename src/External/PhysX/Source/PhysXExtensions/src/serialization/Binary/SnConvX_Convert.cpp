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

#include "SnConvX.h"
#include "SnConvX_Convert.h"
#include "PxErrorCallback.h"
#include "PxSerializer.h"
#include "../SnSerializationRegistry.h"
#include "PsAlloca.h"

using namespace physx;
using namespace physx::Sn;
using namespace Cm;

static const bool gIncludeSentinels = false;

// ---- heightfield surgery ----
#define HF_TILE_SIZE_U (4)
#define HF_TILE_SIZE_V (4)

static char* convertToTiled(const char* src, int sizeOfSample, int nbColumns, int nbRows, int columnsPadded, int rowsPadded, int tilesU, int tilesV)
{
	char* samplesTiled = (char*)PX_ALLOC(sizeof(char)*columnsPadded*rowsPadded*sizeOfSample, PX_DEBUG_EXP("samplesTiled"));

	for(int i=0;i<tilesV;i++)
	{
		for(int j=0;j<tilesU;j++)
		{
			const int tileBase = (i*tilesU+j)*HF_TILE_SIZE_V*HF_TILE_SIZE_U;

			for(int v=0;v<HF_TILE_SIZE_V;v++)
			{
				for(int u=0;u<HF_TILE_SIZE_U;u++)
				{
					const int tileOffset = tileBase+v*HF_TILE_SIZE_U+u;

					if ((j < tilesU-1 || u < HF_TILE_SIZE_U-(columnsPadded - nbColumns)) &&
						(i < tilesV-1 || v < HF_TILE_SIZE_V-(rowsPadded - nbRows)))
					{
						const int offset2 = (i*HF_TILE_SIZE_V+v)*nbColumns+(j*HF_TILE_SIZE_U+u);

						assert(tileOffset<columnsPadded*rowsPadded);
						assert(offset2<nbColumns*nbRows);
						memcpy(samplesTiled + tileOffset*sizeOfSample, src + offset2*sizeOfSample, sizeOfSample);
						//						samplesTiled[tileOffset] = src[offset2];
					}
					else
					{
						assert(tileOffset<columnsPadded*rowsPadded);
						memset(samplesTiled + tileOffset*sizeOfSample, 0, sizeOfSample);
						//						samplesTiled[tileOffset] = emptySample;
					}
				}
			}
		}
	}
	return samplesTiled;
}
// ---- heightfield surgery ----

void Sn::ConvX::resetConvexFlags()
{
	mConvexFlags.clear();
	mHeightFields.clear();
}

void Sn::ConvX::_enumerateFields(const MetaClass* mc, ExtraDataEntry2* entries, int& nb, int baseOffset, MetaDataType type) const
{
	int nbFields = (int)mc->mFields.size();
	int offsetCheck = baseOffset;
	for(int j=0;j<nbFields;j++)
	{
		const PxMetaDataEntry& entry = mc->mFields[j];
		if(entry.mFlags & PxMetaDataFlag::eCLASS || entry.mFlags & PxMetaDataFlag::eEXTRA_DATA)
			continue;

		assert(offsetCheck == baseOffset + entry.mOffset);

		int currentOffset = baseOffset + entry.mOffset;

		//for(int c=0;c<entry.mCount;c++)
		{
			if(entry.mFlags & PxMetaDataFlag::eUNION)
			{
				entries[nb].entry	= entry;
				entries[nb].offset	= currentOffset;
				entries[nb].cb		= 0;
				nb++;
			}
			else if(entry.mFlags & PxMetaDataFlag::ePTR)	// This also takes care of the vtable pointer
			{
				entries[nb].entry	= entry;
				entries[nb].offset	= currentOffset;
				entries[nb].cb		= &Sn::ConvX::convertPtr;
				nb++;
			}
			else
			{
				MetaClass* fieldType = getMetaClass(entry.mType, type);
				assert(fieldType);
				if(fieldType->mCallback)
				{
					entries[nb].entry	= entry;
					entries[nb].offset	= currentOffset;
					entries[nb].cb		= fieldType->mCallback;
					nb++;
				}
				else
				{
					for(int c=0;c<entry.mCount;c++)
					{
						_enumerateFields(fieldType, entries, nb, currentOffset, type);
						currentOffset += entry.mSize/entry.mCount;
					}
				}
			}
		}
		offsetCheck += entry.mSize;
	}
}

void Sn::ConvX::_enumerateExtraData(const char* address, const MetaClass* mc, ExtraDataEntry* entries, int& nb, int offset, MetaDataType type) const
{
	int nbFields = (int)mc->mFields.size();
	for(int j=0;j<nbFields;j++)
	{
		const PxMetaDataEntry& entry = mc->mFields[j];
		if(entry.mFlags & PxMetaDataFlag::eCLASS /*|| entry.mFlags & PxMetaDataFlag::ePTR*/ || entry.mFlags & PxMetaDataFlag::eTYPEDEF)
			continue;

		const char* entryType = entry.mType;

		//
		// Insanely Twisted Shadow GeometryUnion
		//
		// Special code is needed as long as there are no meta data tags to describe our unions properly. The way it is done here is
		// not future-proof at all. There should be a tag to describe where the union type can be found and the number of bytes
		// this type id needs. Then a mapping needs to get added from each union type id to the proper meta class name.
		//
		if (entry.mFlags & PxMetaDataFlag::eUNION)
		{
			if (!mc->mClassName || strcmp(mc->mClassName, "Gu::GeometryUnion")!=0)
				continue;
			else
			{
				// ### hardcoded bit here, will only work when union type is the first int of the struct
				const int* tmp = (const int*)(address + offset);
				const int unionType = *tmp;

				ConvX* tmpConv = const_cast<ConvX*>(this);  // ... don't ask
				const char* typeName = tmpConv->getTypeName(entry.mType, unionType);
				assert(typeName);

				bool isTriMesh = (strcmp(typeName, "PxTriangleMeshGeometryLL") == 0);
				bool isHeightField = (strcmp(typeName, "PxHeightFieldGeometryLL") == 0);
				if (!isTriMesh && !isHeightField)
				{
					continue;
				}
				else
				{
					entryType = typeName;
				}
			}
		}

		//		MetaClass* extraDataType = getMetaClass(entry.mType, type);
		//		if(!extraDataType)
		//			continue;

		if(entry.mFlags & PxMetaDataFlag::eEXTRA_DATA)
		{
			entries[nb].entry = entry;
			entries[nb].offset = offset+entry.mOffset;
			nb++;
		}
		else
		{
			if(entry.mFlags & PxMetaDataFlag::ePTR)
				continue;

			MetaClass* extraDataType = getMetaClass(entryType, type);
			if(!extraDataType)
				continue;

			if(!extraDataType->mCallback)
				_enumerateExtraData(address, extraDataType, entries, nb, offset+entry.mOffset, type);
		}
	}
}

/*
static short read16(const void*& buffer)
{
const short* buf16 = reinterpret_cast<const short*>(buffer);
buffer = (const void*)(size_t(buffer) + sizeof(short));
short value = *buf16;
output(value);
return value;
}
*/

int Sn::ConvX::read32(const void*& buffer)
{
	const int* buf32 = reinterpret_cast<const int*>(buffer);
	buffer = (const void*)(size_t(buffer) + sizeof(int));
	int value = *buf32;
	output(value);
	return value;
}

#ifdef PX_DEBUG
extern const char* gVTable;
static bool compareEntries(const ExtraDataEntry2& e0, const ExtraDataEntry2& e1)
{
	if(e0.entry.isVTablePtr() && e1.entry.isVTablePtr())
		return true;

	if((e0.entry.mFlags & PxMetaDataFlag::eUNION) && (e1.entry.mFlags & PxMetaDataFlag::eUNION))
	{
		assert(e0.entry.mType);
		assert(e1.entry.mType);
		// We can't compare the ptrs since they index different string tables
		if(strcmp(e0.entry.mType, e1.entry.mType)==0)
			return true;
		assert(0);
		return false;
	}

	assert(e0.entry.mName);
	assert(e1.entry.mName);
	// We can't compare the ptrs since they index different string tables
	if(strcmp(e0.entry.mName, e1.entry.mName)==0)
		return true;

	return false;
}
#endif

#define PRINT_CONVERT_CLASS_INFORMATION 0

// TODO: optimize this
void Sn::ConvX::convertClass(const char* buffer, const MetaClass* mc, int offset)
{
	// ---- big convex surgery ----
	bool convexSurgery = false;
	bool foundNbVerts = false;
	bool removeBigData = false;

	// force reference
	(void)foundNbVerts;


#if PRINT_CONVERT_CLASS_INFORMATION
	printf("%s\n", mc->mClassName);
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
#endif

	if(strcmp(mc->mClassName, "ConvexMesh")==0)
	{
		convexSurgery = true;
	}
	// ---- big convex surgery ----

	// ---- heightfield surgery ----
	bool heightfieldSurgery = false;
	Sn::HeightFieldData hfData;
	if(strcmp(mc->mClassName, "HeightField")==0)
	{
		if(getBinaryMetaData(META_DATA_DST)->getHFTiling())
			heightfieldSurgery = true;
	}
	// ---- heightfield surgery ----


	int nbSrcEntries = 0;
	PX_ALLOCA(srcEntries, ExtraDataEntry2, 256);  // ### painful ctors here
	int nbDstEntries = 0;
	PX_ALLOCA(dstEntries, ExtraDataEntry2, 256);  // ### painful ctors here
	// Find corresponding meta-class for target platform
	const MetaClass* target_mc = getMetaClass(mc->mClassName, META_DATA_DST);
	assert(target_mc);

	if(mc->mCallback)
	{
		srcEntries[0].cb			= mc->mCallback;
		srcEntries[0].offset		= offset;
		srcEntries[0].entry.mType	= mc->mClassName;
		srcEntries[0].entry.mName	= mc->mClassName;
		srcEntries[0].entry.mOffset	= offset;
		srcEntries[0].entry.mSize	= mc->mSize;
		srcEntries[0].entry.mCount	= 1;
		srcEntries[0].entry.mFlags	= 0;
		nbSrcEntries = 1;

		assert(target_mc->mCallback);
		dstEntries[0].cb			= target_mc->mCallback;
		dstEntries[0].offset		= offset;
		dstEntries[0].entry.mType	= target_mc->mClassName;
		dstEntries[0].entry.mName	= target_mc->mClassName;
		dstEntries[0].entry.mOffset	= offset;
		dstEntries[0].entry.mSize	= target_mc->mSize;
		dstEntries[0].entry.mCount	= 1;
		dstEntries[0].entry.mFlags	= 0;
		nbDstEntries = 1;
	}
	else
	{
		nbSrcEntries = 0;
		_enumerateFields(mc, srcEntries, nbSrcEntries, 0, META_DATA_SRC);
		assert(nbSrcEntries<256);

		nbDstEntries = 0;
		_enumerateFields(target_mc, dstEntries, nbDstEntries, 0, META_DATA_DST);
		assert(nbDstEntries<256);

		//		nb = mc->mNbEntries;
		//		assert(nb>=0);
		//		memcpy(entries, mc->mEntries, nb*sizeof(ExtraDataEntry2));
	}

	int srcOffsetCheck = 0;
	int dstOffsetCheck = 0;
	int j = 0;
	// Track cases where the vtable pointer location is different for different platforms.
	// The variables indicate whether a platform has a vtable pointer entry that has not been converted yet
	// and they will remember the index of the corrssponding entry. This works because there can only
	// be one open vtable pointer entry at a time.
	int srcOpenVTablePtrEntry = -1;
	int dstOpenVTablePtrEntry = -1;
	int addVTablePtrShiftIteration = 0;  // if the src and dst platform place the vtable pointers at different locations some fiddling with the iteration count can be necessary.

	const int maxNb = nbSrcEntries > nbDstEntries ? nbSrcEntries : nbDstEntries;
	for(int i=0; i < (maxNb + addVTablePtrShiftIteration); i++)
	{


#if PRINT_CONVERT_CLASS_INFORMATION
		if (i < nbSrcEntries)
		{
			printf("\t0x%p\t%x\t%d\t%d\t%s\n", buffer + srcOffsetCheck, (unsigned char)buffer[srcOffsetCheck], srcOffsetCheck, srcEntries[i].entry.mOffset, srcEntries[i].entry.mName);
			for (int byteCount = 1; byteCount < srcEntries[i].entry.mSize; ++byteCount) 
				printf("\t0x%p\t%x\t%d\t%d\t.\n", buffer + srcOffsetCheck + byteCount, (unsigned char)buffer[srcOffsetCheck + byteCount], srcOffsetCheck + byteCount, srcEntries[i].entry.mOffset + byteCount);
		}

#endif

		bool handlePadding = true;
		bool skipLoop = false;
		while(handlePadding)
		{
			const int pad0 = i<nbSrcEntries ? srcEntries[i].entry.mFlags & PxMetaDataFlag::ePADDING : 0;
			const int pad1 = j<nbDstEntries ? dstEntries[j].entry.mFlags & PxMetaDataFlag::ePADDING : 0;
			if(pad0 || pad1)
			{
				if(pad0)
				{
					if (mMarkedPadding && (strcmp(srcEntries[i].entry.mType, "paddingByte")==0))
						checkPaddingBytes(buffer + srcOffsetCheck, srcEntries[i].entry.mSize);

					if(pad1)
					{
						// Both have padding
						// ### check sizes, output bytes
						if(srcEntries[i].entry.mSize==dstEntries[j].entry.mSize)
						{
							// I guess we can just go on with the normal code here
							handlePadding = false;
						}
						else
						{
							// Output padding
							assert(srcEntries[i].cb);
							assert(srcEntries[i].offset == srcOffsetCheck);

							const int padSize = dstEntries[j].entry.mSize;
							char* paddingBytes = (char*)PX_ALLOC(sizeof(char)*padSize, PX_DEBUG_EXP("paddingByte"));
							memset(paddingBytes, 0, padSize);
							assert(dstEntries[j].cb);
							(this->*dstEntries[j].cb)(paddingBytes, dstEntries[j].entry, dstEntries[j].entry);
							assert(dstOffsetCheck==dstEntries[j].offset);
							dstOffsetCheck += padSize;
							PX_FREE(paddingBytes);

							//							srcEntries[i].cb(buffer+srcOffsetCheck, srcEntries[i].entry, dstEntries[j].entry);
							//							assert(dstOffsetCheck==dstEntries[j].offset);
							//							dstOffsetCheck += dstEntries[j].entry.mSize;
							srcOffsetCheck += srcEntries[i].entry.mSize;

							// Skip dest padding field
							j++;

							//							continue;	// ### BUG, doesn't go back to the "for"
							skipLoop = true;
							handlePadding = false;
						}
					}
					else
					{
						// Src has padding, dst has not => skip conversion
						// Don't increase j
						skipLoop = true;
						handlePadding = false;
						srcOffsetCheck += srcEntries[i].entry.mSize;
					}
				}
				else
				{
					if(pad1)
					{
						// Dst has padding, src has not

						// Output padding
						const int padSize = dstEntries[j].entry.mSize;
						char* paddingBytes = (char*)PX_ALLOC(sizeof(char)*padSize, PX_DEBUG_EXP("paddingByte"));
						memset(paddingBytes, 0, padSize);
						assert(dstEntries[j].cb);
						(this->*dstEntries[j].cb)(paddingBytes, dstEntries[j].entry, dstEntries[j].entry);
						assert(dstOffsetCheck==dstEntries[j].offset);
						dstOffsetCheck += padSize;
						PX_FREE(paddingBytes);

						// Skip dest padding field, keep same src field
						j++;
					}
					else
					{
						assert(0);
					}
				}
			}
			else handlePadding = false;
		}

		if(skipLoop)
			continue;

		int modSrcOffsetCheck = srcOffsetCheck;
		const ExtraDataEntry2* srcEntryPtr = &srcEntries[i];
		const ExtraDataEntry2* dstEntryPtr = &dstEntries[j];

		bool isSrcVTablePtr = (i < nbSrcEntries) ? srcEntryPtr->entry.isVTablePtr() : false;
		if (isSrcVTablePtr && (dstOpenVTablePtrEntry != -1))
		{
			// vtable ptr position mismatch:
			// this check is necessary to align src and dst index again when the
			// dst vtable pointer has been written already and the src vtable ptr
			// element is reached.
			//
			//                       i
			// src: | a      | b | vt-ptr | c | ...
			// dst: | vt-ptr | a | b      | c | ...
			//                              j 
			//
			// it needs special treatment because the following case fails otherwise
			//                       i
			// src: | a      | b | vt-ptr | c      | vt-ptr | ...
			// dst: | vt-ptr | a | b      | vt-ptr | c      | ...
			//                                j 

			//
			// This entry has been written already -> advance to next src entry
			//
			srcOffsetCheck += srcEntryPtr->entry.mSize;
			i++;
			isSrcVTablePtr = (i < nbSrcEntries) ? srcEntryPtr->entry.isVTablePtr() : false;
			PX_ASSERT(dstOpenVTablePtrEntry < nbDstEntries);
			PX_ASSERT(dstEntries[dstOpenVTablePtrEntry].entry.isVTablePtr());
			dstOpenVTablePtrEntry = -1;
			PX_ASSERT(addVTablePtrShiftIteration == 0);
		}
		bool isDstVTablePtr = (j < nbDstEntries) ? dstEntryPtr->entry.isVTablePtr() : false;
		if (isDstVTablePtr && (srcOpenVTablePtrEntry != -1))
		{
			//                              i
			// src: | vt-ptr | a | b      | c | ...
			// dst: | a      | b | vt-ptr | c | ...
			//                       j 

			i--;  // next iteration the current element should get processed
			isSrcVTablePtr = true;
			PX_ASSERT(srcOpenVTablePtrEntry < nbSrcEntries);
			srcEntryPtr = &srcEntries[srcOpenVTablePtrEntry];
			PX_ASSERT(srcEntryPtr->entry.isVTablePtr());
			modSrcOffsetCheck = srcEntryPtr->offset;
			srcOffsetCheck -= srcEntryPtr->entry.mSize;  // to make sure total change is 0 after this iteration
			srcOpenVTablePtrEntry = -1;
			PX_ASSERT(addVTablePtrShiftIteration == 1);
			addVTablePtrShiftIteration = 0;
		}

		if(i==nbSrcEntries && j==nbDstEntries)
		{
			PX_ASSERT((srcOpenVTablePtrEntry == -1) && (dstOpenVTablePtrEntry == -1));
			break;
		}

		if (!(isSrcVTablePtr || isDstVTablePtr))
		{
			PX_ASSERT(compareEntries(*srcEntryPtr, *dstEntryPtr));
		}
		else
		{
			if (!isSrcVTablePtr)
			{
				//          i
				// src: | a      | b | vt-ptr | c | ...
				// dst: | vt-ptr | a | b      | c | ...
				//          j 

				PX_ASSERT(dstOpenVTablePtrEntry == -1);  // the other case should be detected and treated earlier
				PX_ASSERT(srcOpenVTablePtrEntry == -1);
				PX_ASSERT(addVTablePtrShiftIteration == 0);

				int k;
				for(k=i+1; k < nbSrcEntries; k++)
				{
					if (srcEntries[k].entry.isVTablePtr())
						break;
				}
				PX_ASSERT(k < nbSrcEntries);

				srcEntryPtr = &srcEntries[k];
				modSrcOffsetCheck = srcEntryPtr->offset;
				srcOffsetCheck -= srcEntryPtr->entry.mSize;  // to make sure total change is 0 after this iteration

				dstOpenVTablePtrEntry = j;
				i--; // to make sure the original entry gets processed in the next iteration
			}
			else if (!isDstVTablePtr)
			{
				//          i ---> i
				// src: | vt-ptr | a | b      | c | ...
				// dst: | a      | b | vt-ptr | c | ...
				//          j 

				PX_ASSERT(srcOpenVTablePtrEntry == -1);  // the other case should be detected and treated earlier
				PX_ASSERT(dstOpenVTablePtrEntry == -1);
				PX_ASSERT(addVTablePtrShiftIteration == 0);

				srcOffsetCheck += srcEntryPtr->entry.mSize;
				modSrcOffsetCheck = srcOffsetCheck;
				srcOpenVTablePtrEntry = i;
				i++;
				srcEntryPtr = &srcEntries[i];

				addVTablePtrShiftIteration = 1;  // additional iteration might be needed to process vtable pointer at the end of a class

				PX_ASSERT((i < nbSrcEntries) && ((srcEntryPtr->entry.mFlags & PxMetaDataFlag::ePADDING) == 0));
				// if the second check fails, this whole section might have to be done before the padding bytes get processed. Not sure
				// what other consequences that might have though.
			}
		}

		const ExtraDataEntry2& srcEntry = *srcEntryPtr;
		const ExtraDataEntry2& dstEntry = *dstEntryPtr;

		if(srcEntry.entry.mFlags & PxMetaDataFlag::eUNION)
		{
			// ### hardcoded bit here, will only work when union type is the first int of the struct
			const int* tmp = (const int*)(buffer + modSrcOffsetCheck);
			const int unionType = *tmp;

			const char* typeName = getTypeName(srcEntry.entry.mType, unionType);
			assert(typeName);

			MetaClass* unionMC = getMetaClass(typeName, META_DATA_SRC);
			assert(unionMC);

			convertClass(buffer + modSrcOffsetCheck, unionMC, 0);			// ### recurse

			dstOffsetCheck += dstEntry.entry.mSize;

			MetaClass* targetUnionMC = getMetaClass(typeName, META_DATA_DST);
			assert(targetUnionMC);

			const int delta = dstEntry.entry.mSize - targetUnionMC->mSize;
			char* deltaBytes = (char*)PX_ALLOC(sizeof(char)*delta, PX_DEBUG_EXP("deltaBytes"));
			memset(deltaBytes, 0, delta);
			output(deltaBytes, delta);	// Skip unused bytes at the end of the union
			PX_FREE(deltaBytes);
			srcOffsetCheck += srcEntry.entry.mSize;  // do not use modSrcOffsetCheck here!
		}
		else
		{
			assert(srcEntry.cb);
			assert(srcEntry.offset == modSrcOffsetCheck);

			// ---- heightfield surgery ----
			if(heightfieldSurgery)
			{
				if(strcmp(srcEntry.entry.mName, "samples")==0)
				{
					assert(srcEntry.entry.mFlags & PxMetaDataFlag::ePTR);
					hfData.srcSamples = *(void**)(buffer+modSrcOffsetCheck);
				}
				if(strcmp(srcEntry.entry.mName, "columns")==0)
				{
					assert(srcEntry.entry.mSize==4);
					hfData.columns = *(int*)(buffer+modSrcOffsetCheck);
				}
				if(strcmp(srcEntry.entry.mName, "rows")==0)
				{
					assert(srcEntry.entry.mSize==4);
					hfData.rows = *(int*)(buffer+modSrcOffsetCheck);
				}
				if(strcmp(srcEntry.entry.mName, "columnsPadded")==0)
				{
					assert(srcEntry.entry.mSize==4);
					hfData.columnsPadded = *(int*)(buffer+modSrcOffsetCheck);
				}
				if(strcmp(srcEntry.entry.mName, "rowsPadded")==0)
				{
					assert(srcEntry.entry.mSize==4);
					hfData.rowsPadded = *(int*)(buffer+modSrcOffsetCheck);
				}
				if(strcmp(srcEntry.entry.mName, "tilesU")==0)
				{
					assert(srcEntry.entry.mSize==4);
					hfData.tilesU = *(int*)(buffer+modSrcOffsetCheck);
				}
				if(strcmp(srcEntry.entry.mName, "tilesV")==0)
				{
					assert(srcEntry.entry.mSize==4);
					hfData.tilesV = *(int*)(buffer+modSrcOffsetCheck);
				}
			}
			// ---- heightfield surgery ----

			// ---- big convex surgery ----
			if(convexSurgery)
			{
				if(strcmp(srcEntry.entry.mName, "mNbHullVertices")==0)
				{
					assert(srcEntry.entry.mSize==1);
					const int nbVerts = (int) *(buffer+modSrcOffsetCheck);
					assert(!foundNbVerts);
					foundNbVerts = true;

					const int gaussMapLimit = getBinaryMetaData(META_DATA_DST)->getGaussMapLimit();
					if(nbVerts > gaussMapLimit)
					{
						// We need a gauss map and we have one => keep it
					}
					else
					{
						// We don't need a gauss map and we have one => remove it
						removeBigData = true;
					}
				}
				else
				{
					if(removeBigData)
					{
						const bool isBigConvexData = strcmp(srcEntry.entry.mType, "BigConvexData")==0 || strcmp(srcEntry.entry.mType, "BigConvexRawData")==0;
						if(isBigConvexData)
						{
							assert(foundNbVerts);
							setNullPtr(true);
						}
					}
				}
			}
			// ---- big convex surgery ----

			(this->*srcEntry.cb)(buffer+modSrcOffsetCheck, srcEntry.entry, dstEntry.entry);
			assert(dstOffsetCheck==dstEntry.offset);
			dstOffsetCheck += dstEntry.entry.mSize;
			srcOffsetCheck += srcEntry.entry.mSize;  // do not use modSrcOffsetCheck here!

			// ---- big convex surgery ----
			if(convexSurgery && removeBigData)
				setNullPtr(false);
			// ---- big convex surgery ----
		}

		j++;
	}

#if PRINT_CONVERT_CLASS_INFORMATION
	printf("---------------------------------------------\n");
#endif

	while(j<nbDstEntries)
	{
		assert(dstEntries[j].entry.mFlags & PxMetaDataFlag::ePADDING);
		if(dstEntries[j].entry.mFlags & PxMetaDataFlag::ePADDING)
		{
			dstOffsetCheck += dstEntries[j].entry.mSize;
		}
		j++;
	}

	assert(j==nbDstEntries);
	assert(dstOffsetCheck==target_mc->mSize);
	assert(srcOffsetCheck==mc->mSize);

	// ---- big convex surgery ----
	if(convexSurgery)
		mConvexFlags.pushBack(removeBigData);
	// ---- big convex surgery ----

	// ---- heightfield surgery ----
	if(heightfieldSurgery)
		mHeightFields.pushBack(hfData);
	// ---- heightfield surgery ----
}

// Handles data defined with PX_DEF_BIN_METADATA_EXTRA_ARRAY
const char* Sn::ConvX::convertExtraData_Array(const char* Address, const char* lastAddress, const char* objectAddress, const ExtraDataEntry& ed)
{
	(void)lastAddress;
	MetaClass* mc = getMetaClass(ed.entry.mType, META_DATA_SRC);
	assert(mc);

	// PT: safe to cast to int here since we're reading a count.
	const int count = (int)peek(ed.entry.mSize, objectAddress + ed.offset, ed.entry.mFlags);

	//	if(ed.entry.mCount)	// Reused as align value
	if(ed.entry.mAlignment)
	{
		Address = PxAlignStream(Address, ed.entry.mAlignment);
		//		Address = PxAlignStream(Address, ed.entry.mCount);
		assert(Address<=lastAddress);
	}

	for(int c=0;c<count;c++)
	{
		convertClass(Address, mc, 0);
		Address += mc->mSize;
		assert(Address<=lastAddress);
	}
	return Address;
}

const char* Sn::ConvX::convertExtraData_Ptr(const char* Address, const char* lastAddress, const PxMetaDataEntry& entry, int count, int ptrSize_Src, int ptrSize_Dst)
{
	(void)lastAddress;

	PxMetaDataEntry tmpSrc = entry;
	tmpSrc.mCount = count;
	tmpSrc.mSize = count * ptrSize_Src;

	PxMetaDataEntry tmpDst = entry;
	tmpDst.mCount = count;
	tmpDst.mSize = count * ptrSize_Dst;

#if PRINT_CONVERT_CLASS_INFORMATION
	printf("extra data ptrs\n");
	printf("+++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("\t0x%p\t%x\t\t\t%s\n", Address, (unsigned char)Address[0], entry.mName);
	for (int byteCount = 1; byteCount < ptrSize_Src*count; ++byteCount) 
		printf("\t0x%p\t%x\t\t\t.\n", Address + byteCount, (unsigned char)Address[byteCount]);
#endif

	convertPtr(Address, tmpSrc, tmpDst);
	Address += count * ptrSize_Src;
	assert(Address<=lastAddress);
	return Address;
}

static bool decodeControl(PxU64 control, const ExtraDataEntry& ed, PxU64 controlVal = 0)
{
	if(ed.entry.mFlags & PxMetaDataFlag::eFLIP_CONTROL)
	{
		if(controlVal)
		{
			return (control & controlVal) ? false : true;
		}
		else
		{
			return control==0;
		}
	}
	else
	{
		if(controlVal)
		{
			return (control & controlVal) ? true : false;
		}
		else
		{
			return control!=0;
		}
	}

}

// ### currently hardcoded, should change
int Sn::ConvX::getConcreteType(const char* buffer)
{
	MetaClass* mc = getMetaClass("PxBase", META_DATA_SRC);
	assert(mc);
	PxMetaDataEntry entry;
	if(mc->getFieldByType("PxType", entry))
	{
		// PT: safe to cast to int here since we're reading our own PxType
		return (int)peek(entry.mSize, buffer + entry.mOffset);
	}
	assert(0);
	return 0xffffffff;
}

struct Item : public shdfnd::UserAllocated
{
	MetaClass*	mc;
	const char*	address;
};

bool Sn::ConvX::convertCollection(const void* buffer, int fileSize, int nbObjects)
{
	//	const int ptrSize_Src = getBinaryMetaData(META_DATA_SRC)->getPtrSize();
	//	const int ptrSize_Dst = getBinaryMetaData(META_DATA_DST)->getPtrSize();
	const int ptrSize_Src = mSrcPtrSize;
	const int ptrSize_Dst = mDstPtrSize;
	/*	if(1)
	{
	int nb = getNbMetaClasses(META_DATA_SRC);
	for(int i=0;i<nb;i++)
	{
	MetaClass* mc = getMetaClass(i, META_DATA_SRC);
	mc->mNbEntries = 0;
	_enumerateFields(mc, mc->mEntries, mc->mNbEntries, 0);
	}
	}*/

	const char* lastAddress = ((const char*)buffer) + fileSize;

	//	int nbObjects = read32(buffer);
	//	assert(nbObjects);

	const char* Address = reinterpret_cast<const char*>(buffer);
	Item* objects = PX_NEW(Item)[nbObjects];
	//const char* StartAddress3 = Address;
	//int startDstSize3 = getCurrentOutputSize();

	//const char* StartAddress2 = Address;
	//int startDstSize2 = getCurrentOutputSize();
	for(int i=0;i<nbObjects;i++)
	{
		//const char* StartAddress = Address;
		//int startDstSize = getCurrentOutputSize();
		const float percents = float(i)/float(nbObjects-1);
		if (verboseMode())
			displayMessage(PxErrorCode::eDEBUG_INFO, "\rObject conversion: %d%%", int(percents*100.0f));

		Address = PxAlignStream(Address);
		assert(Address<=lastAddress);

		if(gIncludeSentinels)
		{
			const void* dummy = (const void*)Address;
			int dead0 = read32(dummy);
			assert(dead0==0xdead);
			(void)dead0;
			Address = (const char*)dummy;
		}

		PxConcreteType::Enum classType = (PxConcreteType::Enum)getConcreteType(Address);

		const PxSerializer* bs = mSerializationRegistry.getSerializer(static_cast<PxType>(classType));
		PX_ASSERT(bs);
		const char* className = bs->getConcreteTypeName();

		if( !className )
			assert(!"Found non-supported type in getClassBySerialType(). Did you forget to update this code?");

		MetaClass* metaClass = getMetaClass(className, META_DATA_SRC);
		assert(metaClass && "Missing meta-data! You serialized a class without defining meta-data for it. Or you used an obsolete meta-data file.");

		objects[i].mc = metaClass;
		objects[i].address = Address;

		convertClass(Address, metaClass, 0);

		Address += metaClass->mSize;
		assert(Address<=lastAddress);

		if(gIncludeSentinels)
		{
			const void* dummy = (const void*)Address;
			int dead0 = read32(dummy);
			assert(dead0==0xdead);
			(void)dead0;
			Address = (const char*)dummy;
		}

		/*const char* EndAddress = Address;
		int nbSrcBytes = EndAddress - StartAddress;
		int nbDstBytes = getCurrentOutputSize() - startDstSize;
		assert(nbSrcBytes==nbDstBytes);*/
	}
	/*const char* EndAddress2 = Address;
	int nbSrcBytes2 = EndAddress2 - StartAddress2;
	int nbDstBytes2 = getCurrentOutputSize() - startDstSize2;
	assert(nbSrcBytes2==nbDstBytes2);*/

	// Fields / extra data
	if(1)
	{
		// ---- big convex surgery ----
		unsigned int nbConvexes = 0;
		// ---- big convex surgery ----
		// ---- heightfield surgery ----
		unsigned int nbHeightFields = 0;
		const int needsHFFiling = getBinaryMetaData(META_DATA_DST)->getHFTiling();
		// ---- heightfield surgery ----
		//const char* StartAddress2 = Address;
		//int startDstSize2 = getCurrentOutputSize();
		for(int i=0;i<nbObjects;i++)
		{
			//const char* StartAddress = Address;
			//int startDstSize = getCurrentOutputSize();

			const float percents = float(i)/float(nbObjects-1);

			if(verboseMode())
				displayMessage(PxErrorCode::eDEBUG_INFO, "\rExtra data conversion: %d%%", int(percents*100.0f));

			MetaClass* mc = objects[i].mc;
			const char* objectAddress = objects[i].address;

			//			printf("%d: %s\n", i, mc->mClassName);
			//			if(strcmp(mc->mClassName, "TriangleMesh")==0)
			//			if(strcmp(mc->mClassName, "NpRigidDynamic")==0)
			if(strcmp(mc->mClassName, "HybridModel")==0)
			{
				int stop=1;
				(void)(stop);
			}

			// ### we actually need to collect all extra data for this class, including data from embedded members.
						
			PX_ALLOCA(entries, ExtraDataEntry, 256); 
			int nbEntries = 0;
			_enumerateExtraData(objectAddress, mc, entries, nbEntries, 0, META_DATA_SRC);
			assert(nbEntries<256);

			Address = PxAlignStream(Address);
			assert(Address<=lastAddress);

			if(gIncludeSentinels)
			{
				const void* dummy = (const void*)Address;
				int dead0 = read32(dummy);
				assert(dead0==0xdead);
				(void)dead0;
				Address = (const char*)dummy;
			}

			for(int j=0;j<nbEntries;j++)
			{
				const ExtraDataEntry& ed = entries[j];
				assert(ed.entry.mFlags & PxMetaDataFlag::eEXTRA_DATA);

				if(ed.entry.mFlags & PxMetaDataFlag::eEXTRA_ITEM)
				{
					// ---- big convex surgery ----
					if(1)
					{
						const bool isBigConvexData = strcmp(ed.entry.mType, "BigConvexData")==0;
						if(isBigConvexData)
						{
							assert(nbConvexes<mConvexFlags.size());
							if(mConvexFlags[nbConvexes++])
								setNoOutput(true);
						}
					}
					// ---- big convex surgery ----

					MetaClass* extraDataType = getMetaClass(ed.entry.mType, META_DATA_SRC);
					assert(extraDataType);

					//sschirm: we used to have ed.entry.mOffset here, but that made cloth deserialization fail.
					const char* controlAddress = objectAddress + ed.offset; 
					const PxU64 controlValue = peek(ed.entry.mOffsetSize, controlAddress);

					if(controlValue)
					{
						if(ed.entry.mAlignment)
						{
							Address = PxAlignStream(Address, ed.entry.mAlignment);
							assert(Address<=lastAddress);
						}

						const char* classAddress = Address;
						convertClass(Address, extraDataType, 0);
						Address += extraDataType->mSize;
						assert(Address<=lastAddress);

						// Enumerate extra data for this optional class, and convert it too.
						// This assumes the extra data for the optional class is always appended to the class itself,
						// which is something we'll need to enforce in the SDK. So far this is only to handle optional
						// inline arrays. 

						// ### this should probably be recursive eventually						
						PX_ALLOCA(entries2, ExtraDataEntry, 256); 
						int nbEntries2 = 0;
						_enumerateExtraData(objectAddress, extraDataType, entries2, nbEntries2, 0, META_DATA_SRC);
						assert(nbEntries2<256);
						for(int k=0;k<nbEntries2;k++)
						{
							const ExtraDataEntry& ed2 = entries2[k];
							assert(ed2.entry.mFlags & PxMetaDataFlag::eEXTRA_DATA);
							if(ed2.entry.mFlags & PxMetaDataFlag::eEXTRA_ITEMS)
							{
								const int controlOffset = ed2.entry.mOffset;
								const int controlSize = ed2.entry.mSize;
								const int countOffset = ed2.entry.mCount;
								const int countSize = ed2.entry.mOffsetSize;

								const PxU64 controlValue2 = peek(controlSize, classAddress + controlOffset);

								PxU64 controlVal = 0;
								if(ed2.entry.mFlags & PxMetaDataFlag::eFLAG_CONTROL)
								{
									controlVal = ed2.entry.mFlags & PxMetaDataFlag::eFLAG_CONTROL_MASK;
									controlVal = controlVal >> 16;
								}

								if(decodeControl(controlValue2, ed2, controlVal))
								{
									// PT: safe to cast to int here since we're reading a count
									int count = (int)peek(countSize, classAddress + countOffset, ed2.entry.mFlags);

									if(ed2.entry.mAlignment)
									{
										assert(0);	// Never tested
										Address = PxAlignStream(Address, ed2.entry.mAlignment);
										assert(Address<=lastAddress);
									}

									if(ed2.entry.mFlags & PxMetaDataFlag::ePTR)
									{
										assert(0);	// Never tested
									}
									else
									{
										MetaClass* mc = getMetaClass(ed2.entry.mType, META_DATA_SRC);
										assert(mc);

										while(count--)
										{
											convertClass(Address, mc, 0);
											Address += mc->mSize;
											assert(Address<=lastAddress);
										}
									}

								}
							}
							else
							{
								if( (ed2.entry.mFlags & PxMetaDataFlag::eALIGNMENT) && ed2.entry.mAlignment)
								{
									Address = PxAlignStream(Address, ed2.entry.mAlignment);
									assert(Address<=lastAddress);
								}
								else
								{
									// We assume it's an normal array, e.g. the ones from "big convexes"
									assert(!(ed2.entry.mFlags & PxMetaDataFlag::eEXTRA_ITEM));

									Address = convertExtraData_Array(Address, lastAddress, classAddress, ed2);
								}
							}
						}

					}
					else					
					{
						int stop = 0;
						(void)(stop);
					}

					// ---- big convex surgery ----
					setNoOutput(false);
					// ---- big convex surgery ----
				}
				else if(ed.entry.mFlags & PxMetaDataFlag::eEXTRA_ITEMS)
				{
					// PX_DEF_BIN_METADATA_EXTRA_ITEMS
					int reloc = ed.offset - ed.entry.mOffset;	// ### because the enum code only fixed the "controlOffset"!
					const int controlOffset = ed.entry.mOffset;
					const int controlSize = ed.entry.mSize;
					const int countOffset = ed.entry.mCount;
					const int countSize = ed.entry.mOffsetSize;

					//					const int controlValue2 = peek(controlSize, objectAddress + controlOffset);
					const PxU64 controlValue2 = peek(controlSize, objectAddress + controlOffset + reloc);

					PxU64 controlVal = 0;
					if(ed.entry.mFlags & PxMetaDataFlag::eFLAG_CONTROL)
					{
						controlVal = ed.entry.mFlags & PxMetaDataFlag::eFLAG_CONTROL_MASK;
						controlVal = controlVal >> 16;
					}

					if(decodeControl(controlValue2, ed,controlVal))
					{
						// PT: safe to cast to int here since we're reading a count
						//						int count = peek(countSize, objectAddress + countOffset);	// ###
						int count = (int)peek(countSize, objectAddress + countOffset + reloc, ed.entry.mFlags);	// ###

						if(ed.entry.mAlignment)
						{
							Address = PxAlignStream(Address, ed.entry.mAlignment);
							assert(Address<=lastAddress);
						}

						if(ed.entry.mFlags & PxMetaDataFlag::ePTR)
						{
							Address = convertExtraData_Ptr(Address, lastAddress, ed.entry, count, ptrSize_Src, ptrSize_Dst);
						}
						else
						{
							MetaClass* mc = getMetaClass(ed.entry.mType, META_DATA_SRC);
							assert(mc);

							while(count--)
							{
								convertClass(Address, mc, 0);
								Address += mc->mSize;
								assert(Address<=lastAddress);
							}
						}
					}

				}
				else if(ed.entry.mFlags & PxMetaDataFlag::eALIGNMENT)
				{
					if(ed.entry.mAlignment)
					{
#if PRINT_CONVERT_CLASS_INFORMATION
						printf(" align to %d bytes\n", ed.entry.mAlignment);
						printf("---------------------------------------------\n");
#endif
						Address = PxAlignStream(Address, ed.entry.mAlignment);
						assert(Address<=lastAddress);
					}
				}
				else
				{
					// ---- heightfield surgery ----
					if(needsHFFiling && strcmp(ed.entry.mType, "PxHeightFieldSample")==0)
					{
						const HeightFieldData& hfData = mHeightFields[nbHeightFields++];

						// Read source data
						MetaClass* mcSample = getMetaClass(ed.entry.mType, META_DATA_SRC);
						assert(mcSample);

						//						static const char* convertExtraData_Array(const char* Address, const char* lastAddress, const char* objectAddress, const ExtraDataEntry& ed)
						const char* srcData;
						{
							// PT: safe to cast to int here since we're reading a count
							const int count = (int)peek(ed.entry.mSize, objectAddress + ed.offset, ed.entry.mFlags);

							if(ed.entry.mCount)	// Reused as align value
							{
								Address = PxAlignStream(Address, ed.entry.mCount);
								assert(Address<=lastAddress);
							}

							srcData = Address;
							/*							for(int c=0;c<count;c++)
							{
							convertClass(Address, mc, 0);
							Address += mc->mSize;
							assert(Address<=lastAddress);
							}
							return Address;*/
							Address += mcSample->mSize * count;
							assert(Address<=lastAddress);
						}
						// #### mNbSamples is not fixed, but it's only used during binary conversion anyway. So it's a problem if you try to convert an already converted file, e.g.
						// a heightfield converted from PC to PS3 to PC again wouldn't work. But then again we don't even have the code to "untile" the data from PS3 to PC anyway...

						// Convert to tiled
						const int sizeOfSample = 4;	// ### hardcoded
						char* const tiled = convertToTiled(srcData, sizeOfSample, hfData.columns, hfData.rows, hfData.columnsPadded, hfData.rowsPadded, hfData.tilesU, hfData.tilesV);

						// Output tiled data
						alignTarget(16);	// PS3 samples are 16-bytes aligned, while PC/Xbox samples are not
						int convertedCount = hfData.columnsPadded * hfData.rowsPadded;
						const char* tiledDst = tiled;
						while(convertedCount--)
						{
							convertClass(tiledDst, mcSample, 0);
							tiledDst += sizeOfSample;
						}

						// Free temp memory
						PX_FREE(tiled);
					}
					else
						// ---- heightfield surgery ----
					{
						Address = convertExtraData_Array(Address, lastAddress, objectAddress, ed);
					}
				}
			}

			if(gIncludeSentinels)
			{
				const void* dummy = (const void*)Address;
				int dead1 = read32(dummy);
				assert(dead1==0xdead);
				(void)dead1;
				Address = (const char*)dummy;
			}
			/*
			const char* EndAddress = Address;
			int nbSrcBytes = EndAddress - StartAddress;
			int nbDstBytes = getCurrentOutputSize() - startDstSize;
			assert(nbSrcBytes==nbDstBytes);
			*/
		}
		PX_DELETE_ARRAY(objects);
		assert(nbConvexes==mConvexFlags.size());
		/*
		const char* EndAddress2 = Address;
		int nbSrcBytes2 = EndAddress2 - StartAddress2;
		int nbDstBytes2 = getCurrentOutputSize() - startDstSize2;
		assert(nbSrcBytes2==nbDstBytes2);
		*/
	}
	/*
	const char* EndAddress3 = Address;
	int nbSrcBytes3 = EndAddress3 - StartAddress3;
	int nbDstBytes3 = getCurrentOutputSize() - startDstSize3;
	assert(nbSrcBytes3==nbDstBytes3);
	*/


	// String table
	if(1)
	{
		const void* p = (const void*)Address;
		int stringTableLength = read32(p);
		Address = (const char*)p;
		if(stringTableLength)
		{
			PxMetaDataEntry entry;
			entry.mSize = stringTableLength;
			entry.mCount = stringTableLength;
			convert8(Address, entry, entry);
		}
		Address += stringTableLength;
	}

	assert(Address==lastAddress);

	return true;
}

const void* Sn::ConvX::convertImportExportNames(const void* buffer, int nbNames, int& fileSize)
{
	if(!nbNames)
		return buffer;

	MetaClass* mc = getMetaClass("CollectedObject", META_DATA_SRC);
	assert(mc);

	const char* address = (const char*)buffer;
	for(int i=0;i<nbNames;i++)
	{
		convertClass(address, mc, 0);
		address += mc->mSize;
		fileSize -= mc->mSize; 
		assert(fileSize>=0);
	}
	return address;
}

bool Sn::ConvX::convert(const void* buffer, int fileSize)
{
	// Test initial alignment
	if(size_t(buffer) & (ALIGN_DEFAULT-1))
	{
		assert(0);
		return false;
	}

	const int header = read32(buffer);						fileSize -= 4;	(void)header;

	if (header != PX_MAKE_FOURCC('S','E','B','D'))
	{
		Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
			"Buffer contains data with wrong header indicating invalid binary data.");
		return false;
	}

	const int version = read32(buffer);						fileSize -= 4;	(void)version;

	if (version != PX_PHYSICS_VERSION)
	{
		Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
			"Mismatch of serialization format version:\nBinary Input Data: 0x%x \nExpected: 0x%x\n",
			version,
			PX_PHYSICS_VERSION);
		return false;
	}
	const int buildNumber = read32(buffer);						fileSize -= 4;	(void)buildNumber;

	//read src platform tag and write dst platform tag according dst meta data
	const int srcPlatformTag = *reinterpret_cast<const int*>(buffer);
	buffer = (const void*)(size_t(buffer) + 4);
	fileSize -= 4;
	const int dstPlatformTag = mMetaData_Dst->getPlatformTag();
	output(dstPlatformTag);

	if (srcPlatformTag != mMetaData_Src->getPlatformTag())
	{
		Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
			"Mismatch of platform tags of source serialized data and source meta data:\nBinary Input Data: %s \nSource Meta Data: %s\n",
			Cm::getBinaryPlatformName(srcPlatformTag),
			Cm::getBinaryPlatformName(mMetaData_Src->getPlatformTag()));
		return false;
	}

	//read whether input data has marked padding, and set it for the output data (since 0xcd is written into pads on conversion)
	const int srcMarkedPadding = *reinterpret_cast<const int*>(buffer);
	buffer = (const void*)(size_t(buffer) + 4);
	fileSize -= 4; 
	mMarkedPadding = srcMarkedPadding != 0;
	const int dstMarkedPadding = 1;
	output(dstMarkedPadding);


	//	const short materialOffset = (short)read32(buffer);		fileSize -= 4;	(void)materialOffset;
	//	const short articulationOffset = (short)read32(buffer);	fileSize -= 4;	(void)articulationOffset;



	int nbObjectsInCollection;
	if(1)
	{
		buffer = remapPointersTo32Bits(buffer, fileSize, nbObjectsInCollection);
	}
	else
	{
		// Export names
		{
			const int nbNames = read32(buffer);
			fileSize -= 4;

			buffer = convertImportExportNames(buffer, nbNames, fileSize);
		}

		// Import names
		{
			const int nbNames = read32(buffer);
			fileSize -= 4;

			buffer = convertImportExportNames(buffer, nbNames, fileSize);
		}

		// Old addresses
		nbObjectsInCollection = read32(buffer);
		assert(nbObjectsInCollection);
		fileSize -= 4;

		const int nbOldAddresses = read32(buffer);
		assert(nbOldAddresses>=nbObjectsInCollection);
		fileSize -= 4;

		if(1)
		{
			const char* lastAddress = ((const char*)buffer) + fileSize;
			const int ptrSize_Src = mSrcPtrSize;
			const int ptrSize_Dst = mDstPtrSize;

			PxMetaDataEntry tmp;
			buffer = convertExtraData_Ptr((const char*)buffer, lastAddress, tmp, nbOldAddresses, ptrSize_Src, ptrSize_Dst);
			fileSize -= ptrSize_Src * nbOldAddresses;
		}
	}

	bool ret = convertCollection(buffer, fileSize, nbObjectsInCollection);
	mMarkedPadding = false;
	return ret;
}

// PT: code below added to support 64bit-to-32bit conversions

#include "CmSerialFramework.h"
void Sn::ConvX::exportIntAsPtr(int value)
{
	const int ptrSize_Src = mSrcPtrSize;
	const int ptrSize_Dst = mDstPtrSize;

	PxMetaDataEntry entry;

	const char* address = NULL;
	const PxU32 value32 = PxU32(value);
	const PxU64 value64 = PxU64(value)&0xffffffff;

	if(ptrSize_Src==4)
	{
		address = reinterpret_cast<const char*>(&value32);
	}
	else if(ptrSize_Src==8)
	{
		address = reinterpret_cast<const char*>(&value64);
	}
	else assert(0);

	convertExtraData_Ptr(address, address + ptrSize_Src, entry, 1, ptrSize_Src, ptrSize_Dst);
}

void Sn::ConvX::exportInt(int value)
{
	output(value);
}

void Sn::ConvX::exportInt64(PxU64 value)
{
	output(value);
}

static const void* fetchCollectedObject(int srcPtrSize, const void* buffer, PxU64& object, PxSerialObjectRef& userID, PxU32& flags)
{
	const char* address = (const char*)buffer;
	{
		if(srcPtrSize==4)
		{
			const PxU32 ptrValue = *(PxU32*)(address);
			object = PxU64(ptrValue);
		}
		else if(srcPtrSize==8)
		{
			object = *(PxU64*)(address);
		}
		else PX_ASSERT(0);
		address += srcPtrSize;

		userID = *(PxSerialObjectRef*)(address);
		address += sizeof(PxSerialObjectRef);

		flags = *(PxU32*)(address);
		address += sizeof(PxU32);
	}
	return address;
}

static const void* fetchPtr(int srcPtrSize, const void* buffer, PxU64& ptr)
{
	const char* address = (const char*)buffer;
	{
		if(srcPtrSize==4)
		{
			const PxU32 ptrValue = *(PxU32*)(address);
			ptr = PxU64(ptrValue);
		}
		else if(srcPtrSize==8)
		{
			ptr = *(PxU64*)(address);
		}
		else PX_ASSERT(0);
	}
	address += srcPtrSize;
	return address;
}

PointerRemap::PointerRemap()
{
}

PointerRemap::~PointerRemap()
{
}

bool PointerRemap::checkRefIsNotUsed(PxU32 ref) const
{
	const PxU32 size = mData.size();
	for(PxU32 i=0;i<size;i++)
	{
		if(mData[i].id==ref)
			return false;
	}
	return true;
}

void PointerRemap::setObjectRef(PxU64 object64, PxU32 ref)
{
	const PxU32 size = mData.size();
	for(PxU32 i=0;i<size;i++)
	{
#ifdef PX_PSP2 // DE4606
		if(mData[i].object==(PxU32)object64)
#else
		if(mData[i].object==object64)
#endif
		{
			mData[i].id = ref;
			return;
		}
	}
	InternalData data;
	data.object	= object64;
	data.id		= ref;
	mData.pushBack(data);
}

bool PointerRemap::getObjectRef(PxU64 object64, PxU32& ref) const
{	
	const PxU32 size = mData.size();
	for(PxU32 i=0;i<size;i++)
	{
#ifdef PX_PSP2 // DE4606
		if(mData[i].object==(PxU32)object64)
#else
		if(mData[i].object==object64)
#endif
		{
			ref = mData[i].id;
			return true;
		}
	}
	return false;
}

const void* Sn::ConvX::remapPointersTo32Bits(const void* buffer, int& fileSize, int& nbObjectsInCollection)
{
	// PT: special version to:
	// - convert "export names"
	// - convert "import names"
	// - convert "old addresses"
	// - create the hashmap used to remap pointers to 32bits

	const int ptrSize_Src = mSrcPtrSize;

	// PT: the map should not be used while creating it, so use one indirection
	mActiveRemap = NULL;
	mRemap.mData.clear();

	// Export/import names
	for(int j=0;j<2;j++)	// Once for export names, once for import names
	{
		const int nbNames = read32(buffer);
		fileSize -= 4;

		if(nbNames)
		{
			for(int i=0;i<nbNames;i++)
			{
				// Read 'collected object' data
				PxU64 object64;
				PxSerialObjectRef userID;
				PxU32 flags;
				buffer = fetchCollectedObject(mSrcPtrSize, buffer, object64, userID, flags);

				// Create hash-table
				const PxU32 remappedRef = 0x80000000 | j*0x40000000 | (i+1);
				assert(mRemap.checkRefIsNotUsed(remappedRef));
				mRemap.setObjectRef(object64, remappedRef);

				// Output remapped data
				exportIntAsPtr(remappedRef);
				exportInt64(userID);
				exportInt(flags);

				fileSize -= mSrcPtrSize + sizeof(PxU64) + sizeof(PxU32);	// That should be the size of a serialized CollectedObject struct
				assert(fileSize>=0);
			}
		}
	}

	// Old addresses
	nbObjectsInCollection = read32(buffer);
	if (nbObjectsInCollection == 0)
		displayMessage(PxErrorCode::eDEBUG_INFO, "\n\nConverting empty collection!\n\n");
	fileSize -= 4;

	const int nbOldAddresses = read32(buffer);
	assert(nbOldAddresses>=nbObjectsInCollection);
	fileSize -= 4;

	for(int i=0;i<nbOldAddresses;i++)
	{
		// Read 'old address'
		PxU64 ptr;
		buffer = fetchPtr(ptrSize_Src, buffer, ptr);

		// Remap address only if it doesn't have a userID yet
		PxU32 ref = i+1;	// Keep 0 for null pointers
		if(!mRemap.getObjectRef(ptr, ref))
		{
			assert(mRemap.checkRefIsNotUsed(ref));
			mRemap.setObjectRef(ptr, ref);
		}

		// Output remapped old address
		exportIntAsPtr(ref);

		fileSize -= ptrSize_Src;
		assert(fileSize>=0);
	}

	// PT: the map can now be used
	mActiveRemap = &mRemap;

	return buffer;
}

void Sn::ConvX::checkPaddingBytes(const char* buffer, int byteCount)
{
	const unsigned char* src = (const unsigned char*)buffer;

	int i = 0;
	while ((i < byteCount) && (src[i] == 0xcd)) 
		i++;
	if (i < byteCount)
	{
		displayMessage(PxErrorCode::eDEBUG_WARNING, "\nMissing bytes don't look like padding bytes. Converted file is probably busted.\n");
	}
}
