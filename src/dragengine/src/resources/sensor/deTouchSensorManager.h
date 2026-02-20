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

#ifndef _DETOUCHSENSORMANAGER_H_
#define _DETOUCHSENSORMANAGER_H_

#include "deTouchSensor.h"
#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;


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
	deTouchSensorManager(deEngine *engine);
	
	/** \brief Clean up touch sensor resource manager and reports leaking resources. */
	~deTouchSensorManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of touch sensors. */
	int GetTouchSensorCount() const;
	
	/** \brief Root touch sensor resource for iteration purpose. */
	deTouchSensor *GetRootTouchSensor() const;
	
	/** \brief Create new touch sensor. */
	deTouchSensor::Ref CreateTouchSensor();
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemPhysicsLoad() override;
	void SystemPhysicsUnload() override;
	void SystemScriptingLoad() override;
	void SystemScriptingUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	/*@}*/
};

#endif
