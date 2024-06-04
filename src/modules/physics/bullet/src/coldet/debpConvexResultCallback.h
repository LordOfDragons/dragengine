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
