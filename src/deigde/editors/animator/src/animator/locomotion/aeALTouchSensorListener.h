/* 
 * Drag[en]gine IGDE Animator Editor
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

// include only once
#ifndef _AEALTOUCHSENSORLISTENER_H_
#define _AEALTOUCHSENSORLISTENER_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h>

// predefinitions
class aeAnimatorLocomotion;



/**
 * Animator Locomotion TouchSensor Listener.
 *
 * Scripting module peer for collider of the the animator. Provides
 * typical Player AI type collision handling.
 */
class aeALTouchSensorListener : public deBaseScriptingTouchSensor{
private:
	aeAnimatorLocomotion *pLocomotion;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	aeALTouchSensorListener();
	/** Cleans up the peer. */
	virtual ~aeALTouchSensorListener();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Sets the animator locomotion. */
	void SetAnimatorLocomotion( aeAnimatorLocomotion *locomotion );
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/**
	 * Notify scripting module peer collider entered touch sensor.
	 * \param collider Collider entering the shape.
	 */
	virtual void ColliderEntered( deCollider *collider );
	
	/**
	 * Notify scripting module peer collider left touch sensor.
	 * \param collider Collider entering the shape.
	 */
	virtual void ColliderLeft( deCollider *collider );
	/*@}*/
};

// end of include only once
#endif
