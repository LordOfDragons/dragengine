/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPCONVEXRESULTCALLBACK_H_
#define _DEBPCONVEXRESULTCALLBACK_H_

#include "debpCollisionDetection.h"

#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class decCollisionFilter;
class deCollisionInfo;
class deBaseScriptingCollider;
class debpShape;
class debpCollider;


/**
 * \brief Modification of the \ref btCollisionWorld::ConvexResultCallback struct for
 *        collision testing with bullet specific collision tests in btCollisionWorld.
 */
class debpConvexResultCallback : public btCollisionWorld::ConvexResultCallback{
private:
	deCollisionInfo *pColInfo;
	
	const decCollisionFilter *pRayCollisionFilter;
	
	debpCollider *pCollider;
	debpShape *pShape;
	
	debpCollisionResult pResult;
	deBaseScriptingCollider *pListener;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new result callback. */
	debpConvexResultCallback( deCollisionInfo *colinfo );
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Resets the visitor. */
	void Reset();
	
	/** \brief Sets the ray to test with. */
	void SetTestRay( const decCollisionFilter *collisionFilter, deBaseScriptingCollider *listener );
	/** \brief Set the shape to test with. */
	void SetTestShape( debpShape *shape, deBaseScriptingCollider *listener );
	/** \brief Sets the collider to test with. */
	void SetTestCollider( debpCollider *collider, deBaseScriptingCollider *listener );
	/*@}*/
	
	/** \name Bullet */
	/*@{*/
	/** \brief Determines if a collision is possible. */
	virtual bool needsCollision( btBroadphaseProxy *proxy0 ) const;
	/** \brief Adds a result. This fills in the parameters for later delivery to the collision info. */
	virtual btScalar addSingleResult( btCollisionWorld::LocalConvexResult &convexResult, bool normalInWorldSpace );
	/*@}*/
};

#endif
