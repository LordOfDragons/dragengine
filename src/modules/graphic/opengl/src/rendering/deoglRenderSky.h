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

#ifndef _DEOGLRENDERSKY_H_
#define _DEOGLRENDERSKY_H_

#include "deoglRenderBase.h"

#include <dragengine/common/collection/decPointerList.h>


class deoglRSkyInstance;
class deoglShaderProgram;
class deoglRenderPlan;
class deoglEnvironmentMap;
class deoglRWorld;
class decLayerMask;



/**
 * \brief OpenGL sky renderer.
 */
class deoglRenderSky : public deoglRenderBase{
private:
	deoglShaderProgram *pShaderSkySphere;
	deoglShaderProgram *pShaderSkyBox;
	deoglShaderProgram *pShaderBody;
	
	decPointerList pSkyInstances;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderer. */
	deoglRenderSky( deoglRenderThread &renderThread );
	
	/** \brief Clean up renderer. */
	~deoglRenderSky();
	/*@}*/
	
	
	
	/** \name Rendering */
	/*@{*/
	/**
	 * \brief Render sky.
	 * 
	 * Uses FBO Depth+Color. Clears color buffer. Depth used for rendering only background.
	 * - RenderSkyBox
	 * - RenderSkySphere
	 * Invalidates no attachments.
	 */
	void RenderSky( deoglRenderPlan &plan );
	
	/** \brief Render sky. */
	//void RenderSkyOld( deoglRenderPlan &plan );
	
	/** \brief Render sky box layer. */
	bool RenderSkyBox( deoglRenderPlan &plan, deoglRSkyInstance &instance,
		int layerIndex, bool first );
	
	/** \brief Render sky sphere layer. */
	bool RenderSkySphere( deoglRenderPlan &plan, deoglRSkyInstance &instance,
		int layerIndex, bool first );
	
	/** \brief Render sky bodies layer. */
	void RenderSkyLayerBodies( deoglRenderPlan &plan, deoglRSkyInstance &instance,
		int layerIndex );
	
	/** \brief Render sky into an environment map. */
	void RenderSkyIntoEnvMap( deoglRWorld &world, const decLayerMask &layerMask,
		deoglEnvironmentMap &envmap );
	
	/** \brief Render empty sky into an environment map. */
	void RenderEmptySkyIntoEnvMap( deoglRWorld &world, deoglEnvironmentMap &envmap );
	
	/** \brief Get sky background color in linear space. */
	decColor LinearBgColor( const deoglRSkyInstance &instance, bool first ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
