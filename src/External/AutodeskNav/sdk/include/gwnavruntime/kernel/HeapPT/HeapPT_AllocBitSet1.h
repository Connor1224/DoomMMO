/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

/**************************************************************************

Filename    :   HeapPT_AllocBitSet1.h
Content     :   Bit-set based allocator, 1 bit per block.

Created     :   2009
Authors     :   Maxim Shemanarev

**************************************************************************/

#ifndef INC_KY_Kernel_HeapPT_AllocBitSet1_H
#define INC_KY_Kernel_HeapPT_AllocBitSet1_H

#include "gwnavruntime/kernel/HeapPT/HeapPT_FreeBin.h"

namespace Kaim { namespace HeapPT {

//------------------------------------------------------------------------
class AllocBitSet1
{
public:
    AllocBitSet1(UPInt minAlignShift);

    static UInt32* GetBitSet(const HeapSegment* seg)
    {
        return (UInt32*)(((UByte*)seg) + sizeof(HeapSegment));
    }

    UPInt GetBitSetWords(UPInt dataSize) const
    {
        UPInt blocks = (dataSize + MinAlignMask) >> MinAlignShift;
        return (blocks + 31) >> 5;
    }

    UPInt GetBitSetBytes(UPInt dataSize) const
    {
        return GetBitSetWords(dataSize) * sizeof(UInt32);
    }

    void  InitSegment(HeapSegment* seg);
    void  ReleaseSegment(HeapSegment* seg);

    UPInt AlignSize(UPInt size) const 
    { 
        return (size + MinAlignMask) & ~MinAlignMask; 
    }

    void* Alloc(UPInt size, HeapSegment** allocSeg);
    void  Free(HeapSegment* seg, void* ptr, UPInt size);

    UPInt GetTotalFreeSpace() const
    {
        return Bin.GetTotalFreeSpace(MinAlignShift);
    }

    void VisitMem(MemVisitor* visitor, MemVisitor::Category cat) const
    {
        Bin.VisitMem(visitor, MinAlignShift, cat);
    }

    void VisitUnused(SegVisitor* visitor, unsigned cat) const
    {
        Bin.VisitUnused(visitor, MinAlignShift, cat);
    }

private:
    KY_INLINE static void clrBit(UInt32* bitSet, UPInt idx)
    {
        bitSet[idx >> 5] &= ~(UInt32(1) << (idx & 31));
    }

    KY_INLINE static void setBit(UInt32* bitSet, UPInt idx)
    {
        bitSet[idx >> 5] |= UInt32(1) << (idx & 31);
    }

    KY_INLINE static void markBusy(UInt32* bitSet, UPInt start, UPInt size)
    {
        setBit(bitSet, start);
        setBit(bitSet, start+size-1);
    }

    KY_INLINE static void markFree(UInt32* bitSet, UPInt start, UPInt size)
    {
        clrBit(bitSet, start);
        clrBit(bitSet, start+size-1);
    }

    KY_INLINE static bool isZero(const UInt32* bitSet, UPInt idx)
    {
        return (bitSet[idx >> 5] & (UInt32(1) << (idx & 31))) == 0;
    }

    UPInt   MinAlignShift;
    UPInt   MinAlignMask;
    FreeBin Bin;
};


}} // Kaim::Heap

#endif
