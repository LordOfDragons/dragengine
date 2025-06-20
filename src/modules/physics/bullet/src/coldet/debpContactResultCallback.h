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

#ifndef _DEBPCONTACTRESULTCALLBACK_H_
#define _DEBPCONTACTRESULTCALLBACK_H_

#include "debpCollisionDetection.h"

#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerList.h>

class decCollisionFilter;
class deCollisionInfo;
class deBaseScriptingCollider;
class debpShape;
class debpCollider;
class debpHTSector;


/**
 * \brief Contact result callback.
 * 
 * Modification version of btCollisionWorld::ContactResultCallback to support dragengine
 * specific collision filtering and calling deBaseScriptingCollider.
 * 
 * One of the main problems here is that bullet calls addSingleResult multiple times to
 * add multiple contact points including the deepest. We have though no knowledge about
 * the number of contact points added nor which one is the deepest unless we store the
 * results and compare them. We need to call the listener only once for unique combination
 * of collider, bone and shape/face ignoring multiple contact points found for them.
 * Since contact points are send one after the other a trick can be used to report a
 * collision only once. The last collision parameters are stored in the visitor. If a new
 * collision result matches the previous one we know that another point is send for the
 * same collision and the result is not send to the listener. This works well enough for
 * the situation at hand. This check can be disabled if it is known that only single
 * points are send to the visitor by the collision routines.
 */
class debpContactResultCallback : public btCollisionWorld::ContactResultCallback{
private:
	deCollisionInfo *pColInfo;
	
	const decCollisionFilter *pCollisionFilter;
	
	btCollisionObject *pCollisionObject;
	debpCollider *pCollider;
	debpShape *pShape;
	
	debpCollisionResult pResult;
	deBaseScriptingCollider *pListener;
	
	bool pIgnoreMultipleContactPoints;
	const debpCollider *pLastCollider;
	int pLastBone;
	int pLastShape;
	int pLastFace;
	const debpHTSector *pLastHTSector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create result callback. */
	debpContactResultCallback( deCollisionInfo *colinfo );
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset visitor. */
	void Reset();
	
	/** \brief Set collision filter only test. */
	void SetTestFilter( const decCollisionFilter *collisionFilter,
		deBaseScriptingCollider *listener );
	
	/** \brief Set the shape to test with. */
	void SetTestShape( btCollisionObject *collisionObject,
		debpShape *shape, deBaseScriptingCollider *listener );
	
	/** \brief Sets the collider to test with. */
	void SetTestCollider( btCollisionObject *collisionObject,
		debpCollider *collider, deBaseScriptingCollider *listener );
	
	void SetTestCollider(debpCollider *collider, deBaseScriptingCollider *listener);
	
	/** \brief Set test collision object. */
	void SetTestCollisionObject(btCollisionObject *collisionObject);
	
	/** \brief Ignore multiple contact points to not send them multiple times to the listener. */
	void SetIgnoreMultipleContactPoints( bool ignore );
	/*@}*/
	
	
	
	/** \name Bullet */
	/*@{*/
	/** \brief Collision is possible. */
	virtual bool needsCollision( btBroadphaseProxy *proxy0 ) const;
	
	/** \brief Add result filling parameters in collision info. */
	virtual btScalar addSingleResult( btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0Wrap,
		int partId0, int index0, const btCollisionObjectWrapper *colObj1Wrap, int partId1, int index1 );
	/*@}*/
};

#endif
