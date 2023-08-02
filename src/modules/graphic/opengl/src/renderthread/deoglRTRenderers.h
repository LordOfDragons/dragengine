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
class deoglRenderConstructed;
class deoglRenderCompute;
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
class deoglRenderTransparentPasses;
class deoglRenderTranspCounting;
class deoglRenderVR;
class deoglRenderWorld;


/**
 * Render thread renderers.
 */
class deoglRTRenderers{
private:
	deoglRenderCanvas *pCanvas;
	deoglRenderConstructed *pConstructed;
	deoglRenderCompute *pCompute;
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
	deoglRenderVR *pVR;
	deoglRenderWorld *pWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render thread renderers. */
	deoglRTRenderers( deoglRenderThread &renderThread );
	
	/** Clean up render thread renderers. */
	~deoglRTRenderers();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Canvas renderer. */
	inline deoglRenderCanvas &GetCanvas() const{ return *pCanvas; }
	
	/** Constructed renderer. */
	inline deoglRenderConstructed &GetConstructed() const{ return *pConstructed; }
	
	/** Compute renderer. */
	inline deoglRenderCompute &GetCompute() const{ return *pCompute; }
	
	/** Debug renderer. */
	inline deoglRenderDebug &GetDebug() const{ return *pDebug; }
	
	/** Debug drawer renderer. */
	inline deoglRenderDebugDrawer &GetDebugDrawer() const{ return *pDebugDrawer; }
	
	/** Render depth pass. */
	inline deoglRenderDepthPass &GetDepthPass(){ return *pDepthPass; }
	
	/** Developer mode renderer. */
	inline deoglRenderDevMode &GetDevMode() const{ return *pDevMode; }
	
	/** Geometry renderer. */
	inline deoglRenderGeometry &GetGeometry() const{ return *pGeometry; }
	
	/** Geometry pass renderer. */
	inline deoglRenderGeometryPass &GetGeometryPass() const{ return *pGeometryPass; }
	
	/** Occlusion renderer. */
	inline deoglRenderOcclusion &GetOcclusion() const{ return *pOcclusion; }
	
	/** Light renderer. */
	inline deoglRenderLight &GetLight() const{ return *pLight; }
	
	/** Particles renderer. */
	inline deoglRenderParticles &GetParticles() const{ return *pParticles; }
	
	/** Reflection renderer. */
	inline deoglRenderReflection &GetReflection() const{ return *pReflection; }
	
	/** Sky renderer. */
	inline deoglRenderSky &GetSky() const{ return *pSky; }
	
	/** Tone mapping renderer. */
	inline deoglRenderToneMap &GetToneMap() const{ return *pToneMap; }
	
	/** Transparent passes renderer. */
	inline deoglRenderTransparentPasses &GetTransparentPasses(){ return *pTransparentPasses; }
	
	/** Transparency counter. */
	inline deoglRenderTranspCounting &GetTransparencyCounter(){ return *pTransparencyCounter; }
	
	/** VR renderer. */
	inline deoglRenderVR &GetVR() const{ return *pVR; }
	
	/** World renderer. */
	inline deoglRenderWorld &GetWorld() const{ return *pWorld; }
	
	
	
	/** Developer mode debug information changed. */
	void DevModeDebugInfoChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
