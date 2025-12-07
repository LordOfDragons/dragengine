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

#ifndef _DEOGLPARTICLEEMITTER_H_
#define _DEOGLPARTICLEEMITTER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicParticleEmitter.h>
#include "deoglRParticleEmitter.h"

class deoglParticleEmitterType;

class deGraphicOpenGl;
class deParticleEmitter;
class deParticleEmitterParameter;


/**
 * Particle emitter peer.
 */
class deoglParticleEmitter : public deBaseGraphicParticleEmitter{
private:
	deGraphicOpenGl &pOgl;
	const deParticleEmitter &pParticleEmitter;
	
	deoglRParticleEmitter::Ref pREmitter;
	
	deoglParticleEmitterType **pTypes;
	int pTypeCount;
	
	bool pDirtyTypes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglParticleEmitter(deGraphicOpenGl &ogl, const deParticleEmitter &emitter);
	
	/** Clean up peer. */
	virtual ~deoglParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Particle emitter. */
	inline const deParticleEmitter &GetParticleEmitter() const{ return pParticleEmitter; }
	
	
	
	/** Render particle emitter. */
	inline const deoglRParticleEmitter::Ref &GetREmitter() const{ return pREmitter; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** Type at index. */
	deoglParticleEmitterType &GetTypeAt(int index) const;
	
	/** Update parameter samples if required. */
	void UpdateParameterSamples();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Controller count changed. */
	virtual void ControllerCountChanged();
	
	/** Controller changed. */
	virtual void ControllerChanged(int controller);
	
	/** Type count changed. */
	virtual void TypeCountChanged();
	
	/** Type changed. */
	virtual void TypeChanged(int type);
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
