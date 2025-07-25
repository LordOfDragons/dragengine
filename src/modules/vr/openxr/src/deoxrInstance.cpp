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

#include "deVROpenXR.h"
#include "deoxrGlobalFunctions.h"
#include "deoxrInstance.h"
#include "action/deoxrAction.h"
#include "loader/deoxrLoader.h"
#include "loader/deoxrApiLayer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>

#ifdef OS_ANDROID
#include <dragengine/deEngine.h>
#include <dragengine/app/deOSAndroid.h>
#endif


// class deoxrInstance
////////////////////////

#ifndef MODULE_VERSION
#include "module_version.h"
#endif

void deoxrInstance::sSuggestBinding::Set( deoxrAction *paction, const deoxrPath &pbinding ){
	action = paction;
	binding = pbinding;
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
	#ifdef OS_ANDROID
		pSupportsExtension[ extKHROpenglEnable ].name = XR_KHR_OPENGL_ES_ENABLE_EXTENSION_NAME;
	#else
		pSupportsExtension[ extKHROpenglEnable ].name = XR_KHR_OPENGL_ENABLE_EXTENSION_NAME;
	#endif
	
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
	pSupportsExtension[ extKHRCompositionLayerDepth ].name = XR_KHR_COMPOSITION_LAYER_DEPTH_EXTENSION_NAME;
	pSupportsExtension[ extEXTHandInteraction ].name = XR_EXT_HAND_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[ extHTCHandInteraction ].name = XR_HTC_HAND_INTERACTION_EXTENSION_NAME;
	pSupportsExtension[extFBTouchControllerPro].name = XR_FB_TOUCH_CONTROLLER_PRO_EXTENSION_NAME;
	pSupportsExtension[extFBTouchControllerProximity].name = XR_FB_TOUCH_CONTROLLER_PROXIMITY_EXTENSION_NAME;
	pSupportsExtension[extMETATouchControllerPlus].name = XR_META_TOUCH_CONTROLLER_PLUS_EXTENSION_NAME;
	
	#ifdef OS_ANDROID
		pSupportsExtension[extKHRAndroidCreateInstance].name = XR_KHR_ANDROID_CREATE_INSTANCE_EXTENSION_NAME;
	#else
		pSupportsExtension[extKHRAndroidCreateInstance].name = "XR_KHR_android_create_instance";
	#endif
	
	pSupportsExtension[ extKHROpenglEnable ].enableIfSupported = true;
	pSupportsExtension[ extKHRVisibilityMask ].enableIfSupported = true;
	pSupportsExtension[ extEXTEyeGazeInteraction ].enableIfSupported =
		oxr.GetRequestFeatureEyeGazeTracking() != deBaseVRModule::efslDisabled;
	pSupportsExtension[ extEXTHandJointsMotionRange ].enableIfSupported = true;
	pSupportsExtension[ extEXTHandTracking ].enableIfSupported = true;
// 	pSupportsExtension[ extEXTPerformanceSettings ].enableIfSupported = true;
// 	pSupportsExtension[ extEXTViewConfigurationDepthRange ].enableIfSupported = true;
// 	pSupportsExtension[ extEPICViewConfigurationFov ].enableIfSupported = true;
// 	pSupportsExtension[ extFBDisplayRefreshRate ].enableIfSupported = true;
// 	pSupportsExtension[ extFBFoveation ].enableIfSupported = true;
// 	pSupportsExtension[ extFBFoveationConfiguration ].enableIfSupported = true;
// 	pSupportsExtension[ extFBHandTrackingAim ].enableIfSupported = true;
// 	pSupportsExtension[ extFBHandTrackingCapsules ].enableIfSupported = true;
// 	pSupportsExtension[ extFBHandTrackingMesh ].enableIfSupported = true;
// 	pSupportsExtension[ extFBKeyboardTracking ].enableIfSupported = true;
	pSupportsExtension[ extFBPassthrough ].enableIfSupported = true;
// 	pSupportsExtension[ extFBPassthroughKeyboardHands ].enableIfSupported = true;
// 	pSupportsExtension[ extFBRenderModel ].enableIfSupported = true;
// 	pSupportsExtension[ extFBSpaceWarp ].enableIfSupported = true;
// 	pSupportsExtension[ extFBTriangleMesh ].enableIfSupported = true;
	pSupportsExtension[ extHTCFacialTracking ].enableIfSupported =
		oxr.GetRequestFeatureFacialTracking() != deBaseVRModule::efslDisabled;
	pSupportsExtension[ extHTCXViveTrackerInteraction ].enableIfSupported = true;
// 	extMNDHeadless: do not enable. this causes HMD to not work anymore
	pSupportsExtension[ extEXTDebugUtils ].enableIfSupported = true;
	pSupportsExtension[ extEXTHPMixedRealityController ].enableIfSupported = true;
	pSupportsExtension[ extEXTSamsungOdysseyController ].enableIfSupported = true;
	pSupportsExtension[ extHTCViveCosmosControllerInteraction ].enableIfSupported = true;
	pSupportsExtension[ extHTCViveFocus3ControllerInteraction ].enableIfSupported = true;
	pSupportsExtension[ extHUAWEIControllerInteraction ].enableIfSupported = true;
	pSupportsExtension[ extMSFTHandInteraction ].enableIfSupported = true;
	pSupportsExtension[ extKHRCompositionLayerDepth ].enableIfSupported = true;
	pSupportsExtension[ extEXTHandInteraction ].enableIfSupported = true;
	pSupportsExtension[ extHTCHandInteraction ].enableIfSupported = true;
	pSupportsExtension[extKHRAndroidCreateInstance].enableIfSupported = true;
	pSupportsExtension[extFBTouchControllerPro].enableIfSupported = true;
	pSupportsExtension[extFBTouchControllerProximity].enableIfSupported = true;
	pSupportsExtension[extMETATouchControllerPlus].enableIfSupported = true;
	
	memset( &pSupportsLayer, 0, sizeof( pSupportsLayer ) );
	pSupportsLayer[ layerLunarCoreValidation ].name = "XR_APILAYER_LUNARG_core_validation";
	pSupportsLayer[ layerApiDump ].name = "XR_APILAYER_LUNARG_api_dump";

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
		
		pPathHandLeft = deoxrPath(*this, "/user/hand/left");
		pPathHandRight = deoxrPath(*this, "/user/hand/right");
		pPathHead = deoxrPath(*this, "/user/head");
		pPathGamepad = deoxrPath(*this, "/user/gamepad");
		
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
	return pSupportsExtension[ extension ].enableIfSupported
		&& pSupportsExtension[ extension ].version != 0;
}

uint32_t deoxrInstance::ExtensionVersion( eExtension extension ) const{
	return pSupportsExtension[ extension ].version;
}

bool deoxrInstance::SupportsLayer( eLayer layer ) const{
	return pSupportsLayer[ layer ].enableIfSupported
		&& pSupportsLayer[ layer ].layerVersion != 0;
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
		return;
	}
	
	XrActionSuggestedBinding * const xrbindings = new XrActionSuggestedBinding[ count ];
	int i;
	
	try{
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
		OXR_CHECK( xrSuggestInteractionProfileBindings( pInstance, &psb ) );
		
		delete [] xrbindings;
		
	}catch( const deException & ){
		if( xrbindings ){
			delete [] xrbindings;
		}
		
		pOxr.LogErrorFormat( "SuggestBindings failed for profile '%s' (action -> binding)",
			profile.GetName().GetString() );
		for( i=0; i<count; i++ ){
			pOxr.LogErrorFormat( "- '%s' -> '%s'", bindings[ i ].action->GetName().GetString(),
				bindings[ i ].binding.GetName().GetString() );
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
	uint32_t i, count = 0;
	OXR_CHECK( xrEnumerateInstanceExtensionProperties( XR_NULL_HANDLE, 0, &count, XR_NULL_HANDLE ) );
	if( count == 0 ){
		return;
	}
	
	XrExtensionProperties * const extensions = new XrExtensionProperties[ count ];
	memset( extensions, 0, sizeof( XrExtensionProperties ) * count );
	for( i=0; i<count; i++ ){
		extensions[ i ].type = XR_TYPE_EXTENSION_PROPERTIES;
	}
	
	try{
		OXR_CHECK( xrEnumerateInstanceExtensionProperties( XR_NULL_HANDLE, count, &count, extensions ) );
		
		// report all extensions for debug purpose
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
			if( pSupportsExtension[ i ].version && pSupportsExtension[ i ].enableIfSupported ){
				pOxr.LogInfoFormat( "- %s: %d", pSupportsExtension[ i ].name,
					pSupportsExtension[ i ].version );
			}
		}
		
		// report support extensions
		pOxr.LogInfo( "Not upported Extensions:" );
		for( i=0; i<ExtensionCount; i++ ){
			if( ! pSupportsExtension[ i ].version && pSupportsExtension[ i ].enableIfSupported ){
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
	OXR_CHECK( xrEnumerateApiLayerProperties( 0, &runtimeCount, XR_NULL_HANDLE ) );
	
	#ifdef INTERNAL_XR_LOADER
	const uint32_t loaderCount = pOxr.GetLoader()->GetApiLayerCount();
	#else
	const uint32_t loaderCount = 0;
	#endif
	
	uint32_t i, count = runtimeCount;
	
	XrApiLayerProperties *layers = nullptr;
	try{
		if( runtimeCount + loaderCount > 0 ){
			layers = new XrApiLayerProperties[ runtimeCount + loaderCount ];
			memset( layers, 0, sizeof( XrApiLayerProperties ) * ( runtimeCount + loaderCount ) );
			for( i=0; i<runtimeCount+loaderCount; i++ ){
				layers[ i ].type = XR_TYPE_API_LAYER_PROPERTIES;
			}
			
			OXR_CHECK( xrEnumerateApiLayerProperties( runtimeCount, &runtimeCount, layers ) );
		}
		
		#ifdef INTERNAL_XR_LOADER
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
		#endif // INTERNAL_XR_LOADER
		
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
					pSupportsLayer[ j ].version = ( uint32_t )layers[ i ].specVersion;
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
					XR_VERSION_MAJOR( pSupportsLayer[ i ].version ),
					XR_VERSION_MINOR( pSupportsLayer[ i ].version ),
					XR_VERSION_PATCH( pSupportsLayer[ i ].version ) );
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
	
	pOxr.LogInfoFormat( "Request Feature Eye Gaze Tracking: %d",
		pOxr.GetRequestFeatureEyeGazeTracking() );
	pOxr.LogInfoFormat( "Request Feature Facial Tracking: %d",
		pOxr.GetRequestFeatureFacialTracking() );

	pSupportsLayer[ layerLunarCoreValidation ].enableIfSupported = enableValidationLayers;
	pSupportsLayer[ layerApiDump ].enableIfSupported = false; //enableValidationLayers;
	
#ifdef OS_ANDROID
	{
	PFN_xrInitializeLoaderKHR initFunc = nullptr;
	xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrInitializeLoaderKHR", (PFN_xrVoidFunction*)&initFunc);
	if(initFunc){
		pOxr.LogInfo("Initialize Android XR Loader");
		deOSAndroid &os = *GetOxr().GetGameEngine()->GetOS()->CastToOSAndroid();
		XrLoaderInitInfoAndroidKHR info{XR_TYPE_LOADER_INIT_INFO_ANDROID_KHR};
		info.applicationVM = os.GetJavaVM();
		info.applicationContext = os.GetActivity();
		initFunc((XrLoaderInitInfoBaseHeaderKHR*)&info);
	}
	}
#endif
	
	// create instance
	XrInstanceCreateInfo instanceCreateInfo;
	memset( &instanceCreateInfo, 0, sizeof( instanceCreateInfo ) );
	
	instanceCreateInfo.type = XR_TYPE_INSTANCE_CREATE_INFO;
	
	decStringList moduleVersion( decString( MODULE_VERSION ).Split( '.' ) );
	while( moduleVersion.GetCount() < 3 ){
		moduleVersion.Add( "0" );
	}
	
	#ifdef OS_W32_VS
		strcpy_s( instanceCreateInfo.applicationInfo.applicationName,
			sizeof( instanceCreateInfo.applicationInfo ), "Drag[en]gine" );
		strcpy_s( instanceCreateInfo.applicationInfo.engineName,
			sizeof( instanceCreateInfo.applicationInfo.engineName ), "Drag[en]gine" );
	#else
		strcpy( instanceCreateInfo.applicationInfo.applicationName, "Drag[en]gine" );
		strcpy( instanceCreateInfo.applicationInfo.engineName, "Drag[en]gine" );
	#endif
	instanceCreateInfo.applicationInfo.applicationVersion = 
		( ( moduleVersion.GetAt( 0 ).ToInt() & 0xffff ) << 16 )
		| ( ( moduleVersion.GetAt( 1 ).ToInt() & 0xff ) << 8 )
		| ( moduleVersion.GetAt( 2 ).ToInt() & 0xff );
	instanceCreateInfo.applicationInfo.engineVersion =
		instanceCreateInfo.applicationInfo.applicationVersion;

	// we have to use a low enough version to not trip runtimes.
	// should be no problem since we use extension loading
	instanceCreateInfo.applicationInfo.apiVersion = XR_API_VERSION_1_0;
	
	// detect extensions and layers
	pDetectExtensions();
	pDetectLayers();
	
	// enable layers
	const char *layers[ LayerCount ];
	uint32_t i, layerCount = 0;
	
	memset( layers, 0, sizeof( layers ) );

	for( i=0; i<LayerCount; i++ ){
		if( pSupportsLayer[ i ].layerVersion && pSupportsLayer[ i ].enableIfSupported ){
			layers[ layerCount++ ] = pSupportsLayer[ i ].name;
		}
	}
	
	pOxr.LogInfo( "Enable Layers:" );
	for( i=0; i<layerCount; i++ ){
		pOxr.LogInfoFormat( "- %s", layers[ i ] );
	}
	
	// enable extensions
	const char *extensions[ ExtensionCount ];
	uint32_t extensionCount = 0;
	
	for( i=0; i<ExtensionCount; i++ ){
		if( pSupportsExtension[ i ].version && pSupportsExtension[ i ].enableIfSupported ){
			extensions[ extensionCount++ ] = pSupportsExtension[ i ].name;
		}
	}
	
	pOxr.LogInfo( "Enable Extensions:" );
	for( i=0; i<extensionCount; i++ ){
		pOxr.LogInfoFormat( "- %s", extensions[ i ] );
	}
	
	if( layerCount > 0 ){
		instanceCreateInfo.enabledApiLayerNames = layers;
		instanceCreateInfo.enabledApiLayerCount = layerCount;
	}
	if( extensionCount > 0 ){
		instanceCreateInfo.enabledExtensionNames = extensions;
		instanceCreateInfo.enabledExtensionCount = extensionCount;
	}
	
	#ifdef OS_ANDROID
	void **pcreateNext = (void**)&instanceCreateInfo.next;
	
	XrInstanceCreateInfoAndroidKHR ciAndroid{XR_TYPE_INSTANCE_CREATE_INFO_ANDROID_KHR};
	if(pSupportsExtension[extKHRAndroidCreateInstance].version){
		deOSAndroid &os = *GetOxr().GetGameEngine()->GetOS()->CastToOSAndroid();
		ciAndroid.applicationVM = os.GetJavaVM();
		ciAndroid.applicationActivity = os.GetActivity();
		*pcreateNext = &ciAndroid;
		pcreateNext = (void**)&ciAndroid.next;
	}
	#endif
	
	// create device
	OXR_CHECK( xrCreateInstance( &instanceCreateInfo, &pInstance ) );
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
