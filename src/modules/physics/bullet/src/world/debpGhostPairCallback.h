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

#ifndef _DEBPGHOSTPAIRCALLBACK_H_
#define _DEBPGHOSTPAIRCALLBACK_H_

#include "BulletCollision/BroadphaseCollision/btOverlappingPairCallback.h"

class debpCollisionObject;
class debpTSShape;
struct btBroadphaseProxy; // this is required since btOverlappingPairCallback.h fails to do it


/**
 * \brief Enhances the btGhostPairCallback to properly work with classes using ghost objects.
 */
class debpGhostPairCallback : public btOverlappingPairCallback{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new dispatcher. */
	debpGhostPairCallback();
	/** \brief Cleans up the dispatcher. */
	virtual ~debpGhostPairCallback();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Add overlapping pair to a ghost object. */
	virtual btBroadphasePair *addOverlappingPair( btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1 );
	/** \brief Remove an overlapping pair from a ghost object. */
	virtual void *removeOverlappingPair( btBroadphaseProxy *proxy0, btBroadphaseProxy *proxy1, btDispatcher *dispatcher );
	/** \brief Remove an overlapping pair from all ghost objects. */
	virtual void removeOverlappingPairsContainingProxy( btBroadphaseProxy *proxy0, btDispatcher *dispatcher );
	/*@}*/
};

#endif
