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

#ifndef _IGDEGDPARTICLEEMITTERLIST_H_
#define _IGDEGDPARTICLEEMITTERLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeGDParticleEmitter;



/**
 * \brief Game Definition Particle Emitter List.
 */
class DE_DLL_EXPORT igdeGDParticleEmitterList{
private:
	decObjectOrderedSet pEmitters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create list. */
	igdeGDParticleEmitterList();
	
	/** \brief Clean up list. */
	~igdeGDParticleEmitterList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of emitters. */
	int GetCount() const;
	
	/** \brief Emitter by index. */
	igdeGDParticleEmitter *GetAt( int index ) const;
	
	/** \brief Emitter with the given path or NULL if not found. */
	igdeGDParticleEmitter *GetWithPath( const char *path ) const;
	
	/** \brief Emitter with the given name or NULL if not found. */
	igdeGDParticleEmitter *GetWithName( const char *name ) const;
	
	/** \brief Index of an emitter or -1 if not found. */
	int IndexOf( igdeGDParticleEmitter *emitter ) const;
	
	/** \brief Index of the enitter with the given path or -1 if not found. */
	int IndexOfWithPath( const char *path ) const;
	
	/** \brief Index of the emitter with the given name or -1 if not found. */
	int IndexOfWithName( const char *name ) const;
	
	/** \brief Index of the emitter with the given name or -1 if not found. */
	int IndexOfWithPathOrName( const char *path, const char *name ) const;
	
	/** \brief Determines if an emitter exists. */
	bool Has( igdeGDParticleEmitter *emitter ) const;
	
	/** \brief Determines if an emitter with the given path exists. */
	bool HasWithPath( const char *path ) const;
	
	/** \brief Determines if an emitter with the given name exists. */
	bool HasWithName( const char *name ) const;
	
	/** \brief Determines if an emitter with the given name or path exists. */
	bool HasWithPathOrName( const char *path, const char *name ) const;
	
	/** \brief Adds an emitter. */
	void Add( igdeGDParticleEmitter *emitter );
	
	/** \brief Inserts a new emitter. */
	void InsertAt( igdeGDParticleEmitter *emitter, int index );
	
	/** \brief Moves an emitter to a new position. */
	void MoveTo( igdeGDParticleEmitter *emitter, int index );
	
	/** \brief Removes an emitter. */
	void Remove( igdeGDParticleEmitter *emitter );
	
	/** \brief Removes all emitters. */
	void RemoveAll();
	
	/** \brief Set list from another list. */
	igdeGDParticleEmitterList &operator=( const igdeGDParticleEmitterList &list );
	/*@}*/
};

#endif
