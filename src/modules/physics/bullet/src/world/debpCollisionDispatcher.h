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

#ifndef _DEBPCOLLISIONDISPATCHER_H_
#define _DEBPCOLLISIONDISPATCHER_H_

#include <dragengine/common/math/decMath.h>

#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"

class btCollisionConfiguration;
class debpSharedCollisionFiltering;



/**
 * \brief Collision dispatcher.
 * 
 * Enhances the CollisionDispatcher to determine the need for collision detection and
 * response using the callback functions build into the scripting peers.
 */
class debpCollisionDispatcher : public btCollisionDispatcher{
private:
	debpSharedCollisionFiltering &pCollisionFiltering;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dispatcher. */
	debpCollisionDispatcher(debpSharedCollisionFiltering &collisionFiltering,
		btCollisionConfiguration *collisionConfiguration);
	
	/** \brief Clean up dispatcher. */
	virtual ~debpCollisionDispatcher();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Collision objects can collide. */
// 	virtual bool needsCollision( const btCollisionObject *body0, const btCollisionObject *body1 );
	
	/** \brief Collision objects need collision response. */
// 	virtual bool needsResponse( const btCollisionObject *body0, const btCollisionObject *body1 );
	/*@}*/
};

#endif
