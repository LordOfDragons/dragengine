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

#ifndef _DEOGLRRTRENDRERS_H_
#define _DEOGLRRTRENDRERS_H_

#include "../deoglBasics.h"

class deoglRenderCanvas;
class deoglRenderDebug;
class deoglRenderDebugDrawer;
class deoglRenderDepthPass;
class deoglRenderDevMode;
class deoglRenderGeometry;
class deoglRenderGeometryPass;
class deoglRenderLight;
class deoglRenderOcclusion;
class deoglRenderParticles;
class deoglRenderReflection;
class deoglRenderSky;
class deoglRenderThread;
class deoglRenderToneMap;
class deoglRenderTranspCounting;
class deoglRenderTransparentPasses;
class deoglRenderWorld;


/**
 * \brief Render thread renderers.
 */
class deoglRTRenderers{
private:
	deoglRenderCanvas *pCanvas;
	deoglRenderDebugDrawer *pDebugDrawer;
	deoglRenderDebug *pDebug;
	deoglRenderDepthPass *pDepthPass;
	deoglRenderDevMode *pDevMode;
	deoglRenderGeometry *pGeometry;
	deoglRenderGeometryPass *pGeometryPass;
	deoglRenderLight *pLight;
	deoglRenderOcclusion *pOcclusion;
	deoglRenderParticles *pParticles;
	deoglRenderReflection *pReflection;
	deoglRenderSky *pSky;
	deoglRenderToneMap *pToneMap;
	deoglRenderTranspCounting *pTransparencyCounter;
	deoglRenderTransparentPasses *pTransparentPasses;
	deoglRenderWorld *pWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render thread renderers. */
	deoglRTRenderers( deoglRenderThread &renderThread );
	
	/** \brief Clean up render thread renderers. */
	~deoglRTRenderers();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas renderer. */
	inline deoglRenderCanvas &GetCanvas() const{ return *pCanvas; }
	
	/** \brief Debug renderer. */
	inline deoglRenderDebug &GetDebug() const{ return *pDebug; }
	
	/** \brief Debug drawer renderer. */
	inline deoglRenderDebugDrawer &GetDebugDrawer() const{ return *pDebugDrawer; }
	
	/** \brief Render depth pass. */
	inline deoglRenderDepthPass &GetDepthPass(){ return *pDepthPass; }
	
	/** \brief Developer mode renderer. */
	inline deoglRenderDevMode &GetDevMode() const{ return *pDevMode; }
	
	/** \brief Geometry renderer. */
	inline deoglRenderGeometry &GetGeometry() const{ return *pGeometry; }
	
	/** \brief Geometry pass renderer. */
	inline deoglRenderGeometryPass &GetGeometryPass() const{ return *pGeometryPass; }
	
	/** \brief Occlusion renderer. */
	inline deoglRenderOcclusion &GetOcclusion() const{ return *pOcclusion; }
	
	/** \brief Light renderer. */
	inline deoglRenderLight &GetLight() const{ return *pLight; }
	
	/** \brief Particles renderer. */
	inline deoglRenderParticles &GetParticles() const{ return *pParticles; }
	
	/** \brief Reflection renderer. */
	inline deoglRenderReflection &GetReflection() const{ return *pReflection; }
	
	/** \brief Sky renderer. */
	inline deoglRenderSky &GetSky() const{ return *pSky; }
	
	/** \brief Tone mapping renderer. */
	inline deoglRenderToneMap &GetToneMap() const{ return *pToneMap; }
	
	/** \brief Transparent passes renderer. */
	inline deoglRenderTransparentPasses &GetTransparentPasses(){ return *pTransparentPasses; }
	
	/** \brief Transparency counter. */
	inline deoglRenderTranspCounting &GetTransparencyCounter(){ return *pTransparencyCounter; }
	
	/** \brief World renderer. */
	inline deoglRenderWorld &GetWorld() const{ return *pWorld; }
	
	
	
	/** \brief Developer mode debug information changed. */
	void DevModeDebugInfoChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
