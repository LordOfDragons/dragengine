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
#ifndef _IGDESMTOUCHSENSOR_H_
#define _IGDESMTOUCHSENSOR_H_

// includes
#include <dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h>



/**
 * @brief IGDE Script Module Touch Sensor Peer.
 *
 * Touch sensor peer for the IGDRE Script Module. If a delegee is assigned
 * all events are forwarded to the delegee.
 */
class igdeSMTouchSensor : public deBaseScriptingTouchSensor{
private:
	deBaseScriptingTouchSensor *pDelegee;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new gui peer. */
	igdeSMTouchSensor();
	/** Cleans up the peer. */
	virtual ~igdeSMTouchSensor();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the delegee or NULL. */
	inline deBaseScriptingTouchSensor *GetDelegee() const{ return pDelegee; }
	/** Sets the delegee or NULL. */
	void SetDelegee( deBaseScriptingTouchSensor *delegee );
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/**
	 * \brief Notify scripting module peer collider entered touch sensor.
	 * \param collider Collider entering the shape.
	 */
	virtual void ColliderEntered( deCollider *collider );
	
	/**
	 * \brief Notify scripting module peer collider left touch sensor.
	 * \param collider Collider entering the shape.
	 */
	virtual void ColliderLeft( deCollider *collider );
	/*@}*/
};

// end of include only once
#endif
