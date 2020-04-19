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
