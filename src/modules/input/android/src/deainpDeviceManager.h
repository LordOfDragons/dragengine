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

#ifndef _DEAINPDEVICEMANAGER_H_
#define _DEAINPDEVICEMANAGER_H_

#include "deainpDeviceMouse.h"
#include "deainpDeviceKeyboard.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deAndroidInput;
class deainpDevice;



/**
 * \brief Android input devices.
 */
class deainpDeviceManager{
private:
	deAndroidInput &pModule;
	
	decObjectOrderedSet pDevices;
	
	deainpDeviceMouse::Ref pMouse;
	deainpDeviceKeyboard::Ref pKeyboard;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create device list. */
	deainpDeviceManager(deAndroidInput &module);
	
	/** \brief Clean up device list. */
	~deainpDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** \brief Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** \brief Number of devices. */
	int GetCount() const;
	
	/** \brief Device at index. */
	deainpDevice *GetAt(int index) const;
	
	/** \brief Device with identifier or \em NULL if absent. */
	deainpDevice *GetWithID(const char *id) const;
	
	/** \brief Index of device with identifier or -1 if absent. */
	int IndexOfWithID(const char *id) const;
	
	
	
	/** \brief  mouse device. */
	inline const deainpDeviceMouse::Ref &GetMouse() const{ return pMouse; }
	
	/** \brief  keyboard device. */
	inline const deainpDeviceKeyboard::Ref &GetKeyboard() const{ return pKeyboard; }
	
	
	
	/** \brief Log list of input devices. */
	void LogDevices();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pCreateDevices();
};

#endif
 
