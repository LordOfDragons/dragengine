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

#ifndef _DEOGLPARTICLEEMITTERINSTANCETYPE_H_
#define _DEOGLPARTICLEEMITTERINSTANCETYPE_H_

class deoglRParticleEmitterInstanceType;
class deParticleEmitterInstanceType;

class deoglParticleEmitterInstance;


/**
 * Particle emitter instance type.
 */
class deoglParticleEmitterInstanceType{
private:
	deoglParticleEmitterInstance &pEmitterInstance;
	const int pIndex;
	
	deoglRParticleEmitterInstanceType *pRType;
	
	bool pDirtyType;
	bool pDirtyParamBlocks;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create new type. */
	deoglParticleEmitterInstanceType( deoglParticleEmitterInstance &instance, int index );
	
	/** Clean up type. */
	~deoglParticleEmitterInstanceType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Parent emitter instance. */
	inline deoglParticleEmitterInstance &GetEmitterInstance(){ return pEmitterInstance; }
	
	/** Render type. */
	inline deoglRParticleEmitterInstanceType *GetRType() const{ return pRType; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	
	
	
	/** Update texture parameters to use. */
	void UpdateUseSkin();
	
	/** Invalidate parameter blocks. */
	void InvalidateParamBlocks();
	
	
	
	/** Type changed. */
	void TypeChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
