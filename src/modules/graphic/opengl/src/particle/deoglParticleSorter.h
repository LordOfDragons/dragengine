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

#ifndef _DEOGLPARTICLESORTER_H_
#define _DEOGLPARTICLESORTER_H_

class deoglAddToRenderTaskParticles;
class deoglRParticleEmitterInstance;



/**
 * @brief Particle Sorter.
 */
class deoglParticleSorter{
public:
	struct sParticle{
		deoglRParticleEmitterInstance *instance;
		int particle;
		float distance;
	};
	
private:
	sParticle **pParticles;
	int pParticleCount;
	int pParticleSize;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new particle sorter. */
	deoglParticleSorter();
	/** Cleans up the particle sorter. */
	virtual ~deoglParticleSorter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the number of particles. */
	inline int GetParticleCount() const{ return pParticleCount; }
	/** Retrieves the particle at the given position. */
	const sParticle &GetParticleAt( int index ) const;
	/** Add particles to a render task particles. */
	void AddToRenderTask( deoglAddToRenderTaskParticles &renderTask );
	/** Clears the sorter. */
	void Clear();
	/** Adds a particle. */
	void AddParticle( deoglRParticleEmitterInstance *instance, int particle, float distance );
	/** Sort particles. */
	void Sort();
	/*@}*/
	
private:
	void pSortParticle( int left, int right );
};

#endif
