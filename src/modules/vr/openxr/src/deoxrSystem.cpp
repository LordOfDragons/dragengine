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
pSupportsHandTracking( false ),
pSupportsEyeGazeTracking( false ),
pSupportsFaceEyeTracking( false ),
pSupportsFaceLipTracking( false ),
pSupportsPassthrough( false )
{
	try{
		// create system
		XrSystemGetInfo getInfo;
		memset( &getInfo, 0, sizeof( getInfo ) );
		getInfo.type = XR_TYPE_SYSTEM_GET_INFO;
		getInfo.formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
		
		OXR_CHECK( instance.xrGetSystem( instance.GetInstance(), &getInfo, &pSystemId ) );
		
		// get system properties
		XrSystemProperties sysProps;
		memset( &sysProps, 0, sizeof( sysProps ) );
		sysProps.type = XR_TYPE_SYSTEM_PROPERTIES;
		void **next = &sysProps.next;
		
		XrSystemHandTrackingPropertiesEXT sysHandTrackProps;
		memset( &sysHandTrackProps, 0, sizeof( sysHandTrackProps ) );
		if( instance.SupportsExtension( deoxrInstance::extEXTHandTracking ) ){
			sysHandTrackProps.type = XR_TYPE_SYSTEM_HAND_TRACKING_PROPERTIES_EXT;
			*next = &sysHandTrackProps;
			next = &sysHandTrackProps.next;
		}
		
		XrSystemEyeGazeInteractionPropertiesEXT sysEyeGazeProps;
		memset( &sysEyeGazeProps, 0, sizeof( sysEyeGazeProps ) );
		if( instance.SupportsExtension( deoxrInstance::extEXTEyeGazeInteraction ) ){
			sysEyeGazeProps.type = XR_TYPE_SYSTEM_EYE_GAZE_INTERACTION_PROPERTIES_EXT;
			*next = &sysEyeGazeProps;
			next = &sysEyeGazeProps.next;
		}
		
		XrSystemFacialTrackingPropertiesHTC sysFaceTrackProps;
		memset( &sysFaceTrackProps, 0, sizeof( sysFaceTrackProps ) );
		if( instance.SupportsExtension( deoxrInstance::extHTCFacialTracking ) ){
			sysFaceTrackProps.type = XR_TYPE_SYSTEM_FACIAL_TRACKING_PROPERTIES_HTC;
			*next = &sysFaceTrackProps;
			next = &sysFaceTrackProps.next;
		}
		
		XrSystemPassthroughPropertiesFB sysPassThroughProps;
		memset( &sysPassThroughProps, 0, sizeof( sysPassThroughProps ) );
		if( instance.SupportsExtension( deoxrInstance::extFBPassthrough ) ){
			sysPassThroughProps.type = XR_TYPE_SYSTEM_PASSTHROUGH_PROPERTIES_FB;
			*next = &sysPassThroughProps;
			next = ( void** )&sysPassThroughProps.next;
		}
		
		OXR_CHECK( instance.xrGetSystemProperties( instance.GetInstance(), pSystemId, &sysProps ) );
		
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
			pSupportsHandTracking = sysHandTrackProps.supportsHandTracking;
		}
		
		if( instance.SupportsExtension( deoxrInstance::extEXTEyeGazeInteraction ) ){
			pSupportsEyeGazeTracking = sysEyeGazeProps.supportsEyeGazeInteraction;
		}
		
		if( instance.SupportsExtension( deoxrInstance::extHTCFacialTracking ) ){
			pSupportsFaceEyeTracking = sysFaceTrackProps.supportEyeFacialTracking;
			pSupportsFaceLipTracking = sysFaceTrackProps.supportLipFacialTracking;
		}
		
		if( instance.SupportsExtension( deoxrInstance::extFBPassthrough ) ){
			pSupportsPassthrough = sysPassThroughProps.supportsPassthrough;
		}
		
		deVROpenXR &oxr = instance.GetOxr();
		oxr.LogInfoFormat( "System name: %s", pSystemName.GetString() );
		oxr.LogInfoFormat( "Maximum render image size: %d x %d", pMaxRenderImageSize.x, pMaxRenderImageSize.y );
		oxr.LogInfoFormat( "Maximum layer count: %d", pMaxLayerCount);
		oxr.LogInfoFormat( "Supports orientation tracking: %s", pSupportsOrientationTracking ? "yes" : "no" );
		oxr.LogInfoFormat( "Supports position tracking: %s", pSupportsPositionTracking ? "yes" : "no" );
		oxr.LogInfoFormat( "Supports hand tracking: %s", pSupportsHandTracking ? "yes" : "no" );
		oxr.LogInfoFormat( "Supports eye gaze tracking: %s", pSupportsEyeGazeTracking ? "yes" : "no" );
		oxr.LogInfoFormat( "Supports face eye tracking: %s", pSupportsFaceEyeTracking ? "yes" : "no" );
		oxr.LogInfoFormat( "Supports face mouth tracking: %s", pSupportsFaceLipTracking ? "yes" : "no" );
		oxr.LogInfoFormat( "Supports passthrough: %s", pSupportsPassthrough ? "yes" : "no" );
		
		// required features check
		if( oxr.GetRequestFeatureEyeGazeTracking() == deBaseVRModule::efslRequired && ! pSupportsEyeGazeTracking ){
			DETHROW_INFO( deeInvalidParam, "Requires eye gaze tracking but required extension is absent" );
		}
		
		if( oxr.GetRequestFeatureFacialTracking() == deBaseVRModule::efslRequired ){
			if( ! pSupportsFaceEyeTracking && ! pSupportsFaceLipTracking ){
				DETHROW_INFO( deeInvalidParam, "Requires facial tracking but required extensions are absent" );
			}
		}
		
		// get view configuration properties
		uint32_t viewConfigCount;
		OXR_CHECK( instance.xrEnumerateViewConfigurations( instance.GetInstance(),
			pSystemId, 0, &viewConfigCount, nullptr ) );
		oxr.LogInfoFormat( "View configurations: %d", viewConfigCount );
		
		XrViewConfigurationProperties viewConfProp;
		bool viewConfFound = false;
		
		XrViewConfigurationType *viewConfigs = nullptr;
		if( viewConfigCount > 0 ){
			viewConfigs = new XrViewConfigurationType[ viewConfigCount ];
			try{
				OXR_CHECK( instance.xrEnumerateViewConfigurations( instance.GetInstance(),
					pSystemId, viewConfigCount, &viewConfigCount, viewConfigs ) );
				
				XrViewConfigurationProperties props;
				memset( &props, 0, sizeof( props ) );
				props.type = XR_TYPE_VIEW_CONFIGURATION_PROPERTIES;
				
				uint32_t i;
				for( i=0; i<viewConfigCount; i++ ){
					OXR_CHECK( instance.xrGetViewConfigurationProperties(
						instance.GetInstance(), pSystemId, viewConfigs[ i ], &props ) );
					
					switch( viewConfigs[ i ] ){
					case XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO:
						instance.GetOxr().LogInfoFormat( "- %d: Primary Stereo", i );
						
						if( ! viewConfFound ){
							memcpy( &viewConfProp, &props, sizeof( viewConfFound ) );
							viewConfFound = true;
						}
						break;
						
					case XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO:
						instance.GetOxr().LogInfoFormat( "- %d: Primary Mono", i );
						break;
						
					case XR_VIEW_CONFIGURATION_TYPE_PRIMARY_QUAD_VARJO:
						instance.GetOxr().LogInfoFormat( "- %d: Primary Quad Varjo", i );
						break;
						
					case XR_VIEW_CONFIGURATION_TYPE_SECONDARY_MONO_FIRST_PERSON_OBSERVER_MSFT:
						instance.GetOxr().LogInfoFormat( "- %d: Secondary Mono FP Observer", i );
						break;
						
					default:
						instance.GetOxr().LogInfoFormat( "- %d: Unknown type 0x%x", i, viewConfigs[ i ] );
					}
				}
				
				delete [] viewConfigs;
				
			}catch( const deException & ){
				delete [] viewConfigs;
				throw;
			}
		}
		
		if( ! viewConfFound ){
			DETHROW_INFO( deeInvalidAction, "required view configuration not found" );
		}
		
		(void) viewConfProp.fovMutable;
		
		// enumerate view configuration views
		XrViewConfigurationView viewConfViews[ 2 ];
		memset( &viewConfViews, 0, sizeof( viewConfViews ) );
		viewConfViews[ 0 ].type = XR_TYPE_VIEW_CONFIGURATION_VIEW;
		viewConfViews[ 1 ].type = XR_TYPE_VIEW_CONFIGURATION_VIEW;
		
		uint32_t viewCount;
		
		OXR_CHECK( instance.xrEnumerateViewConfigurationViews( instance.GetInstance(),
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
		
		if( pRenderSize == decPoint() ){
			DETHROW_INFO( deeInvalidParam, "openxr runtime requested 0-size view" );
		}
		
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
