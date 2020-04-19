/* 
 * Drag[en]gine IGDE Game Definition Editor
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
	gdeParticleEmitterList( const gdeParticleEmitterList &list );
	
	/** \brief Clean up particleEmitter list. */
	~gdeParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of particle emitters. */
	int GetCount() const;
	
	/** \brief Particle emitter at position. */
	gdeParticleEmitter *GetAt( int index ) const;
	
	/** \brief Particle emitter with path or \em NULL if absent. */
	gdeParticleEmitter *GetWithPath( const char *path ) const;
	
	/** \brief Index of particle emitter or -1 if absent. */
	int IndexOf( gdeParticleEmitter *particleEmitter ) const;
	
	/** \brief Index of  particle emitter with path or -1 if absent. */
	int IndexOfWithPath( const char *path ) const;
	
	/** \brief Particle emitter is present. */
	bool Has( gdeParticleEmitter *particleEmitter ) const;
	
	/** \brief Particle emitter with path is present. */
	bool HasWithPath( const char *path ) const;
	
	/** \brief Add particle emitter. */
	void Add( gdeParticleEmitter *particleEmitter );
	
	/** \brief Remove particle emitter. */
	void Remove( gdeParticleEmitter *particleEmitter );
	
	/** \brief Remove all particle emitters. */
	void RemoveAll();
	
	/** \brief Set from another particleEmitter list. */
	gdeParticleEmitterList &operator=( const gdeParticleEmitterList &list );
	/*@}*/
};

#endif
