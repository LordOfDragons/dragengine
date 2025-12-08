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

#ifndef _IGDEGDCPARTICLEEMITTERLIST_H_
#define _IGDEGDCPARTICLEEMITTERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDCParticleEmitter;



/**
 * \brief Game Definition Class Particle Emitter List.
 */
class DE_DLL_EXPORT igdeGDCParticleEmitterList{
private:
	decObjectOrderedSet pEmitters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDCParticleEmitterList();
	
	/** \brief Create copy of a game definition class particle emitter list. */
	igdeGDCParticleEmitterList(const igdeGDCParticleEmitterList &list);
	
	/** \brief Clean up list. */
	~igdeGDCParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of emitters. */
	int GetCount() const;
	
	/** \brief Emitter by index. */
	igdeGDCParticleEmitter *GetAt(int index) const;
	
	/** \brief Index of an emitter or -1 if not found. */
	int IndexOf(igdeGDCParticleEmitter::Ref emitter) const;
	
	/** \brief Determines if an emitter exists. */
	bool Has(igdeGDCParticleEmitter::Ref emitter) const;
	
	/** \brief Adds an emitter. */
	void Add(igdeGDCParticleEmitter::Ref emitter);
	
	/** \brief Inserts a new emitter. */
	void InsertAt(igdeGDCParticleEmitter::Ref emitter, int index);
	
	/** \brief Moves an emitter to a new position. */
	void MoveTo(igdeGDCParticleEmitter::Ref emitter, int index);
	
	/** \brief Removes an emitter. */
	void Remove(igdeGDCParticleEmitter::Ref emitter);
	
	/** \brief Removes all emitters. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom(const igdeGDCParticleEmitterList &list);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCParticleEmitterList &operator=(const igdeGDCParticleEmitterList &list);
	/*@}*/
};

#endif
