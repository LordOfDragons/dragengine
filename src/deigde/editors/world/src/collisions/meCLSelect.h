/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	
	/** \brief Get hit list. */
	inline meCLHitList &GetHitList(){ return pHitList; }
	inline const meCLHitList &GetHitList() const{ return pHitList; }
	
	/** \brief Prepare for a test clearing only the hit list. */
	void Prepare();
	
	/** \brief Run action after testing. */
	void RunAction(int selectIndex = 0);
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
