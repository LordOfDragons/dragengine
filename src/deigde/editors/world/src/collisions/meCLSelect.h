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

#ifndef _MECLSELECT_H_
#define _MECLSELECT_H_

#include "meCLHitList.h"
#include "../world/object/meObjectList.h"
#include "../world/objectshape/meObjectShapeList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decIntList.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingCollider.h>

class meWorld;
class meObject;
class deCollider;



/**
 * \brief Select collision listener.
 * 
 * Collision listener for selecting objects. The first object hit by a ray cast into the world
 * from a given point is selected. If multi select is enabled the hit object
 * selection is toggled. Otherwise all other objects are unselected and this one selected.
 */
class meCLSelect : public deBaseScriptingCollider{
private:
	meWorld &pWorld;
	
	decDVector pRayOrigin;
	decDVector pRayDirection;
	
	bool pSingleSelect;
	bool pRectSelect;
	
	bool pCanHitObjects;
	bool pCanHitObjectShapes;
	bool pCanHitDecals;
	bool pCanHitNavSpaces;
	bool pCanHitHTNavPoints;
	
	bool pCanSelectObjects;
	bool pCanSelectObjectShapes;
	bool pCanSelectDecals;
	bool pCanSelectNavSpaces;
	bool pCanSelectHTNavPoints;
	
	meCLHitList pHitList;
	
	meObjectList pRectSelOldObjectList;
	meObjectShapeList pRectSelOldObjectShapeList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meCLSelect( meWorld &world );
	
	/** \brief Clean up listener. */
	virtual ~meCLSelect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Ray origin. */
	inline const decDVector &GetRayOrigin() const{ return pRayOrigin; }
	
	/** \brief Ray direction. */
	inline const decDVector &GetRayDirection() const{ return pRayDirection; }
	
	/** \brief Set ray origin and direction. */
	void SetRay( const decDVector &origin, const decVector &direction );
	
	
	
	/** \brief Single selection is used. */
	inline bool GetSingleSelect() const{ return pSingleSelect; }
	
	/** \brief Set if single selection is used. */
	void SetSingleSelect( bool singleSelect );
	
	/** \brief Rectangular selection is used. */
	inline bool GetRectSelect() const{ return pRectSelect; }
	
	/** \brief Set if rectangular selection is used. */
	void SetRectSelect( bool rectSelect );
	
	
	
	/** \brief Set if colliders of objects can be hit. */
	void SetCanHitObjects( bool canHit );
	
	/** \brief Set if colliders of object shapes can be ht. */
	void SetCanHitObjectShapes( bool canHit );
	
	/** \brief Set if colliders of decals can be hit. */
	void SetCanHitDecals( bool canHit );
	
	/** \brief Set if colliders of navigation spaces can be hit. */
	void SetCanHitNavSpaces( bool canHit );
	
	/** \brief Set if colliders of height terrain can be hit. */
	void SetCanHitHTNavPoints( bool canHit );
	
	/** \brief Set all can hit. */
	void SetCanHitAll( bool canHit );
	
	
	
	/** \brief Set if objects can be selected. */
	void SetCanSelectObjects( bool canSelect );
	
	/** \brief Set if object shapes can be selected. */
	void SetCanSelectObjectShapes( bool canSelect );
	
	/** \brief Set if decals can be selected. */
	void SetCanSelectDecals( bool canSelect );
	
	/** \brief Set if navigation spaces can be selected. */
	void SetCanSelectNavSpaces( bool canSelect );
	
	/** \brief Set if height terrain navigation points can be selected. */
	void SetCanSelectHTNavPoints( bool canSelect );
	
	/** \brief Set can select all. */
	void SetCanSelectAll( bool canSelect );
	
	
	
	/** \brief Clear hit list and rectangular selection list. */
	void Reset();
	
	/** \brief Store current selection for rectangular selection. */
	void StoreRectSelect();
	
	/** \brief Prepare for a test clearing only the hit list. */
	void Prepare();
	
	/** \brief Run action after testing. */
	void RunAction();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Collision response. */
	virtual void CollisionResponse( deCollider *owner, deCollisionInfo *info );
	
	/** \brief Collider can be hit. */
	virtual bool CanHitCollider( deCollider *owner, deCollider *collider );
	
	/** \brief Collider changed during physics processing. */
	virtual void ColliderChanged( deCollider *owner );
	/*@}*/
};

#endif
