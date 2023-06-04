/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _MECLIDENTIFY_H_
#define _MECLIDENTIFY_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>
#include <dragengine/common/math/decMath.h>

class meWorld;
class meObject;
class meDecal;
class deCollider;


/**
 * Class for identifying an object.
 */
class meCLIdentify : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	meObject *pObject;
	meDecal *pDecal;
	float pBestDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	meCLIdentify( meWorld &world );
	
	/** Clean up visitor. */
	virtual ~meCLIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Reset visitor. */
	void Reset();
	
	/** Identified object or nullptr. */
	inline meObject *GetObject() const{ return pObject; }
	
	/** Identified decal or nullptr. */
	inline meDecal *GetDecal() const{ return pDecal; }
	
	/** Has identified object. */
	bool HasObject() const;
	
	/** Has identified decal. */
	bool HasDecal() const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
	
	
	
private:
	meObject *pGetObjectForCollider( deCollider *collider ) const;
};

#endif
