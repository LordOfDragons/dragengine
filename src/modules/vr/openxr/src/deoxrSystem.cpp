/* 
 * Drag[en]gine OpenXR
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

#include <stdlib.h>
#include <string.h>

#include "deoxrSystem.h"
#include "deoxrInstance.h"
#include "deVROpenXR.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrSystem
//////////////////////

deoxrSystem::deoxrSystem( deoxrInstance &instance ) :
pInstance( instance ),
pSystemId( XR_NULL_SYSTEM_ID ),
pMaxLayerCount( 0 ),
pSupportsOrientationTracking( false ),
pSupportsPositionTracking( false )
{
	try{
		XrSystemGetInfo getInfo;
		memset( &getInfo, 0, sizeof( getInfo ) );
		getInfo.type = XR_TYPE_SYSTEM_GET_INFO;
		getInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
		
		OXR_CHECK( instance.GetOxr(), instance.xrGetSystem(
			instance.GetInstance(), &getInfo, &pSystemId ) );
		
		XrSystemProperties sysProps;
		memset( &sysProps, 0, sizeof( sysProps ) );
		sysProps.type = XR_TYPE_SYSTEM_PROPERTIES;
		
		OXR_CHECK( instance.GetOxr(), instance.xrGetSystemProperties(
			instance.GetInstance(), pSystemId, &sysProps ) );
		
		pMaxRenderImageSize.x = sysProps.graphicsProperties.maxSwapchainImageWidth;
		pMaxRenderImageSize.y = sysProps.graphicsProperties.maxSwapchainImageHeight;
		pMaxLayerCount = sysProps.graphicsProperties.maxLayerCount;
		pSupportsOrientationTracking = sysProps.trackingProperties.orientationTracking;
		pSupportsPositionTracking = sysProps.trackingProperties.positionTracking;
		
		instance.GetOxr().LogInfoFormat( "System name: %s", sysProps.systemName );
		instance.GetOxr().LogInfoFormat( "Maximum render image size: %d x %d",
			pMaxRenderImageSize.x, pMaxRenderImageSize.y );
		instance.GetOxr().LogInfoFormat( "Maximum layer count: %d", pMaxLayerCount);
		instance.GetOxr().LogInfoFormat( "Supports orientation tracking: %s",
			pSupportsOrientationTracking ? "yes" : "no" );
		instance.GetOxr().LogInfoFormat( "Supports position tracking: %s",
			pSupportsOrientationTracking ? "yes" : "no" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrSystem::~deoxrSystem(){
	pCleanUp();
}



// Management
///////////////



// Private Functions
//////////////////////

void deoxrSystem::pCleanUp(){
}
