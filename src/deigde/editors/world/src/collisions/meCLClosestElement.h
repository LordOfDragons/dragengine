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

#ifndef _MECLCLOSESTELEMENT_H_
#define _MECLCLOSESTELEMENT_H_

#include "meCLHitList.h"
#include "../world/object/meObjectList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class meWorld;
class meObject;
class meDecal;
class meHeightTerrainSector;
class meFilterObjects;
class meObjectSnapPoint;
class deCollider;



/**
 *  Closest element visitor.
 *
 * Collision listener storing the closes hit element. In addition an object and decal can
 * be defined to ignore during testing. This avoids self-collision results.
 */
class meCLClosestElement : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	
	bool pTestHeightTerrain;
	bool pTestObjects;
	bool pTestDecals;
	bool pTestSnapPoints;
	
	float pHitDistance;
	decVector pHitNormal;
	meHeightTerrainSector *pHitHTSector;
	meObject *pHitObject;
	meDecal *pHitDecal;
	meObjectSnapPoint *pHitSnapPoint;
	bool pHasHit;
	
	const meFilterObjects *pFilterObjects;
	
	meObjectList pIgnoreObjects;
	meDecal *pIgnoreDecal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/**  Create visitor. */
	meCLClosestElement( meWorld &world );
	
	/**  Clean up visitor. */
	virtual ~meCLClosestElement();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**  Set if height terrain are tested. */
	void SetTestHeightTerrain( bool test );
	
	/**  Set if objects are tested. */
	void SetTestObjects( bool test );
	
	/**  Set if decals are tested. */
	void SetTestDecals( bool test );
	
	/**  Set if object snap points are tested. */
	void SetTestSnapPoints( bool test );
	
	
	
	/**  Set decal to ignore or nullptr. */
	void SetIgnoreDecal( meDecal *decal );
	
	/** \breif Object filter or nullptr to not use filtering. */
	inline const meFilterObjects *GetFilterObjects() const{ return pFilterObjects; }
	
	/**  Set object filter or nullptr to not use filtering. */
	void SetFilterObjects( const meFilterObjects *filter );
	
	/**  List of objects to ignore. */
	inline meObjectList &GetIgnoreObjects(){ return pIgnoreObjects; }
	inline const meObjectList &GetIgnoreObjects() const{ return pIgnoreObjects; }
	
	
	
	/**  Reset visitor. */
	void Reset();
	
	
	
	/** \breief Element has been hit. */
	inline bool GetHasHit() const{ return pHasHit; }
	
	/**  Hit distance. */
	inline float GetHitDistance() const{ return pHitDistance; }
	
	/**  Hit normal. */
	inline const decVector &GetHitNormal() const{ return pHitNormal; }
	
	/**  Hit height terrain sector or nullptr. */
	inline meHeightTerrainSector *GetHitHTSector() const{ return pHitHTSector; }
	
	/**  Hit object or nullptr. */
	inline meObject *GetHitObject() const{ return pHitObject; }
	
	/**  Hit decal or nullptr. */
	inline meDecal *GetHitDecal() const{ return pHitDecal; }
	
	/**  Hit object snap point or nullptr. */
	inline meObjectSnapPoint *GetHitSnapPoint() const{ return pHitSnapPoint; }
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/**  Collision response. */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/**  Collider can hit another collider. */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/**  Collider changed. */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
};

#endif
