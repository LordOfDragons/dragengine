/* 
 * Drag[en]gine IGDE
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
	igdeGDCParticleEmitterList( const igdeGDCParticleEmitterList &list );
	
	/** \brief Clean up list. */
	~igdeGDCParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of emitters. */
	int GetCount() const;
	
	/** \brief Emitter by index. */
	igdeGDCParticleEmitter *GetAt( int index ) const;
	
	/** \brief Index of an emitter or -1 if not found. */
	int IndexOf( igdeGDCParticleEmitter *emitter ) const;
	
	/** \brief Determines if an emitter exists. */
	bool Has( igdeGDCParticleEmitter *emitter ) const;
	
	/** \brief Adds an emitter. */
	void Add( igdeGDCParticleEmitter *emitter );
	
	/** \brief Inserts a new emitter. */
	void InsertAt( igdeGDCParticleEmitter *emitter, int index );
	
	/** \brief Moves an emitter to a new position. */
	void MoveTo( igdeGDCParticleEmitter *emitter, int index );
	
	/** \brief Removes an emitter. */
	void Remove( igdeGDCParticleEmitter *emitter );
	
	/** \brief Removes all emitters. */
	void RemoveAll();
	
	/** \brief Set this list to a deep copy of another list. */
	void SetToDeepCopyFrom( const igdeGDCParticleEmitterList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set list from another list. */
	igdeGDCParticleEmitterList &operator=( const igdeGDCParticleEmitterList &list );
	/*@}*/
};

#endif
