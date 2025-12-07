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

// include only once
#ifndef _DEOGLSMOKEEMITTER_H_
#define _DEOGLSMOKEEMITTER_H_

// includes
#include "../deoglGL.h"

#include <dragengine/systems/modules/graphic/deBaseGraphicSmokeEmitter.h>
#include <dragengine/common/math/decMath.h>

// predefinitions
class deoglWorld;
class deGraphicOpenGl;

class deSmokeEmitter;



/**
 * Smoke Emitter Peer.
 *
 * Peer for the smoke emitter resource.
 */
class deoglSmokeEmitter : public deBaseGraphicSmokeEmitter{
private:
	deGraphicOpenGl *pOgl;
	deoglWorld *pParentWorld;
	deSmokeEmitter *pSmokeEmitter;
	
	decDVector pMinExtend;
	decDVector pMaxExtend;
	bool pDirtyExtends;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	deoglSmokeEmitter(deGraphicOpenGl *ogl, deSmokeEmitter *smokeEmitter);
	/** Cleans up the peer. */
	virtual ~deoglSmokeEmitter();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the opengl object. */
	inline deGraphicOpenGl *GetOpenGL() const{return pOgl;}
	/** Retrieves the prop field. */
	inline deSmokeEmitter *GetSmokeEmitter() const{return pSmokeEmitter;}
	
	/** Retrieves the parent world. */
	inline deoglWorld *GetParentWorld() const{return pParentWorld;}
	/** Sets the parent world. */
	void SetParentWorld(deoglWorld *parentWorld);
	
	/** Retrieves the minimum extend. */
	inline const decDVector &GetMinimumExtend() const{return pMinExtend;}
	/** Retrieves the maximum extend. */
	inline const decDVector &GetMaximumExtend() const{return pMaxExtend;}
	
	/** Updates the smoke emitter if required. */
	void Update(float elapsed);
	/** Prepares the smoke emitter for rendering. */
	void PrepareForRender();
	/*@}*/
	
	/** \name Notifications */
	/*@{*/
	/** Volume position changed. */
	virtual void VolumePositionChanged();
	/** Skin changed. */
	virtual void SkinChanged();
	/** Points changed. */
	virtual void PointsChanged();
	/*@}*/
	
private:
	void pCleanUp();
	
	void pUpdateExtends();
};

// end of include only once
#endif
