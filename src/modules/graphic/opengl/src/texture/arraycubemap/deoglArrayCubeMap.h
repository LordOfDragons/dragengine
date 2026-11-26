/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLARRAYCUBEMAP_H_
#define _DEOGLARRAYCUBEMAP_H_

#include "../cubemap/deoglCubeMap.h"

#include <dragengine/deTUniqueReference.h>

class deoglPixelBuffer;
class deoglRenderThread;
class deoglCapsTextureFormat;


/**
 * OpenGL array cube map texture.
 */
class deoglArrayCubeMap{
public:
	/** \brief Reference. */
	using Ref = deTUniqueReference<deoglArrayCubeMap>;
	
	
public:
	deoglRenderThread &pRenderThread;
	
	GLuint pTexture;
	const deoglCapsTextureFormat *pFormat;
	
	int pSize, pLayerCount;
	bool pMipMapped;
	int pMipMapLevelCount;
	int pRealMipMapLevelCount;
	
	deoglMemoryConsumptionTextureUse pMemUse;
	decString pDebugObjectLabel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new opengl array cube map. */
	explicit deoglArrayCubeMap(deoglRenderThread &renderThread);
	/** Cleans up the opengl array cube map. */
	~deoglArrayCubeMap();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the texture handle. */
	inline GLuint GetTexture() const{ return pTexture; }
	
	/** Size. */
	inline int GetSize() const{ return pSize; }
	
	/** Layer count. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Sets size and layer count. */
	void SetSize(int width, int layerCount);
	
	/** Retrieves the texture format. */
	inline const deoglCapsTextureFormat *GetFormat() const{ return pFormat; }
	
	/** Set texture format. */
	void SetFormat(const deoglCapsTextureFormat *format);
	
	/** Set texture format suitable for texture mapping according to the provided texture description. */
	void SetMapingFormat(int channels, bool useFloat, bool compressed);
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormat(int channels, bool useFloat);
	
	/** Set texture format suitable for rendering to an integral texture using an FBO. */
	void SetFBOFormatIntegral(int channels, int bpp, bool useUnsigned);
	
	/** Set texture format suitable for attaching as FBO render target. */
	void SetFBOFormatSNorm(int channels, int bpp);
	
	/** Set depth texture format suitable for attaching as FBO render target. */
	void SetDepthFormat(bool packedStencil, bool useFloat);
	
	/** Set texture format by number from the list of mapping texture formats to use. */
	void SetFormatMappingByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber);
	
	/** Set texture format by number from the list of fbo texture formats to use. */
	void SetFormatFBOByNumber(deoglCapsFmtSupport::eUseTextureFormats formatNumber);
	
	/** Determines if mip mapping has to be used on this texture. */
	inline bool GetMipMapped() const{ return pMipMapped; }
	/** Sets if mip mapping has to be used on this texture. */
	void SetMipMapped(bool mipmapped);
	/** Retrieves the mip map level count or 0 to let the hardware auto-generate them. */
	inline int GetMipMapLevelCount() const{ return pMipMapLevelCount; }
	/** Sets the mip map level count or 0 to let the hardware auto-generate them. */
	void SetMipMapLevelCount(int count);
	/** Retrieves the real mip map level count which is either mipMapLevelCount or the real texture mip map level count. */
	inline int GetRealMipMapLevelCount() const{ return pRealMipMapLevelCount; }
	
	/** Creates the texture if not created yet. */
	void CreateTexture();
	/** Destroys the texture if existing. */
	void DestroyTexture();
	/** Sets the base level texture pixels from a pixel buffer. */
	void SetPixels(const deoglPixelBuffer &pixels);
	/** Sets texture level pixels from a pixel buffer. */
	void SetPixelsLevel(int level, const deoglPixelBuffer &pixels);
	
	/** Copy pixels from first level into pixel buffer. */
	void GetPixels(deoglPixelBuffer &pixelBuffer) const;
	
	/** Copy pixels from level into pixel buffer. */
	void GetPixelsLevel(int level, deoglPixelBuffer &pixelBuffer) const;
	
	/** Retrieves the size of a mip map level. */
	int GetLevelSize(int level) const;
	
	/** Force mip map creation if the texture is mip mapped. */
	void CreateMipMaps();
	
	/** Copy from another array cube map to this texture. */
	void CopyFrom(const deoglArrayCubeMap &texture, bool withMipMaps);
	
	/** Copy from another array cube map to this texture. */
	void CopyFrom(const deoglArrayCubeMap &texture, bool withMipMaps, int srcLayer, int destLayer);
	
	/** Copy area from another array cube map to this texture. */
	void CopyFrom(const deoglArrayCubeMap &texture, bool withMipMaps, int srcLayer, int destLayer,
		int size, int layerCount, int srcX, int srcY, int destX, int destY);
	
	/** Copy from another cube map to this texture. */
	void CopyFrom(const deoglCubeMap &cubemap, bool withMipMaps, int destLayer);
	
	/** Copy area from another cube map to this texture. */
	void CopyFrom(const deoglCubeMap &cubemap, bool withMipMaps, int destLayer,
		int size, int srcX, int srcY, int destX, int destY);
	
	/** Memory consumption. */
	inline const deoglMemoryConsumptionTextureUse &GetMemoryConsumption() const{ return pMemUse; }
	
	/** Update memory usage. */
	void UpdateMemoryUsage();
	
	/** Set debug object label. */
	void SetDebugObjectLabel(const char *name);
	/*@}*/
	
	/** \name Helper Functions */
	/*@{*/
	/** Creates a camera matrix for a cube map face. */
	static void CreateMatrixForFace(decMatrix &matrix, const decVector &position, int face);
	static void CreateMatrixForFace(decDMatrix &matrix, const decDVector &position, int face);
	/*@}*/
	
private:
	void pUpdateDebugObjectLabel();
};

#endif