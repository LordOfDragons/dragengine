/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MECOLLIDEROWNER_H_
#define _MECOLLIDEROWNER_H_

class meDecal;
class meNavigationSpace;
class meObject;
class meObjectShape;
class meObjectSnapPoint;

class igdeEnvironment;

class deCollider;



/**
 * \brief Collider owner.
 * 
 * Used as user pointer for colliders to identify the owner of a collider. This class is
 * immutable. Owners have to use the matching constructor to set the parameters.
 * 
 * All class members store only weak references.
 */
class meColliderOwner{
private:
	meObject *pObject;
	meObjectShape *pShape;
	meObjectSnapPoint *pSnapPoint;
	meDecal *pDecal;
	meNavigationSpace *pNavigationSpace;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create collider owner for object. */
	meColliderOwner( meObject *object );
	
	/** \brief Create collider owner for object shape. */
	meColliderOwner( meObjectShape *shape );
	
	/** \brief Create collider owner for object snap point. */
	meColliderOwner( meObjectSnapPoint *snapPoint );
	
	/** \brief Create collider owner for decal. */
	meColliderOwner( meDecal *decal );
	
	/** \brief Create collider owner for navigation space. */
	meColliderOwner( meNavigationSpace *navspace );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Owner object class or \em NULL if not set. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Object shape or \em NULL if not set. */
	inline meObjectShape *GetShape() const{ return pShape; }
	
	/** \brief Snap point or \em NULL if not set. */
	inline meObjectSnapPoint *GetSnapPoint() const{ return pSnapPoint; }
	
	/** \brief Owner decal or \em NULL if not set. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** \brief Owner navigation space or \em NULL if not set. */
	inline meNavigationSpace *GetNavigationSpace() const{ return pNavigationSpace; }
	
	
	
	/** \brief Get collider owner for collider or \em NULL if not set. */
	static const meColliderOwner *GetColliderOwner(
		igdeEnvironment &environment, deCollider *collider );
	/*@}*/
};

#endif
