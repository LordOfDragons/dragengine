#ifndef EXPORTED_OPENXR_FUNCTION
	#define EXPORTED_OPENXR_FUNCTION( name )
#endif
#ifndef GLOBAL_LEVEL_OPENXR_FUNCTION
	#define GLOBAL_LEVEL_OPENXR_FUNCTION( name )
#endif
#ifndef GLOBAL_LEVEL_OPENXR_FUNCTION_WARN
	#define GLOBAL_LEVEL_OPENXR_FUNCTION_WARN( name )
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

// certain runtimes like SteamVR are broken and do not export this core function
GLOBAL_LEVEL_OPENXR_FUNCTION_WARN( xrEnumerateApiLayerProperties ) 

INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroyInstance )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrPollEvent )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetSystem )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetSystemProperties )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrStringToPath )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrPathToString )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateReferenceSpace )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetReferenceSpaceBoundsRect )

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

INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetCurrentInteractionProfile )

INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateSwapchainFormats )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateSwapchain )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrDestroySwapchain )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateSwapchainImages )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrAcquireSwapchainImage )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrWaitSwapchainImage )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrReleaseSwapchainImage )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrCreateDebugUtilsMessengerEXT, extEXTDebugUtils )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrDestroyDebugUtilsMessengerEXT, extEXTDebugUtils )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrGetOpenGLGraphicsRequirementsKHR, extKHROpenglEnable )

/*
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumeratePhysicalDevices )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetPhysicalDeviceProperties )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrCreateDevice )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetPhysicalDeviceQueueFamilyProperties )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrGetPhysicalDeviceMemoryProperties )
INSTANCE_LEVEL_OPENXR_FUNCTION( xrEnumerateDeviceExtensionProperties )

INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrCreateDebugReportCallbackEXT, extEXTDebugReport )
INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION( xrDestroyDebugReportCallbackEXT, extEXTDebugReport )

DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyDevice )

DEVICE_LEVEL_OPENXR_FUNCTION( xrGetDeviceQueue )
DEVICE_LEVEL_OPENXR_FUNCTION( xrQueueSubmit )
DEVICE_LEVEL_OPENXR_FUNCTION( xrQueueWaitIdle )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateCommandPool )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyCommandPool )
DEVICE_LEVEL_OPENXR_FUNCTION( xrAllocateCommandBuffers )
DEVICE_LEVEL_OPENXR_FUNCTION( xrFreeCommandBuffers )
DEVICE_LEVEL_OPENXR_FUNCTION( xrBeginCommandBuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrEndCommandBuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdCopyBuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdCopyBufferToImage )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdCopyImageToBuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdPipelineBarrier )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdBindPipeline )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdBindDescriptorSets )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdBindVertexBuffers )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdDispatch )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdDraw )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdBeginRenderPass )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdEndRenderPass )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateBuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrGetBufferMemoryRequirements )
DEVICE_LEVEL_OPENXR_FUNCTION( xrBindBufferMemory )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyBuffer )

DEVICE_LEVEL_OPENXR_FUNCTION( xrAllocateMemory )
DEVICE_LEVEL_OPENXR_FUNCTION( xrFreeMemory )
DEVICE_LEVEL_OPENXR_FUNCTION( xrMapMemory )
DEVICE_LEVEL_OPENXR_FUNCTION( xrUnmapMemory )
DEVICE_LEVEL_OPENXR_FUNCTION( xrFlushMappedMemoryRanges )
DEVICE_LEVEL_OPENXR_FUNCTION( xrInvalidateMappedMemoryRanges )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateFence )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyFence )
DEVICE_LEVEL_OPENXR_FUNCTION( xrResetFences )
DEVICE_LEVEL_OPENXR_FUNCTION( xrWaitForFences )
DEVICE_LEVEL_OPENXR_FUNCTION( xrGetFenceStatus )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateDescriptorPool )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyDescriptorPool )
DEVICE_LEVEL_OPENXR_FUNCTION( xrResetDescriptorPool )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateDescriptorSetLayout )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyDescriptorSetLayout )
DEVICE_LEVEL_OPENXR_FUNCTION( xrAllocateDescriptorSets )
DEVICE_LEVEL_OPENXR_FUNCTION( xrUpdateDescriptorSets )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreatePipelineLayout )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyPipelineLayout )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCreatePipelineCache )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyPipelineCache )
DEVICE_LEVEL_OPENXR_FUNCTION( xrGetPipelineCacheData )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateComputePipelines )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateGraphicsPipelines )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyPipeline )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateShaderModule )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyShaderModule )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateRenderPass )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyRenderPass )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateImage )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyImage )
DEVICE_LEVEL_OPENXR_FUNCTION( xrGetImageMemoryRequirements )
DEVICE_LEVEL_OPENXR_FUNCTION( xrBindImageMemory )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateImageView )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyImageView )

DEVICE_LEVEL_OPENXR_FUNCTION( xrCreateFramebuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrDestroyFramebuffer )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdSetViewport )
DEVICE_LEVEL_OPENXR_FUNCTION( xrCmdSetScissor )
*/


#undef EXPORTED_OPENXR_FUNCTION
#undef GLOBAL_LEVEL_OPENXR_FUNCTION
#undef GLOBAL_LEVEL_OPENXR_FUNCTION_WARN
#undef INSTANCE_LEVEL_OPENXR_FUNCTION
#undef INSTANCE_LEVEL_OPENXR_FUNCTION_FROM_EXTENSION
