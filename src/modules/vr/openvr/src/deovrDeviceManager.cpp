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

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "deovrDevice.h"
#include "deovrDeviceAxis.h"
#include "deovrDeviceButton.h"
#include "deovrDeviceFeedback.h"
#include "deovrDeviceComponent.h"
#include "deovrDeviceManager.h"
#include "deVROpenVR.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/input/deInputEventQueue.h>
#include <dragengine/systems/deVRSystem.h>



// Class deovrDeviceManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deovrDeviceManager::deovrDeviceManager( deVROpenVR &ovr ) :
pOvr( ovr )
{
	memset( pDevicePoses, 0, sizeof( pDevicePoses ) );
}

deovrDeviceManager::~deovrDeviceManager(){
}



// Management
///////////////

void deovrDeviceManager::Clear(){
	deInputEvent event;
	event.SetType( deInputEvent::eeDeviceDetached );
	event.SetSource( deInputEvent::esVR );
	pOvr.InputEventSetTimestamp( event );
	
	int count = pDevices.GetCount();
	
	while( count > 0 ){
		count--;
		event.SetDevice( count );
		pOvr.GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
	}
	
	pDevices.RemoveAll();
}

void deovrDeviceManager::InitDevices(){
	vr::TrackedDeviceIndex_t index;
	deovrDevice::Ref device;
	
	Clear();
	
	deInputEvent event;
	event.SetType( deInputEvent::eeDeviceAttached );
	event.SetSource( deInputEvent::esVR );
	pOvr.InputEventSetTimestamp( event );
	
	for( index=0; index<vr::k_unMaxTrackedDeviceCount; index++ ){
		if( ! pOvr.GetVRSystem().IsTrackedDeviceConnected( index ) ){
			// openvr potentially reports previously attached devices which are no more
			// attached right now. avoid listing them confusing the user
			continue;
		}
		
		device.TakeOver( new deovrDevice( pOvr, index ) );
		if( device->GetType() == deInputDevice::edtGeneric ){
			continue;
		}
		
		device->SetIndex( pDevices.GetCount() );
		pDevices.Add( device );
		
		event.SetDevice( device->GetIndex() );
		pOvr.GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
	}
}

/*
void deovrDeviceManager::UpdateDeviceList(){
	deInputEventQueue &queue = pOvr.GetGameEngine()->GetVRSystem()->GetEventQueue();
	const timeval now = { decDateTime().ToSystemTime(), 0 };
	
	// remove all devices not connected anymore
	int i = pDevices.GetCount();
	while( i > 0 ){
		i--;
		deovrDevice &device = *( ( deovrDevice* )pDevices.GetAt( i ) );
		device.UpdateParameters();
		if( device.GetType() != deInputDevice::edtGeneric ){
			continue;
		}
		
		pDevices.RemoveFrom( i );
		
		deInputEvent event;
		event.SetType( deInputEvent::eeDeviceDetached );
		event.SetSource( deInputEvent::esVR );
		event.SetDevice( i );
		event.SetTime( now );
		queue.AddEvent( event );
	}
	
	// 
}
*/


int deovrDeviceManager::GetCount() const{
	return pDevices.GetCount();
}

deovrDevice *deovrDeviceManager::GetAt( int index ) const{
	return ( deovrDevice* )pDevices.GetAt( index );
}

deovrDevice *deovrDeviceManager::GetWithID( const char *id ) const{
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDevice * const device = ( deovrDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return device;
		}
	}
	
	return nullptr;
}

deovrDevice *deovrDeviceManager::GetWithIndex( vr::TrackedDeviceIndex_t index ) const{
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDevice * const device = ( deovrDevice* )pDevices.GetAt( i );
		if( device->GetDeviceIndex() == index ){
			return device;
		}
	}
	
	return nullptr;
}

int deovrDeviceManager::IndexOfWithID( const char *id ) const{
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDevice * const device = ( deovrDevice* )pDevices.GetAt( i );
		if( device->GetID() == id ){
			return i;
		}
	}
	
	return -1;
}

int deovrDeviceManager::IndexOfWithIndex( vr::TrackedDeviceIndex_t index ) const{
	const int count = pDevices.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deovrDevice * const device = ( deovrDevice* )pDevices.GetAt( i );
		if( device->GetDeviceIndex() == index ){
			return i;
		}
	}
	
	return -1;
}

void deovrDeviceManager::Add( vr::TrackedDeviceIndex_t index ){
	if( IndexOfWithIndex( index ) != -1 ){
		return;
	}
	
	const int realIndex  = pDevices.GetCount();
	const deovrDevice::Ref device( deovrDevice::Ref::New( new deovrDevice( pOvr, index ) ) );
	device->SetIndex( pDevices.GetCount() );
	pDevices.Add( device );
	
	pOvr.LogInfoFormat( "Input Device Added: id='%s' type=%d axes=%d buttons=%d feedbacks=%d",
		device->GetID().GetString(), device->GetType(), device->GetAxisCount(),
		device->GetButtonCount(), device->GetFeedbackCount() );
	
	deInputEvent event;
	event.SetType( deInputEvent::eeDeviceAttached );
	event.SetSource( deInputEvent::esVR );
	event.SetDevice( realIndex );
	pOvr.InputEventSetTimestamp( event );
	pOvr.GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
}

void deovrDeviceManager::Remove( vr::TrackedDeviceIndex_t index ){
	const int realIndex = IndexOfWithIndex( index );
	if( realIndex == -1 ){
		return;
	}
	
	pOvr.LogInfoFormat( "Input Device Removed: id='%s'", GetAt( realIndex )->GetID().GetString() );
	
	pDevices.RemoveFrom( realIndex );
	
	const int count = pDevices.GetCount();
	int i;
	for( i=realIndex; i<count; i++ ){
		GetAt( i )->SetIndex( i );
	}
	
	deInputEvent event;
	event.SetType( deInputEvent::eeDeviceDetached );
	event.SetSource( deInputEvent::esVR );
	event.SetDevice( realIndex );
	pOvr.InputEventSetTimestamp( event );
	pOvr.GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
}

void deovrDeviceManager::UpdateParameters( vr::TrackedDeviceIndex_t index ){
	const int realIndex = IndexOfWithIndex( index );
	if( realIndex == -1 ){
		return;
	}
	
	deovrDevice &device = *GetAt( index );
	device.UpdateParameters();
	
	if( device.GetType() == deInputDevice::edtGeneric ){
		Remove( index );
		return;
	}
	
	deInputEvent event;
	event.SetType( deInputEvent::eeDeviceParamsChanged );
	event.SetSource( deInputEvent::esVR );
	event.SetDevice( realIndex );
	pOvr.InputEventSetTimestamp( event );
	pOvr.GetGameEngine()->GetVRSystem()->GetEventQueue().AddEvent( event );
}

int deovrDeviceManager::NextNameNumber( vr::TrackedDeviceClass deviceClass ) const{
	const int count = pDevices.GetCount();
	int i, nameNumber = 1;
	
	while( true ){
		for( i=0; i<count; i++ ){
			const deovrDevice &device = *GetAt( i );
			if( device.GetDeviceClass() == deviceClass && device.GetNameNumber() == nameNumber ){
				break;
			}
		}
		if( i == count ){
			return nameNumber;
		}
		nameNumber++;
	}
}

void deovrDeviceManager::TrackDeviceStates(){
// 	float photonsFromNow = 0.0f;
// 	pOvr.GetVRSystem().GetDeviceToAbsoluteTrackingPose( vr::TrackingUniverseStanding,
// 		photonsFromNow, pDevicePoses, vr::k_unMaxTrackedDeviceCount );
	
	pOvr.CopyDevicesPoses( pDevicePoses );
	
	const int count = pDevices.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		GetAt( i )->TrackStates();
	}
}

const vr::TrackedDevicePose_t &deovrDeviceManager::GetDevicePoseAt( int index ) const{
	if( index < 0 || index >= ( int )vr::k_unMaxTrackedDeviceCount ){
		DETHROW( deeInvalidParam );
	}
	return pDevicePoses[ index ];
}



void deovrDeviceManager::LogDevices(){
	const int count = pDevices.GetCount();
	int i, j;
	
	pOvr.LogInfo( "Input Devices:" );
	
	for( i=0; i<count; i++ ){
		const deovrDevice &device = *( ( deovrDevice* )pDevices.GetAt( i ) );
		pOvr.LogInfoFormat( "- '%s' (%s) %d [%d]", device.GetName().GetString(),
			device.GetID().GetString(), device.GetDeviceIndex(), device.GetType() );
		
		const int componentCount = device.GetComponentCount();
		if( componentCount > 0 ){
			pOvr.LogInfo( "  Components:" );
			for( j=0; j<componentCount; j++ ){
				const deovrDeviceComponent &component = *device.GetComponentAt( j );
				pOvr.LogInfoFormat( "    - '%s' (%s)", component.GetName().GetString(),
					component.GetID().GetString() );
			}
		}
		
		const int axisCount = device.GetAxisCount();
		if( axisCount > 0 ){
			pOvr.LogInfo( "  Axes:" );
			for( j=0; j<axisCount; j++ ){
				const deovrDeviceAxis &axis = *device.GetAxisAt( j );
				pOvr.LogInfoFormat( "    - '%s' (%s) [%s]", axis.GetName().GetString(),
					axis.GetID().GetString(), axis.GetInputDeviceComponent()
						? axis.GetInputDeviceComponent()->GetID().GetString() : "" );
			}
		}
		
		const int buttonCount = device.GetButtonCount();
		if( buttonCount > 0 ){
			pOvr.LogInfo( "  Buttons:" );
			for( j=0; j<buttonCount; j++ ){
				const deovrDeviceButton &button = *device.GetButtonAt( j );
				pOvr.LogInfoFormat( "    - '%s' (%s) [%s] => %d",
					button.GetName().GetString(), button.GetID().GetString(), button.GetInputDeviceComponent()
						? button.GetInputDeviceComponent()->GetID().GetString() : "", j );
			}
		}
	}
}



decString deovrDeviceManager::NormalizeID( const char *id ){
	if( ! id ){
		DETHROW( deeInvalidParam );
	}
	
	const int len = strlen( id );
	if( len == 0 ){
		return decString();
	}
	
	decString nid;
	nid.Set( ' ', len );
	
	int i;
	for( i=0; i<len; i++ ){
		if( ( id[ i ]  >= 'A' && id[ i ] <= 'Z' )
		|| ( id[ i ] >= 'a' && id[ i ] <= 'z' )
		|| ( id[ i ] >= '0' && id[ i ] <= '9' )
		|| id[ i ] == '_' ){
			nid[ i ] = id[ i ];
			
		}else{
			nid[ i ] = '_';
		}
	}
	
	return nid;
}



// Private functions
//////////////////////
