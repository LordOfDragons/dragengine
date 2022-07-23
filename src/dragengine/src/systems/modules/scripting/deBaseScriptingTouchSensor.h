/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASESCRIPTINGTOUCHSENSOR_H_
#define _DEBASESCRIPTINGTOUCHSENSOR_H_

#include "../../../dragengine_export.h"

class deCollider;


/**
 * \brief Scripting Module Touch Sensor Peer.
 */
class DE_DLL_EXPORT deBaseScriptingTouchSensor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deBaseScriptingTouchSensor();
	
	/** \brief Clean up peer. */
	virtual ~deBaseScriptingTouchSensor();
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
