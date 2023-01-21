/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
