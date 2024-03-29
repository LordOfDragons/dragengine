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
	deoxrDeviceProfile *GetAt( int index ) const;
	
	/** Add profile. */
	void Add( deoxrDeviceProfile *profile );
	
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
	void RemoveDevice( deInputDevice::eDeviceTypes type );
	/*@}*/
};

#endif
 
