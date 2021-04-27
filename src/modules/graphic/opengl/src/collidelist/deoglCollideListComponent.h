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

#ifndef _DEOGLCOLLIDELISTCOMPONENT_H_
#define _DEOGLCOLLIDELISTCOMPONENT_H_

class deoglRComponent;



/**
 * Keeps track of a component in a collide list. Stores the component as well
 * as the used lod level. It is possible for the component to be NULL. This
 * allows the collide list to clear a colider list component not in use
 * to avoid costly allocations. The component object is only a pointer and is
 * not referenced. Care has to be taken to clear collide lists of a component
 * is no more existing.
 */
class deoglCollideListComponent{
private:
	deoglRComponent *pComponent;
	int pLODLevel;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new collide list component. */
	deoglCollideListComponent();
	/** Cleans up the collide list component. */
	~deoglCollideListComponent();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Clears the collide list component setting the component to NULL and removing all strips. */
	void Clear();
	
	/** Retrieves the component. */
	inline deoglRComponent *GetComponent() const{ return pComponent; }
	/** Sets the component. */
	void SetComponent( deoglRComponent *component );
	
	/** Retrieves the lod level to use. */
	inline int GetLODLevel() const{ return pLODLevel; }
	/** Sets the lod level to use. */
	void SetLODLevel( int lodLevel );
	/*@}*/
};

#endif
