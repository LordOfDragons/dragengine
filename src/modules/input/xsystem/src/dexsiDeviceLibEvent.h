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

#ifndef _DEXSIDEVICELIBEVENT_H_
#define _DEXSIDEVICELIBEVENT_H_

#include "dexsiDevice.h"



/**
 * \brief X-System libEvent input device.
 */
class dexsiDeviceLibEvent : public dexsiDevice{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<dexsiDeviceLibEvent> Ref;
	
	
	
private:
	decString pEvdevPath;
	int pEvdevFile;
	libevdev *pEvdevDevice;
	short *pEvdevMapRelAxis;
	short *pEvdevMapAbsAxis;
	short *pEvdevMapKeys;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device. */
	dexsiDeviceLibEvent( deXSystemInput &module, const char *pathDevice );
	
protected:
	/** \brief Clean up device. */
	virtual ~dexsiDeviceLibEvent();
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** \brief Libevdev event file path or empty string if not using libevdev. */
	inline const decString &GetEvdevPath() const{ return pEvdevPath; }
	
	/** \brief Libevdev device or \em NULL if not using libevdev. */
	inline libevdev *GetEvdevDevice() const{ return pEvdevDevice; }
	
	
	
	/** \brief Update device state. */
	virtual void Update();
	/*@}*/
};

#endif
