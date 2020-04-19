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

#ifndef _MECLHITLISTENTRY_H_
#define _MECLHITLISTENTRY_H_

#include <dragengine/common/math/decMath.h>

class meObject;
class meObjectShape;
class meDecal;
class meNavigationSpace;



/**
 * \brief Hit list entry.
 */
class meCLHitListEntry{
private:
	meObject *pObject;
	meObjectShape *pObjectShape;
	meDecal *pDecal;
	meNavigationSpace *pNavSpace;
	int pHTNavSpacePoint;
	
	float pDistance;
	decVector pNormal;
	
	
	
public:
	/** \name Constructors, Destructors */
	/*@{*/
	/** \brief Create hit list entry. */
	meCLHitListEntry();
	
	/** \brief Clean up hit list entry. */
	~meCLHitListEntry();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Clear entry. */
	void Clear();
	
	/** \brief Object or \em NULL. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** \brief Set object or \em NULL. */
	void SetObject( meObject *object );
	
	/** \brief Object shape or \em NULL. */
	inline meObjectShape *GetObjectShape() const{ return pObjectShape; }
	
	/** \brief Set object shape or \em NULL. */
	void SetObjectShape( meObjectShape *objectShape );
	
	/** \brief Decal or \em NULL. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** \brief Set decal or \em NULL. */
	void SetDecal( meDecal *decal );
	
	/** \brief Navigation space or \em NULL. */
	inline meNavigationSpace *GetNavigationSpace() const{ return pNavSpace; }
	
	/** \brief Set navigation space or \em NULL. */
	void SetNavigationSpace( meNavigationSpace *navspac3 );
	
	/** \brief Height terrain navigation space point or -1. */
	inline int GetHTNavSpacePoint() const{ return pHTNavSpacePoint; }
	
	/** \brief Set height terrain navigation space point or -1. */
	void SetHTNavSpacePoint( int point );
	
	
	
	/** \brief Distance. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance. */
	void SetDistance( float distance );
	
	/** \brief Normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set normal. */
	void SetNormal( const decVector &normal );
	
	
	
	/** \brief Sort decals. */
	void SortDecals();
	
	/**
	 * \brief Compare entry to another one.
	 * \returns 1 if entry is ordinally larger, -1 if smaller or 0 if equal.
	 */
	int CompareTo( const meCLHitListEntry &entry ) const;
	/*@}*/
};

#endif
