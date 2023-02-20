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

#ifndef _DEOGLPARTICLEEMITTERINSTANCELIST_H_
#define _DEOGLPARTICLEEMITTERINSTANCELIST_H_

class deoglRParticleEmitterInstance;



/**
 * Particle Emitter Instance List.
 * 
 * List of pointers to particle emitter instances. The ordering of the instances in the list is
 * not defined and can change when emitters are removed. This allows to remove elements with
 * constant time requirements.
 */
class deoglParticleEmitterInstanceList{
private:
	deoglRParticleEmitterInstance **pInstances;
	int pInstanceCount;
	int pInstanceSize;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create particle emitter instance list. */
	deoglParticleEmitterInstanceList();
	
	/** Clean up particle emitter instance list. */
	~deoglParticleEmitterInstanceList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of instances. */
	inline int GetCount() const{ return pInstanceCount; }
	
	/** Index of the instance or -1 if not in the list. */
	int IndexOf( deoglRParticleEmitterInstance *instance ) const;
	
	/** Instance exists. */
	bool Has( deoglRParticleEmitterInstance *instance ) const;
	
	/** Instance at the given index. */
	deoglRParticleEmitterInstance *GetAt( int index ) const;
	
	/** Add instance. */
	void Add( deoglRParticleEmitterInstance *instance );
	
	/** Add instance if missing in the list. Returns true if added. */
	bool AddIfMissing( deoglRParticleEmitterInstance *instance );
	
	/** Remove instance. */
	void Remove( deoglRParticleEmitterInstance *instance );
	
	/** Remove instance if existing in the list. Returns true if removed. */
	bool RemoveIfExisting( deoglRParticleEmitterInstance *instance );
	
	/** Remove instance from the given index. */
	void RemoveFrom( int index );
	
	/** Remove all instances. */
	void RemoveAll();
	/*@}*/
	
private:
	void pAddInstance( deoglRParticleEmitterInstance *instance );
};

#endif
