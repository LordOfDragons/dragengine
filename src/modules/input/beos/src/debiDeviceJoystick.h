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

#ifndef _DEBIDEVICEJOYSTICK_H_
#define _DEBIDEVICEJOYSTICK_H_

#include <dragengine/common/string/decString.h>
#include <device/Joystick.h>

#include "debiDevice.h"


/**
 * \brief Joystick input device using BJoystick API.
 */
class debiDeviceJoystick : public debiDevice{
private:
	decString pDevName;
	BJoystick *pJoystick;
	
	int pStateAxisCount;
	int16 *pStateAxis;
	int pStateHatCount;
	uint8 *pStateHat;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	debiDeviceJoystick(deBeOSInput &module, const char *name);
	
	
	
protected:
	/** \brief Clean up device. */
	virtual ~debiDeviceJoystick();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Device name. */
	inline const decString &GetName() const{ return pDevName; }
	
	/** \brief Joystick instance. */
	inline BJoystick *GetJoystick() const{ return pJoystick; }
	
	
	
	/** \brief Update device state. */
	virtual void Update();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
