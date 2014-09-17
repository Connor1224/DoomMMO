/**************************************************************************

Filename    :   D3D1x_Texture.h
Content     :   
Created     :   Mar 2011
Authors     :   Bart Muzzin

Copyright   :   Copyright 2011 Autodesk, Inc. All Rights reserved.

Use of this software is subject to the terms of the Autodesk license
agreement provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

**************************************************************************/

#ifndef INC_SF_D3D1X_TEXTURE_H
#define INC_SF_D3D1X_TEXTURE_H
#pragma once

#include "Render/D3D1x/D3D1x_Config.h"
#include "Kernel/SF_List.h"
#include "Kernel/SF_Threads.h"
#include "Render/Render_Image.h"
#include "Render/Render_ThreadCommandQueue.h"
#include "Kernel/SF_HeapNew.h"

namespace Scaleform { namespace Render { namespace D3D1x {


// TextureFormat describes format of the texture and its caps.
// Format includes allowed usage capabilities and ImageFormat
// from which texture is supposed to be initialized.

struct TextureFormat
{
    struct Mapping
    {
        ImageFormat              Format;
        DXGI_FORMAT              D3DFormat;
        UByte                    BytesPerPixel;
        bool                     Mappable;
        Image::CopyScanlineFunc  CopyFunc;
        Image::CopyScanlineFunc  UncopyFunc;
    };

    const Mapping*  pMapping;
    DWORD           D3DUsage;

    ImageFormat     GetImageFormat() const { return pMapping->Format; }
    DXGI_FORMAT     GetD3DFormat() const   { return pMapping->D3DFormat; }

    unsigned        GetPlaneCount() const
    { return ImageData::GetFormatPlaneCount(GetImageFormat()); }
};

class MappedTexture;
class TextureManager;

// D3D1x Texture class implementation; it many actually include several HW 
// textures (one for each ImageFormat plane).

class Texture : public Render::Texture
{
public:
    // Bits stored in TextureFlags.
    enum TextureFlagBits
    {
        TF_Rescale    = 0x01,
        TF_SWMipGen   = 0x02,
        TF_UserAlloc  = 0x04,
    };

    const TextureFormat*    pFormat;
    
    static const UByte      MaxTextureCount = 4;

    // If texture is currently mapped, it is here.
    MappedTexture*          pMap;
    
    struct HWTextureDesc
    {        
        ImageSize                   Size;
        ID3D1x(Texture2D)*            pTexture;
        ID3D1x(ShaderResourceView)*   pView;
    };

    // TextureDesc array is allocated if more then one is needed.
    HWTextureDesc*          pTextures;
    HWTextureDesc           Texture0;

    Texture(TextureManagerLocks* pmanagerLocks, const TextureFormat* pformat, unsigned mipLevels,
            const ImageSize& size, unsigned use, ImageBase* pimage);
    Texture(TextureManagerLocks* pmanagerLocks, ID3D1x(Texture2D)* ptexture, ImageSize imgSize, ImageBase* pimage);
    ~Texture();

    ImageFormat             GetImageFormat() const { return pFormat->pMapping->Format; }
    TextureManager*         GetManager() const     { return (TextureManager*)pManagerLocks->pManager; }
    bool                    IsValid() const        { return pTextures != 0; }

    void                    LoseManager();
    void                    LoseTextureData();
    bool                    Initialize();
    bool                    Initialize(ID3D1x(Texture2D)* ptexture);
    void                    ReleaseHWTextures(bool staging = true);

    // Applies a texture to device starting at pstageIndex, advances index
    // TBD: Texture matrix may need to be adjusted if image scaling is done.
    void                    ApplyTexture(DWORD stageIndex, ImageFillMode fm);

    // *** Interface implementation

    virtual Image*          GetImage() const
    { SF_ASSERT(!pImage || (pImage->GetImageType() != Image::Type_ImageBase)); return (Image*)pImage; }
    virtual ImageFormat     GetFormat() const         { return GetImageFormat(); }

    virtual void            GetUVGenMatrix(Matrix2F* mat) const;
    
    virtual bool            Map(ImageData* pdata, unsigned mipLevel, unsigned levelCount);
    virtual bool            Unmap();

    virtual bool            Update(const UpdateDesc* updates, unsigned count = 1, unsigned mipLevel = 0);    
    virtual bool            Update();

    // Copies the image data from the hardware.
    SF_AMP_CODE( virtual bool Copy(ImageData* pdata); )
};

// D3D9 DepthStencilSurface implementation. 
class DepthStencilSurface : public Render::DepthStencilSurface
{
public:
    DepthStencilSurface(TextureManagerLocks* pmanagerLocks, const ImageSize& size);
    ~DepthStencilSurface();

    virtual ImageSize               GetSize() const { return Size; }
    bool                            Initialize();

    ImageSize                 Size;
    ID3D1x(Texture2D)*        pDepthStencilSurface;
    ID3D1x(DepthStencilView)* pDepthStencilSurfaceView;
    Texture::CreateState      State;

};

// MappedTexture object repents a Texture mapped into memory with Texture::Map() call;
// it is also used internally during updates.
// The key part of this class is the Data object, stored Locked texture level plains.

class MappedTexture : public NewOverrideBase<StatRender_TextureManager_Mem>
{
    friend class Texture;

    Texture*                  pTexture;
    ID3D1x(DeviceContext)*    pDeviceContext;
    ID3D1x(Texture2D)*        pStagingTextures[Texture::MaxTextureCount];

    // We support mapping sub-range of levels, in which case
    // StartMipLevel may be non-zero.
    unsigned      StartMipLevel;
    int           LevelCount;
    // Pointer data that can be copied to.
    ImageData     Data;

    enum { PlaneReserveSize = 4 };
    ImagePlane    Planes[PlaneReserveSize];

public:
    MappedTexture()
        : pTexture(0), StartMipLevel(false), LevelCount(0) { memset(pStagingTextures, 0, sizeof pStagingTextures); }
    ~MappedTexture()
    {
        SF_ASSERT(!IsMapped());
    }

    bool        Reserve()  { AtomicOps<int>::CompareAndSet_Sync(&LevelCount, 0, -1); return LevelCount == -1; }
    bool        IsMapped() { return (LevelCount > 0); }
    bool        Map(Texture* ptexture, ID3D1x(Device)* pdevice, ID3D1x(DeviceContext)* pcontext, unsigned mipLevel, unsigned levelCount);
    void        Unmap();
};




// D3D11 Texture Manger.
// This class is responsible for creating textures and keeping track of them
// in the list.
// 

class TextureManager : public Render::TextureManager
{
    friend class Texture;
    friend class DepthStencilSurface;

    typedef ArrayConstPolicy<8, 8, false>   KillListArrayPolicy;
    typedef ArrayLH<ID3D1x(Resource)*,
                    StatRender_TextureManager_Mem,
                    KillListArrayPolicy>    D3DResourceArray;
    typedef ArrayLH<ID3D1x(View)*,
                    StatRender_TextureManager_Mem,
                    KillListArrayPolicy>    D3DViewArray;
    typedef List<Texture, Render::Texture>  TextureList;
    typedef List<DepthStencilSurface, Render::DepthStencilSurface> DepthStencilList;

    ID3D1x(Device)*         pDevice;
    ID3D1x(DeviceContext)*  pDeviceContext;
    ThreadId                RenderThreadId;
    MappedTexture           MappedTexture0;    
    ThreadCommandQueue*     pRTCommandQueue;    

    // Lists protected by TextureManagerLocks::TextureMutex.
    TextureList               Textures;
    TextureList               TextureInitQueue;
    DepthStencilList          DepthStencilInitQueue;
    D3DResourceArray          D3DTextureKillList;
    D3DViewArray              D3DTexViewKillList;
    
    // Texture format table, organized by supported HW features.
    ArrayLH<TextureFormat>   TextureFormats;

    static const unsigned       SamplerTypeCount = (Sample_Count * Wrap_Count);
    ID3D1x(SamplerState)*       SamplerStates[SamplerTypeCount];

    // Detecting redundant sampler/address setting.
    static const int MaximumStages = 4;
    ID3D1x(SamplerState)*       CurrentSamplers[MaximumStages];
    ID3D1x(View)*               CurrentTextures[MaximumStages];

    // Detects supported D3DFormats and capabilities.
    void            initTextureFormats();
    const Render::TextureFormat* getTextureFormat(ImageFormat format) const;    
    virtual bool    isMappable(const Render::TextureFormat* ptformat) { return ((TextureFormat*)ptformat)->pMapping->Mappable; }

    // Texture Memory-mapping support.
    MappedTexture*  mapTexture(Texture* p, unsigned mipLevel, unsigned levelCount);
    MappedTexture*  mapTexture(Texture* p) { return mapTexture(p, 0, p->MipLevels); }
    void            unmapTexture(Texture *ptexture);    
    
    virtual void    processTextureKillList();
    virtual void    processInitTextures();    


public:
    TextureManager(ID3D1x(Device)* pdevice,
                   ID3D1x(DeviceContext) * pcontext,
                   ThreadId renderThreadId, 
                   ThreadCommandQueue* commandQueue = 0);
    ~TextureManager();

    // Used once texture manager is no longer necessary.
    void    Reset();

    // Does rendundancy checking on state setting.
    void            SetSamplerState( unsigned stage, unsigned viewCount, ID3D1x(ShaderResourceView)** views, ID3D1x(SamplerState)* state = 0);

    virtual void    BeginScene();

    // *** TextureManager
    virtual Render::Texture* CreateTexture(ImageFormat format, unsigned mipLevels,
                                           const ImageSize& size,
                                           unsigned use, ImageBase* pimage,
                                           Render::MemoryManager* manager = 0);
    virtual Render::Texture* CreateTexture(ID3D1x(Texture2D)* pd3dtexture,
                                           ImageSize imgSize = ImageSize(0), ImageBase* image = 0);

    virtual unsigned        GetTextureUseCaps(ImageFormat format);
    bool IsMultiThreaded() const    { return RenderThreadId != 0; }

    virtual Render::DepthStencilSurface* CreateDepthStencilSurface(const ImageSize& size,
                                                           MemoryManager* manager = 0);
    virtual Render::DepthStencilSurface* CreateDepthStencilSurface(ID3D1x(Texture2D)* psurface);

	virtual bool			IsDrawableImageFormat(ImageFormat format) const { return (format == Image_B8G8R8A8) || (format == Image_R8G8B8A8); }
};


}}};  // namespace Scaleform::Render::D3D1x

#endif // INC_SF_D3D1X_TEXTURE_H
