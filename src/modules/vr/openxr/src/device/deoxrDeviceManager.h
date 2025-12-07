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

#ifndef _DEOXRDEVICEMANAGER_H_
#define _DEOXRDEVICEMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class deVROpenXR;
class deoxrDevice;
class decString;


/**
 * VR device manager.
 */
class deoxrDeviceManager{
private:
	deVROpenXR &pOxr;
	
	decObjectOrderedSet pDevices;
	bool pNotifyAttachedDetached;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	deoxrDeviceManager(deVROpenXR &oxr);
	
	/** Clean up device list. */
	~deoxrDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** Clear list. Queues events for all removed devices. */
	void Clear();
	
	
	
	/** Number of devices. */
	int GetCount() const;
	
	/** Device at index. */
	deoxrDevice *GetAt(int index) const;
	
	/** Device with identifier or nullptr if absent. */
	deoxrDevice *GetWithID(const char *id) const;
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID(const char *id) const;
	
	/** Add device if absent and queue event. */
	void Add(deoxrDevice *device);
	
	/** Remove device if present and queue event. */
	void Remove(deoxrDevice *device);
	
	/**
	 * Update device parameters if present and queue event. If device becomes invalid
	 * remove it instead.
	 */
// 	void UpdateParameters( vr::TrackedDeviceIndex_t index );
	
	/** Free name number for device type. */
// 	int NextNameNumber( vr::TrackedDeviceClass deviceClass ) const;
	
	/** Track device states. */
	void TrackDeviceStates();
	
	/** Check if devices have been added or removed sending event to game engine. */
	void CheckNotifyAttachedDetached();
	
	/** Reference space changed. */
	void ReferenceSpaceChanged();
	
	
	
	/** Log list of input devices. */
	void LogDevices();
	
	/** Log input device. */
	void LogDevice(const deoxrDevice &device);
	
	
	
	/** Normalize identifier. */
	static decString NormalizeID(const char *id);
	/*@}*/
	
	
	
	
private:
};

#endif
 
