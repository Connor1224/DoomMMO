/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

/**************************************************************************

Filename    :   HeapPT_AllocBitSet2.h
Content     :   Bit-set based allocator, 2 bits per block.

Created     :   2009
Authors     :   Maxim Shemanarev

**************************************************************************/

#ifndef INC_KY_Kernel_HeapPT_AllocBitSet2_H
#define INC_KY_Kernel_HeapPT_AllocBitSet2_H

#include "gwnavruntime/kernel/HeapPT/HeapPT_FreeBin.h"
#include "gwnavruntime/kernel/SF_HeapBitSet2.h"

namespace Kaim { namespace HeapPT {

//------------------------------------------------------------------------
class AllocBitSet2
{
public:
    AllocBitSet2(UPInt minAlignShift);

    void Reset() { Bin.Reset(); }

    static UInt32* GetBitSet(const HeapSegment* seg)
    {
        return (UInt32*)(((UByte*)seg) + sizeof(HeapSegment));
    }

    UPInt GetBitSetWords(UPInt dataSize) const
    {
        return BitSet2::GetBitSetSize(dataSize, MinAlignShift);
    }

    UPInt GetBitSetBytes(UPInt dataSize) const
    {
        return GetBitSetWords(dataSize) * sizeof(UInt32);
    }

    void  InitSegment(HeapSegment* seg);
    void  ReleaseSegment(HeapSegment* seg);

    void* Alloc(UPInt size, HeapSegment** allocSeg);
    void* Alloc(UPInt size, UPInt alignSize, HeapSegment** allocSeg);

    void  Free(HeapSegment* seg, void* ptr);
    void* ReallocInPlace(HeapSegment* seg, void* oldPtr, 
                         UPInt newSize, UPInt* oldSize);

    UPInt GetUsableSize(const HeapSegment* seg, const void* ptr) const;
    UPInt GetAlignShift(const HeapSegment* seg, const void* ptr, UPInt size) const;

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

    void CheckIntegrity() const
    {
        Bin.CheckIntegrity(MinAlignShift);
    }

private:
    UPInt   MinAlignShift;
    UPInt   MinAlignMask;
    FreeBin Bin;
};

}} // Kaim::Heap


#endif
