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

#ifndef _DEOGLSKYINSTANCE_H_
#define _DEOGLSKYINSTANCE_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicSkyInstance.h>
#include "deoglRSkyInstance.h"

class deoglSky;

class deGraphicOpenGl;
class deSkyInstance;
class deoglWorld;



/**
 * Sky peer.
 */
class deoglSkyInstance : public deBaseGraphicSkyInstance{
private:
	deGraphicOpenGl &pOgl;
	const deSkyInstance &pInstance;
	
	deoglRSkyInstance::Ref pRInstance;
	deoglWorld *pParentWorld;
	
	deoglSky *pOglSky;
	float pEnvMapTimer;
	
	bool pDirtySky;
	bool pDirtyControllers;
	bool pDirtyLayerMask;
	unsigned int pSkyUpdateState;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new sky. */
	deoglSkyInstance(deGraphicOpenGl &ogl, const deSkyInstance &instance);
	
	/** Cleans up the sky. */
	virtual ~deoglSkyInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Sky instance. */
	inline const deSkyInstance &GetInstance() const{ return pInstance; }
	
	
	
	/** Render instance. */
	inline const deoglRSkyInstance::Ref &GetRInstance() const{ return pRInstance; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	/** Update sky instance. */
	void Update(float elapsed);
	
	
	
	/** Parent world or \em NULL if not in a world. */
	inline deoglWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or \em NULL if not in a world.
	 * \details For use by deoglWorld only.
	 */
	void SetParentWorld(deoglWorld *world);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Sky changed. */
	virtual void SkyChanged();
	
	/** order changed. */
	virtual void OrderChanged();
	
	/** Controller changed. */
	virtual void ControllerChanged(int index);
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	/*@}*/
};

#endif
