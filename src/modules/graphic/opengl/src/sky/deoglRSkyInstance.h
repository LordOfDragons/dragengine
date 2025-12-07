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

#ifndef _DEOGLRSKYINSTANCE_H_
#define _DEOGLRSKYINSTANCE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include "deoglRSky.h"

class deoglRWorld;
class deoglRenderThread;
class deoglRSkyInstanceLayer;
class deoglGIState;
class deoglRComponent;

class deSkyInstance;



/**
 * Render sky instance.
 */
class deoglRSkyInstance : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	
	deoglRSky::Ref pRSky;
	
	int pOrder;
	decLayerMask pLayerMask;
	float pPassthroughTransparency;
	
	float *pControllerStates;
	int pControllerStateCount;
	
	deoglRSkyInstanceLayer **pLayers;
	int pLayerCount;
	
	float pTotalSkyLightIntensity;
	float pTotalSkyAmbientIntensity;
	decColor pTotalSkyLightColor;
	
	float pEnvMapTimer;
	
	bool pSkyNeedsUpdate;
	bool pWorldMarkedRemove;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRSkyInstance> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create sky instance. */
	deoglRSkyInstance(deoglRenderThread &renderThread);
	
	/** Clean up render sky instance. */
	virtual ~deoglRSkyInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Parent world or \em NULL. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL. */
	void SetParentWorld(deoglRWorld *world);
	
	
	
	/** Render sky or \em NULL. */
	inline const deoglRSky::Ref &GetRSky() const{ return pRSky; }
	
	/**
	 * Set render sky or \em NULL.
	 * 
	 * Called during synchronization time.
	 */
	void SetRSky(deoglRSky *rsky);
	
	
	
	/** Rendering order. */
	inline int GetOrder() const{ return pOrder; }
	
	/** Set rendering order. */
	void SetOrder(int order);
	
	/** Layer mask. */
	inline  const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask(const decLayerMask &layerMask);
	
	/** Passthrough transparency factor. */
	inline float GetPassthroughTransparency() const{ return pPassthroughTransparency; }
	
	/** Set passthrough transparency factor. */
	void SetPassthroughTransparency(float transparency);
	
	
	/** Number of controller states. */
	inline int GetControllerStateCount() const{ return pControllerStateCount; }
	
	/** Controller state at index. */
	float GetControllerStateAt(int index) const;
	
	/**
	 * Update controller states.
	 * 
	 * Called during synchronization time.
	 */
	void UpdateControllerStates(const deSkyInstance &instance);
	
	
	
	/** Number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** Layer at index. */
	deoglRSkyInstanceLayer &GetLayerAt(int index) const;
	
	/** Rebuild layers. */
	void RebuildLayers();
	
	/** Update layers. */
	void UpdateLayers();
	
	
	
	/** Total sky light intensity. */
	inline float GetTotalSkyLightIntensity() const{ return pTotalSkyLightIntensity; }
	
	/** Total sky ambient only light intensity. */
	inline float GetTotalSkyAmbientIntensity() const{ return pTotalSkyAmbientIntensity; }
	
	/** Total sky light color. */
	inline const decColor &GetTotalSkyLightColor() const{ return pTotalSkyLightColor; }
	
	
	
	/** Environment map timer. */
	inline float GetEnvironmentMapTimer() const{ return pEnvMapTimer; }
	
	/** Set environment map timer. */
	void SetEnvironmentMapTimer(float timer);
	
	
	
	/** Prepare for rendering. */
	void PrepareForRender();
	
	/** Notifiy world about changes in the sky if required. */
	void NotifySkyChanged();
	
	/** Drop all pointers to GI State. */
	void DropGIState(const deoglGIState *giState);
	
	/** Drop all pointers to GI States. */
	void DropAllGIStates();
	
	
	
	/** Prepare for quick disposal of sky instance. */
	void PrepareQuickDispose();
	
	/** Notify skies render static component changed requiring updates. */
	void NotifyUpdateStaticComponent(deoglRComponent *component);
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove(bool marked);
	/*@}*/
};

#endif
