/* 
 * Drag[en]gine OpenXR VR Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create device list. */
	deoxrDeviceManager( deVROpenXR &oxr );
	
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
	deoxrDevice *GetAt( int index ) const;
	
	/** Device with identifier or nullptr if absent. */
	deoxrDevice *GetWithID( const char *id ) const;
	
	/** Index of device with identifier or -1 if absent. */
	int IndexOfWithID( const char *id ) const;
	
	/** Add device if absent and queue event. */
	void Add( deoxrDevice *device );
	
	/** Remove device if present and queue event. */
	void Remove( deoxrDevice *device );
	
	/**
	 * Update device parameters if present and queue event. If device becomes invalid
	 * remove it instead.
	 */
// 	void UpdateParameters( vr::TrackedDeviceIndex_t index );
	
	/** Free name number for device type. */
// 	int NextNameNumber( vr::TrackedDeviceClass deviceClass ) const;
	
	/** Track device states. */
	void TrackDeviceStates();
	
	
	
	/** Log list of input devices. */
	void LogDevices();
	
	/** Log input device. */
	void LogDevice( const deoxrDevice &device );
	
	
	
	/** Normalize identifier. */
	static decString NormalizeID( const char *id );
	/*@}*/
	
	
	
	
private:
};

#endif
 
