/* 
 * Drag[en]gine Shared Vulkan
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

#include "devkBasics.h"
#include "deSharedVulkan.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


#ifdef VK_CHECKCOMMANDS

void devkDebugCheckCommand( VkResult result, deSharedVulkan &vulkan, const char *file, int line ){
	#define CASE_ERROR(e) \
		case e: \
			vulkan.GetModule().LogException( deeInvalidAction( file, line, #e ) ); \
			break;
	
	switch( result ){
	case VK_SUCCESS:
		break;
		
	CASE_ERROR( VK_NOT_READY )
	CASE_ERROR( VK_TIMEOUT )
	CASE_ERROR( VK_EVENT_SET )
	CASE_ERROR( VK_EVENT_RESET )
	CASE_ERROR( VK_INCOMPLETE )
	CASE_ERROR( VK_ERROR_OUT_OF_HOST_MEMORY )
	CASE_ERROR( VK_ERROR_OUT_OF_DEVICE_MEMORY )
	CASE_ERROR( VK_ERROR_INITIALIZATION_FAILED )
	CASE_ERROR( VK_ERROR_DEVICE_LOST )
	CASE_ERROR( VK_ERROR_MEMORY_MAP_FAILED )
	CASE_ERROR( VK_ERROR_LAYER_NOT_PRESENT )
	CASE_ERROR( VK_ERROR_EXTENSION_NOT_PRESENT )
	CASE_ERROR( VK_ERROR_FEATURE_NOT_PRESENT )
	CASE_ERROR( VK_ERROR_INCOMPATIBLE_DRIVER )
	CASE_ERROR( VK_ERROR_TOO_MANY_OBJECTS )
	CASE_ERROR( VK_ERROR_FORMAT_NOT_SUPPORTED )
	CASE_ERROR( VK_ERROR_FRAGMENTED_POOL )
	CASE_ERROR( VK_ERROR_UNKNOWN )
	
	// VK_VERSION_1_1
	CASE_ERROR( VK_ERROR_OUT_OF_POOL_MEMORY )
	CASE_ERROR( VK_ERROR_INVALID_EXTERNAL_HANDLE )
	
	// VK_VERSION_1_2
	CASE_ERROR( VK_ERROR_FRAGMENTATION )
	CASE_ERROR( VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS )
	
	// VK_KHR_surface
	CASE_ERROR( VK_ERROR_SURFACE_LOST_KHR )
	CASE_ERROR( VK_ERROR_NATIVE_WINDOW_IN_USE_KHR )
	
	// VK_KHR_swapchain
	CASE_ERROR( VK_SUBOPTIMAL_KHR )
	CASE_ERROR( VK_ERROR_OUT_OF_DATE_KHR )
	
	// VK_KHR_display_swapchain
	CASE_ERROR( VK_ERROR_INCOMPATIBLE_DISPLAY_KHR )
	
	// VK_EXT_debug_report
	CASE_ERROR( VK_ERROR_VALIDATION_FAILED_EXT )
	
	// VK_NV_glsl_shader
	CASE_ERROR( VK_ERROR_INVALID_SHADER_NV )
	
	// VK_EXT_image_drm_format_modifier
	CASE_ERROR( VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT )
	
	// VK_EXT_global_priority
	CASE_ERROR( VK_ERROR_NOT_PERMITTED_EXT )
	
	// VK_EXT_full_screen_exclusive
	CASE_ERROR( VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT )
	
	// VK_KHR_deferred_host_operations
	CASE_ERROR( VK_THREAD_IDLE_KHR )
	CASE_ERROR( VK_THREAD_DONE_KHR )
	CASE_ERROR( VK_OPERATION_DEFERRED_KHR )
	CASE_ERROR( VK_OPERATION_NOT_DEFERRED_KHR )
	
	// VK_EXT_pipeline_creation_cache_control
	CASE_ERROR( VK_PIPELINE_COMPILE_REQUIRED_EXT )
	
	default:{
		decString message;
		message.Format( "Error %x (%d)", result, result );
		vulkan.GetModule().LogException( deeInvalidAction( __FILE__, __LINE__, message ) );
		}
	}
	
	#undef CASE_ERROR
}

#endif
