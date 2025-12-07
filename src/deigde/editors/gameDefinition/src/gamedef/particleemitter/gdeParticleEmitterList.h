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

#ifndef _GDEPARTICLEEMITTERLIST_H_
#define _GDEPARTICLEEMITTERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeParticleEmitter;



/**
 * \brief Particle emitter list.
 */
class gdeParticleEmitterList{
private:
	decObjectOrderedSet pParticleEmitters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create particle emitter list. */
	gdeParticleEmitterList();
	
	/** \brief Create copy of particle emitter list. */
	gdeParticleEmitterList(const gdeParticleEmitterList &list);
	
	/** \brief Clean up particleEmitter list. */
	~gdeParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of particle emitters. */
	int GetCount() const;
	
	/** \brief Particle emitter at position. */
	gdeParticleEmitter *GetAt(int index) const;
	
	/** \brief Particle emitter with path or \em NULL if absent. */
	gdeParticleEmitter *GetWithPath(const char *path) const;
	
	/** \brief Index of particle emitter or -1 if absent. */
	int IndexOf(gdeParticleEmitter *particleEmitter) const;
	
	/** \brief Index of  particle emitter with path or -1 if absent. */
	int IndexOfWithPath(const char *path) const;
	
	/** \brief Particle emitter is present. */
	bool Has(gdeParticleEmitter *particleEmitter) const;
	
	/** \brief Particle emitter with path is present. */
	bool HasWithPath(const char *path) const;
	
	/** \brief Add particle emitter. */
	void Add(gdeParticleEmitter *particleEmitter);
	
	/** \brief Remove particle emitter. */
	void Remove(gdeParticleEmitter *particleEmitter);
	
	/** \brief Remove all particle emitters. */
	void RemoveAll();
	
	/** \brief Set from another particleEmitter list. */
	gdeParticleEmitterList &operator=(const gdeParticleEmitterList &list);
	/*@}*/
};

#endif
