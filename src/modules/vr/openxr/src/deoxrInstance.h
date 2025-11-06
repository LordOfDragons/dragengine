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

#ifndef _DEOXRINSTANCE_H_
#define _DEOXRINSTANCE_H_

#include "deoxrBasics.h"
// #include "deoxrDevice.h"
#include "deoxrDebug.h"
#include "deoxrPath.h"
#include "extension/xdev/XR_MNDX_xdev_space.h"

#include <dragengine/deObject.h>

class deVROpenXR;
class deoxrAction;


/**
 * Oxr instance.
 */
class deoxrInstance : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrInstance> Ref;
	
	/** Extension. */
	enum eExtension{	
		extKHROpenglEnable, //<! XR_KHR_opengl_enable
		extKHRVisibilityMask, //<! XR_KHR_visibility_mask
		extEXTEyeGazeInteraction, //<! XR_EXT_eye_gaze_interaction
		extEXTHandJointsMotionRange, //<! XR_EXT_hand_joints_motion_range
		extEXTHandTracking, //<! XR_EXT_hand_tracking
		extEXTPerformanceSettings, //<! XR_EXT_performance_settings
		extEXTViewConfigurationDepthRange, //<! XR_EXT_view_configuration_depth_range
		extEPICViewConfigurationFov, //<! XR_EPIC_view_configuration_fov
		extFBDisplayRefreshRate, //<! XR_FB_display_refresh_rate
		extFBFoveation, //<! XR_FB_foveation
		extFBFoveationConfiguration, //<! XR_FB_foveation_configuration
		extFBHandTrackingAim, //<! XR_FB_hand_tracking_aim
		extFBHandTrackingCapsules, //<! XR_FB_hand_tracking_capsules
		extFBHandTrackingMesh, //<! XR_FB_hand_tracking_mesh
		extFBKeyboardTracking, //<! XR_FB_keyboard_tracking
		extFBPassthrough, //<! XR_FB_passthrough
		extFBPassthroughKeyboardHands, //<! XR_FB_passthrough_keyboard_hands
		extFBRenderModel, //<! XR_FB_render_model
		extFBSpaceWarp, //<! XR_FB_space_warp
		extFBTriangleMesh, //<! XR_FB_triangle_mesh
		extHTCFacialTracking, //<! XR_HTC_facial_tracking
		extHTCXViveTrackerInteraction, //<! XR_HTCX_vive_tracker_interaction
		extMNDHeadless, //<! XR_MND_headless
		extEXTHPMixedRealityController, // <! XR_EXT_hp_mixed_reality_controller
		extEXTSamsungOdysseyController, // <! XR_EXT_samsung_odyssey_controller
		extHTCViveCosmosControllerInteraction, // <! XR_HTC_vive_cosmos_controller_interaction
		extHTCViveFocus3ControllerInteraction, // <! XR_HTC_vive_focus3_controller_interaction
		extHUAWEIControllerInteraction, // <! XR_HUAWEI_controller_interaction
		extMSFTHandInteraction, // <! XR_MSFT_hand_interaction
		extEXTDebugUtils, //<! XR_EXT_debug_utils
		extKHRCompositionLayerDepth, //<! XR_KHR_composition_layer_depth
		extEXTHandInteraction, //<! XR_EXT_hand_interaction
		extHTCHandInteraction, //<! XR_HTC_hand_interaction
		extKHRAndroidCreateInstance, //<! XR_KHR_android_create_instance
		extFBTouchControllerPro, //<! XR_FB_touch_controller_pro
		extFBTouchControllerProximity, //<! XR_FB_touch_controller_proximity
		extMETATouchControllerPlus, //<! XR_META_touch_controller_plus
		extMNDXXDevSpace //<! XR_MNDX_xdev_space
	};
	
	static const int ExtensionCount = extMNDXXDevSpace + 1;
	
	/** Layers. */
	enum eLayer{
		layerLunarCoreValidation, //<! XR_APILAYER_LUNARG_core_validation
		layerApiDump, //<! XR_APILAYER_LUNARG_api_dump
	};
	
	static const int LayerCount = layerApiDump + 1;
	
	/** Suggest binding. */
	struct sSuggestBinding{
		deoxrAction *action;
		deoxrPath binding;
		void Set( deoxrAction *action, const deoxrPath &binding );
	};
	
	
	
private:
	struct sExtension{
		const char *name;
		uint32_t version;
		bool enableIfSupported;
	};
	struct sLayer{
		const char *name;
		uint32_t version;
		uint32_t layerVersion;
		bool enableIfSupported;
	};
	
	deVROpenXR &pOxr;
	deoxrDebug pDebug;
	
	XrInstance pInstance;
	
	sExtension pSupportsExtension[ ExtensionCount ];
	sLayer pSupportsLayer[ LayerCount ];
	
	deoxrPath pPathHandRight, pPathHandLeft, pPathHead, pPathGamepad;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create instance. */
	deoxrInstance( deVROpenXR &oxr, bool enableDebug );
	
protected:
	/** Clean up instance. */
	virtual ~deoxrInstance();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Shared oxr. */
	inline deVROpenXR &GetOxr() const{ return pOxr; }
	
	/** Debug support. */
	inline deoxrDebug &GetDebug(){ return pDebug; }
	
	
	
	/** Extension is supported. */
	bool SupportsExtension( eExtension extension ) const;
	
	/** Extension version or 0 if not supported. */
	uint32_t ExtensionVersion( eExtension extension ) const;
	
	/** Disable extension support. */
	void DisableExtension(eExtension extension);
	
	
	/** Layer is supported. */
	bool SupportsLayer( eLayer layer ) const;
	
	/** Layer version or 0 if not supported. */
	uint32_t LayerVersion( eLayer layer ) const;
	
	
	
	/** Instance. */
	inline XrInstance GetInstance() const{ return pInstance; }
	
	/** Right hand path. */
	inline const deoxrPath &GetPathHandRight() const{ return pPathHandRight; }
	
	/** Left hand path. */
	inline const deoxrPath &GetPathHandLeft() const{ return pPathHandLeft; }
	
	/** Head path. */
	inline const deoxrPath &GetPathHead() const{ return pPathHead; }
	
	/** Gamepad path. */
	inline const deoxrPath &GetPathGamepad() const{ return pPathGamepad; }
	
	/** Suggest input bindings. */
	void SuggestBindings( const deoxrPath &profile, const sSuggestBinding *bindings, int count );
	/*@}*/
	
	
	
	/** \name Oxr Functions */
	/*@{*/
	#define INSTANCE_LEVEL_OPENXR_FUNCTION( name ) PFN_##name name;
	#define INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( name, extension ) PFN_##name name;
	
	#include "deoxrFunctionNames.h"
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pDetectExtensions();
	void pDetectLayers();
	void pCreateInstance( bool enableValidationLayers );
	void pLoadFunctions();
};

#endif

