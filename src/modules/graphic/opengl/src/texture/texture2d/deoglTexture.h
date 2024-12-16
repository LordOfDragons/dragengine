/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLTEXTURE_H_
#define _DEOGLTEXTURE_H_

#ifdef BACKEND_OPENGL
	#include "../../deoglBasics.h"
	#include "../../capabilities/deoglCapsFmtSupport.h"
#elif defined BACKEND_VULKAN
	#include "image/devkImage.h"
	#include "devkDevice.h"
#endif

#include "../../memory/consumption/deoglMemoryConsumptionTextureUse.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglPixelBuffer;
class deoglCapsTextureFormat;



/**
 * OpenGL 2D texture
 * 
 * Manages an OpenGL texture. The texture is resizable and can be created
 * once required. By default the texture is not created. After changing the
 * size the texture is destroyed too. The property flags like mipmapped are
 * informative flags and do not influence the texture creation. They are
 * stored for external code to know what sort of texture they deal with.
 * Pixel data can be loaded into the texture using pixel buffer objects. Any
 * kind of pixel buffer can be used.
 */
class deoglTexture{
public:
	deoglRenderThread &pRenderThread;
	
#ifdef BACKEND_OPENGL
	GLuint pTexture;
	const deoglCapsTextureFormat *pFormat;
	
#elif defined BACKEND_VULKAN
	devkImage::Ref pImage;
	const devkFormat *pFormat;
#endif
	
	decPoint pSize;
	int pMipMapLevelCount;
	int pRealMipMapLevelCount;
	bool pMipMapped;
	
	deoglMemoryConsumptionTextureUse pMemUse;
	decString pDebugObjectLabel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create opengl texture. */
	deoglTexture(deoglRenderThread &renderThread);
	
	/** Clean up opengl texture. */
	~deoglTexture();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
#ifdef BACKEND_OPENGL
	/** Texture handle. */
	inline GLuint GetTexture() const{ return pTexture; }
	
	/** Texture format. */
	inline const deoglCapsTextureFormat *GetFormat() const{ return pFormat; }
	
	/** Set texture format. */
	void SetFormat(const deoglCapsTextureFormat *format);
	
	/** Set texture format by number from the list of mapping texture formats to use. */
	void SetFormatMappingByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber);
	
	/** Set texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber);
	
#elif defined BACKEND_VULKAN
	/** Image or nullptr. */
	inline const devkImage::Ref &GetImage() const{ return pImage; }
	
	/** Texture format. */
	inline const devkFormat *GetFormat() const{ return pFormat; }
	
	/** Set texture format. */
	void SetFormat(const devkFormat *format);
	
	/** Set texture format by number from the list of image formats to use. */
	void SetFormatMappingByNumber(devkDevice::eFormats format);
	
	/** Set texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber(devkDevice::eFormats format);
#endif
	
	
	/** Width in pixels. */
	inline int GetWidth() const{ return pSize.x; }
	
	/** Height in pixels. */
	inline int GetHeight() const{ return pSize.y; }
	
	/** Size. */
	inline const decPoint &GetSize() const{ return pSize; }
	
	/** Set size in pixels destroying opengl texture if present. */
	void SetSize(int width, int height);
	
	/** Set size in pixels destroying opengl texture if present. */
	void SetSize(const decPoint &size);
	
	
	
	/** Mip mapping has to be used on this texture. */
	inline bool GetMipMapped() const{ return pMipMapped; }
	
	/** Set if mip mapping has to be used on this texture. */
	void SetMipMapped(bool mipmapped);
	
	/** Mip map level count or 0 to let the hardware auto-generate them. */
	inline int GetMipMapLevelCount() const{ return pMipMapLevelCount; }
	
	/** Set mip map level count or 0 to let the hardware auto-generate them. */
	void SetMipMapLevelCount(int count);
	
	/**
	 * Real mip map level count.
	 * \details Is either mipMapLevelCount or the real texture mip map level count.
	 */
	inline int GetRealMipMapLevelCount() const{ return pRealMipMapLevelCount; }
	
	
	
	/** Create texture if not created yet. */
	void CreateTexture();
	
	/** Destroy texture if existing. */
	void DestroyTexture();
	
	/** Set base level texture pixels from a pixel buffer. */
	void SetPixels(const deoglPixelBuffer &pixelBuffer);
	
	/** Set texture level pixels from a pixel buffer. */
	void SetPixelsLevel(int level, const deoglPixelBuffer &pixelBuffer);
	
	/** Set texture level pixels from a pixel buffer layer. */
	void SetPixelsLevelLayer(int level, const deoglPixelBuffer &pixelBuffer, int layer);
	
	/** Copy pixels from first level into pixel buffer. */
	void GetPixels(deoglPixelBuffer &pixelBuffer) const;
	
	/** Copy pixels from level into pixel buffer. */
	void GetPixelsLevel(int level, deoglPixelBuffer &pixelBuffer) const;
	
	/** Size of a mip map level. */
	void GetLevelSize(int level, int &width, int &height) const;
	
	/** Create mip maps if texture foramt is mip mapped. */
	void CreateMipMaps();
	
	/** Copy from another texture to this texture. */
	void CopyFrom(const deoglTexture &texture, bool withMipMaps);
	
	/** Copy area from another texture to this texture. */
	void CopyFrom(const deoglTexture &texture, bool withMipMaps,
		int width, int height, int srcX, int srcY, int destX, int destY);
	
	
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionTextureUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** Update memory usage. */
	void UpdateMemoryUsage();
	/*@}*/
	
	
	
	/** \name Helper Functions */
	/*@{*/
	/** Set texture format suitable for texture mapping. */
	void SetMapingFormat(int channels, bool useFloat, bool compressed);
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormat(int channels, bool useFloat);
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatFloat32(int channels);
	
	/** Set texture format suitable for rendering to an integral texture using an FBO. */
	void SetFBOFormatIntegral(int channels, int bpp, bool useUnsigned);
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatSNorm(int channels, int bpp);
	
	/** Set depth texture format suitable for attaching as FBO render target. */
	void SetDepthFormat(bool packedStencil, bool useFloat);
	
	/** Set debug object label. */
	void SetDebugObjectLabel(const char *name);
	/*@}*/
	
private:
	void pUpdateDebugObjectLabel();
};

#endif
