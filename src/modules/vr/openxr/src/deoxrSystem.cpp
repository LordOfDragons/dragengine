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
pSystem( esUnknown ),
pMaxLayerCount( 0 ),
pSupportsOrientationTracking( false ),
pSupportsPositionTracking( false ),
pSupportsHandTracking( false )
{
	deVROpenXR &oxr = instance.GetOxr();
	
	try{
		// create system
		XrSystemGetInfo getInfo;
		memset( &getInfo, 0, sizeof( getInfo ) );
		getInfo.type = XR_TYPE_SYSTEM_GET_INFO;
		getInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
		
		OXR_CHECK( oxr, instance.xrGetSystem( instance.GetInstance(), &getInfo, &pSystemId ) );
		
		// get system properties
		XrSystemProperties sysProps;
		memset( &sysProps, 0, sizeof( sysProps ) );
		sysProps.type = XR_TYPE_SYSTEM_PROPERTIES;
		void **next = &sysProps.next;
		
		XrSystemHandTrackingPropertiesEXT sysHTProps;
		
		if( instance.SupportsExtension( deoxrInstance::extEXTHandTracking ) ){
			memset( &sysHTProps, 0, sizeof( sysHTProps ) );
			sysHTProps.type = XR_TYPE_SYSTEM_HAND_TRACKING_PROPERTIES_EXT;
			*next = &sysHTProps;
			next = &sysHTProps.next;
		}
		
		OXR_CHECK( oxr, instance.xrGetSystemProperties( instance.GetInstance(), pSystemId, &sysProps ) );
		
		pSystemName = sysProps.systemName;
		pMaxRenderImageSize.x = sysProps.graphicsProperties.maxSwapchainImageWidth;
		pMaxRenderImageSize.y = sysProps.graphicsProperties.maxSwapchainImageHeight;
		pMaxLayerCount = sysProps.graphicsProperties.maxLayerCount;
		pSupportsOrientationTracking = sysProps.trackingProperties.orientationTracking;
		pSupportsPositionTracking = sysProps.trackingProperties.positionTracking;
		
		if( pSystemName.FindString( "SteamVR" ) != -1 ){
			pSystem = esSteamVR;
			
		}else{
			pSystem = esUnknown;
		}
		
		if( instance.SupportsExtension( deoxrInstance::extEXTHandTracking ) ){
			pSupportsHandTracking = sysHTProps.supportsHandTracking;
		}
		
		instance.GetOxr().LogInfoFormat( "System name: %s", pSystemName.GetString() );
		instance.GetOxr().LogInfoFormat( "Maximum render image size: %d x %d",
			pMaxRenderImageSize.x, pMaxRenderImageSize.y );
		instance.GetOxr().LogInfoFormat( "Maximum layer count: %d", pMaxLayerCount);
		instance.GetOxr().LogInfoFormat( "Supports orientation tracking: %s",
			pSupportsOrientationTracking ? "yes" : "no" );
		instance.GetOxr().LogInfoFormat( "Supports position tracking: %s",
			pSupportsOrientationTracking ? "yes" : "no" );
		instance.GetOxr().LogInfoFormat( "Supports hand tracking: %s",
			pSupportsHandTracking ? "yes" : "no" );
		
		// get view configuration properties
		XrViewConfigurationProperties viewConfProp;
		memset( &viewConfProp, 0, sizeof( viewConfProp ) );
		viewConfProp.type = XR_TYPE_VIEW_CONFIGURATION_PROPERTIES;
		
		OXR_CHECK( oxr, instance.xrGetViewConfigurationProperties( instance.GetInstance(),
			pSystemId, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, &viewConfProp ) );
		
		(void) viewConfProp.fovMutable;
		
		// enumerate view configuration views
		XrViewConfigurationView viewConfViews[ 2 ];
		memset( &viewConfViews, 0, sizeof( viewConfViews ) );
		viewConfViews[ 0 ].type = XR_TYPE_VIEW_CONFIGURATION_VIEW;
		viewConfViews[ 1 ].type = XR_TYPE_VIEW_CONFIGURATION_VIEW;
		
		uint32_t viewCount;
		
		OXR_CHECK( oxr, instance.xrEnumerateViewConfigurationViews( instance.GetInstance(),
			pSystemId, XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 2, &viewCount, viewConfViews ) );
		
		pLeftEyeViewSize.x = viewConfViews[ 0 ].recommendedImageRectWidth;
		pLeftEyeViewSize.y = viewConfViews[ 0 ].recommendedImageRectHeight;
		pLeftEyeViewMaxSize.x = viewConfViews[ 0 ].maxImageRectWidth;
		pLeftEyeViewMaxSize.y = viewConfViews[ 0 ].maxImageRectHeight;
		
		pRightEyeViewSize.x = viewConfViews[ 1 ].recommendedImageRectWidth;
		pRightEyeViewSize.y = viewConfViews[ 1 ].recommendedImageRectHeight;
		pRightEyeViewMaxSize.x = viewConfViews[ 1 ].maxImageRectWidth;
		pRightEyeViewMaxSize.y = viewConfViews[ 1 ].maxImageRectHeight;
		
		// use the largest view size as render size to request from the graphic module
		pRenderSize = pLeftEyeViewSize.Largest( pRightEyeViewSize );
		
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
