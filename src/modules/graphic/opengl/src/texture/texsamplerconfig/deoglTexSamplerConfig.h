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

#ifndef _DEOGLTEXSAMPLERCONFIG_H_
#define _DEOGLTEXSAMPLERCONFIG_H_

#include "../deoglTextureStageManager.h"

class deoglRenderThread;



/**
 * \brief OpenGL texture sampler configuration.
 */
class deoglTexSamplerConfig{
private:
	deoglRenderThread &pRenderThread;
	
	GLuint pSamplerObject;
	
	int pMinLodLevel;
	int pMaxLodLevel;
	deoglTextureStageManager::eTextureFiltering pFilterMode;
	GLenum pWrapMode;
	bool pDepthCompareMode;
	GLenum pDepthCompareFunc;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create opengl texture sampler configuration. */
	deoglTexSamplerConfig( deoglRenderThread &renderThread );
	
	/** \brief Clean up opengl texture sampler configuration. */
	~deoglTexSamplerConfig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Sampler object handle. */
	inline GLuint GetSamplerObject() const{ return pSamplerObject; }
	
	
	
	/** \brief Minimum lod level. */
	inline int GetMinLodLevel() const{ return pMinLodLevel; }
	
	/** \brief Set minimum lod level. */
	void SetMinLodLevel( int level );
	
	/** \brief Maximum lod level. */
	inline int GetMaxLodLevel() const{ return pMaxLodLevel; }
	
	/** \brief Set maximum lod level. */
	void SetMaxLodLevel( int level );
	
	/** \brief Filter mode. */
	inline deoglTextureStageManager::eTextureFiltering GetFilterMode() const{ return pFilterMode; }
	
	/** \brief Set filter mode. */
	void SetFilterMode( deoglTextureStageManager::eTextureFiltering filterMode );
	
	/** \brief Wrapping mode. */
	inline GLenum GetWrapMode() const{ return pWrapMode; }
	
	/** \brief Set wrapping mode. */
	void SetWrapMode( GLenum wrapMode );
	
	/** \brief Texture uses depth compare mode. */
	inline bool GetDepthCompareMode() const{ return pDepthCompareMode; }
	
	/** \brief Set if texture uses depth compare mode. */
	void SetDepthCompareMode( bool depthCompareMode );
	
	/** \brief Depth compare function. */
	inline GLenum GetDepthCompareFunc() const{ return pDepthCompareFunc; }
	
	/** \brief Set depth compare function. */
	void SetDepthCompareFunc( GLenum func );
	/*@}*/
};

#endif
