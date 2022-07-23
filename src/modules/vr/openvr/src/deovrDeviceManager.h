/* 
 * Drag[en]gine OpenVR VR Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
	
	vr::TrackedDevicePose_t pDevicePoses[ vr::k_unMaxTrackedDeviceCount ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	deovrDeviceManager( deVROpenVR &ovr );
	
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
	deovrDevice *GetAt( int index ) const;
	
	/** Device with identifier or nullptr if absent. */
	deovrDevice *GetWithID( const char *id ) const;
	
	/** Device with device index or nullptr if absent. */
	deovrDevice *GetWithIndex( vr::TrackedDeviceIndex_t index ) const;
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id ) const;
	
	/** Index of device with index or -1 if absent. */
	int IndexOfWithIndex( vr::TrackedDeviceIndex_t index ) const;
	
	/** Add device if absent and queue event. */
	void Add( vr::TrackedDeviceIndex_t index );
	
	/** Remove device if present and queue event. */
	void Remove( vr::TrackedDeviceIndex_t index );
	
	/**
	 * Update device parameters if present and queue event. If device becomes invalid
	 * remove it instead.
	 */
	void UpdateParameters( vr::TrackedDeviceIndex_t index );
	
	/** Free name number for device type. */
	int NextNameNumber( vr::TrackedDeviceClass deviceClass ) const;
	
	/** Track device states. */
	void TrackDeviceStates();
	
	/** Device pose at index. */
	const vr::TrackedDevicePose_t &GetDevicePoseAt( int index ) const;
	
	
	
	/** Log list of input devices. */
	void LogDevices();
	
	
	
	/** Normalize identifier. */
	static decString NormalizeID( const char *id );
	/*@}*/
	
	
	
	
private:
};

#endif
 
