/* 
 * Drag[en]gine DragonScript Script Module
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

#include <stdio.h>
#include <stdlib.h>

#include <libdscript/libdscript.h>

#include "dedsInputDevice.h"
#include "../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>


// Class dedsInputDevice
//////////////////////////

// Constructor, destructor
////////////////////////////

dedsInputDevice::dedsInputDevice( deScriptingDragonScript &ds, deBaseInputModule &module, int deviceIndex ) :
pDS( ds ),
pDeviceSource( deInputEvent::esInput ),
pDeviceIndex( deviceIndex ),
pDevice( deInputDevice::Ref::New( module.GetDeviceAt( deviceIndex ) ) ),
pBonePoses( nullptr ),
pBonePoseCount( 0 )
{
	if( pDevice->GetBoneConfiguration() == deInputDevice::ebcHand ){
		pBonePoses = new deInputDevicePose[ deInputDevice::HandBoneCount * 2 ];
		pBonePoseCount = deInputDevice::HandBoneCount;
	}
}

dedsInputDevice::dedsInputDevice( deScriptingDragonScript &ds, deBaseVRModule &module, int deviceIndex ) :
pDS( ds ),
pDeviceSource( deInputEvent::esVR ),
pDeviceIndex( deviceIndex ),
pDevice( deInputDevice::Ref::New( module.GetDeviceAt( deviceIndex ) ) ),
pBonePoses( nullptr ),
pBonePoseCount( 0 )
{
	if( pDevice->GetBoneConfiguration() == deInputDevice::ebcHand ){
		pBonePoses = new deInputDevicePose[ deInputDevice::HandBoneCount * 2 ];
		pBonePoseCount = deInputDevice::HandBoneCount;
	}
}

dedsInputDevice::~dedsInputDevice(){
	if( pBonePoses ){
		delete [] pBonePoses;
	}
}



// Management
///////////////

const deInputDevicePose &dedsInputDevice::GetBonePoseAt( int index, bool withController ) const{
	if( index < 0 || index >= pBonePoseCount ){
		static const deInputDevicePose defaultPose;
		return defaultPose;
	}
	
	if( withController ){
		return pBonePoses[ pBonePoseCount + index ];
		
	}else{
		return pBonePoses[ index ];
	}
}

void dedsInputDevice::OnFrameUpdate(){
	int i;
	
	switch( pDeviceSource ){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *pDS.GetGameEngine()->GetInputSystem()->GetActiveModule();
		
		module.GetDevicePose( pDeviceIndex, pDevicePose );
		
		for( i=0; i<pBonePoseCount; i++ ){
			module.GetDeviceBonePose( pDeviceIndex, i, false, pBonePoses[ i ] );
		}
		for( i=0; i<pBonePoseCount; i++ ){
			module.GetDeviceBonePose( pDeviceIndex, i, true, pBonePoses[ pBonePoseCount + i ] );
		}
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *pDS.GetGameEngine()->GetVRSystem()->GetActiveModule();
		
		module.GetDevicePose( pDeviceIndex, pDevicePose );
		
		for( i=0; i<pBonePoseCount; i++ ){
			module.GetDeviceBonePose( pDeviceIndex, i, false, pBonePoses[ i ] );
		}
		for( i=0; i<pBonePoseCount; i++ ){
			module.GetDeviceBonePose( pDeviceIndex, i, true, pBonePoses[ pBonePoseCount + i ] );
		}
		}break;
	}
}

void dedsInputDevice::Update( const dedsInputDevice &device ){
	if( device.pDeviceSource != pDeviceSource ){
		DETHROW_INFO( deeInvalidParam, "device source differs" );
	}
	if( device.pDevice->GetID() != pDevice->GetID() ){
		DETHROW_INFO( deeInvalidParam, "device id differs" );
	}
	
	pDeviceIndex = device.pDeviceIndex;
	pDevice = device.pDevice;
	
	if( pBonePoseCount != device.pBonePoseCount ){
		if( pBonePoses ){
			delete [] pBonePoses;
			pBonePoses = nullptr;
			pBonePoseCount = 0;
		}
		
		if( device.pBonePoses ){
			pBonePoses = new deInputDevicePose[ deInputDevice::HandBoneCount * 2 ];
			pBonePoseCount = deInputDevice::HandBoneCount;
		}
	}
}
