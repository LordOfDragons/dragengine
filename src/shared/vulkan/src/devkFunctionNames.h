#ifndef EXPORTED_VULKAN_FUNCTION
	#define EXPORTED_VULKAN_FUNCTION( name )
#endif
#ifndef GLOBAL_LEVEL_VULKAN_FUNCTION
	#define GLOBAL_LEVEL_VULKAN_FUNCTION( name )
#endif
#ifndef INSTANCE_LEVEL_VULKAN_FUNCTION
	#define INSTANCE_LEVEL_VULKAN_FUNCTION( name )
#endif
#ifndef INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
	#define INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )
#endif
#ifndef DEVICE_LEVEL_VULKAN_FUNCTION
	#define DEVICE_LEVEL_VULKAN_FUNCTION( name )
#endif
#ifndef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
	#define DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION( name, extension )
#endif


EXPORTED_VULKAN_FUNCTION( vkGetInstanceProcAddr )
EXPORTED_VULKAN_FUNCTION( vkGetDeviceProcAddr )

GLOBAL_LEVEL_VULKAN_FUNCTION( vkEnumerateInstanceExtensionProperties ) 
GLOBAL_LEVEL_VULKAN_FUNCTION( vkEnumerateInstanceLayerProperties ) 
GLOBAL_LEVEL_VULKAN_FUNCTION( vkCreateInstance )

INSTANCE_LEVEL_VULKAN_FUNCTION( vkDestroyInstance )

INSTANCE_LEVEL_VULKAN_FUNCTION( vkEnumeratePhysicalDevices )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetPhysicalDeviceProperties )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkCreateDevice )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetPhysicalDeviceQueueFamilyProperties )
INSTANCE_LEVEL_VULKAN_FUNCTION( vkGetPhysicalDeviceMemoryProperties )

DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyDevice )

DEVICE_LEVEL_VULKAN_FUNCTION( vkGetDeviceQueue )
DEVICE_LEVEL_VULKAN_FUNCTION( vkQueueSubmit )
DEVICE_LEVEL_VULKAN_FUNCTION( vkQueueWaitIdle )

DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateCommandPool )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyCommandPool )
DEVICE_LEVEL_VULKAN_FUNCTION( vkAllocateCommandBuffers )
DEVICE_LEVEL_VULKAN_FUNCTION( vkFreeCommandBuffers )
DEVICE_LEVEL_VULKAN_FUNCTION( vkBeginCommandBuffer )
DEVICE_LEVEL_VULKAN_FUNCTION( vkEndCommandBuffer )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCmdCopyBuffer )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCmdPipelineBarrier )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCmdBindPipeline )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCmdBindDescriptorSets )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCmdDispatch )

DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateBuffer )
DEVICE_LEVEL_VULKAN_FUNCTION( vkGetBufferMemoryRequirements )
DEVICE_LEVEL_VULKAN_FUNCTION( vkBindBufferMemory )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyBuffer )

DEVICE_LEVEL_VULKAN_FUNCTION( vkAllocateMemory )
DEVICE_LEVEL_VULKAN_FUNCTION( vkFreeMemory )
DEVICE_LEVEL_VULKAN_FUNCTION( vkMapMemory )
DEVICE_LEVEL_VULKAN_FUNCTION( vkUnmapMemory )
DEVICE_LEVEL_VULKAN_FUNCTION( vkFlushMappedMemoryRanges )
DEVICE_LEVEL_VULKAN_FUNCTION( vkInvalidateMappedMemoryRanges )

DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateFence )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyFence )
DEVICE_LEVEL_VULKAN_FUNCTION( vkResetFences )
DEVICE_LEVEL_VULKAN_FUNCTION( vkWaitForFences )
DEVICE_LEVEL_VULKAN_FUNCTION( vkGetFenceStatus )

DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateDescriptorPool )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyDescriptorPool )
DEVICE_LEVEL_VULKAN_FUNCTION( vkResetDescriptorPool )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateDescriptorSetLayout )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyDescriptorSetLayout )
DEVICE_LEVEL_VULKAN_FUNCTION( vkAllocateDescriptorSets )
DEVICE_LEVEL_VULKAN_FUNCTION( vkUpdateDescriptorSets )

DEVICE_LEVEL_VULKAN_FUNCTION( vkCreatePipelineLayout )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyPipelineLayout )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCreatePipelineCache )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyPipelineCache )
DEVICE_LEVEL_VULKAN_FUNCTION( vkGetPipelineCacheData )

DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateComputePipelines )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyPipeline )
DEVICE_LEVEL_VULKAN_FUNCTION( vkCreateShaderModule )
DEVICE_LEVEL_VULKAN_FUNCTION( vkDestroyShaderModule )


#undef EXPORTED_VULKAN_FUNCTION
#undef GLOBAL_LEVEL_VULKAN_FUNCTION
#undef INSTANCE_LEVEL_VULKAN_FUNCTION
#undef INSTANCE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
#undef DEVICE_LEVEL_VULKAN_FUNCTION
#undef DEVICE_LEVEL_VULKAN_FUNCTION_FROM_EXTENSION
