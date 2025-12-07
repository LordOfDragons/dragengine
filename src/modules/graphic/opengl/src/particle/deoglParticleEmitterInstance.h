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

#ifndef _DEOGLPARTICLEEMITTERINSTANCE_H_
#define _DEOGLPARTICLEEMITTERINSTANCE_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicParticleEmitterInstance.h>

#include "../deoglBasics.h"

class deoglParticleEmitter;
class deoglParticleEmitterInstanceType;
class deoglRParticleEmitterInstance;

class deGraphicOpenGl;
class deParticleEmitterInstance;


/**
 * Particle emitter instance peer.
 */
class deoglParticleEmitterInstance : public deBaseGraphicParticleEmitterInstance{
private:
	deGraphicOpenGl &pOgl;
	const deParticleEmitterInstance &pInstance;
	deoglRParticleEmitterInstance *pRInstance;
	
	deoglParticleEmitter *pEmitter;
	
	deoglParticleEmitterInstanceType **pTypes;
	int pTypeCount;
	
	bool pDirtyEmitter;
	bool pDirtyExtends;
	bool pDirtyOctreeNode;
	bool pDirtyParticles;
	bool pDirtyInstance;
	bool pDirtyTypes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglParticleEmitterInstance(deGraphicOpenGl &ogl, const deParticleEmitterInstance &instance);
	
	/** Clean up peer. */
	virtual ~deoglParticleEmitterInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Particle emitter instance. */
	inline const deParticleEmitterInstance &GetInstance() const{ return pInstance; }
	
	
	
	/** Render particle emitter instance. */
	inline deoglRParticleEmitterInstance *GetRInstance() const{ return pRInstance; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Emitter or \em NULL if not set. */
	inline deoglParticleEmitter *GetEmitter() const{ return pEmitter; }
	
	/** Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** Type at index. */
	deoglParticleEmitterInstanceType &GetTypeAt(int index);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Emitter changed. */
	virtual void EmitterChanged();
	
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Orientation changed. */
	virtual void OrientationChanged();
	
	/** Reference position changed. */
	virtual void ReferencePositionChanged();
	
	/** Enable casting changed. */
	virtual void EnableCastingChanged();
	
	/** Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** Warm up time changed. */
	virtual void WarmUpTimeChanged();
	
	/** Burst time changed. */
	virtual void BurstTimeChanged();
	
	/** Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** Controller changed. */
	virtual void ControllerChanged(int controller);
	
	/** Type changed. */
	virtual void TypeChanged(int type);
	
	/** Type particles changed. */
	virtual void TypeParticlesChanged(int type);
	
	/** Reset burst particles. */
	virtual void ResetBurst();
	
	/** Kill all particles. */
	virtual void KillAllParticles();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateTypes();
};

#endif
