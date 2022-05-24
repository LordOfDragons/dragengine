#ifndef EXPORTED_OPENXR_FUNCTION
	#define EXPORTED_OPENXR_FUNCTION( name )
#endif
#ifndef GLOBAL_LEVEL_OPENXR_FUNCTION
	#define GLOBAL_LEVEL_OPENXR_FUNCTION( name )
#endif
#ifndef INSTANCE_LEVEL_OPENXR_FUNCTION
	#define INSTANCE_LEVEL_OPENXR_FUNCTION( name )
#endif
#ifndef INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION
	#define INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( name, extension )
#endif


EXPORTED_OPENXR_FUNCTION( xrGetInstanceProcAddr )

GLOBAL_LEVEL_OPENXR_FUNCTION( xrEnumerateInstanceExtensionProperties ) 
GLOBAL_LEVEL_OPENXR_FUNCTION( xrCreateInstance )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroyInstance )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrPollEvent )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetSystem )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetSystemProperties )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrStringToPath )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrPathToString )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateReferenceSpace )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetReferenceSpaceBoundsRect )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateActionSpace )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrLocateSpace )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateSession )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroySession )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrBeginSession )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEndSession )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrRequestExitSession )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrWaitFrame )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrBeginFrame )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEndFrame )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateActionSet )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroyActionSet )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrAttachSessionActionSets )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateAction )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroyAction )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrSuggestInteractionProfileBindings )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrSyncActions )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetActionStateBoolean )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetActionStateFloat )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetActionStateVector2f )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetActionStatePose )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetCurrentInteractionProfile )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateSwapchainFormats )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateSwapchain )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroySwapchain )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateSwapchainImages )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrAcquireSwapchainImage )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrWaitSwapchainImage )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrReleaseSwapchainImage )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateViewConfigurations )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetViewConfigurationProperties )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateViewConfigurationViews )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrLocateViews )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrCreateDebugUtilsMessengerEXT, extEXTDebugUtils )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrDestroyDebugUtilsMessengerEXT, extEXTDebugUtils )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrGetOpenGLGraphicsRequirementsKHR, extKHROpenglEnable )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrGetVisibilityMaskKHR, extKHRVisibilityMask )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrCreateHandTrackerEXT, extEXTHandTracking )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrDestroyHandTrackerEXT, extEXTHandTracking )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrLocateHandJointsEXT, extEXTHandTracking )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrEnumerateViveTrackerPathsHTCX, extHTCXViveTrackerInteraction )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrCreateFacialTrackerHTC, extHTCFacialTracking )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrDestroyFacialTrackerHTC, extHTCFacialTracking )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrGetFacialExpressionsHTC, extHTCFacialTracking )

#undef EXPORTED_OPENXR_FUNCTION
#undef GLOBAL_LEVEL_OPENXR_FUNCTION
#undef INSTANCE_LEVEL_OPENXR_FUNCTION
#undef INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION
