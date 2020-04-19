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
 * \brief Particle emitter instance peer.
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
	/** \brief Create peer. */
	deoglParticleEmitterInstance( deGraphicOpenGl &ogl, const deParticleEmitterInstance &instance );
	
	/** \brief Clean up peer. */
	virtual ~deoglParticleEmitterInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Opengl object. */
	inline deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Particle emitter instance. */
	inline const deParticleEmitterInstance &GetInstance() const{ return pInstance; }
	
	
	
	/** \brief Render particle emitter instance. */
	inline deoglRParticleEmitterInstance *GetRInstance() const{ return pRInstance; }
	
	/** \brief Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** \brief Emitter or \em NULL if not set. */
	inline deoglParticleEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** \brief Type at index. */
	deoglParticleEmitterInstanceType &GetTypeAt( int index );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Emitter changed. */
	virtual void EmitterChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Reference position changed. */
	virtual void ReferencePositionChanged();
	
	/** \brief Enable casting changed. */
	virtual void EnableCastingChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Warm up time changed. */
	virtual void WarmUpTimeChanged();
	
	/** \brief Burst time changed. */
	virtual void BurstTimeChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** \brief Type changed. */
	virtual void TypeChanged( int type );
	
	/** \brief Type particles changed. */
	virtual void TypeParticlesChanged( int type );
	
	/** \brief Reset burst particles. */
	virtual void ResetBurst();
	
	/** \brief Kill all particles. */
	virtual void KillAllParticles();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateTypes();
};

#endif
