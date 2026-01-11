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

#ifndef _DEXSIDEVICEMANAGER_H_
#define _DEXSIDEVICEMANAGER_H_

#include "dexsiXInclude.h"
#include "dexsiDevice.h"
#include "dexsiDeviceCoreMouse.h"
#include "dexsiDeviceCoreKeyboard.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/common/string/decStringList.h>
#include <dragengine/common/utils/decTimer.h>

class deXSystemInput;



/**
 * X-System input devices.
 */
class dexsiDeviceManager : public deObject{
public:
	using Ref = deTObjectReference<dexsiDeviceManager>;
	
	
private:
	deXSystemInput &pModule;
	
	decTObjectOrderedSet<dexsiDevice> pDevices;
	
	dexsiDeviceCoreMouse::Ref pX11CoreMouse;
	dexsiDeviceCoreKeyboard::Ref pX11CoreKeyboard;
	
	dexsiDevice::Ref pPrimaryMouse;
	dexsiDevice::Ref pPrimaryKeyboard;
	
	int pInotifyFd;
	int pInotifyWatchEvdev;
	const ssize_t pInotifyBufferLen;
	uint8_t *pInotifyBuffer;
	
	decStringList pDelayProbeDevices;
	float pTimeoutDelayProbeDevices;
	decTimer pTimerDelayProbeDevices;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	dexsiDeviceManager(deXSystemInput &module);
	
protected:
	/** Clean up device list. */
	~dexsiDeviceManager() override;
	/*@}*/
	
	
	
public:
	/** \name Module Management */
	/*@{*/
	/** Update list of available devices. */
	void UpdateDeviceList();
	
	
	
	/** Devices. */
	inline const decTObjectOrderedSet<dexsiDevice> &GetDevices() const{ return pDevices; }
	
	/** Device with identifier or \em NULL if absent. */
	dexsiDevice *GetWithID(const char *id);
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID(const char *id);
	
	
	
	/** Core mouse device. */
	inline const dexsiDeviceCoreMouse::Ref &GetX11CoreMouse() const{ return pX11CoreMouse; }
	
	/** Core keyboard device. */
	inline const dexsiDeviceCoreKeyboard::Ref &GetX11CoreKeyboard() const{ return pX11CoreKeyboard; }
	
	/** Primary mouse device. */
	inline const dexsiDevice::Ref &GetPrimaryMouse() const{ return pPrimaryMouse; }
	
	/** Primary keyboard device. */
	inline const dexsiDevice::Ref &GetPrimaryKeyboard() const{ return pPrimaryKeyboard; }
	
	
	
	/** Update. */
	void Update();
	
	/** Log list of input devices. */
	void LogDevices();
	
	/** Log device. */
	void LogDevice(const dexsiDevice &device);
	
	
	
	/** Normalize identifier. */
	static decString NormalizeID(const char *id);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateDevices();
	
	void pCreateXInputDevices();
	void pCreateEvdevDevices();
	
	void pFindPrimaryDevices();
	
	void pStartWatchEvdev();
	void pStopWatchEvdev();
	void pUpdateWatchEvdev();
	void pEvdevAppeared(const decString &path);
	bool pEvdevDisappeared(const decString &path);
	
	void pUpdateDelayProbeDevices();
	bool pProbeDevice(const decString &path);
	void pUpdateDeviceIndices();
};

#endif
 
