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

#ifndef _DETOUCHSENSORMANAGER_H_
#define _DETOUCHSENSORMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deTouchSensor;


/**
 * \brief Touch sensor Resourse Manager.
 *
 * Manages touch sensor resources.
 */
class DE_DLL_EXPORT deTouchSensorManager : public deResourceManager{
private:
	deResourceList pTouchSensors;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new touch sensor resource manager linked to the given engine. */
	deTouchSensorManager( deEngine *engine );
	
	/** \brief Clean up touch sensor resource manager and reports leaking resources. */
	virtual ~deTouchSensorManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of touch sensors. */
	int GetTouchSensorCount() const;
	
	/** \brief Root touch sensor resource for iteration purpose. */
	deTouchSensor *GetRootTouchSensor() const;
	
	/** \brief Create new touch sensor. */
	deTouchSensor *CreateTouchSensor();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	virtual void SystemPhysicsLoad();
	virtual void SystemPhysicsUnload();
	virtual void SystemScriptingLoad();
	virtual void SystemScriptingUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif
