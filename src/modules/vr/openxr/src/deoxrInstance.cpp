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

#include "deVROpenXR.h"
#include "deoxrGlobalFunctions.h"
#include "deoxrInstance.h"
#include "deoxrLoader.h"
#include "deoxrApiLayer.h"
#include "action/deoxrAction.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// class deoxrInstance
////////////////////////

void deoxrInstance::sSuggestBinding::Set( deoxrAction *action, const deoxrPath &binding ){
	this->action = action;
	this->binding = binding;
}

deoxrInstance::deoxrInstance( deVROpenXR &oxr, bool enableDebug ) :
pOxr( oxr ),
pDebug( *this ),
pInstance( XR_NULL_HANDLE )
{
	#ifndef WITH_DEBUG
	(void)enableDebug;
	#endif
	
	memset( &pSupportsExtension, 0, sizeof( pSupportsExtension ) );
	pSupportsExtension[ extKHROpenglEnable ].name = XR_KHR_OPENGL_ENABLE_EXTENSION_NAME;
	pSupportsExtension[ extKHRVisibilityMask ].name = XR_KHR_VISIBILITY_MASK_EXTENSION_NAME;
	pSupportsExtension[ extEXTEyeGazeInteraction ].name = XR_EXT_EYE_GAZE_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[ extEXTHandJointsMotionRange ].name = XR_EXT_HAND_JOINTS_MOTION_RANGE_EXTENSION_NAME;
	pSupportsExtension[ extEXTHandTracking ].name = XR_EXT_HAND_TRACKING_EXTENSION_NAME;
	pSupportsExtension[ extEXTPerformanceSettings ].name = XR_EXT_PERFORMANCE_SETTINGS_EXTENSION_NAME;
	pSupportsExtension[ extEXTViewConfigurationDepthRange ].name = XR_EXT_VIEW_CONFIGURATION_DEPTH_RANGE_EXTENSION_NAME;
	pSupportsExtension[ extEPICViewConfigurationFov ].name = XR_EPIC_VIEW_CONFIGURATION_FOV_EXTENSION_NAME;
	pSupportsExtension[ extFBDisplayRefreshRate ].name = XR_FB_DISPLAY_REFRESH_RATE_EXTENSION_NAME;
	pSupportsExtension[ extFBFoveation ].name = XR_FB_FOVEATION_EXTENSION_NAME;
	pSupportsExtension[ extFBFoveationConfiguration ].name = XR_FB_FOVEATION_CONFIGURATION_EXTENSION_NAME;
	pSupportsExtension[ extFBHandTrackingAim ].name = XR_FB_HAND_TRACKING_AIM_EXTENSION_NAME;
	pSupportsExtension[ extFBHandTrackingCapsules ].name = XR_FB_HAND_TRACKING_CAPSULES_EXTENSION_NAME;
	pSupportsExtension[ extFBHandTrackingMesh ].name = XR_FB_HAND_TRACKING_MESH_EXTENSION_NAME;
	pSupportsExtension[ extFBKeyboardTracking ].name = XR_FB_KEYBOARD_TRACKING_EXTENSION_NAME;
	pSupportsExtension[ extFBPassthrough ].name = XR_FB_PASSTHROUGH_EXTENSION_NAME;
	pSupportsExtension[ extFBPassthroughKeyboardHands ].name = XR_FB_PASSTHROUGH_KEYBOARD_HANDS_EXTENSION_NAME;
	pSupportsExtension[ extFBRenderModel ].name = XR_FB_RENDER_MODEL_EXTENSION_NAME;
	pSupportsExtension[ extFBSpaceWarp ].name = XR_FB_SPACE_WARP_EXTENSION_NAME;
	pSupportsExtension[ extFBTriangleMesh ].name = XR_FB_TRIANGLE_MESH_EXTENSION_NAME;
	pSupportsExtension[ extHTCFacialTracking ].name = XR_HTC_FACIAL_TRACKING_EXTENSION_NAME;
	pSupportsExtension[ extHTCXViveTrackerInteraction ].name = XR_HTCX_VIVE_TRACKER_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[ extMNDHeadless ].name = XR_MND_HEADLESS_EXTENSION_NAME;
	pSupportsExtension[ extEXTDebugUtils ].name = XR_EXT_DEBUG_UTILS_EXTENSION_NAME;
	pSupportsExtension[ extEXTHPMixedRealityController ].name = XR_EXT_HP_MIXED_REALITY_CONTROLLER_EXTENSION_NAME;
	pSupportsExtension[ extEXTSamsungOdysseyController ].name = XR_EXT_SAMSUNG_ODYSSEY_CONTROLLER_EXTENSION_NAME;
	pSupportsExtension[ extHTCViveCosmosControllerInteraction ].name = XR_HTC_VIVE_COSMOS_CONTROLLER_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[ extHTCViveFocus3ControllerInteraction ].name = XR_HTC_VIVE_FOCUS3_CONTROLLER_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[ extHUAWEIControllerInteraction ].name = XR_HUAWEI_CONTROLLER_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[ extMSFTHandInteraction ].name = XR_MSFT_HAND_INTERACTION_EXTENSION_NAME;
	
	memset( &pSupportsLayer, 0, sizeof( pSupportsLayer ) );
	pSupportsLayer[ layerLunarCoreValidation ].name = "XR_APILAYER_LUNARG_core_validation";
	
	#define INSTANCE_LEVEL_OPENXR_FUNCTION( name ) name = XR_NULL_HANDLE;
	#define INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( name, extension ) name = XR_NULL_HANDLE;
	
	#include "deoxrFunctionNames.h"
	
	try{
		#ifdef WITH_DEBUG
			pCreateInstance( enableDebug );
		#else
			pCreateInstance( false );
		#endif
		
		pLoadFunctions();
		
		#ifdef WITH_DEBUG
			pDebug.SetEnabled( enableDebug );
		#endif
		
		pPathHandLeft = deoxrPath( *this, "/user/hand/left" );
		pPathHandRight = deoxrPath( *this, "/user/hand/right" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoxrInstance::~deoxrInstance(){
	pCleanUp();
}



// Management
///////////////

bool deoxrInstance::SupportsExtension( eExtension extension ) const{
	return pSupportsExtension[ extension ].version != 0;
}

uint32_t deoxrInstance::ExtensionVersion( eExtension extension ) const{
	return pSupportsExtension[ extension ].version;
}

bool deoxrInstance::SupportsLayer( eLayer layer ) const{
	return pSupportsLayer[ layer ].layerVersion != 0;
}

uint32_t deoxrInstance::LayerVersion( eLayer layer ) const{
	return pSupportsLayer[ layer ].layerVersion;
}

void deoxrInstance::SuggestBindings( const deoxrPath &profile, const sSuggestBinding *bindings, int count ){
	if( ! profile ){
		DETHROW_INFO( deeNullPointer, "profile" );
	}
	if( ! bindings ){
		DETHROW_INFO( deeNullPointer, "bindings" );
	}
	if( count < 1 ){
		DETHROW_INFO( deeInvalidParam, "count < 1" );
	}
	
	XrActionSuggestedBinding * const xrbindings = new XrActionSuggestedBinding[ count ];
	
	try{
		int i;
		for( i=0; i<count; i++ ){
			xrbindings[ i ].action = bindings[ i ].action->GetAction();
			xrbindings[ i ].binding = bindings[ i ].binding;
		}
		
		XrInteractionProfileSuggestedBinding psb;
		memset( &psb, 0, sizeof( psb ) );
		psb.type = XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING;
		psb.interactionProfile = profile;
		psb.suggestedBindings = xrbindings;
		psb.countSuggestedBindings = count;
		OXR_CHECK( pOxr, xrSuggestInteractionProfileBindings( pInstance, &psb ) );
		
		delete [] xrbindings;
		
	}catch( const deException & ){
		if( xrbindings ){
			delete [] xrbindings;
		}
		throw;
	}
}



// Private Functions
//////////////////////

void deoxrInstance::pCleanUp(){
	pDebug.SetEnabled( false );
	
	if( pInstance ){
		xrDestroyInstance( pInstance );
	}
}

void deoxrInstance::pDetectExtensions(){
	uint32_t count = 0;
	OXR_CHECK( pOxr, xrEnumerateInstanceExtensionProperties( XR_NULL_HANDLE, 0, &count, XR_NULL_HANDLE ) );
	if( count == 0 ){
		return;
	}
	
	XrExtensionProperties * const extensions = new XrExtensionProperties[ count ];
	try{
		OXR_CHECK( pOxr, xrEnumerateInstanceExtensionProperties( XR_NULL_HANDLE, count, &count, extensions ) );
		
		// report all extensions for debug purpose
		uint32_t i;
		
		pOxr.LogInfo( "Extensions:" );
		for( i=0; i<count; i++ ){
			pOxr.LogInfoFormat( "- %s: %d", extensions[ i ].extensionName, extensions[ i ].extensionVersion );
		}
		
		// store supported extensions
		int j;
		for( i=0; i<count; i++ ){
			for( j=0; j<ExtensionCount; j++ ){
				if( strcmp( pSupportsExtension[ j ].name, extensions[ i ].extensionName ) == 0 ){
					pSupportsExtension[ j ].version = extensions[ i ].extensionVersion;
					break;
				}
			}
		}
		
		// report support extensions
		pOxr.LogInfo( "Supported Extensions:" );
		for( i=0; i<ExtensionCount; i++ ){
			if( pSupportsExtension[ i ].version ){
				pOxr.LogInfoFormat( "- %s: %d", pSupportsExtension[ i ].name,
					pSupportsExtension[ i ].version );
			}
		}
		
		// report support extensions
		pOxr.LogInfo( "Not upported Extensions:" );
		for( i=0; i<ExtensionCount; i++ ){
			if( ! pSupportsExtension[ i ].version ){
				pOxr.LogInfoFormat( "- %s", pSupportsExtension[ i ].name );
			}
		}
		
		delete [] extensions;
		
	}catch( const deException & ){
		delete [] extensions;
		throw;
	}
}

void deoxrInstance::pDetectLayers(){
	/*
	the OpenXR-SDK loader does not use this method but searches for libraries directly
	
	// linux
	#define OPENXR_RELATIVE_PATH "openxr/"
	#define OPENXR_IMPLICIT_API_LAYER_RELATIVE_PATH "/api_layers/implicit.d"
	#define OPENXR_EXPLICIT_API_LAYER_RELATIVE_PATH "/api_layers/explicit.d"
	
	// windows
	#define OPENXR_REGISTRY_LOCATION "SOFTWARE\\Khronos\\OpenXR\\"
	#define OPENXR_IMPLICIT_API_LAYER_REGISTRY_LOCATION "\\ApiLayers\\Implicit"
	#define OPENXR_EXPLICIT_API_LAYER_REGISTRY_LOCATION "\\ApiLayers\\Explicit"
	
	 */
	uint32_t runtimeCount = 0;
	if( xrEnumerateApiLayerProperties ){
		OXR_CHECK( pOxr, xrEnumerateApiLayerProperties( 0, &runtimeCount, XR_NULL_HANDLE ) );
	}
	
	const uint32_t loaderCount = pOxr.GetLoader()->GetApiLayerCount();
	uint32_t i, count = runtimeCount;
	
	XrApiLayerProperties *layers = nullptr;
	try{
		if( runtimeCount + loaderCount > 0 ){
			layers = new XrApiLayerProperties[ runtimeCount + loaderCount ];
			memset( layers, 0, sizeof( XrApiLayerProperties ) * ( runtimeCount + loaderCount ) );
			for( i=0; i<runtimeCount+loaderCount; i++ ){
				layers[ i ].type = XR_TYPE_API_LAYER_PROPERTIES;
			}
			
			if( xrEnumerateApiLayerProperties ){
				OXR_CHECK( pOxr, xrEnumerateApiLayerProperties( runtimeCount, &runtimeCount, layers ) );
			}
		}
		
		// load api layers the loader found
		for( i=0; i<loaderCount; i++ ){
			deoxrApiLayer &apiLayer = *pOxr.GetLoader()->GetApiLayerAt( i );
			try{
				apiLayer.LoadLibrary();
				if( apiLayer.IsLoaded() ){
					XrApiLayerProperties &next = layers[ count++ ];
					strncpy( next.layerName, apiLayer.GetName(), sizeof( next.layerName ) - 1 );
					strncpy( next.description, apiLayer.GetName(), sizeof( next.layerName ) - 1 );
					next.layerVersion = 1;
					next.specVersion = XR_MAKE_VERSION( 1, 0, 0 );
				}
				
			}catch( const deException &e ){
				pOxr.LogException( e );
			}
		}
		
		// report all layers for debug purpose
		pOxr.LogInfo( "Layers:" );
		for( i=0; i<count; i++ ){
			pOxr.LogInfoFormat( "- %s: %d (%d.%d.%d) [%s]",
				layers[ i ].layerName, layers[ i ].layerVersion,
				XR_VERSION_MAJOR( layers[ i ].specVersion ),
				XR_VERSION_MINOR( layers[ i ].specVersion ),
				XR_VERSION_PATCH( layers[ i ].specVersion ),
				layers[ i ].description );
		}
		
		// store supported layers
		int j;
		for( i=0; i<count; i++ ){
			for( j=0; j<LayerCount; j++ ){
				if( strcmp( pSupportsLayer[ j ].name, layers[ i ].layerName ) == 0 ){
					pSupportsLayer[ j ].version = layers[ i ].specVersion;
					pSupportsLayer[ j ].layerVersion = layers[ i ].layerVersion;
					break;
				}
			}
		}
		
		// report support layers
		pOxr.LogInfo( "Supported Layers:" );
		for( i=0; i<LayerCount; i++ ){
			if( pSupportsLayer[ i ].layerVersion ){
				pOxr.LogInfoFormat( "- %s: %d (%d.%d.%d)",
					pSupportsLayer[ i ].name, pSupportsLayer[ i ].layerVersion,
					XR_VERSION_MAJOR( pSupportsLayer[ i ].layerVersion ),
					XR_VERSION_MINOR( pSupportsLayer[ i ].layerVersion ),
					XR_VERSION_PATCH( pSupportsLayer[ i ].layerVersion ) );
			}
		}
		
		pOxr.LogInfo( "Not Supported Layers:" );
		for( i=0; i<LayerCount; i++ ){
			if( ! pSupportsLayer[ i ].layerVersion ){
				pOxr.LogInfoFormat( "- %s", pSupportsLayer[ i ].name );
			}
		}
		
		if( layers ){
			delete [] layers;
		}
		
	}catch( const deException & ){
		if( layers ){
			delete [] layers;
		}
		throw;
	}
}

void deoxrInstance::pCreateInstance( bool enableValidationLayers ){
	pOxr.LogInfo( "Create OpenXR Instance" );
	
	// create instance
	XrInstanceCreateInfo instanceCreateInfo;
	memset( &instanceCreateInfo, 0, sizeof( instanceCreateInfo ) );
	
	instanceCreateInfo.type = XR_TYPE_INSTANCE_CREATE_INFO;
	
	decStringList moduleVersion( decString( OXR_MODULE_VERSION ).Split( '.' ) );
	while( moduleVersion.GetCount() < 3 ){
		moduleVersion.Add( "0" );
	}
	
	strcpy( instanceCreateInfo.applicationInfo.applicationName, "Drag[en]gine" );
	instanceCreateInfo.applicationInfo.applicationVersion = XR_MAKE_VERSION(
		moduleVersion.GetAt( 0 ).ToInt(), moduleVersion.GetAt( 1 ).ToInt(),
		moduleVersion.GetAt( 2 ).ToInt() );
	strcpy( instanceCreateInfo.applicationInfo.engineName, "Drag[en]gine" );
	instanceCreateInfo.applicationInfo.engineVersion =
		instanceCreateInfo.applicationInfo.applicationVersion;
	instanceCreateInfo.applicationInfo.apiVersion = XR_MAKE_VERSION( 1, 0, 0 );
	
	// detect extensions and layers
	pDetectExtensions();
	pDetectLayers();
	
	// enable layers
	const char *layers[ LayerCount ];
	uint32_t layerCount = 0;
	
	if( enableValidationLayers /* && SupportsExtension( extEXTDebugUtils ) */ ){
		if( SupportsLayer( layerLunarCoreValidation ) ){
			layers[ layerCount++ ] = pSupportsLayer[ layerLunarCoreValidation ].name;
		}
	}
	
	// enable extensions
	const char *extensions[ ExtensionCount ];
	uint32_t i, extensionCount = 0;
	
	for( i=0; i<ExtensionCount; i++ ){
		if( pSupportsExtension[ i ].version ){
			extensions[ extensionCount++ ] = pSupportsExtension[ i ].name;
		}
	}
	
	pOxr.LogInfo( "Enable Layers:" );
	for( i=0; i<layerCount; i++ ){
		pOxr.LogInfoFormat( "- %s", layers[ i ] );
	}
	
	if( layerCount > 0 ){
		instanceCreateInfo.enabledApiLayerNames = layers;
		instanceCreateInfo.enabledApiLayerCount = layerCount;
	}
	if( extensionCount > 0 ){
		instanceCreateInfo.enabledExtensionNames = extensions;
		instanceCreateInfo.enabledExtensionCount = extensionCount;
	}
	
	// create device
	OXR_CHECK( pOxr, xrCreateInstance( &instanceCreateInfo, &pInstance ) );
}

void deoxrInstance::pLoadFunctions(){
	#define INSTANCE_LEVEL_OPENXR_FUNCTION( name ) \
		if( xrGetInstanceProcAddr( pInstance, #name, ( PFN_xrVoidFunction* )&name ) != XR_SUCCESS ){ \
			DETHROW_INFO( deeInvalidAction, "Instance function " #name " not found" ); \
		}
	
	#define INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( name, extension ) \
		if( pSupportsExtension[ extension ].version != 0 ){ \
			INSTANCE_LEVEL_OPENXR_FUNCTION( name ) \
		}
	
	#include "deoxrFunctionNames.h"
}
