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

#ifndef _DEOGLPARTICLESORTER_H_
#define _DEOGLPARTICLESORTER_H_

#include <dragengine/common/collection/decTList.h>

class deoglAddToRenderTaskParticles;
class deoglRParticleEmitterInstance;


/**
 * Particle Sorter.
 */
class deoglParticleSorter{
public:
	struct sParticle{
		deoglRParticleEmitterInstance *instance = nullptr;
		int particle = 0;
		float distance = 0.0f;
		
		inline sParticle() = default;
		inline sParticle(deoglRParticleEmitterInstance *inst, int part, float dist) :
		instance(inst), particle(part), distance(dist){}
	};
	
	
private:
	decTList<sParticle> pParticles;
	decTList<const sParticle*> pSortedParticles;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new particle sorter. */
	deoglParticleSorter() = default;
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Sorted particles. */
	inline const decTList<const sParticle*> &GetSortedParticles() const{ return pSortedParticles; }
	
	/** Retrieves the number of particles. */
	inline int GetParticleCount() const{ return pSortedParticles.GetCount(); }
	/** Retrieves the particle at the given position. */
	const sParticle &GetParticleAt(int index) const;
	/** Add particles to a render task particles. */
	void AddToRenderTask(deoglAddToRenderTaskParticles &renderTask);
	/** Clears the sorter. */
	void Clear();
	/** Adds a particle. */
	void AddParticle(deoglRParticleEmitterInstance *instance, int particle, float distance);
	/** Sort particles. */
	void Sort();
	/*@}*/
};

#endif
