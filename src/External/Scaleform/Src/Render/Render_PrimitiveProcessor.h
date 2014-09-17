/**************************************************************************

Filename    :   Render_PrimitiveProcessor.h
Content     :   Render queue processing for Primitive items and batches
Created     :   April 2010
Authors     :   Michael Antonov

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#include "Render_Primitive.h"
#include "Render_Queue.h"
#include "Render_HAL.h"
#include "Kernel/SF_Debug.h"

#ifndef INC_SF_Render_PrimitiveProcessor_H
#define INC_SF_Render_PrimitiveProcessor_H

namespace Scaleform { namespace Render {


// ***** PrimitivePrepareBuffer, PrimitiveEmitBuffer

// PrimitivePrepareBuffer and PrimitiveEmitBuffer store the temporary
// state for the the Primitive being prepared and/or rendered. This temporary
// data is necessary to enable longer primitives to be rendered
// then can fit into cache at once. When this happens, primitive batches are
// drawn one after another by a Primitive::QueueItem_Prepare + EmitToHAL loop,
// Prepare and Emit buffers storing the intermediate state.
//
// In general, Primitives are processed in several steps:
// 
//   1. Batch Conversion - virtual batches are converted to fixed-size/type
//      batches that can fit the hardware.
//   2. Preparation - this brings the batches into the mesh cache.
//   3. Rendering - queued up batches are drawn.
//
// Both steps (1) and (2) are done by functions of PrimitivePrepareBuffer,
// which tracks where preparation is in the primitives Batches list.
// Step (3) uses PrimitiveEmitBuffer to track how much of the primitive
// has already been drawn.


class PrimitivePrepareBuffer : public RQPrepareBuffer
{
    friend class Primitive;
    friend class TextPrepareBuffer;
public:
    typedef RenderQueueItem::QIPrepareResult QIPrepareResult;

    // StartPrimitive initializes PrimitivePrepareBuffer for the primitive.
    // After that is done, ProcessPrimitive is called one or more times
    // to prepare primitive batches for rendering.
    void            StartPrimitive(void* item,
                                   Primitive* p, 
                                   PrimitiveEmitBuffer* emitBuffer,
                                   HAL* hal, MeshCache* cache);

    QIPrepareResult ProcessPrimitive(bool waitForCache);
    
private:
    enum ProcessorState
    {
        PS_Loop,
        PS_NeedCache
    };    

    PrimitiveEmitBuffer* pEmitBuffer;
    HAL*                 pHal;
    MeshCache*           pCache;
    Primitive*           pPrimitive;
    
    // Source format generated by meshes of pPrimitive.
    const VertexFormat *pSourceVFormat;
    // Destination format into which vertices are converted for rendering.
    const VertexFormat  *pSingleVFormat, *pBatchVFormat, *pInstancedVFormat;

    // Processing location/state.
    ProcessorState  State;
    PrimitiveBatch* pPrepare, *pPrepareTail;
    PrimitiveBatch* pConvert;
    // Conversion process/state.
    bool            Converting;    

    // No constructor
    PrimitivePrepareBuffer()
    { SF_COMPILER_ASSERT(sizeof(PrimitivePrepareBuffer) <= RQPrepareBuffer::BufferSize); }

    Primitive*      getPrimitive() const { return pPrimitive; }
    MeshCache*      getCache() const     { return pCache; }
    
    inline bool     batchConvertBegin();
    void            batchConvertStep();

    // Helper function to merge batches around pConvert (next or previous), used in batchConvertStep.
    bool            attemptMergeBatches(PrimitiveBatch *pfirst, PrimitiveBatch *second,
                                        PrimitiveBatch *pother,
                                        PrimitiveBatch *pknown,
                                        unsigned &knownVerticesSize, unsigned &knownIndexCount);

    // Patches PrimitiveEmitBuffer::pDraw pointer, if needed,
    // to keep is consistent with batch processing.
    inline void     patchEmitDrawStartIfEq(PrimitiveBatch *check, PrimitiveBatch *drawStart);
};

class PrimitiveEmitBuffer : public RQEmitBuffer
{
    friend class Primitive;
    friend class PrimitivePrepareBuffer;
    friend class TextEmitBuffer;

    // First Batch that hasn't yet been drawn within the tail item. If null, tail item
    // will be drawn from the beginning. Next drawing call will try to draw
    //   - From pDraw up to the last item in list if (tail != CurrentItem)
    //   - From pDraw up to the pPrepare batch if (tail == CurrentItem)
    PrimitiveBatch*         pDraw;

    // No constructor
    PrimitiveEmitBuffer()
    { SF_COMPILER_ASSERT(sizeof(PrimitiveEmitBuffer) <= RQEmitBuffer::BufferSize); }

public:
    void    StartEmitting(void* item, PrimitiveBatch* drawStart)
    {
        RQEmitBuffer::StartEmitting(item);
        pDraw = drawStart;
    }
};

}} // Scaleform::Render

#endif
