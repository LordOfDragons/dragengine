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

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>

class deoglPixelBuffer;
class deoglRenderThread;
class deoglTexture;
class deoglCubeMap;
class deoglArrayTexture;
class deImage;



/**
 * \brief Render image.
 */
class deoglRImage : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	const decString pFilename;
	const int pWidth;
	const int pHeight;
	const int pDepth;
	const int pComponentCount;
	const int pBitCount;
	
	deoglPixelBuffer *pPixelBuffer;
	deoglTexture *pTexture;
	deoglCubeMap *pCubeMap;
	deoglArrayTexture *pArrayTexture;
	bool pSkinUse;
	
	float pScaleU;
	float pScaleV;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render image. */
	deoglRImage( deoglRenderThread &renderThread, const deImage &image );
	
	/** \brief Clean up render image. */
	virtual ~deoglRImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Filename of engine image resource or empty string if manually created. */
	inline const decString &GetFilename() const{ return pFilename; }
	
	/** \brief Width of image. */
	inline int GetWidth() const{ return pWidth; }
	
	/** \brief Height of image. */
	inline int GetHeight() const{ return pHeight; }
	
	/** \brief Depth of image. */
	inline int GetDepth() const{ return pDepth; }
	
	/** \brief Component count of image. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Bit count image. */
	inline int GetBitCount() const{ return pBitCount; }
	
	
	
	/**
	 * \brief Set pixel or \em NULL if not set.
	 * \warning Called during synchronization from main thread only.
	 */
	void SetPixelBuffer( deoglPixelBuffer *pixelBuffer );
	
	
	
	/** \brief Texture or \em NULL. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** \brief Cubemap or \em NULL. */
	inline deoglCubeMap *GetCubeMap() const{ return pCubeMap; }
	
	/** \brief Array texture or \em NULL if not created yet. */
	inline deoglArrayTexture *GetArrayTexture() const{ return pArrayTexture; }
	
	/**
	 * \brief Set texture or \em NULL.
	 * 
	 * For use by skin channel building. Once the texture has been set it should not be
	 * replaced by a new texture object or texture unit configurations of existing
	 * skin channels will break. If texture is not NULL the texture parameters have to
	 * be changed instead.
	 * 
	 * \warning Has to be called from render thread only.
	 */
	void SetTexture( deoglTexture *texture );
	
	/**
	 * \brief Set cubemap or \em NULL.
	 * 
	 * For use by skin channel building. Once the texture has been set it should not be
	 * replaced by a new texture object or texture unit configurations of existing
	 * skin channels will break. If texture is not NULL the texture parameters have to
	 * be changed instead.
	 * 
	 * \warning Has to be called from render thread only.
	 */
	void SetCubeMap( deoglCubeMap *cubemap );
	
	/**
	 * \brief Array texture or \em NULL if not created yet.
	 * 
	 * For use by skin channel building. Once the texture has been set it should not be
	 * replaced by a new texture object or texture unit configurations of existing
	 * skin channels will break. If texture is not NULL the texture parameters have to
	 * be changed instead.
	 * 
	 * \warning Has to be called from render thread only.
	 */
	void SetArrayTexture( deoglArrayTexture *arrayTexture );
	
	/** \brief Texture is used by skin. */
	inline bool GetSkinUse() const{ return pSkinUse; }
	
	/** \brief Scaling factor in U direction. */
	inline float GetScaleFactorU() const{ return pScaleU; }
	
	/** \brief Scaling factor in V direction. */
	inline float GetScaleFactorV() const{ return pScaleV; }
	
	
	
	/**
	 * \brief Prepare texture for rendering.
	 * 
	 * Creates the texture if not present and updates the content if a pixel buffer is present.
	 */
	void PrepareForRender();
	/*@}*/
	
private:
	void pCleanUp();
	void pReleaseTextures();
	void pDirectReleaseTextures();
	void pUpdateSkinMemoryUsage( bool add );
};

#endif
