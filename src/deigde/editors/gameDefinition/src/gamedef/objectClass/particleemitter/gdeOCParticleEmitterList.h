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

#ifndef _GDEOCPARTICLEEMITTERLIST_H_
#define _GDEOCPARTICLEEMITTERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class gdeOCParticleEmitter;



/**
 * \brief Object class particle emitter list.
 */
class gdeOCParticleEmitterList{
private:
	decObjectOrderedSet pParticleEmitters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object class particle emitter list. */
	gdeOCParticleEmitterList();
	
	/** \brief Create copy of object class particle emitter list. */
	gdeOCParticleEmitterList( const gdeOCParticleEmitterList &list );
	
	/** \brief Clean up object class particle emitter list. */
	~gdeOCParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of object class particle emitters. */
	int GetCount() const;
	
	/** \brief Object class particle emitter at position. */
	gdeOCParticleEmitter *GetAt( int index ) const;
	
	/** \brief Index of named object class particle emitter or -1 if absent. */
	int IndexOf( gdeOCParticleEmitter *emitter ) const;
	
	/** \brief Object class particle emitter is present. */
	bool Has( gdeOCParticleEmitter *emitter ) const;
	
	/** \brief Add object class particle emitter. */
	void Add( gdeOCParticleEmitter *emitter );
	
	/** \brief Remove object class particle emitter. */
	void Remove( gdeOCParticleEmitter *emitter );
	
	/** \brief Remove all object class particle emitters. */
	void RemoveAll();
	
	/** \brief Set from another object class particle emitter list. */
	gdeOCParticleEmitterList &operator=( const gdeOCParticleEmitterList &list );
	/*@}*/
};

#endif
