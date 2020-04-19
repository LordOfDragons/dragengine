/* 
 * Drag[en]gine IGDE Rig Editor
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

// include only once
#ifndef _RECLHITLISTENTRY_H_
#define _RECLHITLISTENTRY_H_

// includes
#include "dragengine/common/math/decMath.h"

// predefinitions
class reRigBone;
class reRigShape;
class reRigConstraint;
class reRigPush;



/**
 * Entry of a hit list.
 */
class reCLHitListEntry{
private:
	reRigBone *pBone;
	reRigShape *pShape;
	reRigConstraint *pConstraint;
	reRigPush *pPush;
	float pDistance;
	decVector pNormal;
	
public:
	// constructor, destructor
	reCLHitListEntry();
	~reCLHitListEntry();
	
	/** @name Management */
	/*@{*/
	/** Clears the entry. */
	void Clear();
	/** Retrieves the rig bone or NULL. */
	inline reRigBone *GetBone() const{ return pBone; }
	/** Sets the rig bone or NULL. */
	void SetBone( reRigBone *bone );
	/** Retrieves the rig shape or NULL. */
	inline reRigShape *GetShape() const{ return pShape; }
	/** Sets the rig shape. */
	void SetShape( reRigShape *shape );
	/** Retrieves the rig constraint or NULL. */
	inline reRigConstraint *GetConstraint() const{ return pConstraint; }
	/** Sets the rig constraint. */
	void SetConstraint( reRigConstraint *constraint );
	/** Retrieves the rig push or NULL. */
	inline reRigPush *GetPush() const{ return pPush; }
	/** Sets the rig push. */
	void SetPush( reRigPush *push );
	
	/** Retrieves the distance. */
	inline float GetDistance() const{ return pDistance; }
	/** Sets the distance. */
	void SetDistance( float distance );
	/** Retrieves the normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	/** Sets the normal. */
	void SetNormal( const decVector &normal );
	/*@}*/
	
	/**
	 * Compares this entry to another one. Returns 1 if this entry is ordinally larger,
	 * -1 if smaller or 0 if equal.
	 */
	int CompareTo( const reCLHitListEntry &entry ) const;
	/*@}*/
};

// end of include only once
#endif
