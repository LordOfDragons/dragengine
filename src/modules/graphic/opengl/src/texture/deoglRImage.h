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

#ifndef _DEOGLRIMAGE_H_
#define _DEOGLRIMAGE_H_

#include "pixelbuffer/deoglPixelBuffer.h"
#include "../memory/consumption/deoglMemoryConsumptionSkinUse.h"

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglArrayTexture;
class deImage;



/**
 * Render image.
 */
class deoglRImage : public deObject{
public:
	typedef deTObjectReference<deoglRImage> Ref;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	
	const decString pFilename;
	const int pWidth;
	const int pHeight;
	const int pDepth;
	const int pComponentCount;
	const int pBitCount;
	
	deoglPixelBuffer::Ref pPixelBuffer;
	deoglTexture *pTexture;
	deoglCubeMap *pCubeMap;
	deoglArrayTexture *pArrayTexture;
	bool pSkinUse;
	
	float pScaleU;
	float pScaleV;
	
	deoglMemoryConsumptionSkinUse pSkinMemUse;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render image. */
	deoglRImage( deoglRenderThread &renderThread, const deImage &image );
	
protected:
	/** Clean up render image. */
	virtual ~deoglRImage();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Filename of engine image resource or empty string if manually created. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** Width of image. */
	inline int GetWidth() const{ return pWidth; }
	
	/** Height of image. */
	inline int GetHeight() const{ return pHeight; }
	
	/** Depth of image. */
	inline int GetDepth() const{ return pDepth; }
	
	/** Component count of image. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Bit count image. */
	inline int GetBitCount() const{ return pBitCount; }
	
	
	
	/**
	 * Set pixel or nullptr if not set.
	 * \warning Called during synchronization from main thread only.
	 */
	void SetPixelBuffer( deoglPixelBuffer *pixelBuffer );
	
	
	
	/** Texture or nullptr. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Cubemap or nullptr. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** Array texture or nullptr if not created yet. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/**
	 * Set texture or nullptr.
	 * 
	 * For use by skin channel building. Once the texture has been set it should not be
	 * replaced by a new texture object or texture unit configurations of existing
	 * skin channels will break. If texture is not nullptr the texture parameters have to
	 * be changed instead.
	 * 
	 * \warning Has to be called from render thread only.
	 */
	void SetTexture( deoglTexture *texture );
	
	/**
	 * Set cubemap or nullptr.
	 * 
	 * For use by skin channel building. Once the texture has been set it should not be
	 * replaced by a new texture object or texture unit configurations of existing
	 * skin channels will break. If texture is not nullptr the texture parameters have to
	 * be changed instead.
	 * 
	 * \warning Has to be called from render thread only.
	 */
	void SetCubeMap( deoglCubeMap *cubemap );
	
	/**
	 * Array texture or nullptr if not created yet.
	 * 
	 * For use by skin channel building. Once the texture has been set it should not be
	 * replaced by a new texture object or texture unit configurations of existing
	 * skin channels will break. If texture is not nullptr the texture parameters have to
	 * be changed instead.
	 * 
	 * \warning Has to be called from render thread only.
	 */
	void SetArrayTexture( deoglArrayTexture *arrayTexture );
	
	/** Texture is used by skin. */
	inline bool GetSkinUse() const{ return pSkinUse; }
	
	/** Scaling factor in U direction. */
	inline float GetScaleFactorU() const{ return pScaleU; }
	
	/** Scaling factor in V direction. */
	inline float GetScaleFactorV() const{ return pScaleV; }
	
	
	
	/**
	 * Prepare texture for rendering.
	 * 
	 * Creates the texture if not present and updates the content if a pixel buffer is present.
	 */
	void PrepareForRender();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pReleaseTextures();
	void pDirectReleaseTextures();
	void pUpdateSkinMemoryUsage();
};

#endif
