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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	aeALTouchSensorListener();
	/** Cleans up the peer. */
	virtual ~aeALTouchSensorListener();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Sets the animator locomotion. */
	void SetAnimatorLocomotion( aeAnimatorLocomotion *locomotion );
	/*@}*/
	
	/** \name Notifications */
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
