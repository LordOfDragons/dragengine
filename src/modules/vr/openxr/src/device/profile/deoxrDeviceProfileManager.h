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

#ifndef _DEOXRDEVICEPROFILEMANAGER_H_
#define _DEOXRDEVICEPROFILEMANAGER_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/input/deInputDevice.h>

class deoxrDeviceProfile;


/**
 * VR device manager.
 */
class deoxrDeviceProfileManager{
private:
	decObjectOrderedSet pProfiles;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile list. */
	deoxrDeviceProfileManager();
	
	/** Clean up device profile list. */
	~deoxrDeviceProfileManager();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/** Count of devices. */
	int GetCount() const;
	
	/** Profile at index. */
	deoxrDeviceProfile *GetAt(int index) const;
	
	/** Add profile. */
	void Add(deoxrDeviceProfile *profile);
	
	/** Remove all profiles. */
	void RemoveAll();
	
	/** On actions synced. */
	void AllOnActionsSynced();
	
	/** On session state changed. */
	void AllOnSessionStateChanged();
	
	/** Check attached state for all profiles. */
	void CheckAllAttached();
	
	/** Clear actions. */
	void ClearActions();
	
	/** Remove device if matching type. */
	void RemoveDevice(deInputDevice::eDeviceTypes type);
	
	/** Sessions ends. */
	void OnSessionEnd();
	/*@}*/
};

#endif
 
