#pragma once

#include <memory>

#include "d3d9.h"
#include "settings.h"

// Multi-API abstraction of a rendertarget
class RenderTarget {
public:
	typedef enum { SURFACE_USE = 0, TEXTURE_USE = 1 } Usage;
	typedef enum { ARGB_8, RGB_8, ARGB_16, RGB_16, ARGB_FP16, RGB_FP16 } Format;

	static D3DFORMAT d3dformatFromFormat(Format fmt) {
		switch(fmt) {
		case ARGB_8: return D3DFMT_A8R8G8B8;
		case RGB_8: return D3DFMT_R8G8B8;
		case ARGB_16: return D3DFMT_A16B16G16R16;
		case RGB_16: return D3DFMT_A16B16G16R16;
		case ARGB_FP16: return D3DFMT_A16B16G16R16F;
		case RGB_FP16: return D3DFMT_A16B16G16R16F;
		}
	}

	~RenderTarget() {
		SAFERELEASE(surf9);
		SAFERELEASE(tex9);
	}

	// TODO remove once migration is complete
	IDirect3DSurface9 *getSurf() { return surf9; };
	IDirect3DTexture9 *getTex() { return tex9; };

private:
	friend class RenderTargetManager;

	IDirect3DSurface9 *surf9;
	IDirect3DTexture9 *tex9;

	RenderTarget(IDirect3DDevice9* dev, unsigned width, unsigned height, D3DFORMAT fmt, Usage usage) 
			: surf9(NULL), tex9(NULL) {
		if(usage == TEXTURE_USE) {
			dev->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, fmt, D3DPOOL_DEFAULT, &tex9, NULL);
			tex9->GetSurfaceLevel(0, &surf9);
		}
		else {
			dev->CreateRenderTarget(width, height, fmt, D3DMULTISAMPLE_NONE, 0, false, &surf9, NULL);
		}
	}

	RenderTarget(const RenderTarget& rt);
};

typedef std::unique_ptr<RenderTarget> RenderTargetPtr;
