/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

/**************************************************************************

PublicHeader:   Kernel
Filename    :   KY_ArrayStaticBuff.h
Content     :   
Created     :   2010
Authors     :   MaximShemanarev

**************************************************************************/

#ifndef INC_KY_Kernel_ArrayStaticBuff_H
#define INC_KY_Kernel_ArrayStaticBuff_H

#include "gwnavruntime/kernel/SF_Memory.h"
#include <string.h>

namespace Kaim {

//------------------------------------------------------------------------
template<class T, int StaticSize, int SID=Stat_Default_Mem> class ArrayStaticBuffPOD
{
public:
    typedef T ValueType;

    ArrayStaticBuffPOD(MemoryHeap* heap = 0) : 
        pHeap(heap), Size(0), Reserved(StaticSize), Data(Static)
    {}

    ~ArrayStaticBuffPOD()
    {
        Clear();
    }

    void Clear()
    {
        if (Data != Static)
            KY_FREE(Data);
        Data = Static;
        Size = 0;
    }

    void PushBack(const T& val)
    {
        if (Size < StaticSize)
        {
            Static[Size++] = val;
            return;
        }
        if (Size == StaticSize)
        {
            Reserved *= 2;
            Data = (T*)(pHeap ? KY_HEAP_ALLOC(pHeap, Reserved * sizeof(T), SID): 
                                KY_HEAP_AUTO_ALLOC_ID(this, Reserved * sizeof(T), SID));
            memcpy(Data, Static, sizeof(Static));
        }
        else
        if (Size >= Reserved)
        {
            Reserved *= 2;
            Data = (T*)(pHeap ? Memory::ReallocInHeap(pHeap, Data, Reserved * sizeof(T)) :
                                Memory::Realloc(             Data, Reserved * sizeof(T)));
        }
        Data[Size++] = val;
    }

    UPInt GetSize() const 
    { 
        return Size; 
    }

    void CutAt(UPInt size)
    {
        if (size < Size)
            Size = size;
    }

    const T& operator [] (UPInt i) const { return Data[i]; }
          T& operator [] (UPInt i)       { return Data[i]; }
    const T& At(UPInt i) const           { return Data[i]; }
          T& At(UPInt i)                 { return Data[i]; }
    T   ValueAt(UPInt i) const           { return Data[i]; }

private:
    // Copying is prohibited
    ArrayStaticBuffPOD(const ArrayStaticBuffPOD<T, StaticSize, SID>& v);
    const ArrayStaticBuffPOD<T, StaticSize, SID>& operator = (const ArrayStaticBuffPOD<T, StaticSize, SID>& v);

    MemoryHeap* pHeap;
    UPInt       Size;
    UPInt       Reserved;
    ValueType   Static[StaticSize];
    ValueType*  Data;
};


template<class T, int StaticSize, int SID=Stat_Default_Mem> class ArrayStaticBuff
{
public:
    typedef T ValueType;

    ArrayStaticBuff(MemoryHeap* heap = 0) : 
    pHeap(heap), Size(0), Reserved(StaticSize), Data(Static)
    {
    }

    ~ArrayStaticBuff()
    {
        Clear();
    }

    void Clear()
    {
        if (Data != Static)
        {
            DestructArray(Data, Size);
            KY_FREE(Data);
        }
        Data = Static;
        Size = 0;
    }

    void PushBack(const T& val)
    {
        if (Size < StaticSize)
        {
            Static[Size++] = val;
            return;
        }
        if (Size == StaticSize)
        {
            Reserved *= 2;
            Data = (T*)(pHeap ? KY_HEAP_ALLOC(pHeap, Reserved * sizeof(T), SID): 
                KY_HEAP_AUTO_ALLOC_ID(this, Reserved * sizeof(T), SID));
            memcpy(Data, Static, sizeof(Static));
            ConstructArray(Data + Size, Reserved - Size, T());
        }
        else if (Size >= Reserved)
        {
            Reserved *= 2;
            Data = (T*)(pHeap ? Memory::ReallocInHeap(pHeap, Data, Reserved * sizeof(T)) :
                                Memory::Realloc(             Data, Reserved * sizeof(T)));
            ConstructArray(Data + Size, Reserved - Size, T());
        }
        Data[Size++] = val;
    }

    UPInt GetSize() const 
    { 
        return Size; 
    }

    const T& operator [] (UPInt i) const { return Data[i]; }
    T& operator [] (UPInt i)             { return Data[i]; }
    const T& At(UPInt i) const           { return Data[i]; }
    T& At(UPInt i)                       { return Data[i]; }
    T   ValueAt(UPInt i) const           { return Data[i]; }

private:
    // Copying is prohibited
    ArrayStaticBuff(const ArrayStaticBuff<T, StaticSize, SID>& v);
    const ArrayStaticBuff<T, StaticSize, SID>& operator = (const ArrayStaticBuff<T, StaticSize, SID>& v);

    MemoryHeap* pHeap;
    UPInt       Size;
    UPInt       Reserved;
    ValueType   Static[StaticSize];
    ValueType*  Data;
};

} // Scaleform

#endif
