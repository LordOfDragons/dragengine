/* 
 * Drag[en]gine DragonScript Script Module
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

#ifndef _DEDSTOUCHSENSOR_H_
#define _DEDSTOUCHSENSOR_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingTouchSensor.h>

class deScriptingDragonScript;
class deTouchSensor;
class dsValue;
class dsRealObject;



/**
 * \brief Touch sensor peer.
 */
class dedsTouchSensor : public deBaseScriptingTouchSensor{
private:
	deScriptingDragonScript &pDS;
	deTouchSensor *pTouchSensor;
	
	dsValue *pValCB;
	bool pHasCB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsTouchSensor( deScriptingDragonScript &ds, deTouchSensor *collider );
	
	/** \brief Clean up peer. */
	virtual ~dedsTouchSensor();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Callback object or \em NULL. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object or \em NULL. */
	void SetCallback( dsRealObject *object );
	/*@}*/
	
	
	
	/** \name Notifications */
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

#endif
