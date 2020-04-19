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

#ifndef _DEBPTSRAYRESULTCALLBACK_H_
#define _DEBPTSRAYRESULTCALLBACK_H_

#include "../coldet/debpRayResultCallback.h"

class debpTouchSensor;


/**
 * \brief Modification of \ref debpRayResultCallback for use with touch sensor shapes.
 * 
 * \ref debpRayResultCallback provides near phase collision filtering between the
 * moving collider and the colliders overlapping the touch sensor. For use with touch
 * sensor shapes though the collision filtering has to include the touch sensor too.
 * To work properly this class has to provide these collision filtering behaviors:
 * 
 * - Moving collider versus hit collider
 * - Hit collider versus parent touch sensor
 * 
 * The moving collider is not supposed to be tested against the touch sensor. Only the
 * hit collider has to be tested. This supports the situation where the touch sensor
 * filters out a collider the moving collider can hit. This can be for example due to
 * an ignore collider in the touch sensor. The near phase has to do the filtering since
 * the broad phase can not handle this well.
 */
class debpTSRayResultCallback : public debpRayResultCallback{
private:
	const debpTouchSensor &pTouchSensor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates result callback. */
	debpTSRayResultCallback( deCollisionInfo *colinfo, const debpTouchSensor &touchSensor );
	/*@}*/
	
	
	
	/** \name Bullet */
	/*@{*/
	/** \brief Collision is possible. */
	virtual bool needsCollision( btBroadphaseProxy *proxy0 ) const;
	/*@}*/
};

#endif
