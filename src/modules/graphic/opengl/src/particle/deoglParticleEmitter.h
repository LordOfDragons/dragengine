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

#ifndef _DEOGLPARTICLEEMITTER_H_
#define _DEOGLPARTICLEEMITTER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicParticleEmitter.h>

class deoglRParticleEmitter;
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
	
	deoglRParticleEmitter *pREmitter;
	
	deoglParticleEmitterType **pTypes;
	int pTypeCount;
	
	bool pDirtyTypes;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglParticleEmitter( deGraphicOpenGl &ogl, const deParticleEmitter &emitter );
	
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
	inline deoglRParticleEmitter *GetREmitter() const{ return pREmitter; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	
	/** Type at index. */
	deoglParticleEmitterType &GetTypeAt( int index ) const;
	
	/** Update parameter samples if required. */
	void UpdateParameterSamples();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Controller count changed. */
	virtual void ControllerCountChanged();
	
	/** Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** Type count changed. */
	virtual void TypeCountChanged();
	
	/** Type changed. */
	virtual void TypeChanged( int type );
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
