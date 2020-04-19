/* 
 * Drag[en]gine IGDE
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
