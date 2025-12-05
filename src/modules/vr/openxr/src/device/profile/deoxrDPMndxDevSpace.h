/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOXRDPMNDXDEVSPACE_H_
#define _DEOXRDPMNDXDEVSPACE_H_

#include "deoxrDeviceProfile.h"
#include "../../extension/xdev/XR_MNDX_xdev_space.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/utils/decTimer.h>


/**
 * XR_MNDX_xdev_space profile.
 */
class deoxrDPMndxDevSpace : public deoxrDeviceProfile{
private:
	class Device : public deObject{
	public:
		typedef deTObjectReference<Device> Ref;
	
	
		deoxrDPMndxDevSpace &profile;
		const decString name, serial;
		XrXDevIdMNDX id;
		deoxrDevice::Ref device;
		
		Device(deoxrDPMndxDevSpace &profile, const XrXDevPropertiesMNDX &info, XrXDevIdMNDX id);
		
	protected:
		~Device() override;
	};
	
	decObjectOrderedSet pDevices;
	decTimer pTimerCheckAttached;
	float pTimeoutCheckAttached;
	uint64_t pCurGeneration;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device profile. */
	deoxrDPMndxDevSpace(deoxrInstance &instance);
	
protected:
	/** Clean up device profile. */
	~deoxrDPMndxDevSpace() override = default;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** On session state changed. */
	void OnSessionStateChanged() override;
	
	/** On actions synced. */
	void OnActionsSynced() override;
	
	/** Check attached. */
	void CheckAttached() override;
	
	/** Suggest bindings. */
	void SuggestBindings() override;
	
	/** Sessions ends. */
	void OnSessionEnd() override;
	/*@}*/
	
	
private:
	Device *pFindDeviceById(XrXDevIdMNDX id) const;
	Device *pFindDeviceById(const decObjectOrderedSet &list, XrXDevIdMNDX id) const;
	Device *pFindDeviceBySerial(const decObjectOrderedSet &list, const char *serial) const;
	
	decString pSanitizedSerial(const decString &serial) const;
	void pAddDeviceTracker(XrXDevListMNDX list, const Device::Ref &device);
};

#endif

