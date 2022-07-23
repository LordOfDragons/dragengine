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

#include "deoxrDeviceProfile.h"
#include "deoxrDeviceProfileManager.h"

#include <dragengine/common/exceptions.h>



// Class deoxrDeviceProfileManager
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoxrDeviceProfileManager::deoxrDeviceProfileManager(){
}

deoxrDeviceProfileManager::~deoxrDeviceProfileManager(){
}



// Management
///////////////

int deoxrDeviceProfileManager::GetCount() const{
	return pProfiles.GetCount();
}

deoxrDeviceProfile *deoxrDeviceProfileManager::GetAt( int index ) const{
	return ( deoxrDeviceProfile* )pProfiles.GetAt( index );
}

void deoxrDeviceProfileManager::Add( deoxrDeviceProfile *profile ){
	if( ! profile ){
		DETHROW_INFO( deeNullPointer, "profile" );
	}
	pProfiles.Add( profile );
}

void deoxrDeviceProfileManager::RemoveAll(){
	pProfiles.RemoveAll();
}

void deoxrDeviceProfileManager::AllOnActionsSynced(){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoxrDeviceProfile* )pProfiles.GetAt( i ) )->OnActionsSynced();
	}
}

void deoxrDeviceProfileManager::AllOnSessionStateChanged(){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoxrDeviceProfile* )pProfiles.GetAt( i ) )->OnSessionStateChanged();
	}
}

void deoxrDeviceProfileManager::CheckAllAttached(){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoxrDeviceProfile* )pProfiles.GetAt( i ) )->CheckAttached();
	}
}

void deoxrDeviceProfileManager::ClearActions(){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoxrDeviceProfile* )pProfiles.GetAt( i ) )->ClearActions();
	}
}

void deoxrDeviceProfileManager::RemoveDevice( deInputDevice::eDeviceTypes type ){
	const int count = pProfiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoxrDeviceProfile* )pProfiles.GetAt( i ) )->RemoveDevice( type );
	}
}
