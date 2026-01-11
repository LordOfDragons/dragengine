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

#include "../deoglBasics.h"
#include "deoglRParticleEmitterInstance.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicParticleEmitterInstance.h>

class deoglParticleEmitter;
class deoglParticleEmitterInstanceType;

class deGraphicOpenGl;
class deParticleEmitterInstance;


/**
 * Particle emitter instance peer.
 */
class deoglParticleEmitterInstance : public deBaseGraphicParticleEmitterInstance{
private:
	deGraphicOpenGl &pOgl;
	const deParticleEmitterInstance &pInstance;
	deoglRParticleEmitterInstance::Ref pRInstance;
	
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
	~deoglParticleEmitterInstance() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** Particle emitter instance. */
	inline const deParticleEmitterInstance &GetInstance() const{ return pInstance; }
	
	
	
	/** Render particle emitter instance. */
	inline const deoglRParticleEmitterInstance::Ref &GetRInstance() const{ return pRInstance; }
	
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
	void EmitterChanged() override;
	
	/** Position changed. */
	void PositionChanged() override;
	
	/** Orientation changed. */
	void OrientationChanged() override;
	
	/** Reference position changed. */
	void ReferencePositionChanged() override;
	
	/** Enable casting changed. */
	void EnableCastingChanged() override;
	
	/** Collision filter changed. */
	void CollisionFilterChanged() override;
	
	/** Warm up time changed. */
	void WarmUpTimeChanged() override;
	
	/** Burst time changed. */
	void BurstTimeChanged() override;
	
	/** Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** Controller changed. */
	void ControllerChanged(int controller) override;
	
	/** Type changed. */
	void TypeChanged(int type) override;
	
	/** Type particles changed. */
	void TypeParticlesChanged(int type) override;
	
	/** Reset burst particles. */
	void ResetBurst() override;
	
	/** Kill all particles. */
	void KillAllParticles() override;
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateTypes();
};

#endif
