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
	deoglRTRenderers(deoglRenderThread &renderThread);
	
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
