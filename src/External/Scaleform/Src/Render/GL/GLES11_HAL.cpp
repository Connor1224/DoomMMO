/**************************************************************************

Filename    :   GLES11_HAL.cpp
Content     :   GLES 1.1 Renderer HAL implementation.
Created     :   Feb 2012
Authors     :   Bart Muzzin

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#if defined(SF_USE_GLES)	// Use only with GLES 1.1

#include "Kernel/SF_Debug.h"
#include "Kernel/SF_HeapNew.h"
#include "Kernel/SF_Random.h"

#include "Render/GL/GLES11_HAL.h"

namespace Scaleform { namespace Render { namespace GL {


// ***** RenderHAL_GL

HAL::HAL(ThreadCommandQueue* commandQueue) :   
    Render::HAL(commandQueue),
    MultiBitStencil(1),
    EnabledVertexArrays(0),
    ActiveTextures(0),
    MaxTexUnits(0),
    Cache(Memory::GetGlobalHeap(), MeshCacheParams::PC_Defaults),
    Caps(0)
{
    pHeap = Memory::GetGlobalHeap();
}

HAL::~HAL()
{
    ShutdownHAL();
}

// *** RenderHAL_GL Implementation

bool HAL::InitHAL(const GL::HALInitParams& params)
{
    // Clear the error stack.
    glGetError();

    int stencilBits, depthBits;
    glGetIntegerv(GL_STENCIL_BITS, &stencilBits);
    glGetIntegerv(GL_DEPTH_BITS, &depthBits);

    if(stencilBits == 0)
    {
        StencilAvailable = false;
    }
    else
    {
        StencilAvailable = true;
        SF_DEBUG_WARNING(true, "GLES 1.1 hardware is reporting that it supports stencil, please double-check.");
    }
    
    Render::HAL::initHAL(params);

    pTextureManager = params.GetTextureManager();
    if (!pTextureManager)
        pTextureManager = *SF_HEAP_AUTO_NEW(this) TextureManager(params.RenderThreadId, pRTCommandQueue);

    pTextureManager->Initialize(this);

    // Allocate our matrix state
    Matrices = *SF_HEAP_AUTO_NEW(this) MatrixState(this);

#ifdef SF_GL_RUNTIME_LINK
    Extensions::Init();
#endif

    if (!Cache.Initialize(this))
        return false;

    // We need to create a dummy texture, so we can apply texture stages which don't use textures at all.
    glGenTextures(1, &DummyTextureID);
    SF_ASSERT( DummyTextureID != 0 );
    glBindTexture(GL_TEXTURE_2D, DummyTextureID);
    UByte data = 0;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 1, 1, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &data );

    // Create 256 dummy textures for additive alpha hackery; if we don't use GL_TEXTURE1 in a given texenv config,
    // and we have a texenv stage to spare, then it's possible to use these as a secondary "constant" register.
    glGenTextures(256, AddAlphaTextureID);
    SF_ASSERT( AddAlphaTextureID != 0 );
    for (int alphaIndex = 0; alphaIndex < 256; alphaIndex++)
    {
        glBindTexture(GL_TEXTURE_2D, AddAlphaTextureID[alphaIndex]);
        UByte alphaValue = (UByte)alphaIndex;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 1, 1, 0, GL_ALPHA, GL_UNSIGNED_BYTE, &alphaValue );
    }
    
    HALState|= HS_ModeSet;
    notifyHandlers(HAL_Initialize);
    return true;
}

// Returns back to original mode (cleanup)
bool HAL::ShutdownHAL()
{
    if (!(HALState & HS_ModeSet))
        return true;

    Render::HAL::shutdownHAL();

    // Destroy the dummy texture.
    glDeleteTextures(1, &DummyTextureID);
    DummyTextureID = 0;

    pTextureManager.Clear();
    Cache.Reset();

    return true;
}

bool HAL::ResetContext()
{
    notifyHandlers(HAL_PrepareForReset);
    pTextureManager->NotifyLostContext();
    Cache.Reset(true);

    pTextureManager->Initialize(this);
    pTextureManager->RestoreAfterLoss();

#ifdef SF_GL_RUNTIME_LINK
    Extensions::Init();
#endif

    if (!Cache.Initialize(this))
        return false;

    notifyHandlers(HAL_RestoreAfterReset);
    return true;
}


// ***** Rendering

bool HAL::BeginFrame()
{
    // Clear the error state.
    glGetError();

    if ( !Render::HAL::BeginFrame() )
        return false;

    return true;
}

// Set states not changed in our rendering, or that are reset after changes
bool HAL::BeginScene()
{
    if (!checkState(HS_InFrame, "BeginScene"))
        return false;
    HALState |= HS_InScene;

    glEnable(GL_BLEND);
    glActiveTexture(GL_TEXTURE0);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    if(StencilAvailable)
    {
        glStencilMask(0xffffffff);
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Reset vertex array usage (in case it changed between frames).
    EnabledVertexArrays = 0;
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glClientActiveTexture(GL_TEXTURE0);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glClientActiveTexture(GL_TEXTURE1);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    // Disable all texture stages.
    ActiveTextures = 0;
    glGetIntegerv(GL_MAX_TEXTURE_UNITS, &MaxTexUnits);
    for ( int i = 0; i < MaxTexUnits; ++i)
    {
        glActiveTexture(GL_TEXTURE0+i);
        glDisable(GL_TEXTURE_2D);
    }

    return true;
}

void HAL::beginDisplay(BeginDisplayData* data)
{
    if (!checkState(HS_InFrame, "BeginScene"))
        return;

    Render::HAL::beginDisplay(data);
    applyBlendMode(Blend_None);
    if(StencilAvailable)
    {
        glDisable(GL_STENCIL_TEST);
    }
}

void HAL::endDisplay()
{
    Render::HAL::endDisplay();
}

// Updates HW Viewport and ViewportMatrix based on provided viewport
// and view rectangle.
void HAL::updateViewport()
{
    Viewport vp;

    if (HALState & HS_ViewValid)
    {
        int dx = ViewRect.x1 - VP.Left,
            dy = ViewRect.y1 - VP.Top;

        // Modify HW matrix and viewport to clip.
        CalcHWViewMatrix(VP.Flags, &Matrices->View2D, ViewRect, dx, dy);
        Matrices->SetUserMatrix(Matrices->User);
        Matrices->ViewRect    = ViewRect;
        Matrices->UVPOChanged = 1;

        vp.Left     = ViewRect.x1;
        vp.Top      = ViewRect.y1;
        vp.Width    = ViewRect.Width();
        vp.Height   = ViewRect.Height();

        glViewport(vp.Left, VP.BufferHeight-vp.Top-vp.Height, vp.Width, vp.Height);

        if (VP.Flags & Viewport::View_UseScissorRect)
        {
            glEnable(GL_SCISSOR_TEST);
            glScissor(VP.ScissorLeft, VP.BufferHeight-VP.ScissorTop-VP.ScissorHeight, VP.ScissorWidth, VP.ScissorHeight);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }
    else
    {
        glViewport(0,0,0,0);
    }
}

void HAL::GetHWViewMatrix(Matrix* pmatrix, const Viewport& vp)
{
    Rect<int>  viewRect;
    int        dx =0, dy =0;
    vp.GetClippedRect(&viewRect, &dx, &dy);
    CalcHWViewMatrix(vp.Flags, pmatrix, viewRect, dx, dy);
}

void HAL::CalcHWViewMatrix(unsigned VPFlags, Matrix* pmatrix, const Rect<int>& viewRect, int dx, int dy)
{
    float       vpWidth = (float)viewRect.Width();
    float       vpHeight= (float)viewRect.Height();

    pmatrix->SetIdentity();
    if (VPFlags & Viewport::View_IsRenderTexture)
    {
        pmatrix->Sx() = 2.0f  / vpWidth;
        pmatrix->Sy() = 2.0f /  vpHeight;
        pmatrix->Tx() = -1.0f - pmatrix->Sx() * ((float)dx); 
        pmatrix->Ty() = -1.0f - pmatrix->Sy() * ((float)dy);
    }
    else
    {
        pmatrix->Sx() = 2.0f  / vpWidth;
        pmatrix->Sy() = -2.0f / vpHeight;
        pmatrix->Tx() = -1.0f - pmatrix->Sx() * ((float)dx); 
        pmatrix->Ty() = 1.0f  - pmatrix->Sy() * ((float)dy);
    }
}

void   HAL::MapVertexFormat(PrimitiveFillType fill, const VertexFormat* sourceFormat,
                                  const VertexFormat** single,
                                  const VertexFormat** batch, const VertexFormat** instanced,
                                  unsigned)
{
    SF_UNUSED(fill);

    VertexElement       outElements[8];
    VertexFormat        outFormat;
    unsigned            count = 0;

    // figure out which components we actually need, and reject the ones we don't.
    bool colors = false;
    bool factors = false;
    bool uvs = false;
    switch(fill)
    {
    case PrimFill_None: 
    case PrimFill_Mask: 
    case PrimFill_SolidColor: 
    case PrimFill_Texture: 
        break;
    case PrimFill_VColor:
        colors = true;
        break;
    case PrimFill_VColor_EAlpha: 
        colors = true;
        break;
    case PrimFill_Texture_VColor_EAlpha:
    case PrimFill_Texture_VColor: 
        colors = true;
        break;
    case PrimFill_Texture_EAlpha: 
    case PrimFill_2Texture_EAlpha: 
    case PrimFill_2Texture:         // use factors for blending textures
        factors = true;
        break;
    case PrimFill_UVTexture:
        uvs = true;
        break;
    case PrimFill_UVTextureAlpha_VColor:
    case PrimFill_UVTextureDFAlpha_VColor:
        uvs = true;
        colors = true;
        break;

    default: break;
    }

    outFormat.Size = 0;
    outFormat.pElements = outElements;

    const VertexElement* pve = sourceFormat->pElements;
    VertexElement* pveo = outElements;
    for (; pve->Attribute != VET_None; pve++ )
    {
        *pveo = *pve;
        if ((pve->Attribute & (VET_Usage_Mask|VET_CompType_Mask|VET_Components_Mask)) == VET_ColorARGB8)
            pveo->Attribute = VET_ColorRGBA8 | (pve->Attribute & ~(VET_Usage_Mask|VET_CompType_Mask|VET_Components_Mask));

        // Skip colors if we don't need them.
        if (!colors && (pve->Attribute&(VET_Usage_Mask|VET_Index_Mask)) == (VET_Color))
            continue;

        // Skip texcoords if we don't need them.
        if (!uvs && (pve->Attribute&(VET_Usage_Mask|VET_Index_Mask)) == (VET_TexCoord))
            continue;

        // Skip factors if we don't need them.
        if (!factors && (pve->Attribute&(VET_Index_Mask)) != 0)
            continue;

        // If we have factors, expand the weights to each RGB color channel.
        if (((pve->Attribute|pve[1].Attribute)&(VET_Usage_Mask|VET_Index_Mask)) == (VET_Color|VET_Index1|VET_Index2))
        {
            for ( int i=0; i<3;++i)
            {
                pveo->Attribute = VET_T0Weight8;
                pveo->Offset    = outFormat.Size+i;
                pveo++;
            }
            outFormat.Size+=3;
            pveo->Attribute = VET_FactorAlpha8;
            pveo->Offset    = outFormat.Size;
            outFormat.Size++;
            pveo++;
            count += 4;
            pve++;
            continue;
        }

        // Don't copy instance attributes, because we can't instance or batch.
        //if ( (pveo->Attribute & VET_Index_Mask) == 0 )
        {
            outFormat.Size += pveo->Size();
            pveo++;
            count++;
        }
    }
    pveo->Attribute = VET_None;
    pveo->Offset    = 0;
    count++;

    VertexFormat  *pformat   = VFormats.Find(outElements, count);
    VertexElement *pelements;
    if (!pformat)
    {
        pformat = VFormats.Add(&pelements, outElements, count);
        pformat->Size      = outFormat.Size;
        pformat->pElements = pelements;
    }

    *single     = pformat;
    *batch      = 0;
    *instanced  = 0;
}

bool HAL::SetVertexArray( PrimitiveFillType fillType, const VertexFormat* pFormat, GLuint buffer, UByte* vertexOffset )
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    GLenum posvet    = 0;
    GLuint posoffset = 0;
    const VertexElement* pve = pFormat->pElements;
    int vi = 0;
    for (; pve->Attribute != VET_None; pve++, vi++)
    {
        GLenum vet; bool norm;
        GLuint offset = pve->Offset;

        switch (pve->Attribute & VET_CompType_Mask)
        {
        case VET_U8:  vet = GL_UNSIGNED_BYTE; norm = false; break;
        case VET_U8N: vet = GL_UNSIGNED_BYTE; norm = true; break;
        case VET_U16: vet = GL_UNSIGNED_SHORT; norm = false; break;
        case VET_S16: vet = GL_SHORT; norm = false; break;
        case VET_U32: vet = GL_UNSIGNED_BYTE; norm = false; break; // U32 passed as 4 UBYTEs.
        case VET_F32: vet = GL_FLOAT; norm = false;  break;

        default: SF_ASSERT(0); vet = GL_FLOAT; norm = false; break;
        }

        unsigned usage = pve->Attribute & VET_Usage_Mask;

        // Packed factors (RGB=> weight, A=> alpha)
        if (((pve->Attribute|pve[1].Attribute|pve[2].Attribute|pve[3].Attribute) & (VET_Usage_Mask|VET_Index_Mask)) == (VET_Color | (3 << VET_Index_Shift)))
        {
            pve+=3;
            usage = VET_Color;
        }       

        switch( usage )
        {
        case VET_Pos:      
            glVertexPointer( 2, vet, pFormat->Size, (void*)(vertexOffset + offset) ); 
            if ( !(EnabledVertexArrays & VET_Pos ))
                glEnableClientState(GL_VERTEX_ARRAY);
            posvet    = vet;       // Store this, we might have to set texture coordinates later.
            posoffset = offset;
            break;

        case VET_Color:
            glColorPointer( 4, vet, pFormat->Size, (void*)(vertexOffset + offset) ); 
            if ( !(EnabledVertexArrays & VET_Color ))
                glEnableClientState(GL_COLOR_ARRAY);
            break;

        case VET_TexCoord: 
            glClientActiveTexture(GL_TEXTURE0);
            glTexCoordPointer( 2, vet, pFormat->Size, (void*)(vertexOffset + offset) ); 
            if ( !(EnabledVertexArrays & VET_TexCoord ))
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            break;
        }
        EnabledVertexArrays |= (1 << (usage >> VET_Usage_Shift));      
    }

    // If no texture coordinates are required, but there are textures, set it to the input positions.
    if ( fillType >= PrimFill_Texture && fillType < PrimFill_UVTexture && posvet != 0 )
    {
        glClientActiveTexture(GL_TEXTURE0);
        glTexCoordPointer( 2, posvet, pFormat->Size, (void*)(vertexOffset + posoffset) ); 
        if ( !(EnabledVertexArrays & VET_TexCoord))
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        EnabledVertexArrays |= (1 << (VET_TexCoord >> VET_Usage_Shift)); 

        if (fillType == PrimFill_2Texture || fillType == PrimFill_2Texture_EAlpha)
        {
            glClientActiveTexture(GL_TEXTURE1);
            glTexCoordPointer( 2, posvet, pFormat->Size, (void*)(vertexOffset + posoffset) ); 
            if ( !(EnabledVertexArrays & (VET_TexCoord<<1)))
                glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            EnabledVertexArrays |= (1 << ((VET_TexCoord<<1) >> VET_Usage_Shift)); 
        }
    }

    if ( !(EnabledVertexArrays & (1 << (VET_Pos >> VET_Usage_Shift) )))
        glDisableClientState(GL_VERTEX_ARRAY);
    if ( !(EnabledVertexArrays & (1 << (VET_Color >> VET_Usage_Shift) )))
        glDisableClientState(GL_COLOR_ARRAY);
    if ( !(EnabledVertexArrays & (1 << (VET_TexCoord >> VET_Usage_Shift) )))
    {
        glClientActiveTexture(GL_TEXTURE0);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
    if ( !(EnabledVertexArrays & (1 << ((VET_TexCoord<<1) >> VET_Usage_Shift) )))
    {
        glClientActiveTexture(GL_TEXTURE1);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    return true;
}

PrimitiveFill*  HAL::CreatePrimitiveFill(const PrimitiveFillData &data)
{
    return SF_HEAP_NEW(pHeap) PrimitiveFill(data);
}

// Draws a range of pre-cached and preprocessed primitives
void        HAL::DrawProcessedPrimitive(Primitive* pprimitive,
                                        PrimitiveBatch* pstart, PrimitiveBatch *pend)
{
    SF_AMP_SCOPE_RENDER_TIMER("HAL::DrawProcessedPrimitive", Amp_Profile_Level_High);
    if (!checkState(HS_InDisplay, "DrawProcessedPrimitive") ||
        !pprimitive->GetMeshCount() )
        return;

    // If in overdraw profile mode, and this primitive is part of a mask, draw it in color mode.
    static bool drawingMask = false;
    if ( !Profiler.ShouldDrawMask() && !drawingMask && (HALState & HS_DrawingMask) )
    {
        drawingMask = true;
        glColorMask(1,1,1,1);
        if (StencilAvailable)
            glDisable(GL_STENCIL_TEST);
        DrawProcessedPrimitive(pprimitive, pstart, pend );
        glColorMask(0,0,0,0);
        if (StencilAvailable)
            glEnable(GL_STENCIL_TEST);
        drawingMask = false;
    }

    SF_ASSERT(pend != 0);
    
    PrimitiveBatch* pbatch = pstart ? pstart : pprimitive->Batches.GetFirst();

    unsigned bidx = 0;
    while (pbatch != pend)
    {        
        // pBatchMesh can be null in case of error, such as VB/IB lock failure.
        MeshCacheItem* pmesh = (MeshCacheItem*)pbatch->GetCacheItem();
        unsigned       meshIndex = pbatch->GetMeshIndex();
        unsigned       batchMeshCount = pbatch->GetMeshCount();

        if (pmesh)
        {
            Profiler.SetBatch((UPInt)pprimitive, bidx);

            UInt32 fillFlags = FillFlags;
            if ( batchMeshCount > 0 )
                fillFlags |= pprimitive->Meshes[0].M.Has3D() ? FF_3DProjection : 0;

            SetPrimitiveFill(pprimitive->pFill, fillFlags, pbatch->Type, pbatch->pFormat, 
                batchMeshCount, Matrices, &pprimitive->Meshes[meshIndex] );

            if ((HALState & HS_ViewValid) && 
                SetVertexArray(pprimitive->pFill->GetType(), pbatch->pFormat, pmesh->pVertexBuffer->GetBuffer(), 
                               pmesh->pVertexBuffer->GetBufferBase() + pmesh->VBAllocOffset))
            {
                SF_ASSERT((pbatch->Type != PrimitiveBatch::DP_Failed) &&
                          (pbatch->Type != PrimitiveBatch::DP_Virtual) &&
                          (pbatch->Type != PrimitiveBatch::DP_Instanced));

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pmesh->pIndexBuffer->GetBuffer());
                drawIndexedPrimitive(pmesh->IndexCount, pmesh->MeshCount, pmesh->pIndexBuffer->GetBufferBase() + pmesh->IBAllocOffset);
            }

            pmesh->MoveToCacheListFront(MCL_ThisFrame);
        }

        pbatch = pbatch->GetNext();
        bidx++;
    }
}


void HAL::DrawProcessedComplexMeshes(ComplexMesh* complexMesh,
                                           const StrideArray<HMatrix>& matrices)
{    
    typedef ComplexMesh::FillRecord   FillRecord;
    typedef PrimitiveBatch::BatchType BatchType;
    
    MeshCacheItem* pmesh = (MeshCacheItem*)complexMesh->GetCacheItem();
    if (!checkState(HS_InDisplay, "DrawProcessedComplexMeshes") || !pmesh)
        return;
    
    // If in overdraw profile mode, and this primitive is part of a mask, draw it in color mode.
    static bool drawingMask = false;
    if ( !Profiler.ShouldDrawMask() && !drawingMask && (HALState & HS_DrawingMask) )
    {
        drawingMask = true;
        glColorMask(1,1,1,1);
        if (StencilAvailable)
            glDisable(GL_STENCIL_TEST);
        DrawProcessedComplexMeshes(complexMesh, matrices);
        glColorMask(0,0,0,0);
        if (StencilAvailable)
            glEnable(GL_STENCIL_TEST);
        drawingMask = false;
    }

    const FillRecord* fillRecords = complexMesh->GetFillRecords();
    unsigned    fillCount     = complexMesh->GetFillRecordCount();
    unsigned    instanceCount = (unsigned)matrices.GetSize();

    const Matrix2F* textureMatrices = complexMesh->GetFillMatrixCache();

    for (unsigned fillIndex = 0; fillIndex < fillCount; fillIndex++)
    {
        const FillRecord& fr = fillRecords[fillIndex];

        Profiler.SetBatch((UPInt)complexMesh, fillIndex);

        unsigned fillFlags = FillFlags;
        unsigned startIndex = 0;
        if ( instanceCount > 0 )
        {
            const HMatrix& hm = matrices[0];
            fillFlags |= hm.Has3D() ? FF_3DProjection : 0;

            for (unsigned i = 0; i < instanceCount; i++)
            {
                const HMatrix& hm = matrices[startIndex + i];
                if (!(Profiler.GetCxform(hm.GetCxform()) == Cxform::Identity))
                    fillFlags |= FF_Cxform;
            }
        }

        bool WantCxform = (fillFlags & FF_Cxform);
        bool WantVertexColor = false;
        bool WantSolidColor = false;

        // Apply fill.
        PrimitiveFillType fillType = Profiler.GetFillType(fr.pFill->GetType());
        SetFill(fillType, fillFlags, fr.pFormats[0] );
        SetVertexArray(fr.pFill->GetType(), fr.pFormats[0], pmesh->pVertexBuffer->GetBuffer(), 
            pmesh->pVertexBuffer->GetBufferBase() + pmesh->VBAllocOffset + fr.VertexByteOffset);

        UByte textureCount = fr.pFill->GetTextureCount();
        for (unsigned i = 0; i < instanceCount; i++)
        {            
            const HMatrix& hm = matrices[startIndex + i];

            unsigned currentStage = 0;
            SetMatrix(MT_MVP, complexMesh->GetVertexMatrix(), hm, Matrices);

            switch(fillType)
            {
                case PrimFill_Mask:
                case PrimFill_SolidColor:
                    if (WantCxform)
                    {
                        WantSolidColor = true;
                    }
                    else
                    {
                        SetColor(currentStage++, Profiler.GetColor(fr.pFill->GetSolidColor()));
                    }
                    if(textureCount > 0)
                    {
                        SF_DEBUG_MESSAGE(1, "9w84efi98urhyviaesurgh");
                    }
                    break;
                case PrimFill_VColor:
                case PrimFill_VColor_EAlpha:
                    if (WantCxform)
                    {
                        WantVertexColor = true;
                    }
                    else
                    {
                        SetVertexColors(currentStage++);
                    }
                    if(textureCount > 0)
                    {
                        SF_DEBUG_MESSAGE(1, "kasdjflaikjflgkajdflgjadlgf");
                    }
                    break;
                case PrimFill_Texture_VColor:
                case PrimFill_Texture_VColor_EAlpha:
                case PrimFill_UVTextureAlpha_VColor:
                case PrimFill_UVTextureDFAlpha_VColor:
                case PrimFill_Texture:
                case PrimFill_Texture_EAlpha:
                case PrimFill_2Texture:
                case PrimFill_2Texture_EAlpha:
                case PrimFill_UVTexture:
                    for (unsigned tm = 0; tm < textureCount; tm++)
                    {
                        SetMatrix(MT_Texture, textureMatrices[fr.FillMatrixIndex[tm]], tm);
                        Texture* ptex = (Texture*)fr.pFill->GetTexture(tm);
                        SetTexture(fillType, currentStage, ptex, fr.pFill->GetFillMode(tm), WantCxform);
                        currentStage += ptex->GetPlaneCount();
                    }
                    break;
                default:
                    break;
            }
            
            if (WantCxform)
            {
                currentStage = SetComplexCombiners(WantVertexColor, WantSolidColor, textureCount, fr.pFill, Profiler.GetCxform(hm.GetCxform()));
                DisableExtraStages(currentStage);
            }

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pmesh->pIndexBuffer->GetBuffer());
            drawIndexedPrimitive(fr.IndexCount, 1, pmesh->pIndexBuffer->GetBufferBase() + pmesh->IBAllocOffset + sizeof(IndexType) * fr.IndexOffset);
        }
    } // for (fill record)
  
    pmesh->MoveToCacheListFront(MCL_ThisFrame);
}


//--------------------------------------------------------------------
// Background clear helper, expects viewport coordinates.
void HAL::clearSolidRectangle(const Rect<int>& r, Color color)
{
    color = Profiler.GetClearColor(color);

    if (color.GetAlpha() == 0xFF)
    {
        glEnable(GL_SCISSOR_TEST);
        
        PointF tl((float)(VP.Left + r.x1), (float)(VP.Top + r.y1));
        PointF br((float)(VP.Left + r.x2), (float)(VP.Top + r.y2));
        tl = Matrices->Orient2D * tl;
        br = Matrices->Orient2D * br;
        Rect<int> scissor((int)Alg::Min(tl.x, br.x), (int)Alg::Min(tl.y,br.y), (int)Alg::Max(tl.x,br.x), (int)Alg::Max(tl.y,br.y));
        
        glScissor(scissor.x1, scissor.y1, scissor.Width(), scissor.Height());
        glClearColor(color.GetRed() * 1.f/255.f, color.GetGreen() * 1.f/255.f, color.GetBlue() * 1.f/255.f, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        if (VP.Flags & Viewport::View_UseScissorRect)
        {
            glEnable(GL_SCISSOR_TEST);
            glScissor(VP.ScissorLeft, VP.BufferHeight-VP.ScissorTop-VP.ScissorHeight, VP.ScissorWidth, VP.ScissorHeight);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
    }
    else
    {
        Matrix2F m((float)r.Width(), 0.0f, (float)r.x1,
                   0.0f, (float)r.Height(), (float)r.y1);

        Matrix2F mvp(m, Matrices->UserView);
        
        SetColor(0, color);
        SetMatrix(MT_MVP, mvp);
        SetVertexArray(PrimFill_SolidColor, &VertexXY16iInstance::Format, Cache.MaskEraseBatchVertexBuffer, 0);
        drawPrimitive(6,1);
    }
}

//--------------------------------------------------------------------
// *** Mask / Stencil support
//--------------------------------------------------------------------

// Mask support is implemented as a stack, enabling for a number of optimizations:
//
// 1. Large "Clipped" marks are clipped to a custom viewport, allowing to save on
//    fill-rate when rendering both the mask and its content. The mask area threshold
//    that triggers this behavior is determined externally.
//      - Clipped masks can be nested, but not batched. When erased, clipped masks
//        clear the clipped intersection area.
// 2. Small masks can be Batched, having multiple mask areas with multiple mask
//    content items inside.
//      - Small masks can contain clipped masks either regular or clipped masks.
// 3. Mask area dimensions are provided as HMatrix, which maps a unit rectangle {0,0,1,1}
//    to a mask bounding rectangle. This rectangle can be rotated (non-axis aligned),
//    allowing for more efficient fill.
// 4. PopMask stack optimization is implemented that does not erase nested masks; 
//    Stencil Reference value is changed instead. Erase of a mask only becomes
//    necessary if another PushMask_BeginSubmit is called, in which case previous
//    mask bounding rectangles are erased. This setup avoids often unnecessary erase 
//    operations when drawing content following a nested mask.
//      - To implement this MaskStack keeps a previous "stale" MaskPrimitive
//        located above the MaskStackTop.


void HAL::PushMask_BeginSubmit(MaskPrimitive* prim)
{
    if (!checkState(HS_InDisplay, "PushMask_BeginSubmit"))
        return;

    Profiler.SetDrawMode(1);

    if(StencilAvailable)
    {
        glColorMask(0,0,0,0);                       // disable framebuffer writes
        glEnable(GL_STENCIL_TEST);
    }

    bool viewportValid = (HALState & HS_ViewValid) != 0;

    // Erase previous mask if it existed above our current stack top.
    if (StencilAvailable && MaskStackTop && (MaskStack.GetSize() > MaskStackTop) && viewportValid && MultiBitStencil)
    {
        glStencilFunc(GL_LEQUAL, MaskStackTop, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        MaskPrimitive* erasePrim = MaskStack[MaskStackTop].pPrimitive;
        drawMaskClearRectangles(erasePrim->GetMaskAreaMatrices(), erasePrim->GetMaskCount());
    }

    MaskStack.Resize(MaskStackTop+1);
    MaskStackEntry &e = MaskStack[MaskStackTop];
    e.pPrimitive       = prim;
    e.OldViewportValid = viewportValid;
    e.OldViewRect      = ViewRect; // TBD: Must assign
    MaskStackTop++;

    HALState |= HS_DrawingMask;

    if (prim->IsClipped() && viewportValid)
    {
        Rect<int> boundClip;

        // Apply new viewport clipping.
        if (!Matrices->OrientationSet)
        {
            const Matrix2F& m = prim->GetMaskAreaMatrix(0).GetMatrix2D();

            // Clipped matrices are always in View coordinate space, to allow
            // matrix to be use for erase operation above. This means that we don't
            // have to do an EncloseTransform.
            SF_ASSERT((m.Shx() == 0.0f) && (m.Shy() == 0.0f));
            boundClip = Rect<int>(VP.Left + (int)m.Tx(), VP.Top + (int)m.Ty(),
                                  VP.Left + (int)(m.Tx() + m.Sx()), VP.Top + (int)(m.Ty() + m.Sy()));
        }
        else
        {
            Matrix2F m = prim->GetMaskAreaMatrix(0).GetMatrix2D();
            m.Append(Matrices->Orient2D);

            RectF rect = m.EncloseTransform(RectF(0,0,1,1));
            boundClip = Rect<int>(VP.Left + (int)rect.x1, VP.Top + (int)rect.y1,
                                  VP.Left + (int)rect.x2, VP.Top + (int)rect.y2);
        }

        if (!ViewRect.IntersectRect(&ViewRect, boundClip))
        {
            ViewRect.Clear();
            HALState &= ~HS_ViewValid;
            viewportValid = false;
        }
        updateViewport();
        
        // Clear full viewport area, which has been resized to our smaller bounds.
        if ((MaskStackTop == 1) && viewportValid)
        {
            if(StencilAvailable)
            {
                glClearStencil(0);
                glClear(GL_STENCIL_BUFFER_BIT);
            }
            else
            {

                glDisable(GL_SCISSOR_TEST);

                glDepthMask(GL_TRUE);
                glClearDepthf(1.0f);
                glClear(GL_DEPTH_BUFFER_BIT);
                
                if (VP.Flags & Viewport::View_UseScissorRect)
                {
                    glEnable(GL_SCISSOR_TEST);
                    glScissor(VP.ScissorLeft, VP.BufferHeight-VP.ScissorTop-VP.ScissorHeight, VP.ScissorWidth, VP.ScissorHeight);
                }
                else
                {
                    glDisable(GL_SCISSOR_TEST);
                }
                
                glDepthMask(GL_FALSE);
            }
        }
    }
    else if ((MaskStackTop == 1) && viewportValid)
    {
        // Clear view rectangles.
        if (StencilAvailable)
        {
            // Unconditionally overwrite stencil rectangles with a ref value of 0.
            glStencilFunc(GL_ALWAYS, 0, 0xff);
            glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
            
            drawMaskClearRectangles(prim->GetMaskAreaMatrices(), prim->GetMaskCount());
            
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }
        else
        {
            glDepthMask(GL_TRUE);
            glEnable(GL_SCISSOR_TEST);

            // Depth clears bounds. Don't use drawMaskClearRectangles yet as it doesn't set
            // proper Z.
            UPInt maskCount = prim->GetMaskCount();
            for(UPInt i = 0; i < maskCount; i++)
            {
                const Matrix2F &m = prim->GetMaskAreaMatrix(i).GetMatrix2D();
                RectF bounds(m.EncloseTransform(RectF(1.0f)));
                Rect<int> boundClip((int)bounds.x1, (int)bounds.y1,
                                    (int)bounds.x2, (int)bounds.y2);
                boundClip.Offset(VP.Left, VP.Top);
                
                if (boundClip.IntersectRect(&boundClip, ViewRect))
                {
                    int boundWidth = boundClip.x2 - boundClip.x1;
                    int boundHeight = boundClip.y2 - boundClip.y1;
                    glScissor(boundClip.x1, VP.BufferHeight - boundClip.y1 - boundHeight - 1, boundWidth, boundHeight);
                    glClear(GL_DEPTH_BUFFER_BIT);
                }
            }

            if (VP.Flags & Viewport::View_UseScissorRect)
            {
                glEnable(GL_SCISSOR_TEST);
                glScissor(VP.ScissorLeft, VP.BufferHeight-VP.ScissorTop-VP.ScissorHeight, VP.ScissorWidth, VP.ScissorHeight);
            }
            else
            {
                glDisable(GL_SCISSOR_TEST);
            }

            glDepthMask(GL_FALSE);
        }
    }

    if (StencilAvailable)
    {
        if (MultiBitStencil)
        {
            glStencilFunc(GL_EQUAL, MaskStackTop-1, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
        }
        else if (MaskStackTop == 1)
        {
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        }
    }
    else
    {
        if (MaskStackTop == 1)
        {
            glColorMask(0,0,0,1);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
        }
        else
        {
            glColorMask(0,0,0,0);
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);
        }
    }
    ++AccumulatedStats.Masks;
}


void HAL::EndMaskSubmit()
{
    Profiler.SetDrawMode(0);

    if (!checkState(HS_InDisplay|HS_DrawingMask, "EndMaskSubmit"))
        return;

    HALState &= ~HS_DrawingMask;    
    SF_ASSERT(MaskStackTop);

    glColorMask(1,1,1,1);

    if (StencilAvailable)
    {
        glStencilFunc(GL_LEQUAL, MaskStackTop, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    }
    else
    {
        glDepthMask(GL_FALSE);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_EQUAL);
    }
}


void HAL::PopMask()
{
    if (!checkState(HS_InDisplay, "PopMask"))
        return;

    SF_ASSERT(MaskStackTop);
    MaskStackTop--;

    if (MaskStack[MaskStackTop].pPrimitive->IsClipped())
    {
        // Restore viewport
        ViewRect      = MaskStack[MaskStackTop].OldViewRect;

        if (MaskStack[MaskStackTop].OldViewportValid)
            HALState |= HS_ViewValid;
        else
            HALState &= ~HS_ViewValid;
        updateViewport();
    }

    if (StencilAvailable)
    {
        if (MaskStackTop == 0)
            glDisable(GL_STENCIL_TEST);
        else
            glStencilFunc(GL_LEQUAL, MaskStackTop, 0xff);
    }
    else
    {
        glDepthMask(false);
        glDisable(GL_DEPTH_TEST);
        glDepthFunc(GL_ALWAYS);
    }
}


void HAL::drawMaskClearRectangles(const HMatrix* matrices, UPInt count)
{
    // This operation is used to clear bounds for masks.
    // Potential issue: Since our bounds are exact, right/bottom pixels may not
    // be drawn due to HW fill rules.
    //  - This shouldn't matter if a mask is tessellated within bounds of those
    //    coordinates, since same rules are applied to those render shapes.
    //  - EdgeAA must be turned off for masks, as that would extrude the bounds.

    SetVertexArray(PrimFill_SolidColor, &VertexXY16iInstance::Format, Cache.MaskEraseBatchVertexBuffer, 0);

    unsigned drawRangeCount = 1;
    for (UPInt i = 0; i < count; i+= (UPInt)drawRangeCount)
    {
        SetMatrix(MT_MVP, Matrix2F::Identity, matrices[i], Matrices, 0, i);
        drawPrimitive(drawRangeCount * 6, drawRangeCount);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}



//--------------------------------------------------------------------
// *** BlendMode Stack support
//--------------------------------------------------------------------

void HAL::applyBlendModeImpl(BlendMode mode, bool sourceAc, bool forceAc)
{    
    static const UInt32 BlendOps[BlendOp_Count] = 
    {
        GL_FUNC_ADD,                // BlendOp_ADD
        GL_MAX,                     // BlendOp_MAX
        GL_MIN,                     // BlendOp_MIN
        GL_FUNC_REVERSE_SUBTRACT,   // BlendOp_REVSUBTRACT
    };

    static const UInt32 BlendFactors[BlendFactor_Count] = 
    {
        GL_ZERO,                // BlendFactor_ZERO
        GL_ONE,                 // BlendFactor_ONE
        GL_SRC_ALPHA,           // BlendFactor_SRCALPHA
        GL_ONE_MINUS_SRC_ALPHA, // BlendFactor_INVSRCALPHA
        GL_DST_COLOR,           // BlendFactor_DESTCOLOR
    };

    GLenum sourceColor = BlendFactors[BlendModeTable[mode].SourceColor];
    if ( sourceAc && sourceColor == GL_SRC_ALPHA )
        sourceColor = GL_ONE;

    glBlendFunc(sourceColor, BlendFactors[BlendModeTable[mode].DestColor]);
}

//--------------------------------------------------------------------

void HAL::SetPrimitiveFill(PrimitiveFill* pfill, UInt32 fillFlags, PrimitiveBatch::BatchType batchType, const VertexFormat* pformat, 
                      unsigned meshCount, const MatrixState* mstate, const Primitive::MeshEntry* pmeshes )
{
    PrimitiveFillType fillType = pfill->GetType();

    // Check whether we have the identity color xform
    for (unsigned i = 0; i < meshCount; i++)
    {
        if (!(Profiler.GetCxform(pmeshes[i].M.GetCxform()) == Cxform::Identity))
        {
            fillFlags |= FF_Cxform;
            break;
        }
    }

    unsigned currentStage = 0;
    bool WantCxform = (fillFlags & FF_Cxform);
    bool WantVertexColor = false;
    bool WantSolidColor = false;
    int TextureCount = 0;

    // Set vertex colors, or a solid color, if requested
    switch(fillType)
    {
    case PrimFill_VColor:
    case PrimFill_VColor_EAlpha:
    case PrimFill_Texture_VColor:
    case PrimFill_Texture_VColor_EAlpha:
    case PrimFill_UVTextureAlpha_VColor:
    case PrimFill_UVTextureDFAlpha_VColor:
        if (WantCxform)
        {
            WantVertexColor = true;
        }
        else
        {
            SetVertexColors(currentStage++);
        }
        break;

    case PrimFill_Mask:
    case PrimFill_SolidColor:
        if (WantCxform)
        {
            WantSolidColor = true;
        }
        else
        {
            SetColor(currentStage++, Profiler.GetColor(pfill->GetSolidColor()));
        }
        break;
    default:
        break;
    }

    // Now set textures.
    if (fillType >= PrimFill_Texture)
    {
        GL::Texture*  pt0 = (GL::Texture*)pfill->GetTexture(0);
        SF_ASSERT(pt0);

        ImageFillMode fm0 = pfill->GetFillMode(0);

        unsigned stage = 0;
        SetTexture(fillType, stage, pt0, fm0, WantCxform);
        stage += pt0->GetPlaneCount();
        TextureCount++;

        Matrix2F m0;
        if ( fillType < PrimFill_UVTexture )
        {
            Matrix2F m(pmeshes->pMesh->VertexMatrix);
            m.Append(pmeshes->M.GetTextureMatrix(0));
            m0 = m;
        }
        SetMatrix(MT_Texture, m0, 0);

        if ((fillType == PrimFill_2Texture) || (fillType == PrimFill_2Texture_EAlpha))
        {
            GL::Texture* pt1 = (GL::Texture*)pfill->GetTexture(1);
            ImageFillMode  fm1 = pfill->GetFillMode(1);

            SetTexture(fillType, stage, pt1, fm1, WantCxform);
            stage += pt1->GetPlaneCount();
            TextureCount++;

            Matrix2F m1;
            if ( fillType < PrimFill_UVTexture )
            {
                Matrix2F m(pmeshes->pMesh->VertexMatrix);
                m.Append(pmeshes->M.GetTextureMatrix(1));
                m1 = m;
            }
            SetMatrix(MT_Texture, m1, 1);
        }
        if (!WantCxform)
        {
            currentStage = stage;
        }
    }

    SetMatrix(MT_MVP, pmeshes->pMesh->VertexMatrix, pmeshes->M, Matrices, 0);

    if (WantCxform)
    {
        currentStage += SetComplexCombiners(WantVertexColor, WantSolidColor, TextureCount, pfill, Profiler.GetCxform(pmeshes->M.GetCxform()));
    }

    DisableExtraStages(currentStage);
}

// There's a bunch of duplicated setup code here, but for the time being it should be
// left as-is, as the setup code is in a state of flux and might change.
int HAL::SetComplexCombiners(bool vertexColor, bool solidColor, int textureCount, PrimitiveFill* pfill, const Cxform &cx)
{
    float modColors[4] = { cx.M[0][0], cx.M[0][1], cx.M[0][2], cx.M[0][3] };
    float addColors[4] = { cx.M[1][0], cx.M[1][1], cx.M[1][2], cx.M[1][3] };
    bool hasAdditive = false;
    bool hasAdditiveColor = false;
    bool hasAdditiveAlpha = false;
    bool specialAdditive = false;
    bool skipColorModulate = false;
    bool skipAlphaModulate = false;
    GLenum cxformColorCombine = GL_MODULATE;
    GLenum cxformAlphaCombine = GL_MODULATE;
    GLenum cxformColorOperand = GL_SRC_COLOR;
    GLenum cxformAlphaOperand = GL_SRC_ALPHA;
    float cxformColorScale = 1.0f;
    float cxformAlphaScale = 1.0f;
    if (cx.M[1][3] > 0.0f)
    {
        hasAdditiveAlpha = true;
    }
    if (cx.M[1][0] > 0.0f ||
        cx.M[1][1] > 0.0f ||
        cx.M[1][2] > 0.0f)
    {
        hasAdditiveColor = true;
    }
    hasAdditive = hasAdditiveColor || hasAdditiveAlpha;
    
    if (cx.M[0][0] == 1.0f &&
        cx.M[0][1] == 1.0f &&
        cx.M[0][2] == 1.0f)
    {
        skipColorModulate = true;
        if(addColors[0] < 0.0f || addColors[1] < 0.0f || addColors[2] < 0.0f)
        {
            cxformColorCombine = GL_SUBTRACT;
            cxformColorOperand = GL_SRC_COLOR;
            modColors[0] = 0.0f - addColors[0];
            modColors[1] = 0.0f - addColors[1];
            modColors[2] = 0.0f - addColors[2];
        }
        else
        {
            cxformColorCombine = GL_ADD;
            cxformColorOperand = GL_SRC_COLOR;
            modColors[0] = addColors[0];// * 0.5f + 0.5f;
            modColors[1] = addColors[1];// * 0.5f + 0.5f;
            modColors[2] = addColors[2];// * 0.5f + 0.5f;
        }
        cxformColorScale = 1.0f;
    }
    else if (cx.M[0][0] == 0.0f &&
             cx.M[0][1] == 0.0f &&
             cx.M[0][2] == 0.0f)
    {
        skipColorModulate = true;
        cxformColorCombine = GL_REPLACE;
        cxformColorOperand = GL_SRC_COLOR;
        modColors[0] = addColors[0];
        modColors[1] = addColors[1];
        modColors[2] = addColors[2];
    }
    
    if (cx.M[0][3] == 1.0f && cx.M[1][3] != 0.0f)
    {
        skipAlphaModulate = true;
        if(addColors[0] < 0.0f || addColors[1] < 0.0f || addColors[2] < 0.0f)
        {
            cxformColorCombine = GL_SUBTRACT;
            cxformColorOperand = GL_SRC_COLOR;
            modColors[3] = 0.0f - addColors[3];// * 0.5f + 0.5f;
        }
        else
        {
            cxformColorCombine = GL_ADD;
            cxformColorOperand = GL_SRC_COLOR;
            modColors[3] = addColors[3];// * 0.5f + 0.5f;
        }
        cxformAlphaScale = 1.0f;
    }
    
    if(hasAdditive && !skipColorModulate && !skipAlphaModulate)
    {
        specialAdditive = true;
    }
    
    switch(textureCount)
    {
        case 0: // No textures, our life is easy
            if(solidColor || vertexColor)
            {
                glActiveTexture(GL_TEXTURE0);
                ActiveTextures |= 1 << 0;
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, DummyTextureID);

                if(solidColor)
                {
                    Color c = Profiler.GetColor(pfill->GetSolidColor());
                    float rgba[4];
                    c.GetRGBAFloat(rgba);
                    
                    // Manually mult+add; we can't have more than one constant in the entire
                    // combiner chain for GLES 1.1 on PVR MBX, and doing all of four adds on
                    // the CPU saves us from having to do multi-pass.
                    rgba[0] = rgba[0] * cx.M[0][0] + cx.M[1][0];
                    rgba[1] = rgba[1] * cx.M[0][1] + cx.M[1][1];
                    rgba[2] = rgba[2] * cx.M[0][2] + cx.M[1][2];
                    rgba[3] = rgba[3] * cx.M[0][3] + cx.M[1][3];

                    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           GL_REPLACE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         GL_REPLACE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,     rgba);
                    return 1;
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           cxformColorCombine);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         cxformAlphaCombine);
                    
                    if (cxformColorCombine == GL_REPLACE)
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          cxformColorOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_PRIMARY_COLOR);
                    }
                    else
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          cxformColorOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_PRIMARY_COLOR);
                    }
                    if (cxformAlphaCombine == GL_REPLACE)
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        cxformAlphaOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_PRIMARY_COLOR);
                    }
                    else
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        cxformAlphaOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_PRIMARY_COLOR);
                    }
                    
                    glTexEnvi(GL_TEXTURE_ENV,  GL_RGB_SCALE,             cxformColorScale);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_ALPHA_SCALE,           cxformAlphaScale);

                    if(!specialAdditive)
                    {
                        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,     modColors);
                    }
                    else
                    {
                        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,     cx.M[0]);
                    }
                    
                    if(!skipAlphaModulate)
                    {
                        int alpha = (int)(cx.M[1][3] * 255.0f);
                        glActiveTexture(GL_TEXTURE1);
                        ActiveTextures |= 1 << 1;
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, AddAlphaTextureID[alpha]);
                        
                        glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                        
                        glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           GL_REPLACE);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_PREVIOUS);
                        
                        glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         GL_ADD);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_PREVIOUS);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                        return 2;
                    }

                    return 1;
                }

                return 0;
            }
            else
            {
                // Just call the original function if we don't need to do any song-and-dance
                SetCxform(0, cx);
                return 1;
            }
            break;
            
        case 1: // One texture, our life is still pretty easy
        {
            Texture* ptex = (Texture*)pfill->GetTexture(0);
            int planeCount = ptex->GetTextureStageCount();
            
            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            ActiveTextures |= 1 << 0;
            
            if(solidColor || vertexColor)
            {
                if(solidColor)
                {
                    glBindTexture(GL_TEXTURE_2D, ptex->pTextures[0].TexId);
                    
                    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           cxformColorCombine);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         cxformAlphaCombine);
                    
                    if (cxformColorCombine == GL_REPLACE)
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          cxformColorOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_TEXTURE);
                    }
                    else
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          cxformColorOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_TEXTURE);
                    }
                    if (cxformAlphaCombine == GL_REPLACE)
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        cxformAlphaOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                    }
                    else
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        cxformAlphaOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_TEXTURE);
                    }

                    glTexEnvi(GL_TEXTURE_ENV,  GL_RGB_SCALE,             cxformColorScale);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_ALPHA_SCALE,           cxformAlphaScale);
                    
                    if(!specialAdditive)
                    {
                        Color c = Profiler.GetColor(pfill->GetSolidColor());
                        float rgba[4];
                        c.GetRGBAFloat(rgba);

                        rgba[0] *= cx.M[0][0];
                        rgba[1] *= cx.M[0][1];
                        rgba[2] *= cx.M[0][2];
                        rgba[3] *= cx.M[0][3];
                        
                        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,     rgba);
                    }
                    else
                    {
                        glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,     modColors);
                    }
                    
                    if(hasAdditive)
                    {
                        int alpha = (int)(cx.M[1][3] * 255.0f);
                        glActiveTexture(GL_TEXTURE1);
                        ActiveTextures |= 1 << 1;
                        glEnable(GL_TEXTURE_2D);
                        glBindTexture(GL_TEXTURE_2D, AddAlphaTextureID[alpha]);
                        
                        glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                        
                        glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           GL_REPLACE);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_PREVIOUS);
                        
                        glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         GL_ADD);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_PREVIOUS);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                        
                        return 2;
                    }
                    
                    return 1;
                }
                else
                {
                    SF_DEBUG_ASSERT(planeCount == 1, "YUV not presently supported in GLES 1.1 renderer.\n");

                    glBindTexture(GL_TEXTURE_2D, ptex->pTextures[0].TexId);

                    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           GL_MODULATE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         GL_MODULATE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_TEXTURE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_TEXTURE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_PRIMARY_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_PRIMARY_COLOR);

                    glActiveTexture(GL_TEXTURE1);
                    glEnable(GL_TEXTURE_2D);
                    ActiveTextures |= 1 << 1;
                    glBindTexture(GL_TEXTURE_2D, DummyTextureID);

                    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           cxformColorCombine);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         cxformAlphaCombine);
                    if (cxformColorCombine == GL_REPLACE)
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          cxformColorOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_PREVIOUS);
                    }
                    else
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          cxformColorOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_PREVIOUS);
                    }
                    if (cxformAlphaCombine == GL_REPLACE)
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        cxformAlphaOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_PREVIOUS);
                    }
                    else
                    {
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        cxformAlphaOperand);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_CONSTANT);
                        glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_PREVIOUS);
                    }
                    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,     modColors);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_RGB_SCALE,             cxformColorScale);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_ALPHA_SCALE,           cxformAlphaScale);
                }
                
                return 2;
            }
            else
            {
                SF_DEBUG_ASSERT(planeCount == 1, "YUV not presently supported in GLES 1.1 renderer.\n");
                
                glBindTexture(GL_TEXTURE_2D, ptex->pTextures[0].TexId);

                glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       cxformColorCombine);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     cxformAlphaCombine);
                
                if (cxformColorCombine == GL_REPLACE)
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          cxformColorOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_TEXTURE);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          cxformColorOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_TEXTURE);
                }
                if (cxformAlphaCombine == GL_REPLACE)
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        cxformAlphaOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        cxformAlphaOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_TEXTURE);
                }

                glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, modColors);
                glTexEnvi(GL_TEXTURE_ENV,  GL_RGB_SCALE,         cxformColorScale);
                glTexEnvi(GL_TEXTURE_ENV,  GL_ALPHA_SCALE,       cxformAlphaScale);

                if(hasAdditive)
                {
                    int alpha = (int)(cx.M[1][3] * 255.0f);
                    glActiveTexture(GL_TEXTURE1);
                    ActiveTextures |= 1 << 1;
                    glEnable(GL_TEXTURE_2D);
                    glBindTexture(GL_TEXTURE_2D, AddAlphaTextureID[alpha]);
                    
                    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,      GL_COMBINE);
                    
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,           GL_REPLACE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_PREVIOUS);
                    
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,         GL_ADD);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_PREVIOUS);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                    
                    return 2;
                }

                return 1;
            }
            break;
        }
            
        case 2: // Two textures, our life is more difficult
        {
            Texture* ptex0 = (Texture*)pfill->GetTexture(0);
            Texture* ptex1 = (Texture*)pfill->GetTexture(1);

            glActiveTexture(GL_TEXTURE0);
            glEnable(GL_TEXTURE_2D);
            ActiveTextures |= 1 << 0;
            glBindTexture(GL_TEXTURE_2D, ptex0->pTextures[0].TexId);

            if(solidColor)
            {
                if(!specialAdditive)
                {
                    Color c = Profiler.GetColor(pfill->GetSolidColor());
                    float rgba[4];
                    c.GetRGBAFloat(rgba);
                    
                    rgba[0] *= cx.M[0][0];
                    rgba[1] *= cx.M[0][1];
                    rgba[2] *= cx.M[0][2];
                    rgba[3] *= cx.M[0][3];
                    
                    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
                }
                else
                {
                    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, modColors);
                }
                
                glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       cxformColorCombine);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     cxformAlphaCombine);

                if (cxformColorCombine == GL_REPLACE)
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          cxformColorOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_TEXTURE);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          cxformColorOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_TEXTURE);
                }
                if (cxformAlphaCombine == GL_REPLACE)
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        cxformAlphaOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        cxformAlphaOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_TEXTURE);
                }

                glTexEnvi(GL_TEXTURE_ENV,  GL_RGB_SCALE,         cxformColorScale);
                glTexEnvi(GL_TEXTURE_ENV,  GL_ALPHA_SCALE,       cxformAlphaScale);
            }
            else if(vertexColor)
            {
                glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       GL_MODULATE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_MODULATE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,      GL_SRC_COLOR);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,      GL_SRC_COLOR);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,    GL_SRC_ALPHA);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,          GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,          GL_PRIMARY_COLOR);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,        GL_PRIMARY_COLOR);
            }
            else
            {
                if(!specialAdditive)
                {
                    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, cx.M[0]);
                }
                else
                {
                    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, modColors);
                }

                glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       cxformColorCombine);
                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     cxformAlphaCombine);
                
                if (cxformColorCombine == GL_REPLACE)
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          cxformColorOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_TEXTURE);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,          cxformColorOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,          GL_SRC_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,              GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,              GL_TEXTURE);
                }
                if (cxformAlphaCombine == GL_REPLACE)
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        cxformAlphaOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_TEXTURE);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,        cxformAlphaOperand);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,        GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,            GL_CONSTANT);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,            GL_TEXTURE);
                }

                glTexEnvi(GL_TEXTURE_ENV,  GL_RGB_SCALE,         cxformColorScale);
                glTexEnvi(GL_TEXTURE_ENV,  GL_ALPHA_SCALE,       cxformAlphaScale);
            }

            glActiveTexture(GL_TEXTURE1);
            glEnable(GL_TEXTURE_2D);
            ActiveTextures |= 1 << 1;
            glBindTexture(GL_TEXTURE_2D, ptex1->pTextures[0].TexId);

            glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
            glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       GL_INTERPOLATE);
            glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,          GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,          GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV,  GL_SRC2_RGB,          GL_PRIMARY_COLOR);
            glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,      GL_SRC_COLOR);
            glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,      GL_SRC_COLOR);
            glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND2_RGB,      GL_SRC_COLOR);
            
            glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_MODULATE);
            glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_PREVIOUS);
            glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,        GL_TEXTURE);
            glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);
            glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,    GL_SRC_ALPHA);

            return 2;
        }
            
        default: // More than two textures
        {
            SF_DEBUG_ASSERT1(false, "Not supported %d textures!", textureCount);
            return 0;
        }
    }
    return 1;
}

void HAL::SetFill(PrimitiveFillType filleType, unsigned fillFlags, const VertexFormat* vf )
{

}

void HAL::SetMatrix(MatrixType type, const Matrix2F &m1, const HMatrix &m2, const MatrixState* Matrices, unsigned index, unsigned batch )
{
    switch(type)
    {
        case MT_MVP:
        {
            if (m2.Has3D())
            {
                Matrices->GetUVP();
                Matrix4F m(Matrices->View3D * Matrix3F(m2.GetMatrix3D(), m1));
                m.Transpose();
                const float *rows = m.Data();
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(rows);

                glMatrixMode(GL_PROJECTION);
                Matrix4F proj(Matrices->ViewRectCompensated3D * Matrices->Orient3D * Matrices->Proj3D);
                proj.Transpose();
                const float *projRows = proj.Data();
                glLoadMatrixf(projRows);
            }
            else
            {
                Matrix2F m2D(m1, m2.GetMatrix2D(), Matrices->UserView);
                Matrix4F m;
                m.Set(&m2D.M[0][0], 8);
                m.Transpose();
                glMatrixMode(GL_MODELVIEW);
                glLoadMatrixf(m.Data());

                glMatrixMode(GL_PROJECTION);
                glLoadIdentity();
            }
            break;
        }
        case MT_Texture:
        {
            glActiveTexture(GL_TEXTURE0+index);
            glMatrixMode(GL_TEXTURE);
            const Matrix2F& tm = m2.GetTextureMatrix(index);
            Matrix4F m;
            m.Set(&tm.M[0][0], 8);
            m.Transpose();
            glLoadMatrixf(m.Data());
            break;
        }
        default:
        {
            break;
        }
    }
}

void HAL::SetMatrix(MatrixType type, const Matrix2F &m, unsigned index, unsigned batch)
{
    switch(type)
    {
        case MT_MVP:
        {
            Matrix4F  md;
            Matrix2F  m0(m, Matrices->UserView);
            md.Set(&m0.M[0][0], 8);
            md.Transpose();
            glMatrixMode(GL_MODELVIEW);
            glLoadMatrixf(md.Data());

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            break;
        }
        case MT_Texture:
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glMatrixMode(GL_TEXTURE);
            Matrix4F tm;
            tm.Set(&m.M[0][0], 8);
            tm.Transpose();
            glLoadMatrixf(tm.Data());
            break;
        }
        default:
        {
            SF_ASSERT(0);
        }
    }
}

void HAL::SetCxform(unsigned stage, const Cxform & cx, unsigned index, unsigned batch)
{
    glActiveTexture(GL_TEXTURE0 + stage);
    ActiveTextures |= 1 << stage;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, DummyTextureID);

    // cxform multiply
    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,   GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,        GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,      GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,           GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,         GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,           GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,         GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,       GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,     GL_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,       GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,     GL_SRC_ALPHA);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,  cx.M[0]);

    // cxform add (alpha only)
    glActiveTexture(GL_TEXTURE0 + stage + 1);
    ActiveTextures |= 1 << (stage+1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, AddAlphaTextureID[(int)(cx.M[1][3] * 255.0f)]);
    
    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       GL_ADD);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_ADD);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,          GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,          GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,        GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,      GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,      GL_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,    GL_SRC_ALPHA);
}

void HAL::SetColor(unsigned stage, Color c, unsigned index, unsigned batch)
{
    glActiveTexture(GL_TEXTURE0 + stage);
    ActiveTextures |= 1 << stage;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, DummyTextureID);

    float rgba[4];
    c.GetRGBAFloat(rgba);
    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, rgba);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,          GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,      GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);
}

void HAL::SetVertexColors(unsigned stage)
{
    glActiveTexture(GL_TEXTURE0 + stage);
    ActiveTextures |= 1 << stage;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, DummyTextureID);

    glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,          GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,      GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);   
}

void HAL::SetTexture(PrimitiveFillType fillType, unsigned stage, Texture* ptexture, ImageFillMode fm, bool deferredCombine)
{
    GLint minfilter = (fm.GetSampleMode() == Sample_Point) ? GL_NEAREST : (ptexture->MipLevels>1 ? GL_LINEAR_MIPMAP_LINEAR  : GL_LINEAR);
    GLint magfilter = (fm.GetSampleMode() == Sample_Point) ? GL_NEAREST : GL_LINEAR;
    GLint address = (fm.GetWrapMode() == Wrap_Clamp ? GL_CLAMP_TO_EDGE : GL_REPEAT);

    //deferredCombine = false;
    
    for (unsigned plane = 0; plane < ptexture->GetTextureStageCount(); plane++)
    {
        int stageIndex = stage + plane;

        glActiveTexture(GL_TEXTURE0 + stageIndex);
        glEnable(GL_TEXTURE_2D);
        ActiveTextures |= 1 << stageIndex;
        glBindTexture(GL_TEXTURE_2D, ptexture->pTextures[plane].TexId);

        if (!deferredCombine)
        {
            if (stage == 0)
            {
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            }
            else
            {
                glTexEnvi(GL_TEXTURE_ENV,  GL_TEXTURE_ENV_MODE,  GL_COMBINE);

                glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_RGB,       GL_INTERPOLATE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_RGB,          GL_PREVIOUS);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_RGB,          GL_TEXTURE);
                glTexEnvi(GL_TEXTURE_ENV,  GL_SRC2_RGB,          GL_PRIMARY_COLOR);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_RGB,      GL_SRC_COLOR);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_RGB,      GL_SRC_COLOR);
                glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND2_RGB,      GL_SRC_COLOR);

                if ( fillType == PrimFill_2Texture_EAlpha ||
                     fillType == PrimFill_Texture_EAlpha  ||
                     fillType == PrimFill_Texture_VColor_EAlpha )
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_MODULATE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_TEXTURE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,        GL_PRIMARY_COLOR);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,    GL_SRC_ALPHA);
                }
                else
                {
                    glTexEnvi(GL_TEXTURE_ENV,  GL_COMBINE_ALPHA,     GL_MODULATE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC0_ALPHA,        GL_PREVIOUS);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_SRC1_ALPHA,        GL_TEXTURE);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND0_ALPHA,    GL_SRC_ALPHA);
                    glTexEnvi(GL_TEXTURE_ENV,  GL_OPERAND1_ALPHA,    GL_SRC_ALPHA);
                }
            }
        }

        if (ptexture->LastMinFilter != minfilter || ptexture->LastAddress != address)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter);              
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, address);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, address);

            ptexture->LastMinFilter = minfilter;
            ptexture->LastAddress = address;
        }
    }
}

void HAL::DisableExtraStages(unsigned stage)
{
    for ( int i = stage; i < MaxTexUnits; ++i)
    {
        if ( ActiveTextures & (1<<i))
        {
            glActiveTexture(GL_TEXTURE0+i);
            glDisable(GL_TEXTURE_2D);
            ActiveTextures &= ~(1<<i);
        }
    }
}

void HAL::drawPrimitive(unsigned indexCount, unsigned meshCount)
{
    glDrawArrays(GL_TRIANGLES, 0, indexCount);

    SF_UNUSED(meshCount);
#if !defined(SF_BUILD_SHIPPING)
    AccumulatedStats.Meshes += meshCount;
    AccumulatedStats.Triangles += indexCount / 3;
    AccumulatedStats.Primitives++;
#endif
}

void HAL::drawIndexedPrimitive( unsigned indexCount, unsigned meshCount, UByte* indexPtr)
{
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, indexPtr);

    SF_UNUSED(meshCount);
#if !defined(SF_BUILD_SHIPPING)
    AccumulatedStats.Meshes += meshCount;
    AccumulatedStats.Triangles += indexCount / 3;
    AccumulatedStats.Primitives++;
#endif
}

}}} // Scaleform::Render::GL

#endif // defined(SF_USE_GLES)

