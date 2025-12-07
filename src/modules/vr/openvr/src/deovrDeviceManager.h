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

#ifndef _DEOVRDEVICEMANAGER_H_
#define _DEOVRDEVICEMANAGER_H_

#include <openvr/openvr.h>

#include <dragengine/common/collection/decIntList.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class deVROpenVR;
class deovrDevice;
class deovrDeviceCoreMouse;
class deovrDeviceCoreKeyboard;
class decString;


/**
 * VR input devices.
 */
class deovrDeviceManager{
private:
	deVROpenVR &pOvr;
	
	decObjectOrderedSet pDevices;
	
	vr::TrackedDevicePose_t pDevicePoses[vr::k_unMaxTrackedDeviceCount];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	deovrDeviceManager(deVROpenVR &ovr);
	
	/** Clean up device list. */
	~deovrDeviceManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** Clear list. Queues events for all removed devices. */
	void Clear();
	
	/** Init devices. Queues events for all added devices. */
	void InitDevices();
	
	
	
	/** Number of devices. */
	int GetCount() const;
	
	/** Device at index. */
	deovrDevice *GetAt(int index) const;
	
	/** Device with identifier or nullptr if absent. */
	deovrDevice *GetWithID(const char *id) const;
	
	/** Device with device index or nullptr if absent. */
	deovrDevice *GetWithIndex(vr::TrackedDeviceIndex_t index) const;
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID(const char *id) const;
	
	/** Index of device with index or -1 if absent. */
	int IndexOfWithIndex(vr::TrackedDeviceIndex_t index) const;
	
	/** Add device if absent and queue event. */
	void Add(vr::TrackedDeviceIndex_t index);
	
	/** Remove device if present and queue event. */
	void Remove(vr::TrackedDeviceIndex_t index);
	
	/**
	 * Update device parameters if present and queue event. If device becomes invalid
	 * remove it instead.
	 */
	void UpdateParameters(vr::TrackedDeviceIndex_t index);
	
	/** Free name number for device type. */
	int NextNameNumber(vr::TrackedDeviceClass deviceClass) const;
	
	/** Track device states. */
	void TrackDeviceStates();
	
	/** Device pose at index. */
	const vr::TrackedDevicePose_t &GetDevicePoseAt(int index) const;
	
	
	
	/** Log list of input devices. */
	void LogDevices();
	
	
	
	/** Normalize identifier. */
	static decString NormalizeID(const char *id);
	/*@}*/
	
	
	
	
private:
};

#endif
 
